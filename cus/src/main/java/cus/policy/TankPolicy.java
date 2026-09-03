package cus.policy;

import cus.Config;

public class TankPolicy {

    private enum ValveState { CLOSED, WAITING_T1, HALF_OPEN, FULL_OPEN }

    private ValveState state = ValveState.CLOSED;
    private long aboveL1Since = -1;

    public synchronized void reset() {
        state = ValveState.CLOSED;
        aboveL1Since = -1;
    }

    public synchronized int computeTargetOpen(int levelMm) {
        long now = System.currentTimeMillis();

        System.out.println("stato attuale: " + state + " | now-aboveL1Since: " + (now - aboveL1Since));
        
        switch (state) {
            case CLOSED:
                if (levelMm >= Config.LEVEL_L2_MM) {
                    state = ValveState.FULL_OPEN;
                } else if (levelMm >= Config.LEVEL_L1_MM) {
                    state = ValveState.WAITING_T1;
                    aboveL1Since = now;
                }
                break;

            case WAITING_T1:
                if (levelMm >= Config.LEVEL_L2_MM) {
                    state = ValveState.FULL_OPEN;
                } else if (levelMm < Config.LEVEL_L1_MM) {
                    state = ValveState.CLOSED;
                } else if (now - aboveL1Since >= Config.T1_MILLIS) {
                    state = ValveState.HALF_OPEN;
                }
                break;

            case HALF_OPEN:
                if (levelMm >= Config.LEVEL_L2_MM) {
                    state = ValveState.FULL_OPEN;
                } else if (levelMm < Config.LEVEL_L1_MM) {
                    state = ValveState.CLOSED;
                }
                break;

            case FULL_OPEN:
                if (levelMm < Config.LEVEL_L2_MM) {
                    state = (levelMm >= Config.LEVEL_L1_MM) ? ValveState.HALF_OPEN : ValveState.CLOSED;
                }
                break;
        }

        switch (state) {
            case FULL_OPEN: return 100;
            case HALF_OPEN: return 50;
            default: return 0;
        }
    }
}