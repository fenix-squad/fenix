#include "deps.cpp"
#include "config.cpp"

#include "logs/log.cpp"

#include "devs/@package.cpp"
#include "laser/@package.cpp"

#include "web/server.cpp"
#include "web/socket.cpp"

#include "types/@package.cpp"
using namespace types;

#include "./math.cpp"
#include <cmath>  


Galvo gx({ .dac=XDAC, .ref=XREF });
Galvo gy({ .dac=YDAC, .ref=YREF });
Engine eng({ .x=&gx, .y=&gy });

Laser laser({ .eng=&eng });


const char *ssid = "{ Fenix }";
const char *pass = "Fenix548";


func setup() -> None {
    USB.begin();
    stream.begin();
    SPIFFS.begin();

    ledcSetup(0, FREQ, 5);
    ledcSetup(1, FREQ, 5);
    ledcSetup(2, FREQ, 5);

    ledcAttachPin(RPIN, 0);
    ledcAttachPin(GPIN, 1);
    ledcAttachPin(BPIN, 2);

    eng.speed(spd);
    eng.target({0, 0});
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);
    MDNS.begin("fenix");

    WSocket::init();
    WServer::init();

    MDNS.addService("http", "tcp", 80);

    Path *path = new Path();

    WSocket::handlers[POINTS] = [path](u8 *data, u16 len) {
        Vec<Point> points((len - 1) / 4);
        for (u16 ind = 1; ind < len; ind += 4) {
            u32 point = data[ind] << 24
                        | data[ind + 1] << 16
                        | data[ind + 2] << 8
                        | data[ind + 3];

            i32 x = ((point & (0b11111111'10000000'00000000'00000000)) >> 23) - 255;
            i32 y = 255 - ((point & (0b00000000'01111111'11000000'00000000)) >> 14);

            u8 r = ((point & (0b00000000'00000000'00111110'00000000)) >> 9);
            u8 g = ((point & (0b00000000'00000000'00000001'11110000)) >> 4);
            u8 b = ((point & (0b00000000'00000000'00000000'00001111)) >> 0);

            u8 s = (r != 0) * 255;
            points[(ind - 1) / 4] = { x, y, rgb(s) };
        }
        path->points = points;
    };

    WSocket::handlers[SPEED] = [](u8 *data, u16 len) {
        spd = data[1];
    };

    laser.add({
        path
    });
}


func loop() -> None {
    eng.speed(spd);
    laser.tick();
}
