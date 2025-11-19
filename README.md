# NiclaVision-WiFi-NTP

Program Summary – Stable WiFi Connection & NTP Time Sync for Nicla Vision

This program addresses a known issue on the Arduino Nicla Vision where the
board connects to WiFi after uploading a sketch, but fails to reconnect after
pressing Reset or after a power cycle.

The issue is caused by the Murata 1DX WiFi module sometimes failing the WPA2
authentication process during cold startup. In such cases, the board reports
WiFi.status() = 4 (WL_CONNECT_FAILED) even when the SSID and password are
correct.

The program uses a connection procedure designed for the Nicla Vision:
- Resets the WiFi driver before each connection attempt
- Adds the required startup delay for the WiFi module to initialize
- Detects Status = 4 and restarts the connection sequence when needed
- Performs several WiFi.begin() attempts to achieve a stable connection

Main functionalities:
- Establishes a reliable WiFi connection after Reset or power cycling
- Connects to the specified WiFi network with retry logic
- Retrieves UTC time from an NTP server using NTPClient
- Prints the current year, day of month, and formatted time every second

Recommended for:
- Projects using Nicla Vision that require stable WiFi connectivity
- Applications that rely on network time synchronization
- Users encountering WiFi reconnection issues such as Status = 4
