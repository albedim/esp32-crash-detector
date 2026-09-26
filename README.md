# What's this project about

A distributed, hardware rchitecture for real-time crash and rollover detection, designed for wearable devices like smartwatches and IoT telematics. 

---

## Architecture Overview

The core philosophy of this project is **Separation of Concerns**. The system is split into two distinct entities:

1.  **The Client (Hardware Node - ESP32):** Acts exclusively as a high-speed Data Acquisition system.
2.  **The Server (Brain - C UDP Server):** Handles all business logic, mathematics and state memory.

![Architecture Diagram](https://quickchart.io/graphviz?graph=digraph%20G%20%7B%0A%20%20rankdir%3DLR%3B%0A%20%20bgcolor%3D%22white%22%3B%0A%20%20node%20%5Bshape%3Dbox%2C%20style%3D%22filled%22%2C%20fillcolor%3D%22%23e0e0e0%22%2C%20fontname%3D%22Helvetica%22%2C%20color%3D%22black%22%5D%3B%0A%20%20%0A%20%20subgraph%20cluster_wearable%20%7B%0A%20%20%20%20label%3D%22Wearable%20Device%22%3B%0A%20%20%20%20style%3Ddashed%3B%0A%20%20%20%20color%3Dblack%3B%0A%20%20%20%20bgcolor%3D%22white%22%3B%0A%20%20%20%20%0A%20%20%20%20Sensors%20%5Blabel%3D%22Sensors%5Cn-%20MPU6050%20%28IMU%29%5Cn-%20NEO-6M%20%28GPS%29%22%5D%3B%0A%20%20%20%20ESP32%20%5Blabel%3D%22ESP32%5Cn%28Formatter%29%22%5D%3B%0A%20%20%7D%0A%20%20%0A%20%20subgraph%20cluster_server%20%7B%0A%20%20%20%20label%3D%22Central%20Server%22%3B%0A%20%20%20%20style%3Ddashed%3B%0A%20%20%20%20color%3Dblack%3B%0A%20%20%20%20bgcolor%3D%22white%22%3B%0A%20%20%20%20%0A%20%20%20%20UDP%20%5Blabel%3D%22UDP%20Listener%20%28C%29%22%5D%3B%0A%20%20%20%20Parser%20%5Blabel%3D%22Data%20Parser%22%5D%3B%0A%20%20%20%20Brain%20%5Blabel%3D%22Sensor%20Fusion%20Brain%5Cn%28Crash%20Detection%20C%29%22%5D%3B%0A%20%20%20%20%0A%20%20%20%20UDP%20-%3E%20Parser%3B%0A%20%20%20%20Parser%20-%3E%20Brain%3B%0A%20%20%7D%0A%20%20%0A%20%20Sensors%20-%3E%20ESP32%20%5Blabel%3D%22I2C%2FUART%22%2C%20fontname%3D%22Helvetica%22%2C%20fontsize%3D10%5D%3B%0A%20%20ESP32%20-%3E%20UDP%20%5Blabel%3D%22UDP%22%2C%20fontname%3D%22Helvetica%22%2C%20fontsize%3D10%5D%3B%0A%7D)

### Why Decouple the Algorithm?

If you are building a generic smart device like a smartwatch, the microcontroller should not be hard-coded to know what a car crash is.

The ESP32 is responsible only for collecting and sending data from the accelerometer, gyroscope, and GPS to the device's central server. The server can then process and use this data differently depending on the application's requirements.

Keeping the processing logic on the central server also avoids unnecessary hardware duplication. For example, if the device needs GPS data for other features, implementing the entire application directly on the microcontroller could require an additional GPS sensor for the server, increasing both the space and hardware costs.

By centralizing the data processing, the same sensor data can be shared between different parts of the system without requiring multiple sensors.


**The Solution:**
By moving the algorithmic logic to an isolated co-processor in the device, the ESP32 remains a lightweight, universal telemetry tool. It blindly samples physical forces and coordinates, packing them into a clean payload. The server then interprets that payload to deduce if the user is sleeping, running, or experiencing a violent impact.

### Run the Server
Compile the C server, ensuring you link the math library (`-lm`):
```bash
gcc main.c udp_server.c sensor-data/sensor_data.c crash/crash-detection.c -o my_server -lm
./my_server