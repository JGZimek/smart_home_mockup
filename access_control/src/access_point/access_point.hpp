#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

class AccessPoint
{
public:
    AccessPoint(const char *ssid, const char *password);

    /**
     * @brief Starts the Access Point and HTTP server.
     */
    void start();

    /**
     * @brief Stops the Access Point and HTTP server.
     */
    void stop();

    /**
     * @brief Handles HTTP server requests. To be called in the main loop.
     */
    void handleRequests();

private:
    const char *ssid;
    const char *password;
    WebServer server;

    /**
     * @brief Initializes the HTTP server routes.
     */
    void setupRoutes();

    /**
     * @brief Handler for the root endpoint.
     */
    void handleRoot();

    /**
     * @brief Handler for saving WiFi and MQTT settings.
     */
    void handleSave();
};
