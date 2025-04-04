#include "control_neopixel_measure.h"
#include "ansi_abstraction_layer.h"
#include "loop.h"
#include "utils.h"
#include "visual_output.h"

extern global_vars_t global_var;

void control_neopixel_measure_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':

            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'm':
        case 'M':
            if (global_var.device_state == DEV_STATE_ADV_NEOPIXEL_READ) {
                dev_mode_change_mode(DEV_STATE_ADV_NEOPIXEL_WRITE);
            } else {
                dev_mode_change_mode(DEV_STATE_ADV_NEOPIXEL_READ);
            }
            break;
        case 'e':
        case 'E': {
            if (!global_var.adv_neopixel_measure->edit_rgb[1] &&
                !global_var.adv_neopixel_measure->edit_rgb[2]) {
                global_var.adv_neopixel_measure->edit_rgb[0] =
                    !global_var.adv_neopixel_measure->edit_rgb[0];
            }
            dev_mode_request_frontend_change();
            break;
        }
        case 't':
        case 'T': {
            if (!global_var.adv_neopixel_measure->edit_rgb[0] &&
                !global_var.adv_neopixel_measure->edit_rgb[2]) {
                global_var.adv_neopixel_measure->edit_rgb[1] =
                    !global_var.adv_neopixel_measure->edit_rgb[1];
            }
            dev_mode_request_frontend_change();
            break;
        }

        case 's':
        case 'S': {
            if (!global_var.adv_neopixel_measure->edit_rgb[0] &&
                !global_var.adv_neopixel_measure->edit_rgb[1] &&
                !global_var.adv_neopixel_measure->edit_rgb[2]) {
                neopixel_color_t colors = {
                    global_var.adv_neopixel_measure->rgb_send[0],
                    global_var.adv_neopixel_measure->rgb_send[1],
                    global_var.adv_neopixel_measure->rgb_send[2]};
                neopixel_set_color(global_var.visual_output, &colors);
            }
            break;
        }
        case 'y':
        case 'Y': {
            if (!global_var.adv_neopixel_measure->edit_rgb[0] &&
                !global_var.adv_neopixel_measure->edit_rgb[1]) {
                global_var.adv_neopixel_measure->edit_rgb[2] =
                    !global_var.adv_neopixel_measure->edit_rgb[2];
            }
            dev_mode_request_frontend_change();
            break;
        }
        case 'n':
        case 'N':
            neopixel_send_color(global_var.visual_output, NEOPIXEL_BLUE);

            break;
        case 'b':
        case 'B': {
            neopixel_color_t p = {192, 255, 45};
            neopixel_set_color(global_var.visual_output, &p);
            break;
        }

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0': {
            uint8_t index;
            if (global_var.adv_neopixel_measure->edit_rgb[0]) {
                index = 0;
            } else if (global_var.adv_neopixel_measure->edit_rgb[1]) {
                index = 1;
            } else if (global_var.adv_neopixel_measure->edit_rgb[2]) {
                index = 2;
            } else {
                break;
            }
            uint32_t number = global_var.adv_neopixel_measure->rgb_send[index];

            number *= 10;
            number += (uint32_t)cdtoi((char)received_char);

            if (number > 255) {
                number = 255;
            }
            global_var.adv_neopixel_measure->rgb_send[index] = number;
            dev_mode_request_frontend_change();
            break;
        }
        case 'x':
        case 'X': {
            uint8_t index;
            if (global_var.adv_neopixel_measure->edit_rgb[0]) {
                index = 0;
            } else if (global_var.adv_neopixel_measure->edit_rgb[1]) {
                index = 1;
            } else if (global_var.adv_neopixel_measure->edit_rgb[2]) {
                index = 2;
            } else {
                break;
            }
            uint32_t number = global_var.adv_neopixel_measure->rgb_send[index];

            number /= 10;
            global_var.adv_neopixel_measure->rgb_send[index] = number;

            dev_mode_request_frontend_change();
            break;
        }

        default:
            break;
    }
}
