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
        (*context->sock).send(j.dump());
    });

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        ::clients[ws.getAddress().address] = ws;
        trace(to_string(clients.size()));
    });

    h.onDisconnection([&](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
        ::clients.erase(ws.getAddress().address);
        trace(to_string(clients.size()));
    });

    h.listen(3003);
    h.run();
}

void callback(Plugin *context, string action, Channel *c, json *p) {
    trace("WRITE");
    if (c != nullptr) trace(c->getAddressAsString());
    if (c != nullptr) {
        trace(p->dump());
        string response = p->dump();
        for (auto &entry : clients) {
            uWS::WebSocket<uWS::SERVER> client = entry.second;
            client.send(response.c_str(), strlen(response.c_str()), uWS::OpCode::TEXT);
        }
    }

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