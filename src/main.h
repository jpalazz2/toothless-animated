#ifndef MAIN_H_
#define MAIN_H_

#include <pebble.h>

#define KEY_BATTERY 0
#define KEY_DATE_FORMAT 1
#define KEY_TIME_FORMAT 2
	
#define DEBUG 0

GRect s_right_eye_home_frame;
GRect s_left_eye_home_frame;
GRect s_right_eye_center_frame;
GRect s_left_eye_center_frame;
GRect s_right_eye_center_frame;
GRect s_left_eye_left_frame;
GRect s_right_eye_right_frame;
GRect s_right_eyelid_open_position;
GRect s_right_eyelid_close_position;
GRect s_left_eyelid_open_position;
GRect s_left_eyelid_close_position;

BitmapLayer *s_face_layer;
BitmapLayer *s_toothless_layer;
BitmapLayer *s_toothless_left_eye_layer;
BitmapLayer *s_toothless_right_eye_layer;
BitmapLayer *s_right_eyelid;
BitmapLayer *s_left_eyelid;
BitmapLayer *s_left_eye_color;
BitmapLayer *s_right_eye_color;
BitmapLayer *s_battery_layer;
BitmapLayer *s_bluetooth_error_layer;


static Window *s_main_window;
static GBitmap *s_toothless_bitmap;
static GBitmap *s_toothless_left_eye;
static GBitmap *s_toothless_right_eye;
static GBitmap *s_bluetooth_error;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

bool s_show_battery;
bool s_date_format;
bool s_time_format;
bool blink_type; //0 is left 1 is right

static int s_battery_percentage;

void create_animations(void);

void destroy_animations(void);

void rebuild_left_animations(void);

void rebuild_right_animations(void);

Animation* get_anim(PropertyAnimation *a);

#endif

