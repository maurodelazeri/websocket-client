# websocket-client
C++ Wrapper for https://libwebsockets.org/

Very straightforward to use, make sure you compile [libwebsockets](https://github.com/warmcat/libwebsockets) first, please note that I'm using the static lib [libwebsockets.a](https://github.com/maurodelazeri/websocket-client/blob/master/CMakeLists.txt#L22) and you might need to adjust the the path or use the shared library, so adjust accordingly in your [cmake](https://github.com/maurodelazeri/websocket-client/blob/master/CMakeLists.txt) file.


```c++
#include <libwebsockets.h>
#include "src/libs/callback.h"
#include "src/libs/websocket_client.h"

class websocket : public websocket_client, public client_callback_t {
    bool run_ = true;
public:
    websocket() : websocket_client(this, "ws-feed.pro.coinbase.com") {}

    [[nodiscard]] bool working() const noexcept { return run_; }

    void on_connected() override {
        lwsl_user("client connected\n");
        std::string msg = R"({ "type": "subscribe", "product_ids": [ "ETH-USD", "ETH-EUR" ], "channels": [ "level2", "heartbeat", { "name": "ticker", "product_ids": [ "ETH-BTC", "ETH-USD" ] } ] })";
        send(msg.data(), msg.size());
    }

    void on_disconnected() override {
        lwsl_user("client disconnected\n");
    }

    void on_error(const char *msg, size_t len) override {
        lwsl_user("client error\n");
    }

    void on_data(const char *data, size_t len, size_t remaining) override {
        std::string msg(data, len);
        lwsl_user("data from server: %s\n", msg.c_str());
    }
};

int main() {
    lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, nullptr);
    websocket client;
    client.connect();
    while (client.working()) {
    }
    return 0;
}
```

### Testing

```
cmake .
make
./websocket-client
```

```
[2021/04/04 20:35:07:4351] N: LWS: 4.1.99-v4.1.0-361-g010c8c86, loglevel 1031
[2021/04/04 20:35:07:4352] N: NET CLI SRV H1 H2 WS ConMon IPv6-absent
[2021/04/04 20:35:07:4355] N:  ++ [wsi|0|pipe] (1)
[2021/04/04 20:35:07:4356] N:  ++ [vh|0|netlink] (1)
[2021/04/04 20:35:07:4365] N:  ++ [vh|1|default||-1] (2)
[2021/04/04 20:35:07:4368] U: Connecting to ws-feed.pro.coinbase.com:443/
[2021/04/04 20:35:07:4368] N:  ++ [wsicli|0|WS/h1/ws-feed.pro.coinbase.com] (1)
[2021/04/04 20:35:07:5492] U: client connected
[2021/04/04 20:35:07:5689] U: data from server: {"type":"subscriptions","channels":[{"name":"ticker","product_ids":["ETH-BTC","ETH-USD","ETH-EUR"]},{"name":"level2","product_ids":["ETH-USD","ETH-EUR"]},{"name":"heartbeat","product_ids":["ETH-USD","ETH-EUR"]}]}
[2021/04/04 20:35:07:5689] U: data from server: {"type":"ticker","sequence":3137155619,"product_id":"ETH-BTC","price":"0.03565","open_24h":"0.03533","volume_24h":"13506.99212892","low_24h":"0.03492","high_24h":"0.03596","volume_30d":"631591.13023732","best_bid":"0.03564","best_ask...
[2021/04/04 20:35:07:5938] U: data from server: {"type":"snapshot","product_id":"ETH-EUR","asks":[["1763.63","1.02938083"],["1763.64","4.82847755"],["1763.81","3.00000000"],["1763.82","7.24271632"],["1763.87","0.64200000"],["1764.03","2.00000000"],["1764.04","3.29819290"],["1764.1...
[2021/04/04 20:35:07:5939] U: data from server: ,["1806.07","0.11333117"],["1806.09","268.43000000"],["1806.85","0.05170625"],["1806.95","0.50000000"],["1807.00","0.92146852"],["1807.22","0.01000000"],["1807.39","0.09938438"],["1808.00","0.69795682"],["1808.09","0.08519265"],["180...
````
