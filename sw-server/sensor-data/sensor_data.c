#include <stdio.h>
#include "sensor_data.h"

int parse_sensor_data(const char *string, SensorData *data)
{
  int values = sscanf(string,
    "%f,%f,%f,%f,%f,%f,%f,%f,%f",
    &data->ax,
    &data->ay,
    &data->az,
    &data->gx,
    &data->gy,
    &data->gz,
    &data->lat,
    &data->lon,
    &data->speed);

  return values == 9;
}