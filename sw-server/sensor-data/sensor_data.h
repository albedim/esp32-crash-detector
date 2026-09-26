#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

typedef struct
{
  float ax;
  float ay;
  float az;

  float gx;
  float gy;
  float gz;

  float lat;
  float lon;
  float speed;
} SensorData;

int parse_sensor_data(const char *string, SensorData *data);

#endif