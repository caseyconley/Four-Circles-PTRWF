#include <pebble.h>

#include <inttypes.h>

typedef struct {
  Layer *layer;
  uint32_t hours;
  uint32_t minutes;
} AppLayer;

typedef struct {
  Window *mywindow;
  AppLayer container_layer;

} CaseyFaceData;

static void prv_print_grect(const char *rect_name, GRect rect_to_print) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s\t\t - x: %d\t\ty: %d\t\tw: %d\t\th: %d", rect_name,
          rect_to_print.origin.x, rect_to_print.origin.y, rect_to_print.size.w, rect_to_print.size.h);
}

static GRect prv_shim_grect_centered_from_polar(GRect rect, GOvalScaleMode scale_mode, int32_t angle, GSize size) {
  const GPoint point_centered_from_polar = gpoint_from_polar(rect, scale_mode, angle);
  return GRect(point_centered_from_polar.x - (size.w / 2),
               point_centered_from_polar.y - (size.h / 2), size.w, size.h);
}

static void container_layer_update_proc(Layer *layer, GContext *ctx)
{
  AppLayer *app_layer = (AppLayer *)layer;
  GRect container_rect = layer_get_bounds(layer);
  container_rect = grect_inset(container_rect, GEdgeInsets(-10));

  //could be min(w/4) for rectangle watch
  int minute_circle_diameter = container_rect.size.w/4;
  GSize minute_circle_rect_size = GSize(minute_circle_diameter, minute_circle_diameter);

  GRect minute_circle_container_rect = grect_inset(container_rect, GEdgeInsets(container_rect.size.w / 3));
  GRect minute_circle_rect = prv_shim_grect_centered_from_polar(minute_circle_container_rect, GOvalScaleModeFitCircle,
                                                                DEG_TO_TRIGANGLE(30), minute_circle_rect_size);

  graphics_context_set_fill_color(ctx, GColorSpringBud);
  graphics_fill_radial(ctx, minute_circle_rect, GOvalScaleModeFitCircle, minute_circle_diameter/2, 0, TRIG_MAX_ANGLE);

  graphics_context_set_text_color(ctx, GColorWhite);
  char minute_string[4] = {0};
  snprintf(minute_string, sizeof(minute_string), "%"PRIu32, app_layer->minutes);
  const GFont minutes_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  graphics_draw_text(ctx, minute_string, minutes_font, minute_circle_rect, GTextOverflowModeFill, GTextAlignmentCenter,
                     NULL);
}

static void main_window_load(Window *window)
{
  CaseyFaceData *data = window_get_user_data(window);

  Layer *root_layer = window_get_root_layer(window);
  data->container_layer.layer = layer_create(layer_get_bounds(root_layer));

  layer_set_update_proc(data->container_layer.layer, container_layer_update_proc);

  layer_add_child(root_layer, data->container_layer.layer);
}

static void main_window_unload(Window *window)
{
  CaseyFaceData *data = window_get_user_data(window);

  layer_destroy(data->container_layer.layer);

  window_destroy(window);


  free(data);
}

static void init()
{
  CaseyFaceData *data = malloc(sizeof(CaseyFaceData));
  memset(data, 0, sizeof(CaseyFaceData));
  data->mywindow = window_create();

  window_set_user_data(data->mywindow, data);

  window_set_window_handlers(data->mywindow, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload,
  });

  window_stack_push(data->mywindow, true);
}

static void deinit()
{
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
  return 0;
}