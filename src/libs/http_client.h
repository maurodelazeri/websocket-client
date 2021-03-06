//
// Created by mauro delazeri on 4/4/21.
//

#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <memory>
#include <thread>

/**
 * Http Request
 */
struct http_request {
private:
    std::string body_;
    std::string content_type_;

    /**
      * NOTE: Header name must end with ":"
      */
    std::unordered_map<std::string, std::string> headers_;

public:
    http_request() = default;

    /**
     * Add a request header
     * @param key     Header name
     * @param value   Header value
     */
    void add_header(std::string key, std::string value) noexcept {
        if (key.back() != ':') {
            key.append(":");
        }
        headers_.emplace(std::move(key), std::move(value));
    }

    /**
     * Add request body
     * @param body            Body content
     * @param content_type    Content type
     */
    void add_body(std::string body, std::string content_type) noexcept {
        body_ = std::move(body);
        content_type_ = std::move(content_type);
    }

    /**
     * Request Headers
     * @return All request headers
     */
    const std::unordered_map<std::string, std::string> &headers() const noexcept { return headers_; }

    /**
     * Request Body
     * @return Request body content
     */
    const std::string &body() const noexcept { return body_; }

    /**
     * Content Type
     * @return Request body content type
     */
    const std::string &content_type() const noexcept { return content_type_; }
};

/**
 * HTTP Response
 */
struct http_response {
    int32_t status = 0;
    std::string content_type;
    std::string response_text;

    http_response(int32_t stat, std::string &&type, std::string &&response) noexcept
            : status(stat), content_type(std::move(type)), response_text(std::move(response)) {}
};

// forward declaration
class socket_service;

/**
 * HTTP Client
 */
class http_client {
    socket_service *service_;
    std::string address_;
    std::string origin_;
    int16_t port_ = -1;

public:
    using AsyncCallback = std::function<void(http_response)>;

    /**
     * Constructor
     * @param address             Request domain url.
     * @param origin              The Origin of http request. Default to "".
     * @param ca_file_path        ssl certificate file path. Default to "".
     *                            In most of case, system should be able to find a suitable ssl certificate.
     *                            If it can't find ssl certificate, pass in ca_file_path to specify a ssl certificate file.
     * @param cpu_affinity        Pin service thread to specified CPU. Default to -1 means not pin to specific CPU.
     * @param use_global_thread   Use
     */
    http_client(std::string address,
                std::string origin = "",
                std::string ca_file_path = "",
                int32_t cpu_affinity = -1,
                bool use_global_thread = false) noexcept;

    virtual ~http_client() noexcept;

    // Synchronous Requests

    /**
     * Synchronous Request
     * @param method      HTTP request method. e.g. "GET", "POST', "PUT", "DELETE" etc.
     *                    NOTE: method must be all UPPER CASE
     * @param path        Request path.
     * @param request     Http request struct.
     * @return            Request response.
     */
    http_response request(const char *method, std::string path, const std::shared_ptr<http_request> &request = nullptr);

    // Async requests

    /**
     * Asynchronous Request
     *
     * @param method      HTTP request method. e.g. "GET", "POST', "PUT", "DELETE" etc.
     *                    NOTE: method must be all UPPER CASE
     * @param path        Request path.
     * @param callback    Asynchronous callback function.
     */
    void request(const char *method, std::string path, AsyncCallback &&callback);

    /**
     * Asynchronous Request
     *
     * @param method      HTTP request method. e.g. "GET", "POST', "PUT", "DELETE" etc.
     *                    NOTE: method must be all UPPER CASE
     * @param path        Request path.
     * @param request     Http request struct.
     * @param callback    Asynchronous callback function.
     */
    void request(const char *method, std::string path, const std::shared_ptr<http_request> &request,
                 AsyncCallback &&callback);

};