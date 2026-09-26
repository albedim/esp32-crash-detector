#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define IMPACT_THRESHOLD_G 30.0f    // m/s^2 (approximately 3G: extreme braking or crash)
#define ROLLOVER_THRESHOLD_RAD 4.0f // rad/s (unnatural rotation, approx. 230 deg/sec)
#define MIN_SPEED_KMH 10.0f         // km/h (prevents false alarms)
#define SPEED_HISTORY_SIZE 3        // rolling window size for recent valid speeds


typedef struct
{
  float ax, ay, az;
  float gx, gy, gz;
  float lat, lon, speed;
  bool has_gps_fix;      // true if GPS has valid data and not null
} SensorData;

// state memory that keeps track of recent speeds and last known coordinates
typedef struct
{
  float speed_history[SPEED_HISTORY_SIZE];
  int history_index;
  int history_count;

  float last_lat;
  float last_lon;
  bool has_last_coords;
} CrashDetectorState;

// output result from the algorithm
typedef struct
{
  bool alarm;
  char cause[32];
  float lat;
  float lon;
  float impact_force_ms2;
  float pre_crash_speed_kmh;
} CrashResult;

// initializes or resets the state memory
void init_crash_detector(CrashDetectorState *state)
{
  state->history_index = 0;
  state->history_count = 0;
  state->has_last_coords = false;
  for (int i = 0; i < SPEED_HISTORY_SIZE; i++)
  {
    state->speed_history[i] = 0.0f;
  }
}

// main logic function to be called every time new sensor data is received
CrashResult analyze_crash_data(CrashDetectorState *state, SensorData data)
{
  CrashResult result;
  result.alarm = false;
  strcpy(result.cause, "No Crash");
  result.lat = 0.0f;
  result.lon = 0.0f;
  result.impact_force_ms2 = 0.0f;
  result.pre_crash_speed_kmh = 0.0f;

  // if we have valid speed or GPS data, update the last known coordinates and speed history
  if (data.speed > 0.0f || data.lat != 0.0f)
  {
    state->last_lat = data.lat;
    state->last_lon = data.lon;
    state->has_last_coords = true;

    // save the speed in a rolling buffer
    if (data.speed > 0.0f)
    {
      state->speed_history[state->history_index] = data.speed;
      state->history_index = (state->history_index + 1) % SPEED_HISTORY_SIZE;
      if (state->history_count < SPEED_HISTORY_SIZE)
      {
        state->history_count++;
      }
    }
  }

  // calculate the maximum recent speed
  float max_recent_speed = 0.0f;
  for (int i = 0; i < state->history_count; i++)
  {
    if (state->speed_history[i] > max_recent_speed)
    {
      max_recent_speed = state->speed_history[i];
    }
  }

  // VECTOR CALCULATION
  float accel_tot = sqrtf((data.ax * data.ax) + (data.ay * data.ay) + (data.az * data.az));
  float gyro_tot = sqrtf((data.gx * data.gx) + (data.gy * data.gy) + (data.gz * data.gz));

  // CRASH CONDITIONS
  bool is_frontal_impact = accel_tot > IMPACT_THRESHOLD_G;
  bool is_rollover = gyro_tot > ROLLOVER_THRESHOLD_RAD;
  bool in_motion = max_recent_speed > MIN_SPEED_KMH;

  if ((is_frontal_impact || is_rollover) && in_motion)
  {

    result.alarm = true;
    result.impact_force_ms2 = accel_tot;
    result.pre_crash_speed_kmh = max_recent_speed;

    if (is_rollover)
    {
      strcpy(result.cause, "Rollover");
    }
    else
    {
      strcpy(result.cause, "Frontal/Lateral Impact");
    }

    if (state->has_last_coords)
    {
      result.lat = state->last_lat;
      result.lon = state->last_lon;
    }
  }

  return result;
}