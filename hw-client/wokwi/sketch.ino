#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <WiFi.h> 
#include <WiFiUdp.h>

#define GPS_BAUDRATE 9600

const char *ssid = "WIFI_SSID";
const char *password = "WIFI-PASSWORD";

const char *server_ip = "127.0.0.1";
const int server_port = 7000;

WiFiUDP udp;

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;

unsigned long last_print = 0;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(GPS_BAUDRATE, SERIAL_8N1, 16, 17);

  Serial.print("Connecting to Wi-Fi ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  if (!mpu.begin())
  {
    Serial.println("{\"error\": \"MPU6050 not found\"}");
    while (1)
    {
      delay(10);
    }
  }
}

void loop()
{
  while (Serial2.available() > 0)
  {
    gps.encode(Serial2.read());
  }

  if (millis() - last_print > 500)
  {
    last_print = millis();

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    String data =
        String(a.acceleration.x) + "," +
        String(a.acceleration.y) + "," +
        String(a.acceleration.z) + "," +
        String(g.gyro.x) + "," +
        String(g.gyro.y) + "," +
        String(g.gyro.z) + ",";

    if (gps.location.isValid())
    {
      data += String(gps.location.lat(), 6) + "," +
              String(gps.location.lng(), 6) + "," +
              String(gps.speed.kmph());
    }
    else
    {
      data += "0,0,0";
    }

    Serial.println(data);

    udp.beginPacket(server_ip, server_port);
    udp.print(data);
    udp.endPacket();
  }
}