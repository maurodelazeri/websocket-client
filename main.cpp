#include <libwebsockets.h>
#include "src/libs/callback.h"
#include "src/libs/socket_client.h"
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
    void on_error(const char* msg, size_t len) override {
        lwsl_user("client error\n");
    }
    void on_data(const char* data, size_t len, size_t remaining) override {
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