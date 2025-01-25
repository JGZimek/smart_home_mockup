#include "access_point.hpp"

AccessPoint::AccessPoint(const char *ssid, const char *password)
    : ssid(ssid), password(password), server(80) {}

void AccessPoint::start()
{
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.printf("Access Point started. IP address: %s\n", IP.toString().c_str());

    setupRoutes();
    server.begin();
    Serial.println("HTTP server started.");
}

void AccessPoint::stop()
{
    server.stop();
    WiFi.softAPdisconnect(true);
    Serial.println("Access Point stopped.");
}

void AccessPoint::handleRequests()
{
    server.handleClient();
}

void AccessPoint::setupRoutes()
{
    // Use lambdas to bind non-static member functions
    server.on("/", HTTP_GET, [this]()
              { this->handleRoot(); });
    server.on("/save", HTTP_POST, [this]()
              { this->handleSave(); });
}

void AccessPoint::handleRoot()
{
    server.send(200, "text/html",
                "<html><body><h1>ESP Access Point</h1>"
                "<form method='POST' action='/save'>"
                "<label for='ssid'>WiFi SSID:</label>"
                "<input type='text' id='ssid' name='ssid'><br>"
                "<label for='password'>WiFi Password:</label>"
                "<input type='password' id='password' name='password'><br>"
                "<label for='mqtt'>MQTT Broker:</label>"
                "<input type='text' id='mqtt' name='mqtt'><br>"
                "<button type='submit'>Save</button></form>"
                "</body></html>");
}

void AccessPoint::handleSave()
{
    if (server.hasArg("ssid") && server.hasArg("password") && server.hasArg("mqtt"))
    {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        String mqtt = server.arg("mqtt");

        // Save credentials and stop the Access Point
        Serial.printf("Saved settings: SSID=%s, PASSWORD=%s, MQTT=%s\n", ssid.c_str(), password.c_str(), mqtt.c_str());

        server.send(200, "text/html",
                    "<html><body><h1>Settings Saved. AP will stop now.</h1></body></html>");

        // Stop the Access Point
        stop();
    }
    else
    {
        server.send(400, "text/html",
                    "<html><body><h1>Missing parameters</h1></body></html>");
    }
}
