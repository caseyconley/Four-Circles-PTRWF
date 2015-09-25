#include <pebble.h>

typedef struct {
  Window *mywindow;

} CaseyFaceData;

static void main_window_load(Window *window)
{

}

static void main_window_unload(Window *window)
{
  window_destroy(window);

  CaseyFaceData *data = window_get_user_data(window);
  free(data);
}

static void init()
{
  CaseyFaceData *data = malloc(sizeof(CaseyFaceData));

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