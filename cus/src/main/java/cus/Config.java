package cus;

public final class Config {

    private Config() {
    }

    // MQTT (verso TMS)
    public static final String MQTT_BROKER_URL = "tcp://broker.mqtt-dashboard.com:1883";
    public static final String MQTT_CLIENT_ID = "cus-backend"; // fisso
    public static final String MQTT_TOPIC_LEVEL = "tms/level";
    public static final String MQTT_LEVEL_PREFIX = "level: ";
    
    // Seriale (verso WCS)
    public static final String SERIAL_PORT_NAME = "/dev/ttyUSB0";
    public static final int SERIAL_BAUD_RATE = 115200;

    // HTTP (verso DBS)
    public static final int HTTP_PORT = 8080;

    // Soglie della policy (vedi TankPolicy)
    public static final int LEVEL_L1_MM = 400;
    public static final int LEVEL_L2_MM = 700;
    public static final long T1_MILLIS = 5000; // tempo sopra L1 prima di aprire al 50%

    // T2: rete di sicurezza per il caso in cui il will NON arrivi mai (broker
    // irraggiungibile)
    public static final long T2_MILLIS = 25000; // silenzio totale dal TMS -> UNCONNECTED

    /*
     * finestra di grazia dopo un messaggio "offline": se un
     * livello valido arriva entro questo tempo, il segnale viene annullato
     * e nessun altro sottosistema viene mai informato della disconnessione
     */
    public static final long GRACE_PERIOD_AFTER_OFFLINE_MS = 3000;

    // storico esposto alla DBS per il grafico
    public static final int HISTORY_SIZE = 30;
}
