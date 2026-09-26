## Core Concepts

The algorithm utilizes **Sensor Fusion** to cross-reference multiple data streams, entirely preventing false positives common in wearable or portable devices.

### 1. 3D Vector Magnitude
The algorithm calculates the absolute physical force and rotation in 3D space using the Pythagorean theorem:
* `accel_tot = sqrt(ax² + ay² + az²)`
* `gyro_tot = sqrt(gx² + gy² + gz²)`

### 2. Anti-Drop Spatial Filter (Speed Memory)
Dropping a device on the floor generates a massive G-force spike, identical to a car crash. To prevent false alarms, the algorithm maintains a rolling buffer of recent GPS speeds. A crash is only triggered if the `total_g_force > 30` **AND** the device was recently traveling at a valid vehicular speed (e.g., `> 10 km/h`).