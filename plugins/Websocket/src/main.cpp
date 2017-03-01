#include <Logger.hpp>
#include <thread>
#include <uWS.h>
#include "Plugin.hpp"

using json = nlohmann::json;

uWS::Hub h;
std::map<string, uWS::WebSocket<uWS::SERVER>> clients;

void runWebsocketServer(Plugin *context) {
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
        context->outgoingDatagrams.add(j.dump());
    });

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        ::clients[ws.getAddress().address] = ws;
    });

    h.onDisconnection([&](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
        ::clients.erase(ws.getAddress().address);
    });

    h.listen(context->config["port"]);
    h.run();
}

void callback(Plugin *context, string action, Channel *c, json raw) {
    string response = raw.dump();
    for (auto &entry : clients) {
        uWS::WebSocket<uWS::SERVER> client = entry.second;
        client.send(response.c_str(), strlen(response.c_str()), uWS::OpCode::TEXT);
    }
}

int init(Plugin *context) {
    context->createThread(runWebsocketServer);
    return 0;
}

extern "C" Plugin *load_plugin() {
    return new Plugin(callback, init, "Websocket", "0.0.4", true);
}

extern "C" void unload_plugin(Plugin *p) {
    delete p;
}