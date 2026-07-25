#include <zephyr/kernel.h>
#include <stdio.h>

#include <lvgl.h>

#include "conn.h"
#include "util.h"
#include "../assets/custom_fonts.h"

static void draw_prf(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 1, 25, &label_left_dsc, "PRF");

    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

    if (state->selected_endpoint.transport == ZMK_TRANSPORT_BLE) {
        snprintf(text, sizeof(text), "%02d", state->active_profile_index + 1);
    } else {
        snprintf(text, sizeof(text), "--");
    }
    lv_canvas_draw_text(canvas, 26, 1, 42, &label_right_dsc, text);
}

static void draw_out(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 17, 25, &label_left_dsc, "OUT");

    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        snprintf(text, sizeof(text), "USB");
        break;
    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_connected) {
            snprintf(text, sizeof(text), "BLE");
        } else if (state->active_profile_bonded) {
            snprintf(text, sizeof(text), "BLE--");
        } else {
            snprintf(text, sizeof(text), "BLE??");
        }
        break;
    default:
        snprintf(text, sizeof(text), "NONE");
        break;
    }
    lv_canvas_draw_text(canvas, 26, 17, 42, &label_right_dsc, text);
}

static void draw_upt(lv_obj_t *canvas) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 33, 25, &label_left_dsc, "UPT");

    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    int64_t ms = k_uptime_get();
    char text[10] = {};

    int64_t seconds = ms / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;
    int64_t days = hours / 24;

    if (seconds < 60) {
        snprintf(text, sizeof(text), "%llds", seconds);
    } else if (minutes < 60) {
        snprintf(text, sizeof(text), "%lldm", minutes);
    } else if (hours < 24) {
        snprintf(text, sizeof(text), "%lldh%02lldm", hours, minutes % 60);
    } else if (days < 365) {
        snprintf(text, sizeof(text), "%lldd%02lldh", days, hours % 24);
    } else {
        snprintf(text, sizeof(text), "%lldy", days / 365);
    }
    lv_canvas_draw_text(canvas, 26, 33, 42, &label_right_dsc, text);
}

void draw_conn_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_prf(canvas, state);
    draw_out(canvas, state);
    draw_upt(canvas);
}