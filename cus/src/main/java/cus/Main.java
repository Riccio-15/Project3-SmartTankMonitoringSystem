package cus;

import cus.gui.MainFrame;
import cus.http.HttpApiServer;
import cus.model.SystemState;
import cus.mqtt.ConnectionWatchdog;
import cus.mqtt.MqttListener;
import cus.serial.SerialService;
import io.vertx.core.Vertx;

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

        Vertx vertx = Vertx.vertx();
        HttpApiServer http = new HttpApiServer(Config.HTTP_PORT, state, serial);
        vertx.deployVerticle(http);

        SwingUtilities.invokeLater(() -> new MainFrame(state).setVisible(true));

        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            watchdog.stop();
            mqtt.stop();
            serial.stop();
            vertx.close();
        }));
    }
}