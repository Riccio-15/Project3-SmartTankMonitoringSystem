package cus.mqtt;

import cus.Config;
import cus.model.SystemMode;
import cus.model.SystemState;
import cus.policy.TankPolicy;
import cus.serial.SerialService;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.nio.charset.StandardCharsets;

/**
 * <p>
 * Riceve il livello dell'acqua dal TMS via MQTT sul topic tms/level. Lo
 * stesso topic riceve anche il messaggio di Last Will "offline" quando il
 * TMS si disconnette in modo sporco.
 * <p>
 *
 * Il payload "offline" NON viene trattato come UNCONNECTED immediato: si
 * limita a segnalare un sospetto a SystemState (onOfflineSignalReceived),
 * che ConnectionWatchdog confermerà solo se non arriva nessun livello
 * valido entro Config.GRACE_PERIOD_AFTER_OFFLINE_MS. Questo evita falsi
 * allarmi quando il TMS si riconnette rapidamente (vedi discussione sul
 * client take-over MQTT).
 */
public class MqttListener {

    private final SystemState state;
    private final SerialService serial;
    private final TankPolicy policy = new TankPolicy();
    private SystemMode lastObservedMode = null;
    private MqttClient client;

    public MqttListener(SystemState state, SerialService serial) {
        this.state = state;
        this.serial = serial;
    }

    public void start() throws MqttException {
        client = new MqttClient(Config.MQTT_BROKER_URL, Config.MQTT_CLIENT_ID, new MemoryPersistence());

        client.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
                System.err.println("Connessione MQTT persa: " + cause.getMessage());
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) {
                onLevelMessage(new String(message.getPayload(), StandardCharsets.UTF_8));
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });

        MqttConnectOptions options = new MqttConnectOptions();
        options.setAutomaticReconnect(true);
        options.setCleanSession(true);

        client.connect(options);
        client.subscribe(Config.MQTT_TOPIC_LEVEL);
    }

    private void onLevelMessage(String payload) {
        String trimmed = payload.trim();

        if (trimmed.equalsIgnoreCase("offline")) {
            // segnale di possibile disconnessione: non e' ancora una
            // certezza, solo un sospetto che ConnectionWatchdog verifichera'
            // dopo la finestra di grazia
            state.onOfflineSignalReceived();
            return;
        }

        if (!trimmed.startsWith(Config.MQTT_LEVEL_PREFIX)) {
            System.err.println("Payload MQTT non valido su " + Config.MQTT_TOPIC_LEVEL + ": " + payload);
            return;
        }

        String remainder = trimmed.substring(Config.MQTT_LEVEL_PREFIX.length()).trim();
        if (remainder.startsWith("#")) {
            remainder = remainder.substring(1);
        }

        try {
            int level = Integer.parseInt(remainder);
            state.onLevelReceived(level);// annulla anche un eventuale segnale offline pendente

            SystemMode currentMode = state.getMode();
            if (currentMode == SystemMode.AUTOMATIC && lastObservedMode != SystemMode.AUTOMATIC) {
                policy.reset();
            }
            lastObservedMode = currentMode;

            if (currentMode == SystemMode.AUTOMATIC) {
                int target = policy.computeTargetOpen(level);
                serial.sendOpen(target);
            }
        } catch (NumberFormatException e) {
            System.err.println("Payload MQTT non valido su " + Config.MQTT_TOPIC_LEVEL + ": " + payload);
        }
    }

    public void stop() {
        try {
            if (client != null && client.isConnected()) {
                client.disconnect();
            }
        } catch (MqttException ignored) {
        }
    }
}