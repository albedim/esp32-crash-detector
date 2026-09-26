#ifndef CRASH_DETECTION_H
#define CRASH_DETECTION_H

#include <stdbool.h>
#include "../sensor-data/sensor_data.h"

#define IMPACT_THRESHOLD_G 30.0f
#define ROLLOVER_THRESHOLD_RAD 4.0f
#define MIN_SPEED_KMH 10.0f
#define SPEED_HISTORY_SIZE 3

typedef struct
{
  float speed_history[SPEED_HISTORY_SIZE];
  int history_index;
  int history_count;

  float last_lat;
  float last_lon;
  bool has_last_coords;
} CrashDetectorState;

typedef struct
{
  bool alarm;
  char cause[32];
  float lat;
  float lon;
  float impact_force_ms2;
  float pre_crash_speed_kmh;
} CrashResult;

void init_crash_detector(CrashDetectorState *state);
CrashResult analyze_crash_data(CrashDetectorState *state, SensorData data);

#endif