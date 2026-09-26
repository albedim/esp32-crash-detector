#include <stdio.h>
#include <unistd.h>

#include "udp_server.h"
#include "crash/crash-detection.h"
#include "sensor-data/sensor_data.h"
#include "sensor_data.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
  int sockfd = prepare_udp_server(PORT);

  if (sockfd < 0)
  {
    return 1;
  }

  char buffer[BUFFER_SIZE + 1];

  printf("[UDP Server] entering the listening cycle...\n");

  CrashDetectorState state;
  init_crash_detector(&state);

  while (1)
  {

    int bytes_read = receive_single_message(
        sockfd,
        buffer,
        BUFFER_SIZE);

    if (bytes_read > 0)
    {

      buffer[bytes_read] = '\0';

      SensorData data;

      if (parse_sensor_data(buffer, &data))
      {

        printf("ACCEL: %.2f %.2f %.2f\n", data.ax, data.ay, data.az);
        printf("GYRO: %.2f %.2f %.2f\n", data.gx, data.gy, data.gz);
        printf("GPS: %.6f %.6f\n", data.lat, data.lon);
        printf("SPEED: %.2f km/h\n", data.speed);

        CrashResult result = analyze_crash_data(&state, data);

        if (result.alarm)
        {
          printf("\n====================================\n");
          printf("!!! ALARM SOS DETECTED !!!\n");
          printf("Reason: %s\n", result.cause);
          printf("Position: %.6f, %.6f\n", result.lat, result.lon);
          printf("Strength: %.2f m/s^2\n", result.impact_force_ms2);
          printf("Velocity Before Impact: %.2f km/h\n", result.pre_crash_speed_kmh);
          printf("====================================\n\n");
        }
      }
    }
  }

  close(sockfd);
  return 0;
}