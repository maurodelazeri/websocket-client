//
// Created by mauro delazeri on 4/4/21.
//

#pragma once

#include <cstdint>
#include <string>
#include <functional>

class client_callback_t;
class socket_service;
struct request_info;

class socket_client {
  client_callback_t* callback_;
  socket_service* service_;
  request_info* request_ = nullptr;
  uint32_t port_;
  std::string address_;

 public:
  socket_client(client_callback_t *callback,
                std::string address,
                uint32_t port,
                int32_t cpu_affinity = -1,
                bool use_global_thread = false);
  virtual ~socket_client();

  /**
   * Connect to WebSocket server
   * @return False if error occurred. Otherwise True.
   */
  bool connect();

  /**
  * Stop socket_client
  */
  void stop() noexcept;

  /**
   * Send message to WebSocket server
   * @param msg     The message to send
   * @param len     The length of the message
   * @return        True on success. Otherwise False.
   */
  bool send(char* msg, size_t len);
};