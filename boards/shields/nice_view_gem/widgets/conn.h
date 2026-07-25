#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>
#include "util.h"

struct conn_status_state {
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
};

void draw_conn_status(lv_obj_t *canvas, const struct status_state *state);