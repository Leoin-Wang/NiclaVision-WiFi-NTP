/*
 * Program Summary – Stable WiFi Connection & NTP Time Sync for Nicla Vision
 * -------------------------------------------------------------------------
 * This program addresses a known issue on the Arduino Nicla Vision where the
 * board connects to WiFi after uploading a sketch, but fails to reconnect after
 * pressing Reset or after a power cycle.
 *
 * The issue is caused by the Murata 1DX WiFi module sometimes failing the WPA2
 * authentication process during cold startup. In such cases, the board reports
 * WiFi.status() = 4 (WL_CONNECT_FAILED) even when the SSID and password are
 * correct.
 *
 * The program uses a connection procedure designed for the Nicla Vision:
 *   - Resets the WiFi driver before each connection attempt
 *   - Adds the required startup delay for the WiFi module to initialize
 *   - Detects Status = 4 and restarts the connection sequence when needed
 *   - Performs several WiFi.begin() attempts to achieve a stable connection
 *
 * Main functionalities:
 *   - Establishes a reliable WiFi connection after Reset or power cycling
 *   - Connects to the specified WiFi network with retry logic
 *   - Retrieves UTC time from an NTP server using NTPClient
 *   - Prints the current year, day of month, and formatted time every second
 *
 * Recommended for:
 *   - Projects using Nicla Vision that require stable WiFi connectivity
 *   - Applications that rely on network time synchronization
 *   - Users encountering WiFi reconnection issues such as Status = 4
 *
 */


#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "NiclaAP";
const char *password = "12345678";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool connectWiFiStable() {
  Serial.println("[WiFi] Resetting driver...");

  WiFi.disconnect();
  WiFi.end();
  delay(800);

  Serial.println("[WiFi] Starting connection...");

  for (int attempt = 1; attempt <= 4; attempt++) {

    Serial.print("[WiFi] Attempt ");
    Serial.println(attempt);

    WiFi.begin(ssid, password);

    for (int i = 0; i < 20; i++) {
      uint8_t s = WiFi.status();

      Serial.print("Status = ");
      Serial.println(s);

      if (s == WL_CONNECTED) {
        Serial.println("[WiFi] Connected!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        return true;
      }

      if (s == WL_CONNECT_FAILED) {
        Serial.println("[WiFi] Auth failed. Restarting driver...");
        WiFi.disconnect();
        WiFi.end();
        delay(800);
        break;
      }

      delay(500);
    }
  }

  Serial.println("[WiFi] All attempts failed.");
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  connectWiFiStable();
  timeClient.begin();
}

void loop() {
  timeClient.update();

  time_t nowEpoch = timeClient.getEpochTime();
  struct tm *nowStruct = gmtime(&nowEpoch);

  int year = nowStruct->tm_year + 1900;
  int day = nowStruct->tm_mday;

  Serial.print("Year: ");
  Serial.print(year);
  Serial.print(" Day: ");
  Serial.print(day);
  Serial.print(" Time: ");
  Serial.println(timeClient.getFormattedTime());

  delay(1000);
}
