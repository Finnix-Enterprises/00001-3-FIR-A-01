#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "BLE_Feature.h"

// WiFi credentials
const char *ssid = "Agatsa7";
const char *password = "12345678";

int firstTime = 0, count_11 = 0;
bool deviceConnected = false;
unsigned long connectionTime = 0; // Store time of BLE connection
// API endpoints
const char *apiEndpoint1 = "https://json-placeholder.mock.beeceptor.com/companies/1"; //"https://json-placeholder.mock.beeceptor.com/comments/{comment_id}"; //"https://jsonplaceholder.typicode.com/todos/1"; // Corrected API URL

void setup()
{
    Serial.begin(115200);
    StartBLE();
}

void loop()
{
    if (deviceConnected)
    {
        // Check if 15 seconds have passed since BLE connection
        if (indication_1 == 1)
        {
            Serial.println("Switching from BLE to WiFi after 15 seconds");
            deinitBLE();     // Deinitialize BLE
            connectToWiFi(); // Switch to WiFi
            fetchAPIData();  // API fetch testing
            delay(4000);
            updateStatus(12, "Check");
        }
    }
}

void deinitBLE()
{
    BLEDevice::deinit();     // Deinitialize BLE
    deviceConnected = false; // Reset connection flag
    Serial.println("BLE deinitialized");
}

void connectToWiFi()
{
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
}

void fetchAPIData()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(apiEndpoint1); // Specify API URL

        // Add User-Agent header to avoid being detected as a bot
        // http.addHeader("User-Agent", "ESP32/1.0"); // Simulating a browser
        // http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"); // Simulating Chrome browser
        // http.addHeader("Accept", "application/json, text/plain, */*");
        // http.addHeader("Accept-Language", "en-US,en;q=0.9");
        // http.addHeader("Connection", "keep-alive");

        int httpResponseCode = http.GET(); // Send HTTP GET request

        if (httpResponseCode > 0)
        {
            // If the response is successful, print response
            String response = http.getString();
            Serial.println("API Response:");
            Serial.println(response);
        }
        else
        {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpResponseCode);
        }
        http.end(); // Free resources
    }
    else
    {
        Serial.println("WiFi not connected");
    }
}

void updateStatus(int employeeID, String newStatus)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(apiEndpoint1); // Specify the API endpoint

        // Add User-Agent header to avoid being detected as a bot
        http.addHeader("User-Agent", "ESP32/1.0"); // Simulating a browser

        // Set content type to JSON
        http.addHeader("Content-Type", "application/json");

        // Create JSON document
        StaticJsonDocument<200> doc; // Adjust size if needed
        doc["id"] = employeeID;
        doc["status"] = newStatus;

        // Serialize JSON to a string
        String payload;
        serializeJson(doc, payload);

        // Send HTTP POST request
        int httpResponseCode = http.POST(payload);

        if (httpResponseCode > 0)
        {
            // If the response is successful, print the response
            String response = http.getString();
            Serial.println("Response from server:");
            Serial.println(response);
        }
        else
        {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpResponseCode);
        }
        http.end(); // Free resources
    }
    else
    {
        Serial.println("WiFi not connected");
    }
}
