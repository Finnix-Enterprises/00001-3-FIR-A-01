
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
// const char *apiEndpoint = "https://dummy.restapiexample.com/api/v1/employee/1"; // Replace with your actual API URL
const char *apiEndpoint = "https://jsonplaceholder.typicode.com/todos/1";              // Dummy API for reference
const char *apiEndpoint1 = "https://dummy.restapiexample.com/public/api/v1/update/21"; // Dummy API for reference
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
            fetchAPIData();  // API fetch testing((
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
        http.begin(apiEndpoint);           // Specify API URL
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
