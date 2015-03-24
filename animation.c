#include "animation.h"

static Animation* get_anim(PropertyAnimation *a) {
	return property_animation_get_animation(a);
}

void initialize_animations() {
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
	s_left_eyelid_close_stop = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_close_start = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
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
	s_all_animations[13] = s_left_eyelid_close_start;
	s_all_animations[14] = s_right_eyelid_open;
	s_all_animations[15] = s_left_eyelid_open;
	s_all_animations[16] = s_right_eyelid_close_stop;
	s_all_animations[17] = s_left_eyelid_close_stop;

	for (int i = 0; i < NUM_ANIMATIONS; i++)
	{
		animation_set_delay(property_animation_get_animation(s_all_animations[i]), 500);
		animation_set_duration(property_animation_get_animation(s_all_animations[i]), 200);
	}

	#ifdef PBL_COLOR
		s_blink_start = animation_spawn_create(get_anim(s_right_eyelid_close_start), get_anim(s_left_eyelid_close), NULL);
		s_look_right_half = animation_spawn_create(get_anim(s_left_eye_center_to_home), get_anim(s_right_eye_center_to_right), NULL);
		s_look_left_full = animation_spawn_create(get_anim(s_left_eye_home_to_left), get_anim(s_right_eye_right_to_home), NULL);
		s_return_home_left = animation_spawn_create(get_anim(s_left_eye_left_to_center), get_anim(s_right_eye_home_to_center), NULL);
		s_blink_finish = animation_spawn_create(get_anim(s_right_eyelid_close_stop), get_anim(s_right_eyelid, NULL);
		s_blink_open_start = animation_spawn_create(get_anim(s_left_eyelid_open), get_anim(s_right_eyelid_open), NULL);
		s_blink_open_stop = animation_clone(s_blink_open_start);
		animation_set_delay(s_blink_open_stop, 500);
		animation_set_duration(s_blink_open_stop, 200);
		s_look_right_animation = animation_sequence_create(s_blink_start, s_blink_open_start, s_look_right_half, s_look_left_full, s_return_home_left, s_blink_finish, NULL);
		animation_schedule(s_look_right_animation);
	#endif
}

void deinitialize_animations() {
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
}
