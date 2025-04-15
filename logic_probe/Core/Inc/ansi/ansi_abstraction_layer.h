#ifndef ANSI_ABSTRACTION_LAYER_H
#define ANSI_ABSTRACTION_LAYER_H
#include <stdint.h>

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24

#define COLOR_STR_SIZE 6

#define BLACK_BG "\033[40m\0"
#define RED_BG "\033[41m\0"
#define GREEN_BG "\033[42m\0"
#define YELLOW_BG "\033[43m\0"
#define BLUE_BG "\033[44m\0"
#define MAGENTA_BG "\033[45m\0"
#define CYAN_BG "\033[46m\0"
#define WHITE_BG "\033[47m\0"
#define DARK_GRAY_BG "\033[48;5;236m\0"
#define LIGHT_GRAY_BG "\033[48;5;245m\0"
#define DARK_BLUE_BG "\033[48;5;32m\0"
#define LIGHT_BLUE_BG "\033[48;5;68m\0"
#define DARK_GREEN_BG "\033[48;5;34m\0"
#define LIGHT_GREEN_BG "\033[48;5;84m\0"
#define DARK_YELLOW_BG "\033[48;5;184m\0"
#define LIGHT_YELLOW_BG "\033[48;5;226m\0"

#define BLACK_TEXT "\033[30m\0"
#define RED_TEXT "\033[31m\0"
#define GREEN_TEXT "\033[32m\0"
#define YELLOW_TEXT "\033[33m\0"
#define BLUE_TEXT "\033[34m\0"
#define MAGENTA_TEXT "\033[35m\0"
#define CYAN_TEXT "\033[36m\0"
#define WHITE_TEXT "\033[37m\0"
#define BOLD_TEXT "\033[1m\0"

#define ANSI_CLEAR_SCREEN "\033[2J\033[H"
#define ANSI_RESET_FORMAT "\033[0m"
#define ANSI_BOLD "\033[1m"

#define BORDER_VERTICAL "|"
#define BORDER_HORIZONTAL '-'

typedef struct {
    char* color;
    char* bg_color;
    _Bool bold;
} ansi_text_config_t;

static const ansi_text_config_t ansi_default_conf = {"", "", 0};
static const ansi_text_config_t ansi_bold_conf = {"", "", 1};
static const ansi_text_config_t ansi_red_bold_conf = {RED_TEXT, "", 1};

void ansi_send_string(const char* str);

void ansi_send_text(const char* str, const ansi_text_config_t* text_conf);
void ansi_home_cursor(void);
void ansi_clear_format(void);
void ansi_clear_terminal(void);
void ansi_clear_line(uint8_t row, uint8_t offset);
void PrintError(const char* err_str);
void ansi_set_cursor(const uint8_t row, const uint8_t col);

void ansi_set_background(const char* color);

void ansi_render_title(const unsigned short int ascii_type, char* color);

void ansi_render_border(const char horizontal,
                        const char* vertical,
                        char* color);

void ansi_render_button(const char* text,
                        const uint8_t row,
                        const uint8_t col,
                        const ansi_text_config_t* conf);

#endif  // ANSI_ABSTRACTION_LAYER_H
