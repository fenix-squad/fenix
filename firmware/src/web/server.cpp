#ifndef __WEB_SERVER__
#define __WEB_SERVER__


#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

#include "./socket.cpp"
#include "./types.cpp"
#include "./pages.cpp"

#include "../types/@package.cpp"
using namespace types;


namespace WServer {
    inline WebServer server(80);

    inline func init() -> None {
        server.addHandler(&WSocket::socket);
        server.on("/", HTTP_GET, Web::index);
        server.serveStatic("/", SPIFFS, "/");
        server.begin();
    }
}


#endif
