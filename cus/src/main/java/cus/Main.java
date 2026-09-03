package cus;

import cus.gui.MainFrame;
import cus.model.SystemState;
import cus.mqtt.ConnectionWatchdog;
import cus.mqtt.MqttListener;
import cus.serial.SerialService;

import javax.swing.*;

public class Main {

    public static void main(String[] args) throws Exception {
        SystemState state = new SystemState();

        SerialService serial = new SerialService(state);
        serial.start();

        MqttListener mqtt = new MqttListener(state, serial);
        mqtt.start();

        ConnectionWatchdog watchdog = new ConnectionWatchdog(state, serial);
        watchdog.start();

        // possibile implementazione
        //HttpApiServer http = new HttpApiServer(state, serial);
        //http.start();

        SwingUtilities.invokeLater(() -> new MainFrame(state).setVisible(true));

        //andra aggiunto anche http.stop()
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            watchdog.stop();
            mqtt.stop();
            serial.stop();
        }));
    }
}