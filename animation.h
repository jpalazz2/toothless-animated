#ifndef ANIMATION_H_
#define ANIMATION_H_

#define NUM_ANIMATIONS 18

static PropertyAnimation *s_all_animations[NUM_ANIMATIONS];
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
static PropertyAnimation *s_left_eyelid_close_start;
static PropertyAnimation *s_left_eyelid_close_stop;
static PropertyAnimation *s_left_eyelid_open;

#ifdef PBL_COLOR
static Animation *s_look_left_full;
static Animation *s_look_right_full;
static Animation *s_look_left_half;
static Animation *s_look_right_half;
static Animation *s_return_home_right;
static Animation *s_return_home_left;
static Animation *s_blink_start;
static Animation *s_blink_finish;
static Animation *s_blink_open_start;
static Animation *s_blink_open_stop;
static Animation *s_look_right_animation;

// Clones
#endif

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

static Animation* get_anim(PropertyAnimation *a);

void initialize_animations();

void deinitialize_animations();

#endif
