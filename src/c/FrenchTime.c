#include <pebble.h>
#include "FrenchTime.h"
#include "FrenchCalendarConverter.h"
#include "DecimalTime.h"

static Window *s_main_window;
static BitmapLayer *s_background_layer;
static GBitmap *s_watch_background_bmp;
static TextLayer *s_time_layer, *s_day_layer, *s_date_layer, *s_year_layer;
static Layer *s_hands_layer;
static GPath *s_hour_hand, *s_minute_hand;
static AppTimer *s_decimal_timer;

static void update_date() {

  //Get FrenchRepublicanDate
  FrenchRepublicanDate frd;
  GetFrenchRepublicanDate(&frd);
  
	// Get a tm structure This will be replaced by decimal time
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
  
	// Write the current hours and minutes into a buffer
	static char s_buffer[8];
	strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
	"%H:%M" : "%I:%M", tick_time);

	// Display this time on the TextLayer
	text_layer_set_text(s_time_layer, s_buffer);

	text_layer_set_text(s_day_layer, frd.frd_dayName);

	//display the date
	char *dateLine = (char*)malloc(15 * sizeof(char));
	snprintf(dateLine, 15,"%d %s", frd.frd_day, frd.frd_monthName);
	text_layer_set_text(s_date_layer, dateLine);

	//display the year
	text_layer_set_text(s_year_layer, frd.frd_romanYear);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  //const int16_t second_hand_length = PBL_IF_ROUND_ELSE((bounds.size.w / 2) - 19, bounds.size.w / 2);
  const int16_t second_hand_length = 70;  
  
  PblDeciTm t;
  get_decimal_time(&t);  
  
  //I think 0xffff is TRIG_MAX_RATIO in math.h, maybe try that later?
  int32_t second_angle = t.tm_sec * (TRIG_MAX_RATIO/100);
  GPoint second_hand = {
    .x = (int16_t)(sin_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
  };

  // second hand
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_line(ctx, second_hand, center);

  // minute/hour hand
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  //Is there redundency in this math?
  unsigned int minute_angle = t.tm_min * 3.6;
  gpath_rotate_to(s_minute_hand, (TRIG_MAX_ANGLE / 360) * minute_angle);
  //gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  gpath_draw_filled(ctx, s_minute_hand);
  gpath_draw_outline(ctx, s_minute_hand);

  unsigned int hour_angle = (t.tm_hour * 36) + (t.tm_min * .36);
  gpath_rotate_to(s_hour_hand, (TRIG_MAX_ANGLE / 360) * hour_angle);
  //gpath_rotate_to(s_hour_hand, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  gpath_draw_filled(ctx, s_hour_hand);
  gpath_draw_outline(ctx, s_hour_hand);

  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, center, 4);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, center, 3);
}

//date_handler will handle date
static void date_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_date();
}

static void handle_decimal_timer(void *data) {
  layer_mark_dirty(window_get_root_layer(s_main_window));
  //define a refresh rate instead of using 100
  s_decimal_timer = app_timer_register(100, handle_decimal_timer, NULL);
}

static void main_window_load(Window *window) {
	// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
  
  //Create the background image layer
  s_background_layer = bitmap_layer_create(bounds);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  s_watch_background_bmp = gbitmap_create_with_resource(RESOURCE_ID_WATCH_DECIMAL_BACKGROUND_IMAGE);
  bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_background_layer, s_watch_background_bmp);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
	// Create the time TextLayer with specific bounds
	s_time_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(30, 30), bounds.size.w, 30));
	APP_LOG(APP_LOG_LEVEL_DEBUG, "dimensions: %d x %d", bounds.size.w,bounds.size.h);

	// Improve the layout to be more like a watchface
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_text(s_time_layer, "00:00");
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

	
	// Create the day TextLayer with specific bounds
	s_day_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(50, 50), bounds.size.w, 30));

	text_layer_set_background_color(s_day_layer, GColorClear);
	text_layer_set_text_color(s_day_layer, GColorWhite);
	text_layer_set_text(s_day_layer, "day");
	text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_day_layer));

	// Create the month TextLayer with specific bounds
	s_date_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(90, 90), bounds.size.w, 30));

	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorWhite);
	text_layer_set_text(s_date_layer, "month");
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

	// Create the year TextLayer with specific bounds
	s_year_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(110, 110), bounds.size.w, 30));

	text_layer_set_background_color(s_year_layer, GColorClear);
	text_layer_set_text_color(s_year_layer, GColorWhite);
	text_layer_set_text(s_year_layer, "year");
	text_layer_set_font(s_year_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_year_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_year_layer));
  
  // Create the hands layer
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
  
  //start the timer
  s_decimal_timer = app_timer_register(100, handle_decimal_timer, NULL);
}

static void main_window_unload(Window *window) {
	// Destroy TextLayer
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_day_layer);
	text_layer_destroy(s_date_layer);
  text_layer_destroy(s_year_layer);
  gbitmap_destroy(s_watch_background_bmp);
  bitmap_layer_destroy(s_background_layer);
  layer_destroy(s_hands_layer);
}

static void init() {
	// Create main Window element and assign to pointer
	s_main_window = window_create();

	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);
  
  // init hand paths
  s_minute_hand = gpath_create(&MINUTE_HAND_PATH_POINTS);
  s_hour_hand = gpath_create(&HOUR_HAND_PATH_POINTS);
  
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_hand, center);
  gpath_move_to(s_hour_hand, center);

	// Register with TickTimerService
	tick_timer_service_subscribe(HOUR_UNIT, date_handler);

	// Make sure the time is displayed from the start
	update_date();
}

static void deinit() {
  gpath_destroy(s_minute_hand);
  gpath_destroy(s_hour_hand);
  
	// Destroy Window
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}