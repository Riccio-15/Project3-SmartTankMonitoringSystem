package cus.http;

import cus.model.SystemMode;
import cus.model.SystemState;
import cus.serial.SerialService;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;

/**
 * Espone al DBS un'unica risorsa REST che riassume lo stato del sistema.
 * <li>
 * GET /api/data -> { "mode": ..., "opening": ..., "history": [...] }
 * </li>
 * <li>
 * POST /api/data -> { "mode": "AUTOMATIC"|"MANUAL", "opening": 0-100 }
 * </li>
 */
public class HttpApiServer extends AbstractVerticle {

    private final int port;
    private final SystemState state;
    private final SerialService serial;

    public HttpApiServer(int port, SystemState state, SerialService serial) {
        this.port = port;
        this.state = state;
        this.serial = serial;
    }

    /**
     * Avvia il router HTTP: CORS aperto a tutti i domini (il DBS gira come
     * pagina statica separata, quindi le fetch() del browser sono
     * cross-origin), BodyHandler per leggere il body JSON delle POST, e le
     * due route su /api/data.
     */
    @Override
    public void start() {
        Router router = Router.router(vertx);

        // Configurazione CORS
        router.route().handler(CorsHandler.create()
                .addOrigin("*") // tutti i domini
                .allowedMethod(HttpMethod.GET)
                .allowedMethod(HttpMethod.POST)
                .allowedHeader("Content-Type"));

        router.route().handler(BodyHandler.create());

        router.post("/api/data").handler(this::handleSetStatus);
        router.get("/api/data").handler(this::handleGetStatus);

        vertx
                .createHttpServer()
                .requestHandler(router)
                .listen(port);

        log("Service ready on port: " + port);
    }

    /**
     * Risponde con lo stato corrente del sistema cosi' come visto dal CUS:
     * modalita', apertura valvola e le ultime letture di livello.
     */
    private void handleGetStatus(RoutingContext routingContext) {
        JsonObject json = new JsonObject();

        json.put("mode", state.getMode());
        json.put("opening", state.getValveOpen());

        JsonArray history = new JsonArray();
        for (Integer i : state.getHistorySnapshot()) {
            history.add(i);
        }
        json.put("history", history);

        routingContext.response()
                .putHeader("content-type", "application/json")
                .end(json.encodePrettily());
    }

    /**
     * Aggiorna la modalita' e/o l'apertura valvola, sia nello stato interno del CUS 
     *  sia inviandolo al WCS via seriale.
     *<p>
     * Rifiuta la richiesta con 409 se il sistema e' UNCONNECTED.
     */
    private void handleSetStatus(RoutingContext routingContext) {
        HttpServerResponse response = routingContext.response();

        JsonObject res = routingContext.body().asJsonObject();
        if (res == null && state.getMode() != SystemMode.MANUAL) {
            sendError(400, response);// bad request
        } else {
            String mode = res.getString("mode");
            int opening = res.getInteger("opening");

            if (state.getMode() == SystemMode.UNCONNECTED) {
                sendError(409, response);// conflict
                return;
            }
            SystemMode newMode = "MANUAL".equalsIgnoreCase(mode) ? SystemMode.MANUAL : SystemMode.AUTOMATIC;
            state.setMode(newMode);
            serial.sendMode(newMode);

            log("Mode set via Http: " + newMode);

            state.setValveOpen(opening);
            serial.sendOpen(opening);
            log("Valve opening set via Http: " + opening + "%");

            response.setStatusCode(200).end();
        }
    }

    private void sendError(int statusCode, HttpServerResponse response) {
        response.setStatusCode(statusCode).end();
    }

    private void log(String msg) {
        System.out.println("[DATA SERVICE] " + msg);
    }
}