#include "include/health.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/timelib.h"
#include "../utils/include/textutils.h"
#include "../utils/include/distance.h"
#include "../utils/include/calories.h"

static Layer *s_this_layer = NULL;
static void prv_populate_health_layer(Layer *, GContext *);
static GFont statuses_font;

typedef struct HEALTH_DATA {
  int steps;
  int distance;
  int calories;
}  __attribute__((__packed__)) HEALTH_DATA;

HEALTH_DATA Health_Data = { 0, 0, 0 };

#if defined(PBL_HEALTH)
typedef int (*get_distance_ptr)();
static int get_distance_pebble();
static int get_distance_custom();

typedef int (*get_calories_ptr)();
static int get_calories_pebble();
static int get_calories_custom();

static int get_pebble_health_metric(HealthMetric metric);

static get_distance_ptr get_distance_method() {
  if (settings_is_HealthCustomAlgoritm()) {
    #if defined (DEBUG)
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Health: using custom distance methods");
    #endif
    return get_distance_custom;
  }
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Health: using native distance methods");
  #endif
  return get_distance_pebble;  
}

static get_calories_ptr get_calories_method() {
  if (settings_is_HealthCustomAlgoritm()) {
    return get_calories_custom;
  }
  return get_calories_pebble;
}

static int get_distance_pebble() {
  HealthMetric pbl_metric = HealthMetricWalkedDistanceMeters;
  return get_pebble_health_metric(pbl_metric);
}

static int get_distance_custom() {
  const int height = settings_get_HealthHeight();
  return calculateDistance(Health_Data.steps, height);
}

static int get_calories_pebble() {
  HealthMetric pbl_metric = HealthMetricActiveKCalories;
  return get_pebble_health_metric(pbl_metric);
}

static int get_calories_custom() {
  int weight = settings_get_HealthWeight();
  struct tm *now = get_Time();
  int minutes_of_a_day = get_mins(now->tm_hour, now->tm_min);
  return calculateCalories(weight, Health_Data.distance, minutes_of_a_day);
}

static void populate_health_data() {
  HealthMetric pbl_metric = HealthMetricStepCount;      
  Health_Data.steps = get_pebble_health_metric(pbl_metric);
  Health_Data.distance = get_distance_method()();
  Health_Data.calories = get_calories_method()();
}
#endif

// steps, distance m, distance f, calories
static const char* icons[] = {"G", "H", "H", "I"};

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

#if defined(PBL_HEALTH)
static int get_pebble_health_metric(HealthMetric metric) {
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
  
  if (mask & HealthServiceAccessibilityMaskAvailable) {
    return (int)health_service_sum_today(metric);
  } else {    
    #if defined (DEBUG)
      APP_LOG(APP_LOG_LEVEL_ERROR, "Health data unavalilable");
    #endif
    return -1;
  }
}

static int get_health_metric(PEBBLE_HEALTH_METRIC metric) {  
  switch(metric) {
    case PHM_STEPS:
      return Health_Data.steps;
    case PHM_DISTANCE_M:
      // get_distance_ptr get_distace = get_distance_method();      
      return Health_Data.distance;
    case PHM_DISTANCE_FEET:
      // get_distance_ptr get_distace = get_distance_method();      
      return (int)((float)Health_Data.distance * 3.28F);
    case PHM_CALORIES:      
      return Health_Data.calories;
    default:
      return 0;
  }
}
#endif

static void render_bar(GContext *ctx, GRect bar_rect, PEBBLE_HEALTH_METRIC metric_type, int metric) {
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);  
  GRect icon_rect = GRect(bar_rect.origin.x, 0, 30, bar_rect.size.h);
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "type %d, icon: %s", metric_type, icons[metric_type]);
  #endif
  graphics_draw_text(ctx, icons[metric_type], \
    statuses_font, \
    icon_rect, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);

  static char metric_txt[12];
  snprintf(metric_txt, sizeof(metric_txt), "%d", metric);
  GRect pretty_rect = get_pretty_rect_for_text(bar_rect, font);
  GRect text_rect = GRect(pretty_rect.origin.x + icon_rect.size.w, pretty_rect.origin.y, bar_rect.size.w, bar_rect.size.h);
  graphics_draw_text(ctx, metric_txt, \
      font, \
      text_rect, \
      GTextOverflowModeWordWrap, \
      GTextAlignmentLeft, \
      NULL);  
}

static void prv_populate_health_layer(Layer *me, GContext *ctx) {
#if defined(PBL_HEALTH)
  populate_health_data();
#endif
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: HEALTH");
  #endif  
  settings_get_theme(ctx);

  GRect bb = layer_get_bounds(me);
  GRect left_bar = GRect(0, 0, bb.size.w / 2, bb.size.h);
  GRect right_bar = GRect(left_bar.size.w, 0, left_bar.size.w, bb.size.h);
#if defined(PBL_HEALTH)
  PEBBLE_HEALTH_METRIC left_metric_type = settings_get_LeftHealthMetric();
  PEBBLE_HEALTH_METRIC right_metric_type = settings_get_RightHealthMetric();
  int left_health_metric = get_health_metric(left_metric_type);
  int right_health_metric = get_health_metric(right_metric_type);
  render_bar(ctx, left_bar, left_metric_type, left_health_metric);
  render_bar(ctx, right_bar, right_metric_type, right_health_metric);
#endif
}

void update_layer_health() {
  layer_mark_dirty(s_this_layer);
}
