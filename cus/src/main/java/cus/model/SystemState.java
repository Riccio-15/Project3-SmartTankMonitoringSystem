package cus.model;

import cus.Config;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

/**
 * Stato condiviso tra i vari componenti della CUS (MqttListener, SerialService,
 * HttpApiServer, ConnectionWatchdog, MainFrame). Tutti gli accessi sono
 * sincronizzati perche ogni componente gira sul proprio thread.
 */
public class SystemState {

    private SystemMode mode = SystemMode.AUTOMATIC;
    private int level = 0;
    private int valveOpen = 0;

    private long lastTmsMessageAt = 0;
    private long lastWcsMessageAt = 0;

    // 0 = nessun segnale "offline" in sospeso. Diverso da zero = timestamp
    // di quando e' arrivato l'ultimo messaggio "offline" non ancora confermato
    // da un successivo periodo di silenzio (vedi ConnectionWatchdog).
    private long pendingOfflineSince = 0;

    private final Deque<Integer> history = new ArrayDeque<>();

    public synchronized SystemMode getMode() {
        return mode;
    }

    public synchronized void setMode(SystemMode mode) {
        this.mode = mode;
    }

    public synchronized int getLevel() {
        return level;
    }

    public synchronized int getValveOpen() {
        return valveOpen;
    }

    public synchronized void setValveOpen(int percent) {
        this.valveOpen = percent;
    }

    public synchronized void onLevelReceived(int levelMm) {
        this.level = levelMm;
        this.lastTmsMessageAt = System.currentTimeMillis();
        this.pendingOfflineSince = 0; // un dato valido annulla qualunque segnale offline pendente

        history.addLast(levelMm);
        while (history.size() > Config.HISTORY_SIZE) {
            history.removeFirst();
        }
    }

    public synchronized void onWcsMessageReceived() {
        this.lastWcsMessageAt = System.currentTimeMillis();
    }

    public synchronized boolean isTmsTimedOut(long timeoutMillis) {
        if (lastTmsMessageAt == 0) {
            return true; // nessun dato ricevuto dal boot
        }
        return (System.currentTimeMillis() - lastTmsMessageAt) > timeoutMillis;
    }

    // gestione del segnale "offline"

    public synchronized void onOfflineSignalReceived() {
        if (pendingOfflineSince == 0) {
            pendingOfflineSince = System.currentTimeMillis();
        }
    }

    public synchronized void clearPendingOfflineSignal() {
        pendingOfflineSince = 0;
    }

    public synchronized boolean isOfflineGracePeriodElapsed(long graceMillis) {
        return pendingOfflineSince != 0 && (System.currentTimeMillis() - pendingOfflineSince) > graceMillis;
    }

    // getter vari

    public synchronized long getLastTmsMessageAt() {
        return lastTmsMessageAt;
    }

    public synchronized long getLastWcsMessageAt() {
        return lastWcsMessageAt;
    }

    public synchronized List<Integer> getHistorySnapshot() {
        return new ArrayList<>(history);
    }
}
