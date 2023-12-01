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

Group *dvd;
Relygon *bd;


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
    Relygon *sq = new Relygon(60, 24);
    Text *tx = (new Text("548"));

    dvd = new Group {
        sq->rotate(45),
        tx->scale(1.8),
    };
    bd = new Relygon(340, 4),

    WSocket::handlers[POINTS] = [&](Vec<u8> &data, u16 len) {
        stream.println("HERE");
        dvd->display(false);
        bd->display(false);

        auto &points = path->points;
        points.resize((len - 1) / 4);

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
    };

    WSocket::handlers[SPEED] = [](Vec<u8> &data, u16 len) {
        spd = data[1];
    };

    laser.add({
        path,
        dvd,
        bd->rotate(45),
    });
}


u32 last = 0;
i32 sx = 4, sy = 4;
i32 x = 0, y = 0;
i32 s = 0;


func loop() -> None {
    eng.speed(spd);
    laser.tick();

    if (millis() - last > 30) {
        s = (s + 1) % 180;
        x += sx;
        y += sy;

        f32 sc = sin(s  / 180.0 * PI) + 0.7;
        if ((x - 60 * sc) < -200 || (x + 60 * sc) > 200) {
            sx = -sx;
        }
        if ((y - 60 * sc) < -200 || (y + 60 * sc) > 200) {
            sy = -sy;
        }

        dvd->move(x, y);
        dvd->scale(sc);
        last = millis();
    }
}
