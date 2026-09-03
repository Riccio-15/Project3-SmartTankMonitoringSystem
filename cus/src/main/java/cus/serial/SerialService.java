package cus.serial;

import cus.Config;
import cus.model.SystemMode;
import cus.model.SystemState;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortException;

/**
 * Comunica con la WCS (Arduino) sullo stesso protocollo a righe/prefissi
 * usato in SerialCommTask.cpp lato Arduino:
 *
 *   CUS -> WCS: mode:AUTO|MANUAL, net:OK|LOST, open:<0-100>
 *   WCS -> CUS: mode:AUTO|MANUAL, open:<0-100>
 *
 * jssc è event-driven: notifica un listener ogni volta che arrivano byte,
 * non garantendo che coincidano con una riga intera. Per questo teniamo un
 * buffer e ricostruiamo le righe complete separate da '\n'.
 */
public class SerialService {

    private final SystemState state;
    private final StringBuilder inputBuffer = new StringBuilder();
    private SerialPort port;

    public SerialService(SystemState state) {
        this.state = state;
    }

    public void start() {
        port = new SerialPort(Config.SERIAL_PORT_NAME);
        try {
            port.openPort();
            port.setParams(
                Config.SERIAL_BAUD_RATE,
                SerialPort.DATABITS_8,
                SerialPort.STOPBITS_1,
                SerialPort.PARITY_NONE
            );
            port.addEventListener(this::onSerialEvent, SerialPort.MASK_RXCHAR);
        } catch (SerialPortException e) {
            throw new RuntimeException("Impossibile aprire la porta seriale " + Config.SERIAL_PORT_NAME, e);
        }
    }

    public void stop() {
        try {
            if (port != null && port.isOpened()) {
                port.closePort();
            }
        } catch (SerialPortException e) {
            System.err.println("Errore chiusura porta seriale: " + e.getMessage());
        }
    }

    private void onSerialEvent(SerialPortEvent event) {
        if (!event.isRXCHAR() || event.getEventValue() <= 0) {
            return;
        }
        try {
            String chunk = port.readString(event.getEventValue());
            if (chunk != null) {
                appendAndProcess(chunk);
            }
        } catch (SerialPortException e) {
            System.err.println("Errore lettura seriale: " + e.getMessage());
        }
    }

    private synchronized void appendAndProcess(String chunk) {
        inputBuffer.append(chunk);

        int newlineIndex;
        while ((newlineIndex = inputBuffer.indexOf("\n")) >= 0) {
            String line = inputBuffer.substring(0, newlineIndex).trim();
            inputBuffer.delete(0, newlineIndex + 1);
            if (!line.isEmpty()) {
                handleIncoming(line);
            }
        }
    }

    private void handleIncoming(String msg) {
        state.onWcsMessageReceived();

        if (msg.startsWith("mode:")) {
            String value = msg.substring("mode:".length());
            state.setMode("AUTO".equals(value) ? SystemMode.AUTOMATIC : SystemMode.MANUAL);
        } else if (msg.startsWith("open:")) {
            try {
                int percent = Integer.parseInt(msg.substring("open:".length()));
                state.setValveOpen(percent);
            } catch (NumberFormatException ignored) {
                // messaggio malformato, ignorato
            }
        }
    }

    public synchronized void sendMode(SystemMode mode) {
        if (mode == SystemMode.UNCONNECTED) {
            return; // la WCS conosce solo AUTO/MANUAL, UNCONNECTED si comunica con "net:"
        }
        write("mode:" + (mode == SystemMode.AUTOMATIC ? "AUTO" : "MANUAL"));
    }

    public synchronized void sendNet(boolean ok) {
        write("net:" + (ok ? "OK" : "LOST"));
    }

    public synchronized void sendOpen(int percent) {
        write("open:" + percent);
    }

    private void write(String line) {
        try {
            port.writeString(line + "\n");
        } catch (SerialPortException e) {
            System.err.println("Errore scrittura seriale: " + e.getMessage());
        }
    }
}
