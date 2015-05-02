#include <pebble.h>
  
static Window * s_main_window;
static TextLayer * s_time_layer;
static TextLayer * s_time_seconds_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void main_window_load(Window *window) {
  s_time_layer = text_layer_create(GRect(0,55,124,50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  //text_layer_set_text(s_time_layer, "17:02:00");
  
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_VGA_36));
  
  text_layer_set_font(s_time_layer, s_time_font);// fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GAlignRight);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  
  s_time_seconds_layer = text_layer_create(GRect(124,55,20,50));
  text_layer_set_background_color(s_time_seconds_layer, GColorClear);
  text_layer_set_text_color(s_time_seconds_layer, GColorBlack);
  text_layer_set_font(s_time_seconds_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_time_seconds_layer, GAlignRight);

  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TUT);
  s_background_layer = bitmap_layer_create(GRect(0, -5, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_seconds_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_time_seconds_layer);
  
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  
}  
 
static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char buffer[] = "00:00";
  static char buffer_seconds[] = "00";
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  strftime(buffer_seconds, sizeof("00"), "%S", tick_time);
  
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_time_seconds_layer, buffer_seconds);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
}

static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  window_stack_push(s_main_window, true);
  update_time();
}

static void deinit() {
  window_destroy(s_main_window);  
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}