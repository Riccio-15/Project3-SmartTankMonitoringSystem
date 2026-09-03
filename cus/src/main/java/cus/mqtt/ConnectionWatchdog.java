package cus.mqtt;

import cus.Config;
import cus.model.SystemMode;
import cus.model.SystemState;
import cus.serial.SerialService;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * <h4>Decide quando il sistema entra e esce da UNCONNECTED, controllando due
 * <h4>condizioni indipendenti:</h4>
 *
 * <p>
 * 1. Silenzio assoluto: nessun livello ricevuto da piu' di T2 (copre il
 * caso di una disconnessione "silenziosa", senza nemmeno il will).
 * </p>
 * <p>
 * 2. Segnale "offline" non confermato: e' arrivato un will ma nessun
 * livello valido lo ha smentito entro GRACE_PERIOD_AFTER_OFFLINE_MS
 * (piu' rapido del caso 1, perche' qui abbiamo gia' un indizio concreto
 * di disconnessione, non solo silenzio).
 * </p>
 *
 * <p>
 * In entrambi i casi, il ritorno alla modalita' precedente avviene solo
 * quando arriva un livello valido (che azzera sia il timer di silenzio sia
 * il segnale offline pendente, vedi SystemState.onLevelReceived).
 * </p>
 */
public class ConnectionWatchdog {

    private final SystemState state;
    private final SerialService serial;
    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

    private SystemMode modeBeforeDisconnect = SystemMode.AUTOMATIC;

    public ConnectionWatchdog(SystemState state, SerialService serial) {
        this.state = state;
        this.serial = serial;
    }

    public void start() {
        scheduler.scheduleAtFixedRate(this::check, 1, 1, TimeUnit.SECONDS);
    }

    public void stop() {
        scheduler.shutdownNow();
    }

    private void check() {
        boolean silentTimeout = state.isTmsTimedOut(Config.T2_MILLIS);
        boolean offlineSignalConfirmed = state.isOfflineGracePeriodElapsed(Config.GRACE_PERIOD_AFTER_OFFLINE_MS);
        boolean timedOut = silentTimeout || offlineSignalConfirmed;

        boolean currentlyUnconnected = state.getMode() == SystemMode.UNCONNECTED;

        if (timedOut && !currentlyUnconnected) {
            modeBeforeDisconnect = state.getMode();
            state.setMode(SystemMode.UNCONNECTED);
            serial.sendNet(false);
        } else if (!timedOut && currentlyUnconnected) {
            state.setMode(modeBeforeDisconnect);
            state.clearPendingOfflineSignal();
            serial.sendNet(true);
            serial.sendMode(modeBeforeDisconnect);
        }
    }
}
