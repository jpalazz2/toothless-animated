#include <animation.h>
#include <pebble.h>

void create_animations(void) {
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
	s_right_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_close2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_right_eyelid_open2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_left_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);

	s_all_animations = malloc(sizeof(PropertyAnimation*)*NUM_ANIMATIONS);
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
	s_all_animations[12] = s_right_eyelid_close;
	s_all_animations[13] = s_left_eyelid_close;
	s_all_animations[14] = s_right_eyelid_open;
	s_all_animations[15] = s_left_eyelid_open;
	s_all_animations[16] = s_right_eyelid_close2;
	s_all_animations[17] = s_right_eyelid_open2;

	for (int i = 0; i < NUM_ANIMATIONS; i++)
	{
		animation_set_delay(property_animation_get_animation(s_all_animations[i]), 500);
		animation_set_duration(property_animation_get_animation(s_all_animations[i]), 200);
	}
}

void destroy_animations(void) {
	for (int i = 0; i < NUM_ANIMATIONS; i++) { 
		property_animation_destroy(s_all_animations[i]);
	}
	free(s_all_animations);
}
