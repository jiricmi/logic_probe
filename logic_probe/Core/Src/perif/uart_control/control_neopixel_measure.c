#include "control_neopixel_measure.h"
#include "ansi_abstraction_layer.h"
#include "loop.h"
#include "utils.h"
#include "visual_output.h"

extern global_vars_t global_var;

void control_neopixel_measure_page(char received_char) {
    neopixel_measure_t* neopixel = global_var.adv_neopixel_measure;
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
        case 't':
        case 'T': {
            neopixel->edit = !neopixel->edit;
            if (neopixel->rgb_index == 3 && neopixel->rgb_send[3] == 0) {
                neopixel->rgb_send[3] = 1;
            }
            neopixel->rgb_index = 0;
            dev_mode_request_frontend_change();
            break;
        }
        case 'y':
        case 'Y':
            neopixel->rgb_index++;
            if (neopixel->rgb_index == 4) {
                neopixel->rgb_index = 0;
            }
            dev_mode_request_frontend_change();
            break;
        case 's':
        case 'S': {
            if (!neopixel->edit) {
                neopixel_color_t colors = {neopixel->rgb_send[0],
                                           neopixel->rgb_send[1],
                                           neopixel->rgb_send[2]};
                neopixel_set_color(global_var.visual_output, &colors);
            }
            dev_mode_request_frontend_change();
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
            if (neopixel->edit &&
                global_var.device_state == DEV_STATE_ADV_NEOPIXEL_WRITE) {
                uint16_t number = neopixel->rgb_send[neopixel->rgb_index];
                number *= 10;
                number += (uint16_t)cdtoi(received_char);

                if (number > 255) {
                    number = 255;
                }
                neopixel->rgb_send[neopixel->rgb_index] = number;
                dev_mode_request_frontend_change();
            }
            break;
        }
        case 'x':
        case 'X': {
            if (neopixel->edit &&
                global_var.device_state == DEV_STATE_ADV_NEOPIXEL_WRITE) {
                uint32_t number = neopixel->rgb_send[neopixel->rgb_index];
                number /= 10;
                neopixel->rgb_send[neopixel->rgb_index] = number;

                dev_mode_request_frontend_change();
            }
            break;
        }
        default:
            break;
    }
}
