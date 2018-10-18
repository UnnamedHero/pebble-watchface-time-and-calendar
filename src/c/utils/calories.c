#include "include/calories.h"

int calculateCalories(int weight, int distance, int minutes_of_a_day) {
  int support_calories = (int)(weight * minutes_of_a_day / 60 );
  int steps_calories = (int)(0.76 * weight * distance / 1000.0);
  return support_calories + steps_calories;
}
