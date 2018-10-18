#include "include/distance.h"

int calculateDistance(int steps, int height) {
  float height_in_meters = (float)height / 100;
  float step_length = (height_in_meters / 4) + 0.37;
  return (int)(step_length * steps);
}

