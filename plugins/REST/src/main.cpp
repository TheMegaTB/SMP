#include <Logger.hpp>
#include <thread>
#include <uWS.h>
#include "Plugin.hpp"

using json = nlohmann::json;

void runWebsocketServer(Plugin *context) {
    uWS::Hub h;


    h.onMessage([context](uWS::WebSocket<uWS::SERVER> ws, char *message, size_t length, uWS::OpCode opCode) {
        string msg(message, message + length);

        json j;

        try {
            j = json::parse(msg);
        } catch (const std::invalid_argument err) {
            json write = {
                    {"error",   true},
                    {"request", j.dump()}
            };
            string response = write.dump();
            ws.send(response.c_str(), strlen(response.c_str()), uWS::OpCode::TEXT);
            return;
        }
        // TODO: Implement some kind of permission check
        (*context->sock).send(j.dump());
    });

    h.listen(3003);
    h.run();
}

void callback(Plugin *context, string action, Channel c, json p) {
    trace("WRITE");
    trace(c.getAddressAsString());
    trace(p.dump());
}

int init(Plugin *context) {
    thread networking(runWebsocketServer, context);
    networking.detach(); // Let the thread continue to run in the background
    return 0;
}

extern "C" Plugin *load_plugin() {
    return new Plugin(callback, init, "REST", "0.0.2");
}

extern "C" void unload_plugin(Plugin *p) {
    delete p;
}