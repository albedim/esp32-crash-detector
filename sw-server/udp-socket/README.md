## What's this

This part of the project provides the functions required to establish a UDP socket connection between the hardware client and the server.

The hardware is an ESP32 circuit, which can be found in hw-client/wokwi. The server runs as a separate program on the same device that hosts the device software. This approach minimizes the amount of physical hardware required, helping to keep the overall cost and complexity of the system low.