#include "include/health.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/textutils.h"

static Layer *s_this_layer;
static void prv_populate_health_layer(Layer *, GContext *);
static GFont statuses_font;

void init_health_layer(GRect rect) {
  s_this_layer = layer_create(rect);
  statuses_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_STATUSES_18));
  layer_set_update_proc(s_this_layer, prv_populate_health_layer);
}

void deinit_health_layer() {
  fonts_unload_custom_font(statuses_font);
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
}

Layer* get_layer_health() {
  return s_this_layer;
}

static void prv_populate_health_layer(Layer *me, GContext *ctx) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: HEALTH");
  #endif  
  int steps_count = 0;
  settings_get_theme(ctx);
#if defined(PBL_HEALTH)
  HealthMetric metric = HealthMetricStepCount;
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
  
  if (mask & HealthServiceAccessibilityMaskAvailable) {
    steps_count = (int)health_service_sum_today(metric);
  } else {
    #if defined (DEBUG)
      APP_LOG(APP_LOG_LEVEL_ERROR, "Health data unavalilable");
    #endif
  }
#endif  
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GRect bb = layer_get_bounds(me);
  GRect boot_rect = GRect(0, 0, 30, bb.size.h);
  graphics_draw_text(ctx, "G", \
    statuses_font, \
    boot_rect, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);

  static char health_txt[12];
  snprintf(health_txt, sizeof(health_txt), "%d", steps_count);  
  GRect pretty_rect = get_pretty_rect_for_text(bb, font);
  GRect text_rect = GRect(30, pretty_rect.origin.y, 100, bb.size.h);
  graphics_draw_text(ctx, health_txt, \
      font, \
      text_rect, \
      GTextOverflowModeWordWrap, \
      GTextAlignmentLeft, \
      NULL);
}

void update_layer_health() {
  layer_mark_dirty(s_this_layer);
}
