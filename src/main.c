#include <pebble.h>
#include <include.h>
#include <main.h>
	
static void reset_screen() {
	animation_unschedule_all();
	layer_set_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), s_right_eye_home_frame);
	layer_set_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), s_left_eye_home_frame);
	layer_set_frame(bitmap_layer_get_layer(s_left_eyelid), s_left_eye_home_frame);
	layer_set_frame(bitmap_layer_get_layer(s_right_eyelid), s_right_eye_home_frame);
}

void  rebuild_animations(void *data) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Regenerating animations");
	if (*((bool*)data)) 
		rebuild_right_animations();
	else
		rebuild_left_animations();
}

void blink_finished(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_left_eyelid_close2));
		animation_schedule(get_anim(s_right_eyelid_close2));
	} else {
		reset_screen();
	}
#ifdef PBL_COLOR
	app_timer_register(2000, (AppTimerCallback) rebuild_animations, context);
#endif
}

static void look_right_half(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_right_eye_home_to_center));
		animation_schedule(get_anim(s_left_eye_left_to_center));
	} else {
		reset_screen();
	}
}

static void look_left_full(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_right_eye_right_to_home));
		animation_schedule(get_anim(s_left_eye_home_to_left));
	} else {
		reset_screen();
	}
}

static void look_right_start(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_right_eye_center_to_right));
		animation_schedule(get_anim(s_left_eye_center_to_home));
	} else {
		reset_screen();
	}
}

static void schedule_look_right_then_left() {
	blink_type = 1;
	animation_set_handlers(get_anim(s_right_eyelid_open), (AnimationHandlers) {
				.stopped = look_right_start
			}, NULL);
	animation_set_handlers(get_anim(s_right_eye_center_to_right), (AnimationHandlers) {
				.stopped = look_left_full
			}, NULL);
	animation_set_handlers(get_anim(s_right_eye_right_to_home), (AnimationHandlers) {
				.stopped = look_right_half
			}, NULL);
	animation_set_handlers(get_anim(s_right_eye_home_to_center), (AnimationHandlers) {
				.stopped = blink_finished
			}, (void*) &blink_type);
	animation_schedule(get_anim(s_right_eyelid_close));
	animation_schedule(get_anim(s_left_eyelid_close));
}

void look_left_half(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_left_eye_home_to_center));
		animation_schedule(get_anim(s_right_eye_right_to_center));
	} else {
		reset_screen();
	}
}

void look_right_full(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_left_eye_left_to_home));
		animation_schedule(get_anim(s_right_eye_home_to_right));
	} else {
		reset_screen();
	}
}

void look_left_start(Animation *animation, bool finished, void *context) {
	if (finished) {
		animation_schedule(get_anim(s_left_eye_center_to_left));
		animation_schedule(get_anim(s_right_eye_center_to_home));
	} else {
		reset_screen();
	}
}

static void schedule_look_left_then_right() {
	blink_type = 0;
	animation_set_handlers(get_anim(s_right_eyelid_open), (AnimationHandlers) {
				.stopped = look_left_start
			}, NULL);
	animation_set_handlers(get_anim(s_right_eye_center_to_home), (AnimationHandlers) {
				.stopped = look_right_full
			}, NULL);
	animation_set_handlers(get_anim(s_right_eye_home_to_right), (AnimationHandlers) {
				.stopped = look_left_half
			}, NULL);
	animation_set_handlers(get_anim(s_right_eye_right_to_center), (AnimationHandlers) {
				.stopped = blink_finished
			}, (void*) &blink_type);
	animation_schedule(get_anim(s_right_eyelid_close));
	animation_schedule(get_anim(s_left_eyelid_close));
}

static void battery_handler(BatteryStateHandler handler) {
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery layer updating...");
	s_battery_percentage = battery_state_service_peek().charge_percent;
	layer_mark_dirty((Layer*) s_battery_layer);
}

static void battery_layer_update_proc(Layer *layer, GContext *ctx) {
#ifdef PBL_COLOR
	if (s_battery_percentage <= 20) {
		graphics_context_set_fill_color(ctx, GColorRed);
		graphics_context_set_stroke_color(ctx, GColorRed);
	} else {
		graphics_context_set_fill_color(ctx, GColorWhite);
		graphics_context_set_stroke_color(ctx, GColorWhite);
	}
#else
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_context_set_stroke_color(ctx, GColorWhite);
#endif
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
	APP_LOG(APP_LOG_LEVEL_DEBUG, "test log");
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

	if (tick_time->tm_sec <= 55)
	{
		if (tick_time->tm_min % 2 == 0)
			schedule_look_left_then_right();
		else
			schedule_look_right_then_left();
	}
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

void center_eyes(Animation *animation, bool finished, void *context) {
	if (finished) {
		layer_set_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), s_right_eye_center_frame);
		layer_set_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), s_left_eye_center_frame);
		animation_schedule(get_anim(s_right_eyelid_open));
		animation_schedule(get_anim(s_left_eyelid_open));
	} else {
		reset_screen();
	}
}

void home_eyes(Animation *animation, bool finished, void *context) {
	if (finished) {
		layer_set_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), s_right_eye_home_frame);
		layer_set_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), s_left_eye_home_frame);
		animation_schedule(get_anim(s_right_eyelid_open2));
		animation_schedule(get_anim(s_left_eyelid_open2));
	} else {
		reset_screen();
	}
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
	
#ifdef PBL_COLOR
	s_toothless_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TOOTHLESS_FACE_COLOR);
#else
	s_toothless_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TOOTHLESS_FACE);
#endif
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
#ifndef PBL_COLOR
	bitmap_layer_set_compositing_mode(s_toothless_layer, GCompOpAnd);
	bitmap_layer_set_background_color(s_toothless_layer, GColorClear);
#endif
	s_right_eyelid = bitmap_layer_create(s_right_eyelid_open_position);
	s_left_eyelid = bitmap_layer_create(s_left_eyelid_open_position);
	s_toothless_left_eye_layer = bitmap_layer_create(s_left_eye_home_frame);
	s_toothless_right_eye_layer = bitmap_layer_create(s_right_eye_home_frame);

	/*
	#ifdef PBL_COLOR
		s_left_eye_color = bitmap_layer_create(s_left_eyelid_close_position);
		s_right_eye_color = bitmap_layer_create(s_right_eyelid_close_position);
		bitmap_layer_set_background_color(s_left_eye_color, GColorBrightGreen);
		bitmap_layer_set_background_color(s_right_eye_color, GColorBrightGreen);
	#endif
	*/

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
	
	/*
	#ifdef PBL_COLOR 
		// palette[0] is not set to 0, should it be?
		// palette[1] is 255
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Bitmap format: palette[0] = %d, palette[1] = %d", gbitmap_get_palette(s_toothless_left_eye)[0].argb, gbitmap_get_palette(s_toothless_left_eye)[1].argb);
		layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_left_eye_color));
		layer_add_child(bitmap_layer_get_layer(s_face_layer), bitmap_layer_get_layer(s_right_eye_color));
	#endif
	*/

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

	create_animations();

	update_time();	

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
	bitmap_layer_destroy(s_toothless_layer);
	bitmap_layer_destroy(s_toothless_left_eye_layer);
	bitmap_layer_destroy(s_toothless_right_eye_layer);
	bitmap_layer_destroy(s_right_eyelid);
	bitmap_layer_destroy(s_left_eyelid);
	bitmap_layer_destroy(s_left_eye_color);
	bitmap_layer_destroy(s_right_eye_color);
	bitmap_layer_destroy(s_battery_layer);
	bitmap_layer_destroy(s_bluetooth_error_layer);
	
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_date_layer);

	destroy_animations();

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
