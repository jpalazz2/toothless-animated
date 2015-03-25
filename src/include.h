#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <pebble.h>

#define NUM_ANIMATIONS 20 
#define NUM_SIDE_ANIMATIONS 14

extern GRect s_right_eye_home_frame;
extern GRect s_left_eye_home_frame;
extern GRect s_right_eye_center_frame;
extern GRect s_left_eye_center_frame;
extern GRect s_right_eye_center_frame;
extern GRect s_left_eye_left_frame;
extern GRect s_right_eye_right_frame;
extern GRect s_right_eyelid_open_position;
extern GRect s_right_eyelid_close_position;
extern GRect s_left_eyelid_open_position;
extern GRect s_left_eyelid_close_position;

extern BitmapLayer *s_face_layer;
extern BitmapLayer *s_toothless_layer;
extern BitmapLayer *s_toothless_left_eye_layer;
extern BitmapLayer *s_toothless_right_eye_layer;
extern BitmapLayer *s_right_eyelid;
extern BitmapLayer *s_left_eyelid;
extern BitmapLayer *s_left_eye_color;
extern BitmapLayer *s_right_eye_color;
extern BitmapLayer *s_battery_layer;
extern BitmapLayer *s_bluetooth_error_layer;

extern PropertyAnimation *s_right_eye_home_to_right;
extern PropertyAnimation *s_left_eye_home_to_left;
extern PropertyAnimation *s_right_eye_home_to_center;
extern PropertyAnimation *s_left_eye_home_to_center;
extern PropertyAnimation *s_right_eye_right_to_center;
extern PropertyAnimation *s_left_eye_left_to_center;
extern PropertyAnimation *s_right_eye_center_to_right;
extern PropertyAnimation *s_left_eye_center_to_left;
extern PropertyAnimation *s_right_eye_center_to_home;
extern PropertyAnimation *s_left_eye_center_to_home;
extern PropertyAnimation *s_right_eye_right_to_home;
extern PropertyAnimation *s_left_eye_left_to_home;
extern PropertyAnimation *s_right_eyelid_close;
extern PropertyAnimation *s_right_eyelid_close2;
extern PropertyAnimation *s_right_eyelid_open;
extern PropertyAnimation *s_right_eyelid_open2;
extern PropertyAnimation *s_left_eyelid_close;
extern PropertyAnimation *s_left_eyelid_close2;
extern PropertyAnimation *s_left_eyelid_open;
extern PropertyAnimation *s_left_eyelid_open2;

#endif
