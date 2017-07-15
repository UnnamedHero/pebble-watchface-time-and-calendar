#include <pebble.h>
#include "include/textutils.h"


GRect get_pretty_rect_for_text(GRect origin, GFont font) {
  const char *txt = "TEST";
  GSize size = graphics_text_layout_get_content_size(txt, font, origin, \
    GTextOverflowModeWordWrap, GTextAlignmentCenter);
  int diff_height = -(size.h / 3) + size.h / 10;
  return GRect(origin.origin.x, diff_height, origin.size.w, origin.size.h);
}
