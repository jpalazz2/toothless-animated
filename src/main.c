#include <pebble.h>
	
#define KEY_BATTERY 0
#define KEY_DATE_FORMAT 1
#define KEY_TIME_FORMAT 2
	
#define DEBUG 1

static Window *s_main_window;
static GBitmap *s_toothless_bitmap;
static GBitmap *s_toothless_left_eye;
static GBitmap *s_toothless_right_eye;
static GBitmap *s_bluetooth_error;
static BitmapLayer *s_face_layer;
static BitmapLayer *s_toothless_layer;
static BitmapLayer *s_toothless_left_eye_layer;
static BitmapLayer *s_toothless_right_eye_layer;
static BitmapLayer *s_right_eyelid;
static BitmapLayer *s_left_eyelid;
static BitmapLayer *s_left_eye_color;
static BitmapLayer *s_right_eye_color;
static BitmapLayer *s_battery_layer;
static BitmapLayer *s_bluetooth_error_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static PropertyAnimation *s_all_animations[17];
static PropertyAnimation *s_right_eye_home_to_right;
static PropertyAnimation *s_left_eye_home_to_left;
static PropertyAnimation *s_right_eye_home_to_center;
static PropertyAnimation *s_left_eye_home_to_center;
static PropertyAnimation *s_right_eye_right_to_center;
static PropertyAnimation *s_left_eye_left_to_center;
static PropertyAnimation *s_right_eye_center_to_right;
static PropertyAnimation *s_left_eye_center_to_left;
static PropertyAnimation *s_right_eye_center_to_home;
static PropertyAnimation *s_left_eye_center_to_home;
static PropertyAnimation *s_right_eye_right_to_home;
static PropertyAnimation *s_left_eye_left_to_home;
static PropertyAnimation *s_right_eyelid_close_start;
static PropertyAnimation *s_right_eyelid_close_stop;
static PropertyAnimation *s_right_eyelid_open;
static PropertyAnimation *s_left_eyelid_close;
static PropertyAnimation *s_left_eyelid_open;
static Animation *s_look_left_full;
static Animation *s_look_right_full;
static Animation *s_look_left_half;
static Animation *s_look_right_half;
static Animation *s_return_home_right;
static Animation *s_return_home_left;
static Animation *s_blink_start;
static Animation *s_blink_finish;
static Animation *s_blink_open;
static Animation *s_look_right_animation;
static Animation *blah;

bool s_show_battery;
bool s_date_format;
bool s_time_format;

static GRect s_right_eye_home_frame;
static GRect s_left_eye_home_frame;
static GRect s_right_eye_center_frame;
static GRect s_left_eye_center_frame;
static GRect s_right_eye_right_frame;
static GRect s_left_eye_left_frame;
static GRect s_right_eyelid_open_position;
static GRect s_right_eyelid_close_position;
static GRect s_left_eyelid_open_position;
static GRect s_left_eyelid_close_position;

static int s_battery_percentage;

static void battery_handler(BatteryStateHandler handler) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery layer updating...");
	s_battery_percentage = battery_state_service_peek().charge_percent;
	layer_mark_dirty((Layer*) s_battery_layer);
}

static Animation* get_anim(PropertyAnimation *a) {
	return property_animation_get_animation(a);
}

static void battery_layer_update_proc(Layer *layer, GContext *ctx) {
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_context_set_stroke_color(ctx, GColorWhite);
	graphics_draw_rect(ctx, GRect(0,0,22,14));
	graphics_draw_rect(ctx, GRect(1,1,20,12));
	graphics_fill_rect(ctx, GRect(22,5,3,5), 0, GCornerNone);
	graphics_fill_rect(ctx, GRect(2,2,(18 * s_battery_percentage)/100,10), 0, GCornerNone);
	layer_set_hidden(bitmap_layer_get_layer(s_battery_layer), !s_show_battery);
}

static void bluetooth_handler(bool connected) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Bluetooth status changed...");
	layer_set_hidden(bitmap_layer_get_layer(s_bluetooth_error_layer), connected);
}


static void update_time() {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating the time...");
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	
	static char dateBuffer[] = "00.00.0000";
	static char timeBuffer[] = "00:00";
	
	if (clock_is_24h_style())
		strftime(timeBuffer, sizeof("00:00"), "%H:%M", tick_time);
	else
		strftime(timeBuffer, sizeof("00:00"), "%I:%M", tick_time);
	
	if (s_date_format)
		strftime(dateBuffer, sizeof("00.00.0000"), "%m.%d.%Y", tick_time);
	else
		strftime(dateBuffer, sizeof("00.00.0000"), "%d.%m.%Y", tick_time);
	
	text_layer_set_text(s_date_layer, dateBuffer);
	text_layer_set_text(s_time_layer, timeBuffer);
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Time updated...");
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick time called...");
	update_time();

	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Scheduling a blink...");

	/*if (tick_time->tm_sec <= 55)
	{
		if (tick_time->tm_min % 2 == 0)
			schedule_look_left_then_right();
		else
			schedule_look_right_then_left();
	}*/
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "App message received...");
	Tuple *t = dict_read_first(iterator);
	
	while (t != NULL)
	{
		switch (t->key)
		{
			case KEY_BATTERY:
				s_show_battery = (int)t->value->int32;
				break;
			case KEY_DATE_FORMAT:
				s_date_format = (int)t->value->int32;
				break;
		}
		
		t = dict_read_next(iterator);
	}
	layer_set_hidden(bitmap_layer_get_layer(s_battery_layer), !s_show_battery);
	update_time();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox sent success!");
}

static void blink_close_start(void) {
	layer_set_bounds(bitmap_layer_get_layer(s_toothless_right_eye_layer), s_right_eye_center_frame);
	layer_set_bounds(bitmap_layer_get_layer(s_toothless_left_eye_layer), s_left_eye_center_frame);
}

static void blink_close_stop(void) {
	layer_set_bounds(bitmap_layer_get_layer(s_toothless_right_eye_layer), s_right_eye_home_frame);
	layer_set_bounds(bitmap_layer_get_layer(s_toothless_left_eye_layer), s_left_eye_home_frame);
}

void main_window_load(Window *window) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Main window loading...");
	s_battery_percentage = battery_state_service_peek().charge_percent;
	s_right_eye_home_frame = GRect(85,78,30,30);
	s_left_eye_home_frame = GRect(29,78,30,30);
	s_right_eye_center_frame = GRect(93,78,30,30);
	s_left_eye_center_frame = GRect(21,78,30,30);
	s_right_eye_right_frame = GRect(102,78,30,30);
	s_left_eye_left_frame = GRect(12,78,30,30);
	s_right_eyelid_open_position = GRect(90,70,38,0);
	s_right_eyelid_close_position = GRect(90,70,38,37);
	s_left_eyelid_open_position = GRect(15,70,39,0);
	s_left_eyelid_close_position = GRect(15,70,39,37);
	
	s_toothless_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TOOTHLESS_FACE);
	s_toothless_left_eye = gbitmap_create_with_resource(RESOURCE_ID_TOOTHLESS_LEFT_EYE);
	s_toothless_right_eye = gbitmap_create_with_resource(RESOURCE_ID_TOOTHLESS_RIGHT_EYE);
	s_bluetooth_error = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_ERROR);
	
	s_bluetooth_error_layer = bitmap_layer_create(GRect(60,25,25,35));
	layer_set_hidden(bitmap_layer_get_layer(s_bluetooth_error_layer), bluetooth_connection_service_peek());
	bitmap_layer_set_bitmap(s_bluetooth_error_layer, s_bluetooth_error);
	
	s_battery_layer = bitmap_layer_create(GRect(10,7,25,15));
	layer_set_update_proc(bitmap_layer_get_layer(s_battery_layer) , battery_layer_update_proc);
	layer_set_hidden(bitmap_layer_get_layer(s_battery_layer), !s_show_battery);
	
	s_toothless_layer = bitmap_layer_create(GRect(0,0,144,144));
	bitmap_layer_set_alignment(s_toothless_layer, GAlignTop);
	bitmap_layer_set_compositing_mode(s_toothless_layer, GCompOpAnd);
	bitmap_layer_set_background_color(s_toothless_layer, GColorClear);
	s_right_eyelid = bitmap_layer_create(s_right_eyelid_open_position);
	s_left_eyelid = bitmap_layer_create(s_left_eyelid_open_position);
	s_toothless_left_eye_layer = bitmap_layer_create(s_left_eye_home_frame);
	s_toothless_right_eye_layer = bitmap_layer_create(s_right_eye_home_frame);

	#ifdef PBL_COLOR
		s_left_eye_color = bitmap_layer_create(s_left_eyelid_close_position);
		s_right_eye_color = bitmap_layer_create(s_right_eyelid_close_position);
		bitmap_layer_set_background_color(s_left_eye_color, GColorBrightGreen);
		bitmap_layer_set_background_color(s_right_eye_color, GColorBrightGreen);
	#endif

	bitmap_layer_set_bitmap(s_toothless_layer, s_toothless_bitmap);
	bitmap_layer_set_bitmap(s_toothless_left_eye_layer, s_toothless_left_eye);
	bitmap_layer_set_bitmap(s_toothless_right_eye_layer, s_toothless_right_eye);
	
	// Black and white resources set to png and pdi8 do not compost correctly on basalt with GCompOpAnd, pdi works fine
	bitmap_layer_set_compositing_mode(s_toothless_left_eye_layer, GCompOpAnd);
	bitmap_layer_set_compositing_mode(s_toothless_right_eye_layer, GCompOpAnd);
	bitmap_layer_set_background_color(s_toothless_left_eye_layer, GColorClear);
	bitmap_layer_set_background_color(s_right_eyelid, GColorBlack);
	bitmap_layer_set_background_color(s_left_eyelid, GColorBlack);

	s_face_layer = bitmap_layer_create(GRect(0,24,144,144));
	bitmap_layer_set_background_color(s_face_layer, GColorWhite);
	
	s_date_layer = text_layer_create(GRect(0,134,144,34));
	text_layer_set_background_color(s_date_layer, GColorBlack);
	text_layer_set_text_color(s_date_layer, GColorWhite);
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	
	s_time_layer = text_layer_create(GRect(0,0,144,30));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	
	update_time();
		
	#ifdef PBL_COLOR 
		// palette[0] is not set to 0, should it be?
		// palette[1] is 255
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Bitmap format: palette[0] = %d, palette[1] = %d", gbitmap_get_palette(s_toothless_left_eye)[0].argb, gbitmap_get_palette(s_toothless_left_eye)[1].argb);
		layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_left_eye_color));
		layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_right_eye_color));
	#endif
	layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_toothless_layer));
	layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_toothless_left_eye_layer));
	layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_toothless_right_eye_layer));
	layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_right_eyelid));
	layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_left_eyelid));
	layer_add_child(bitmap_layer_get_layer(s_face_layer), text_layer_get_layer(s_date_layer));
	
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_battery_layer));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_face_layer));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bluetooth_error_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));

	s_right_eye_home_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_home_frame, &s_right_eye_center_frame);
	s_left_eye_home_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_home_frame, &s_left_eye_center_frame);
	s_right_eye_home_to_right = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_home_frame, &s_right_eye_right_frame);
	s_left_eye_home_to_left = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_home_frame, &s_left_eye_left_frame);
	s_right_eye_right_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_right_frame, &s_right_eye_center_frame);
	s_left_eye_left_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_left_frame, &s_left_eye_center_frame);
	s_right_eye_right_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_right_frame, &s_right_eye_home_frame);
	s_left_eye_left_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_left_frame, &s_left_eye_home_frame);
	s_right_eye_center_to_right = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_center_frame, &s_right_eye_right_frame);
	s_left_eye_center_to_left = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_center_frame, &s_left_eye_left_frame);
	s_right_eye_center_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_center_frame, &s_right_eye_home_frame);
	s_left_eye_center_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_center_frame, &s_left_eye_home_frame);
	s_right_eyelid_close_start = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_left_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	s_right_eyelid_close_stop = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);

	s_all_animations[0] = s_right_eye_home_to_center;
	s_all_animations[1] = s_left_eye_home_to_center;
	s_all_animations[2] = s_right_eye_home_to_right;
	s_all_animations[3] = s_left_eye_home_to_left;
	s_all_animations[4] = s_right_eye_right_to_home;
	s_all_animations[5] = s_left_eye_left_to_home;
	s_all_animations[6] = s_right_eye_right_to_center;
	s_all_animations[7] = s_left_eye_left_to_center;
	s_all_animations[8] = s_right_eye_center_to_right;
	s_all_animations[9] = s_left_eye_center_to_left;
	s_all_animations[10] = s_right_eye_center_to_home;
	s_all_animations[11] = s_left_eye_center_to_home;
	s_all_animations[12] = s_right_eyelid_close_start;
	s_all_animations[13] = s_left_eyelid_close;
	s_all_animations[14] = s_right_eyelid_open;
	s_all_animations[15] = s_left_eyelid_open;
	s_all_animations[16] = s_right_eyelid_close_stop;

	for (int i = 0; i < 16; i++)
	{
		animation_set_delay(property_animation_get_animation(s_all_animations[i]), 500);
		animation_set_duration(property_animation_get_animation(s_all_animations[i]), 200);
	}

	/*
	animation_set_handlers(get_anim(s_right_eyelid_close_start), (AnimationHandlers) {
			.started = NULL,
			.stopped = (AnimationStoppedHandler) blink_close_start
		}, NULL);
	animation_set_handlers(get_anim(s_right_eyelid_close_stop), (AnimationHandlers) {
			.started = NULL,
			.stopped = (AnimationStoppedHandler) blink_close_stop
		}, NULL);
	*/

	#ifdef PBL_COLOR
		blah = animation_clone(get_anim(s_left_eyelid_close));
		s_blink_start = animation_spawn_create(get_anim(s_right_eyelid_close_start), get_anim(s_left_eyelid_close), NULL);
		s_look_right_half = animation_spawn_create(get_anim(s_left_eye_center_to_home), get_anim(s_right_eye_center_to_right), NULL);
		s_look_left_full = animation_spawn_create(get_anim(s_left_eye_home_to_left), get_anim(s_right_eye_right_to_home), NULL);
		s_return_home_left = animation_spawn_create(get_anim(s_left_eye_left_to_center), get_anim(s_right_eye_home_to_center), NULL);
		s_blink_finish = animation_spawn_create(get_anim(s_right_eyelid_close_stop), blah, NULL);
		s_blink_open = animation_spawn_create(get_anim(s_left_eyelid_open), get_anim(s_right_eyelid_open), NULL);
		s_look_right_animation = animation_sequence_create(s_blink_start, s_blink_open, s_look_right_half, s_look_left_full, s_return_home_left, s_blink_finish, NULL);
	#endif

	animation_schedule(s_look_right_animation);

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	battery_state_service_subscribe((BatteryStateHandler) battery_handler);
	bluetooth_connection_service_subscribe((BluetoothConnectionHandler) bluetooth_handler);
}

static void read_storage() {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Reading from persistent storage...");
	
	if (persist_exists(KEY_BATTERY))
		s_show_battery = persist_read_bool(KEY_BATTERY);
	else
		s_show_battery = true;
	
	if (persist_exists(KEY_DATE_FORMAT))
		s_date_format = persist_read_bool(KEY_DATE_FORMAT);
	else
		s_date_format = true;
	
	if (persist_exists(KEY_TIME_FORMAT))
		s_time_format = persist_read_bool(KEY_TIME_FORMAT);
	else
		s_time_format = true;
}

static void store_storage() {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Storing to persistent storage...");
	persist_write_bool(KEY_BATTERY, s_show_battery);
	persist_write_bool(KEY_DATE_FORMAT, s_date_format);
	persist_write_bool(KEY_TIME_FORMAT, s_time_format);
}

void main_window_unload(Window *window) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloading main window...");
	gbitmap_destroy(s_toothless_bitmap);
	gbitmap_destroy(s_toothless_left_eye);
	gbitmap_destroy(s_toothless_right_eye);
	gbitmap_destroy(s_bluetooth_error);

	bitmap_layer_destroy(s_face_layer);
	bitmap_layer_destroy(s_toothless_left_eye_layer);
	bitmap_layer_destroy(s_toothless_right_eye_layer);
	bitmap_layer_destroy(s_right_eyelid);
	bitmap_layer_destroy(s_left_eyelid);
	bitmap_layer_destroy(s_left_eye_color);
	bitmap_layer_destroy(s_right_eye_color);
	bitmap_layer_destroy(s_toothless_layer);
	bitmap_layer_destroy(s_battery_layer);
	bitmap_layer_destroy(s_bluetooth_error_layer);
	
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_date_layer);

	property_animation_destroy(s_right_eye_home_to_center);
	property_animation_destroy(s_left_eye_home_to_center);
	property_animation_destroy(s_right_eye_home_to_right);
	property_animation_destroy(s_left_eye_home_to_left);
	property_animation_destroy(s_right_eye_right_to_home);
	property_animation_destroy(s_left_eye_left_to_home);
	property_animation_destroy(s_right_eye_right_to_center);
	property_animation_destroy(s_left_eye_left_to_center);
	property_animation_destroy(s_right_eye_center_to_right);
	property_animation_destroy(s_left_eye_center_to_left);
	property_animation_destroy(s_right_eye_center_to_home);
	property_animation_destroy(s_left_eye_center_to_home);
	property_animation_destroy(s_right_eyelid_open);
	property_animation_destroy(s_left_eyelid_open);
	property_animation_destroy(s_right_eyelid_close_start);
	property_animation_destroy(s_left_eyelid_close);
	property_animation_destroy(s_right_eyelid_close_stop);
	
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
}

void handle_init(void) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Initializing...");
	read_storage();
  	s_main_window = window_create();
	window_set_background_color(s_main_window, GColorBlack);
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	window_stack_push(s_main_window, false);
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void handle_deinit(void) {
	window_destroy(s_main_window);
	store_storage();
}

int main(void) {
  	handle_init();
  	app_event_loop();
  	handle_deinit();
}
