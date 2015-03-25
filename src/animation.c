#include <pebble.h>
#include <include.h>

PropertyAnimation *s_all_animations[NUM_ANIMATIONS];
PropertyAnimation *s_right_animations[NUM_SIDE_ANIMATIONS];
PropertyAnimation *s_left_animations[NUM_SIDE_ANIMATIONS];
PropertyAnimation *s_right_eye_home_to_right;
PropertyAnimation *s_left_eye_home_to_left;
PropertyAnimation *s_right_eye_home_to_center;
PropertyAnimation *s_left_eye_home_to_center;
PropertyAnimation *s_right_eye_right_to_center;
PropertyAnimation *s_left_eye_left_to_center;
PropertyAnimation *s_right_eye_center_to_right;
PropertyAnimation *s_left_eye_center_to_left;
PropertyAnimation *s_right_eye_center_to_home;
PropertyAnimation *s_left_eye_center_to_home;
PropertyAnimation *s_right_eye_right_to_home;
PropertyAnimation *s_left_eye_left_to_home;
PropertyAnimation *s_right_eyelid_close;
PropertyAnimation *s_right_eyelid_close2;
PropertyAnimation *s_right_eyelid_open;
PropertyAnimation *s_right_eyelid_open2;
PropertyAnimation *s_left_eyelid_close;
PropertyAnimation *s_left_eyelid_close2;
PropertyAnimation *s_left_eyelid_open;
PropertyAnimation *s_left_eyelid_open2;

void center_eyes(Animation *animation, bool finished, void *context);

void home_eyes(Animation *animation, bool finished, void *context);

Animation* get_anim(PropertyAnimation *a) {
	return property_animation_get_animation(a);
}


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
	s_left_eyelid_close2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	s_left_eyelid_open2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);

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
	s_all_animations[18] = s_left_eyelid_open2;
	s_all_animations[19] = s_left_eyelid_close2;

	for (int i = 0; i < NUM_ANIMATIONS; i++)
	{
		animation_set_delay(property_animation_get_animation(s_all_animations[i]), 500);
		animation_set_duration(property_animation_get_animation(s_all_animations[i]), 200);
	}

	animation_set_delay(property_animation_get_animation(s_right_eyelid_open), 250);
	animation_set_delay(property_animation_get_animation(s_right_eyelid_open2), 250);
	animation_set_delay(property_animation_get_animation(s_left_eyelid_open), 250);
	animation_set_delay(property_animation_get_animation(s_left_eyelid_open2), 250);

	animation_set_handlers(get_anim(s_right_eyelid_close), (AnimationHandlers) {
			.stopped = center_eyes
			}, NULL);
	animation_set_handlers(get_anim(s_right_eyelid_close2), (AnimationHandlers) {
			.stopped = home_eyes
			}, NULL);

	s_right_animations[0] = s_right_eyelid_close;
	s_right_animations[1] = s_left_eyelid_close;
	s_right_animations[2] = s_right_eyelid_open;
	s_right_animations[3] = s_left_eyelid_open;
	s_right_animations[4] = s_right_eye_center_to_right;
	s_right_animations[5] = s_left_eye_center_to_home;
	s_right_animations[6] = s_right_eye_right_to_home;
	s_right_animations[7] = s_left_eye_home_to_left;
	s_right_animations[8] = s_right_eye_home_to_center;
	s_right_animations[9] = s_left_eye_left_to_center;
	s_right_animations[10] = s_right_eyelid_close2;
	s_right_animations[11] = s_left_eyelid_close2;
	s_right_animations[12] = s_left_eyelid_open2;
	s_right_animations[13] = s_right_eyelid_open2;

	s_left_animations[0] = s_right_eyelid_close;
	s_left_animations[1] = s_left_eyelid_close;
	s_left_animations[2] = s_right_eyelid_open;
	s_left_animations[3] = s_left_eyelid_open;
	s_left_animations[4] = s_right_eye_center_to_home;
	s_left_animations[5] = s_left_eye_center_to_left;
	s_left_animations[6] = s_right_eye_home_to_right;
	s_left_animations[7] = s_left_eye_left_to_home;
	s_left_animations[8] = s_right_eye_right_to_center;
	s_left_animations[9] = s_left_eye_home_to_center;
	s_left_animations[10] = s_right_eyelid_close2;
	s_left_animations[11] = s_left_eyelid_close2;
	s_left_animations[12] = s_left_eyelid_open2;
	s_left_animations[13] = s_right_eyelid_open2;
}

void destroy_animations(void) {
	animation_unschedule_all();
	for (int i = 0; i < NUM_ANIMATIONS; i++) { 
		property_animation_destroy(s_all_animations[i]);
	}
}

void rebuild_left_animations(void) {
	s_right_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_close2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_right_eyelid_open2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_left_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_close2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	s_left_eyelid_open2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	s_right_eye_center_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_center_frame, &s_right_eye_home_frame);
	s_left_eye_center_to_left = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_center_frame, &s_left_eye_left_frame);
	s_right_eye_home_to_right = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_home_frame, &s_right_eye_right_frame);
	s_left_eye_left_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_left_frame, &s_left_eye_home_frame);
	s_right_eye_right_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_right_frame, &s_right_eye_center_frame);
	s_left_eye_home_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_home_frame, &s_left_eye_center_frame);

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
	s_all_animations[18] = s_left_eyelid_open2;
	s_all_animations[19] = s_left_eyelid_close2;

	s_right_animations[0] = s_right_eyelid_close;
	s_right_animations[1] = s_left_eyelid_close;
	s_right_animations[2] = s_right_eyelid_open;
	s_right_animations[3] = s_left_eyelid_open;
	s_right_animations[4] = s_right_eye_center_to_right;
	s_right_animations[5] = s_left_eye_center_to_home;
	s_right_animations[6] = s_right_eye_right_to_home;
	s_right_animations[7] = s_left_eye_home_to_left;
	s_right_animations[8] = s_right_eye_home_to_center;
	s_right_animations[9] = s_left_eye_left_to_center;
	s_right_animations[10] = s_right_eyelid_close2;
	s_right_animations[11] = s_left_eyelid_close2;
	s_right_animations[12] = s_left_eyelid_open2;
	s_right_animations[13] = s_right_eyelid_open2;

	s_left_animations[0] = s_right_eyelid_close;
	s_left_animations[1] = s_left_eyelid_close;
	s_left_animations[2] = s_right_eyelid_open;
	s_left_animations[3] = s_left_eyelid_open;
	s_left_animations[4] = s_right_eye_center_to_home;
	s_left_animations[5] = s_left_eye_center_to_left;
	s_left_animations[6] = s_right_eye_home_to_right;
	s_left_animations[7] = s_left_eye_left_to_home;
	s_left_animations[8] = s_right_eye_right_to_center;
	s_left_animations[9] = s_left_eye_home_to_center;
	s_left_animations[10] = s_right_eyelid_close2;
	s_left_animations[11] = s_left_eyelid_close2;
	s_left_animations[12] = s_left_eyelid_open2;
	s_left_animations[13] = s_right_eyelid_open2;

	for (int i = 0; i < NUM_SIDE_ANIMATIONS; i++) {
		animation_set_delay(property_animation_get_animation(s_left_animations[i]), 500);
		animation_set_duration(property_animation_get_animation(s_left_animations[i]), 200);
	}

	animation_set_delay(property_animation_get_animation(s_right_eyelid_open), 250);
	animation_set_delay(property_animation_get_animation(s_right_eyelid_open2), 250);
	animation_set_delay(property_animation_get_animation(s_left_eyelid_open), 250);
	animation_set_delay(property_animation_get_animation(s_left_eyelid_open2), 250);

	animation_set_handlers(get_anim(s_right_eyelid_close), (AnimationHandlers) {
			.stopped = center_eyes
			}, NULL);
	animation_set_handlers(get_anim(s_right_eyelid_close2), (AnimationHandlers) {
			.stopped = home_eyes
			}, NULL);
}

void rebuild_right_animations(void) {
	s_right_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_close2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_open_position, &s_right_eyelid_close_position);
	s_right_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_right_eyelid_open2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_right_eyelid), &s_right_eyelid_close_position, &s_right_eyelid_open_position);
	s_left_eyelid_close = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_close2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_open_position, &s_left_eyelid_close_position);
	s_left_eyelid_open = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	s_left_eyelid_open2 = property_animation_create_layer_frame(bitmap_layer_get_layer(s_left_eyelid), &s_left_eyelid_close_position, &s_left_eyelid_open_position);
	s_right_eye_center_to_right = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_center_frame, &s_right_eye_right_frame);
	s_left_eye_center_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_center_frame, &s_left_eye_home_frame);
	s_right_eye_right_to_home = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_right_frame, &s_right_eye_home_frame);
	s_left_eye_home_to_left = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_home_frame, &s_left_eye_left_frame);
	s_right_eye_home_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_right_eye_layer), &s_right_eye_home_frame, &s_right_eye_center_frame);
	s_left_eye_left_to_center = property_animation_create_layer_frame(bitmap_layer_get_layer(s_toothless_left_eye_layer), &s_left_eye_left_frame, &s_left_eye_center_frame);

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
	s_all_animations[18] = s_left_eyelid_open2;
	s_all_animations[19] = s_left_eyelid_close2;

	s_right_animations[0] = s_right_eyelid_close;
	s_right_animations[1] = s_left_eyelid_close;
	s_right_animations[2] = s_right_eyelid_open;
	s_right_animations[3] = s_left_eyelid_open;
	s_right_animations[4] = s_right_eye_center_to_right;
	s_right_animations[5] = s_left_eye_center_to_home;
	s_right_animations[6] = s_right_eye_right_to_home;
	s_right_animations[7] = s_left_eye_home_to_left;
	s_right_animations[8] = s_right_eye_home_to_center;
	s_right_animations[9] = s_left_eye_left_to_center;
	s_right_animations[10] = s_right_eyelid_close2;
	s_right_animations[11] = s_left_eyelid_close2;
	s_right_animations[12] = s_left_eyelid_open2;
	s_right_animations[13] = s_right_eyelid_open2;

	s_left_animations[0] = s_right_eyelid_close;
	s_left_animations[1] = s_left_eyelid_close;
	s_left_animations[2] = s_right_eyelid_open;
	s_left_animations[3] = s_left_eyelid_open;
	s_left_animations[4] = s_right_eye_center_to_home;
	s_left_animations[5] = s_left_eye_center_to_left;
	s_left_animations[6] = s_right_eye_home_to_right;
	s_left_animations[7] = s_left_eye_left_to_home;
	s_left_animations[8] = s_right_eye_right_to_center;
	s_left_animations[9] = s_left_eye_home_to_center;
	s_left_animations[10] = s_right_eyelid_close2;
	s_left_animations[11] = s_left_eyelid_close2;
	s_left_animations[12] = s_left_eyelid_open2;
	s_left_animations[13] = s_right_eyelid_open2;

	for (int i = 0; i < NUM_SIDE_ANIMATIONS; i++) {
		animation_set_delay(property_animation_get_animation(s_right_animations[i]), 500);
		animation_set_duration(property_animation_get_animation(s_right_animations[i]), 200);
	}

	animation_set_delay(property_animation_get_animation(s_right_eyelid_open), 250);
	animation_set_delay(property_animation_get_animation(s_right_eyelid_open2), 250);
	animation_set_delay(property_animation_get_animation(s_left_eyelid_open), 250);
	animation_set_delay(property_animation_get_animation(s_left_eyelid_open2), 250);

	animation_set_handlers(get_anim(s_right_eyelid_close), (AnimationHandlers) {
			.stopped = center_eyes
			}, NULL);
	animation_set_handlers(get_anim(s_right_eyelid_close2), (AnimationHandlers) {
			.stopped = home_eyes
			}, NULL);
}
