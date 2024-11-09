#ifndef ANSI_PAGES_H
#define ANSI_PAGES_H

#define MAIN_PAGE 0
#define CHANNEL_PAGE 1
#define VOLTAGE_PAGE 2

void ansi_main_page(void);

void generate_channel_menu(void);

void ansi_channel_set_page(void);

void ansi_voltage_page(void);

void render_current_page(void);
#endif
