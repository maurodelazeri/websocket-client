//
// Created by mauro delazeri on 4/4/21.
//

#pragma once

/**
 * callback interface
 */
class client_callback_t {
public:
    virtual ~client_callback_t() = default;

    /**
     * on_connected invoked when connection established
     */
    virtual void on_connected() = 0;

    /**
     * on_disconnected invoked when connection closed
     */
    virtual void on_disconnected() = 0;

    /**
     * on_error invoked when error occurred
     *
     * @param msg     Error message. Might be empty.
     * @param len     Error message length
     */
    virtual void on_error(const char *msg, size_t len) = 0;

    /**
     * on_data invoked when data received
     *
     * @param data        Data string
     * @param len         Current data length
     * @param remaining   How many data remains
     */
    virtual void on_data(const char *data, size_t len, size_t remaining) = 0;
};