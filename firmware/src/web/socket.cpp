#ifndef __WEB_SOCKET__
#define __WEB_SOCKET__


#include <ESPAsyncWebServer.h>
#include "./types.cpp"

#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


enum Types {
    POINTS,
    SPEED = 255,
};


namespace WSocket {
    inline WebSocket::Server socket("/ws");
    inline Vec<Func<None(Vec<u8> &data, u16 len)>> handlers(256);
    inline Vec<u8> cache;
    
    inline func init() -> None {        
        socket.onEvent([](
            WebSocket::Server *server,
            WebSocket::Client *client,
            AwsEventType type,
            void *arg,
            u8 *data,
            size_t len
        ) -> None {
            if (type != WS_EVT_DATA) {
                return;
            }
            AwsFrameInfo *info = (AwsFrameInfo*) arg;
            // stream.printf("\n{ Socket } \n");
            // stream.printf("Message size: %d \n", info->len);
            // stream.printf("Frame size: %d \n", len);
            // stream.printf("Frame ind:  %d \n", info->num);
            // stream.printf("Is final:   %d \n", info->final);
            // stream.printf("Index:      %d \n", info->index);

            if (cache.empty()) {
                cache.resize(info->len);
            }

            for (u32 ind = 0; ind < len; ind++) {
                cache[info->index + ind] = data[ind];
            }

            if ((info->index + len) == info->len) {
                handlers[cache[0]](cache, cache.size());
                cache = {};
            }
        });
    }
}


#endif
