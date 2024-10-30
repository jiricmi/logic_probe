#ifndef ANSI_DISPLAY_H
#define ANSI_DISPLAY_H

#include "ansi_abstraction_layer.h"
#include "main.h"

#define BORDER_VERTICAL "|"
#define BORDER_HORIZONTAL '-'

void ansi_set_background(const char *color);
void ansi_print_logo(const char *color, const char *bg_color);
void ansi_print_border(const char *color, const char *bg_color); 
#endif // ANSI_DISPLAY_H
