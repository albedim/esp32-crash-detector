# Test

This program is used to test the crash-detection algorithm without requiring the physical ESP32 circuit to be running.

## Usage

The server must be running before executing the test program.

```bash
./test <accelerometer_x,accelerometer_y,accelerometer_z,gyroscope_x,gyroscope_y,gyroscope_z,gps_latitude,gps_longitude,speed>
```

The values are provided in the following order:

1. **Accelerometer:** `ax, ay, az` — m/s²
2. **Gyroscope:** `gx, gy, gz` — rad/s
3. **GPS:** `latitude, longitude` — degrees
4. **Speed:** km/h

Normally, these values are sent by the ESP32 circuit. For testing purposes, however, you can run `./test` manually with custom or random values.

The server's console will display the processed data and whether an alarm has been triggered based on the values provided.

> **Note:** Make sure the server is running before starting a test.

---

## Test Cases

### 1. Normal Driving — NO ALARM

The vehicle is cruising at **50 km/h** under normal driving conditions. The accelerometer measures approximately **1G of gravity on the Z-axis**, with minimal rotational movement.

```bash
./test 0.5,0.2,9.8,0.01,0.05,0.02,40.8518,14.2681,50.0
```

**Expected result:** `ALARM: NO`

---

### 2. Severe Frontal Impact — ALARM

The vehicle is traveling at **60 km/h** and experiences a severe sudden deceleration on the X-axis. The measured acceleration exceeds the configured **30 m/s²** crash-detection threshold.

```bash
./test 45.0,2.0,9.8,0.5,0.5,0.2,40.8518,14.2681,60.0
```

**Expected result:** `ALARM: YES`

---

### 3. Rollover / Spin — ALARM

The vehicle is traveling at **80 km/h**. The acceleration values do not exceed the configured crash threshold, but the gyroscope detects a high rotational velocity. The X-axis angular velocity of **5.5 rad/s** exceeds the **4.0 rad/s** threshold, indicating a possible rollover or spin.

```bash
./test 5.0,5.0,5.0,5.5,1.2,0.5,40.8520,14.2685,80.0
```

**Expected result:** `ALARM: YES`

---

### 4. Dropped Device / Fall from a Table — NO ALARM

The device experiences a large **35 m/s²** acceleration spike on the Z-axis, simulating a fall or impact with the ground. However, the GPS reports a speed of **0 km/h**.

The crash-detection algorithm's anti-drop filter recognizes that the device was stationary and prevents a false alarm.

```bash
./test 0.0,0.0,35.0,1.0,2.0,1.0,40.8520,14.2685,0.0
```

**Expected result:** `ALARM: NO`

---

## Running a Test

Start the server first:

```bash
./my_server
```

Then, in another terminal, run one of the test cases:

```bash
./test 0.5,0.2,9.8,0.01,0.05,0.02,40.8518,14.2681,50.0
```

The server will receive the test data through the same UDP communication mechanism used by the ESP32 and print the resulting crash-detection status to the console.
