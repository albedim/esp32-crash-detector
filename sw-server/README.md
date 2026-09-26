## What's the server

`main.c` is the application server responsible for receiving sensor data from the ESP32 circuit via UDP Socket. It processes information from the accelerometer, gyroscope, and GPS to detect whether a crash has occurred.

## Compile

To compile the server, run:

```bash
gcc main.c udp_server.c sensor-data/sensor_data.c crash/crash-detection.c -o my_server
```
