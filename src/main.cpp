#include "deps.cpp"

#include "logs/log.cpp"

#include "devs/@package.cpp"
#include "laser/@package.cpp"

#include "web/server.cpp"
#include "web/socket.cpp"

#include "types/@package.cpp"
using namespace types;

#include <cmath>  


Galvo gx({ .dac_pin=17, .ref_pin=16 });
Galvo gy({ .dac_pin=18, .ref_pin=21 });
Couple eng({ .x=&gx, .y=&gy });

Laser laser({ .eng=&eng, .laser=33 });


const char *ssid = "{ Fenix }";
const char *pass = "Fenix548";

inline Vec<Vec<i32>> staticc = {};
inline Vec<Vec<i32>> points = {};

i16 xoff = 0, yoff = 0;
f32 scale = 1;


func setup() -> None {
    USB.begin();
    stream.begin();

    /* SCK  =  7
       MISO =  9
       MOSI = 11
       SS   = 12 */
    // SPI.begin(SCK, MISO, MOSI, SS);
    // SD.begin(SS, SPI);
    stream.println("Initializing SPIFFS...");
    SPIFFS.begin();

    eng.speed(spd);
    eng.move({0, 0});
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);
    MDNS.begin("fenix");

    WSocket::init();
    WServer::init();

    MDNS.addService("http", "tcp", 80);

    digitalWrite(33, 1);

    // while(true) {
    //     delayMicroseconds(eng.move({255, 255}));
    //     delayMicroseconds(eng.move({-255, 255}));
    //     delayMicroseconds(eng.move({-255, -255}));
    //     delayMicroseconds(eng.move({255, -255}));
    // }

    Flow *flow = new Flow();
    WSocket::handlers[REAL_TIME] = [flow](u8 *data, u16 len) {
        i32 x = (i32(data[1]) - 128);
        i32 y = (128 - i32(data[2]));
        bool s = data[3];

        flow->points.push_back({x, y, s});
    };

    WSocket::handlers[DRAW] = [](u8 *data, u16 len) {
        i32 x = (i32(data[1]) - 128);
        i32 y = (128 - i32(data[2]));

        bool s = data[3];

        // stream.printf("x=%d, y=%d, light=%d", x, y, s);

        staticc.push_back({x, y, s});;
    };

    WSocket::handlers[POINTS] = [](u8 *data, u16 len) {
        points = {};
        stream.println(len);
        for (u16 ind = 1; ind < len; ind += 3) {
            i32 x = (i32(data[ind]) - 128);
            i32 y = (128 - i32(data[ind + 1]));
            bool s = data[ind + 2];

            stream.println(x);
            stream.println(y);
            stream.println(s);
            stream.println();
            points.push_back({x, y, s});
        }
    };

    WSocket::handlers[SCALE] = [](u8 *data, u16 len) {
        scale = f32(data[1]) / 128.0;
        laser.scale = scale;
    };

    WSocket::handlers[XOFFS] = [](u8 *data, u16 len) {
        xoff = i16(data[1]) - 100;
        laser.xoff = xoff;
    };

    WSocket::handlers[YOFFS] = [](u8 *data, u16 len) {
        yoff = i16(data[1]) - 100;
        laser.yoff = yoff;
    };

    WSocket::handlers[SPEED] = [](u8 *data, u16 len) {
        spd = u16(data[1]) * 256 + u16(data[2]);
    };

    WSocket::handlers[CLEAR] = [](u8 *data, u16 len) {
        staticc = {};
        points = {};
    };

    laser.add({ flow });

    // staticc = {
    //     {200, -100, 1}, 
    //     {-200, -100, 1},
    //     {0, 200, 1}
    // };
}


i16 ind = 0, pos = 0;

func loop() -> None {    
    laser.tick();

    if (!staticc.empty()) {
        digitalWrite(33, staticc[ind][2]);

        i32 x = staticc[ind][0] * scale + xoff;
        i32 y = staticc[ind][1] * scale + yoff;

        delayMicroseconds(eng.move({ .x=x, .y=y }));
        ind = (ind + 1) % staticc.size();
    } else {
        ind = 0;
    }

    if (!points.empty()) {
        digitalWrite(33, points[pos][2]);

        i32 x = points[pos][0] * scale + xoff;
        i32 y = points[pos][1] * scale + yoff;

        delayMicroseconds(eng.move({ .x=x, .y=y }));
        pos = (pos + 1) % points.size();
    } else {
        pos = 0;
    }
}
