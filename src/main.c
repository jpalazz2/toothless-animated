#include <pebble.h>
	
#define KEY_BATTERY 0
#define KEY_DATE_FORMAT 1

static Window *s_main_window;
static GBitmap *s_toothless_bitmap;
static GBitmap *s_toothless_left_eye;
static GBitmap *s_toothless_right_eye;
static GBitmap *s_bluetooth_error;
static BitmapLayer *s_toothless_layer;
static BitmapLayer *s_toothless_left_eye_layer;
static BitmapLayer *s_toothless_right_eye_layer;
static BitmapLayer *s_right_eyelid;
static BitmapLayer *s_left_eyelid;
static BitmapLayer *s_battery_layer;
static BitmapLayer *s_bluetooth_error_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_bluetooth_error_text_layer;
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
static PropertyAnimation *s_right_eyelid_close;
static PropertyAnimation *s_right_eyelid_open;
static PropertyAnimation *s_left_eyelid_close;
static PropertyAnimation *s_left_eyelid_open;

bool s_show_battery;
bool s_date_format;

static GRect s_right_eye_home_frame;
static GRect s_left_eye_home_frame;
static GRect s_right_eye_center_frame;
static GRect s_left_eye_center_frame;
static GRect s_right_eye_look_right_frame;
static GRect s_left_eye_look_left_frame;
static GRect s_right_eyelid_open_position;
static GRect s_right_eyelid_close_position;
static GRect s_left_eyelid_open_position;
static GRect s_left_eyelid_close_position;

static void battery_handler(BatteryStateHandler handler)
{
	layer_mark_dirty((Layer*) s_battery_layer);
}

static void battery_layer_update_proc(Layer *layer, GContext *ctx)
{
	BatteryChargeState service = battery_state_service_peek();
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_context_set_stroke_color(ctx, GColorWhite);
	graphics_draw_rect(ctx, GRect(0,0,22,14));
	graphics_draw_rect(ctx, GRect(1,1,20,12));
	graphics_fill_rect(ctx, GRect(22,5,3,5), 0, GCornerNone);
	graphics_fill_rect(ctx, GRect(2,2,(18 * service.charge_percent)/100,10), 0, GCornerNone);
	layer_set_hidden(bitmap_layer_get_layer(s_battery_layer), !s_show_battery);
}

static void bluetooth_handler(bool connected)
{
	layer_set_hidden(bitmap_layer_get_layer(s_bluetooth_error_layer), connected);
	layer_set_hidden(text_layer_get_layer(s_bluetooth_error_text_layer), connected);
}

static void nullify_animation_handler(PropertyAnimation *animation)
{
	animation_set_handlers((Animation*) animation, (AnimationHandlers)
						   {
							   .stopped = NULL
						   }, NULL);
}

static void schedule_both_eyes_right_to_left(Animation *animation, bool finished, void *context)
{
	animation_schedule((Animation*) s_left_eye_home_to_left);
	animation_schedule((Animation*) s_right_eye_right_to_home);
}

static void schedule_both_eyes_left_to_right(Animation *animation, bool finished, void *context)
{
	animation_schedule((Animation*) s_left_eye_left_to_home);
	animation_schedule((Animation*) s_right_eye_home_to_right);
}

static void schedule_both_eyes_center_to_right(Animation *animation, bool finished, void *context)
{
	animation_schedule((Animation*) s_right_eye_center_to_right);
	animation_schedule((Animation*) s_left_eye_center_to_home);
	animation_set_handlers((Animation*) s_right_eye_home_to_center, (AnimationHandlers)
						   {
								.stopped = NULL   
						   }, NULL);
}

static void schedule_both_eyes_center_to_left(Animation *animation, bool finished, void *context)
{
	animation_schedule((Animation*) s_right_eye_center_to_home);
	animation_schedule((Animation*) s_left_eye_center_to_left);
	
}

static void schedule_both_eyes_left_to_center(Animation *animation, bool finished, void *context)
{
	animation_schedule((Animation*) s_right_eye_home_to_center);
	animation_schedule((Animation*) s_left_eye_left_to_center);
}

static void schedule_both_eyes_right_to_center(Animation *animation, bool finished, void *context)
{
	animation_schedule((Animation*) s_right_eye_right_to_center);
	animation_schedule((Animation*) s_left_eye_home_to_center);
}

static void blink_start_open(Animation *animation, bool finished, void *context)
{
	layer_set_frame((Layer*) s_toothless_right_eye_layer, s_right_eye_center_frame);
	layer_set_frame((Layer*) s_toothless_left_eye_layer, s_left_eye_center_frame);
	animation_schedule((Animation*) s_left_eyelid_open);
	animation_schedule((Animation*) s_right_eyelid_open);
}

static void blink_finished_open(Animation *animation, bool finished, void *context)
{
	layer_set_frame((Layer*) s_toothless_left_eye_layer, s_left_eye_home_frame);
	layer_set_frame((Layer*) s_toothless_right_eye_layer, s_right_eye_home_frame);
	nullify_animation_handler(s_right_eyelid_open);
	animation_schedule((Animation*) s_left_eyelid_open);
	animation_schedule((Animation*) s_right_eyelid_open);
}

static void blink_finished_close(Animation *animation, bool finished, void *context)
{
	animation_set_handlers((Animation*) s_right_eyelid_close, (AnimationHandlers)
						   {
							   .stopped = blink_finished_open
						   }, NULL);
	animation_schedule((Animation*) s_left_eyelid_close);
	animation_schedule((Animation*) s_right_eyelid_close);
}

static void schedule_look_right_then_left()
{
	nullify_animation_handler(s_right_eye_home_to_center);
	nullify_animation_handler(s_left_eye_center_to_left);
	nullify_animation_handler(s_left_eye_left_to_home);
	nullify_animation_handler(s_left_eye_home_to_center);
	animation_set_handlers((Animation*) s_right_eyelid_close, (AnimationHandlers)
						   {
							   .stopped = (AnimationStoppedHandler) blink_start_open
						   }, NULL);
	animation_set_handlers((Animation*) s_right_eyelid_open, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) schedule_both_eyes_center_to_right
						  }, NULL);
	animation_set_handlers((Animation*) s_right_eye_center_to_right, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) schedule_both_eyes_right_to_left
						  }, NULL);
	animation_set_handlers((Animation*) s_right_eye_right_to_home, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) schedule_both_eyes_left_to_center
						  }, NULL);
	animation_set_handlers((Animation*) s_left_eye_left_to_center, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) blink_finished_close
						  }, NULL);
	animation_schedule((Animation*) s_right_eyelid_close);
	animation_schedule((Animation*) s_left_eyelid_close);
}

static void schedule_look_left_then_right()
{
	nullify_animation_handler(s_right_eye_home_to_center);
	nullify_animation_handler(s_right_eye_center_to_right);
	nullify_animation_handler(s_right_eye_right_to_home);
	nullify_animation_handler(s_left_eye_left_to_center);
	animation_set_handlers((Animation*) s_right_eyelid_close, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) blink_start_open
						  }, NULL);
	animation_set_handlers((Animation*) s_right_eyelid_open, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) schedule_both_eyes_center_to_left
						  }, NULL);
	animation_set_handlers((Animation*) s_left_eye_center_to_left, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) schedule_both_eyes_left_to_right
						  }, NULL);
	animation_set_handlers((Animation*) s_left_eye_left_to_home, (AnimationHandlers)
						   {
							   .stopped = (AnimationStoppedHandler) schedule_both_eyes_right_to_center
						   }, NULL);
	animation_set_handlers((Animation*) s_left_eye_home_to_center, (AnimationHandlers)
						  {
							  .stopped = (AnimationStoppedHandler) blink_finished_close
						  }, NULL);
	animation_schedule((Animation*) s_right_eyelid_close);
	animation_schedule((Animation*) s_left_eyelid_close);
}

static void update_time()
{
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
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	update_time();
	
	if (tick_time->tm_sec < 55)
	{
		if (tick_time->tm_min % 2 == 0)
			schedule_look_left_then_right();
		else
			schedule_look_right_then_left();
    }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
	Tuple *t = dict_read_first(iterator);
	
	while (t != NULL)
	{
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Tuple value %d", (int)t->value->int32);
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

static void inbox_dropped_callback(AppMessageResult reason, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox sent success!");
}

void main_window_load(Window *window)
{
	s_right_eye_home_frame = GRect(85,78,30,30);
	s_left_eye_home_frame = GRect(29,78,30,30);
	s_right_eye_center_frame = GRect(93,78,30,30);
	s_left_eye_center_frame = GRect(21,78,30,30);
	s_right_eye_look_right_frame = GRect(102,78,30,30);
	s_left_eye_look_left_frame = GRect(12,78,30,30);
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
	
	s_bluetooth_error_text_layer = text_layer_create(GRect(0,50,144,16));
	text_layer_set_text_alignment(s_bluetooth_error_text_layer, GTextAlignmentCenter);
	text_layer_set_font(s_bluetooth_error_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_background_color(s_bluetooth_error_text_layer, GColorClear);
	text_layer_set_text_color(s_bluetooth_error_text_layer, GColorWhite);
	text_layer_set_text(s_bluetooth_error_text_layer, "BTERR");
	layer_set_hidden(text_layer_get_layer(s_bluetooth_error_text_layer), layer_get_hidden(bitmap_layer_get_layer(s_bluetooth_error_layer)));
	
	s_battery_layer = bitmap_layer_create(GRect(10,7,25,15));
	layer_set_update_proc(bitmap_layer_get_layer(s_battery_layer) , battery_layer_update_proc);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Checking before setting hidden %d", !s_show_battery);
	layer_set_hidden(bitmap_layer_get_layer(s_battery_layer), !s_show_battery);
	
	s_toothless_layer = bitmap_layer_create(GRect(0,24,144,144));
	bitmap_layer_set_alignment(s_toothless_layer, GAlignTop);
	s_right_eyelid = bitmap_layer_create(s_right_eyelid_open_position);
	s_left_eyelid = bitmap_layer_create(s_left_eyelid_open_position);
	s_toothless_left_eye_layer = bitmap_layer_create(s_left_eye_home_frame);
	s_toothless_right_eye_layer = bitmap_layer_create(s_right_eye_home_frame);
	
	bitmap_layer_set_bitmap(s_toothless_layer, s_toothless_bitmap);
	bitmap_layer_set_bitmap(s_toothless_left_eye_layer, s_toothless_left_eye);
	bitmap_layer_set_bitmap(s_toothless_right_eye_layer, s_toothless_right_eye);
	
	bitmap_layer_set_compositing_mode(s_toothless_left_eye_layer, GCompOpAnd);
	bitmap_layer_set_compositing_mode(s_toothless_right_eye_layer, GCompOpAnd);
	bitmap_layer_set_background_color(s_right_eyelid, GColorBlack);
	bitmap_layer_set_background_color(s_left_eyelid, GColorBlack);
	
	s_right_eye_home_to_right = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_home_frame, &s_right_eye_look_right_frame);
	s_left_eye_home_to_left = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_home_frame, &s_left_eye_look_left_frame);
	s_right_eye_home_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_home_frame, &s_right_eye_center_frame);
	s_left_eye_home_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_home_frame, &s_left_eye_center_frame);
	s_right_eye_right_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_look_right_frame, &s_right_eye_center_frame);
	s_left_eye_left_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_look_left_frame, &s_left_eye_center_frame);
	s_right_eye_center_to_right = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_center_frame, &s_right_eye_look_right_frame);
	s_left_eye_center_to_left = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_center_frame, &s_left_eye_look_left_frame);
	s_right_eye_center_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_center_frame, &s_right_eye_home_frame);
	s_left_eye_center_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_center_frame, &s_left_eye_home_frame);
	s_right_eye_right_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_look_right_frame, &s_right_eye_home_frame);
	s_left_eye_left_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_look_left_frame, &s_left_eye_home_frame);
	s_right_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_left_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	
	s_date_layer = text_layer_create(GRect(0,105,144,38));
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorWhite);
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	
	s_time_layer = text_layer_create(GRect(0,0,144,30));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	
	update_time();
	
	layer_add_child(bitmap_layer_get_layer(s_toothless_layer), bitmap_layer_get_layer(s_toothless_left_eye_layer));
	layer_add_child(bitmap_layer_get_layer(s_toothless_layer), bitmap_layer_get_layer(s_toothless_right_eye_layer));
	layer_add_child(bitmap_layer_get_layer(s_toothless_layer), bitmap_layer_get_layer(s_right_eyelid));
	layer_add_child(bitmap_layer_get_layer(s_toothless_layer), bitmap_layer_get_layer(s_left_eyelid));
	layer_add_child(bitmap_layer_get_layer(s_toothless_layer), text_layer_get_layer(s_date_layer));
	
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_battery_layer));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_toothless_layer));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bluetooth_error_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bluetooth_error_text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	
	animation_set_delay((Animation*) s_right_eye_home_to_right, 500);
	animation_set_delay((Animation*) s_left_eye_home_to_left, 500);
	animation_set_delay((Animation*) s_right_eye_home_to_center, 500);
	animation_set_delay((Animation*) s_left_eye_home_to_center, 500);
	animation_set_delay((Animation*) s_right_eye_right_to_center, 500);
	animation_set_delay((Animation*) s_left_eye_left_to_center, 500);
	animation_set_delay((Animation*) s_left_eye_home_to_left, 500);
	animation_set_delay((Animation*) s_right_eye_center_to_right, 500);
	animation_set_delay((Animation*) s_left_eye_center_to_left, 500);
	animation_set_delay((Animation*) s_right_eye_center_to_home, 500);
	animation_set_delay((Animation*) s_left_eye_center_to_home, 500);
	animation_set_delay((Animation*) s_right_eye_right_to_home, 500);
	animation_set_delay((Animation*) s_left_eye_left_to_home, 500);
	animation_set_delay((Animation*) s_right_eyelid_open, 100);
	animation_set_delay((Animation*) s_right_eyelid_close, 500);
	animation_set_delay((Animation*) s_left_eyelid_open, 100);
	animation_set_delay((Animation*) s_left_eyelid_close, 500);
	
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	battery_state_service_subscribe((BatteryStateHandler) battery_handler);
	bluetooth_connection_service_subscribe((BluetoothConnectionHandler) bluetooth_handler);
}

static void read_storage()
{
	if (persist_exists(KEY_BATTERY))
		s_show_battery = persist_read_bool(KEY_BATTERY);
	else
		s_show_battery = true;
	if (persist_exists(KEY_DATE_FORMAT))
		s_date_format = persist_read_bool(KEY_DATE_FORMAT);
	else
		s_date_format = true;
}

static void store_storage()
{
	persist_write_bool(KEY_BATTERY, s_show_battery);
	persist_write_bool(KEY_DATE_FORMAT, s_date_format);
}

void main_window_unload(Window *window)
{
	gbitmap_destroy(s_toothless_bitmap);
	gbitmap_destroy(s_toothless_left_eye);
	gbitmap_destroy(s_toothless_right_eye);
	gbitmap_destroy(s_bluetooth_error);
	
	bitmap_layer_destroy(s_toothless_left_eye_layer);
	bitmap_layer_destroy(s_toothless_right_eye_layer);
	bitmap_layer_destroy(s_right_eyelid);
	bitmap_layer_destroy(s_left_eyelid);
	bitmap_layer_destroy(s_toothless_layer);
	bitmap_layer_destroy(s_battery_layer);
	bitmap_layer_destroy(s_bluetooth_error_layer);
	
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_bluetooth_error_text_layer);
	
	property_animation_destroy(s_right_eye_home_to_right);
	property_animation_destroy(s_left_eye_home_to_left);
	property_animation_destroy(s_right_eye_home_to_center);
	property_animation_destroy(s_left_eye_home_to_center);
	property_animation_destroy(s_right_eye_right_to_center);
	property_animation_destroy(s_left_eye_left_to_center);
	property_animation_destroy(s_left_eye_left_to_home);
	property_animation_destroy(s_right_eye_right_to_home);
	property_animation_destroy(s_right_eye_center_to_home);
	property_animation_destroy(s_left_eye_center_to_home);
	property_animation_destroy(s_right_eye_center_to_right);
	property_animation_destroy(s_left_eye_center_to_left);
	property_animation_destroy(s_right_eyelid_open);
	property_animation_destroy(s_right_eyelid_close);
	property_animation_destroy(s_left_eyelid_open);
	property_animation_destroy(s_left_eyelid_close);
	
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
}

void handle_init(void)
{
	read_storage();
  	s_main_window = window_create();
	window_set_background_color(s_main_window, GColorBlack);
	window_set_window_handlers(s_main_window, (WindowHandlers)
	{
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

void handle_deinit(void)
{
	window_destroy(s_main_window);
	store_storage();
}

int main(void)
{
  	handle_init();
  	app_event_loop();
  	handle_deinit();
}
