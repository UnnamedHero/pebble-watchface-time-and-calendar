#include <pebble.h>
#include "include/vibe.h"

static uint32_t segments[] = {100, 200, 100, 200, 100};
static VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments)
};

void do_vibrate(VIBE vibe_pattern) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "VIBE : %i", vibe_pattern);
  switch (vibe_pattern) {
    case VP_SHORT:
      vibes_short_pulse();
      break;
    case VP_DOUBLE_SHORT:
      vibes_double_pulse();
      break;
    case VP_TRIPLE_SHORT:
      vibes_enqueue_custom_pattern(pat);
      break;
    case VP_LONG:
      vibes_long_pulse();
      break;
  }
}
