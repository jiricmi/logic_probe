#ifndef ANSI_ASCII_TEXT_H
#define ANSI_ASCII_TEXT_H

#include <stdio.h>

typedef enum {
    ASCII_LOGO_MAIN,
    ASCII_LOGO_CHANNEL,
    ASCII_LOGO_VOLTAGE,
    ASCII_LOGO_SIGNAL_DETECT,
    ASCII_LOGO_SIGNAL_GENERATOR
} ascii_logo_type_t;

#define MAIN_ASCII_TITLE                           \
    {"    ___  ___       ________   \n",           \
     "   |\\  \\|\\  \\     |\\   __  \\  \n",     \
     "   \\ \\  \\ \\  \\    \\ \\  \\|\\  \\ \n", \
     " __ \\ \\  \\ \\  \\    \\ \\   ____\\\n",   \
     "|\\  \\\\_\\  \\ \\  \\____\\ \\  \\___|\n", \
     "\\ \\________\\ \\_______\\ \\__\\   \n",    \
     " \\|________|\\|_______|\\|__|   \n",        \
     "                               \n"}
#define MAIN_ASCII_TITLE_ROW 8
#define MAIN_ASCII_TITLE_COL 30

#define CHANNEL_ASCII_TITLE                            \
    {"   _____ _                            _ \n",     \
     "  / ____| |                          | |\n",     \
     " | |    | |__   __ _ _ __  _ __   ___| |\n",     \
     " | |    | '_ \\ / _` | '_ \\| '_ \\ / _ \\ |\n", \
     " | |____| | | | (_| | | | | | | |  __/ |\n",     \
     "  \\_____|_| |_|\\__,_|_| |_|_| |_|\\___|_|\n",  \
     "                                        \n",     \
     "                                        \n"}
#define CHANNEL_ASCII_TITLE_ROW 8
#define CHANNEL_ASCII_TITLE_COL 41

#define VOLTAGE_ASCII_TITLE                          \
    {" __      __   _ _                   \n",       \
     " \\ \\    / /  | | |                  \n",     \
     "  \\ \\  / /__ | | |_ __ _  __ _  ___ \n",     \
     "   \\ \\/ / _ \\| | __/ _` |/ _` |/ _ \\\n",   \
     "    \\  / (_) | | || (_| | (_| |  __/\n",      \
     "     \\/ \\___/|_|\\__\\__,_|\\__, |\\___|\n", \
     "                          __/ |     \n",       \
     "                         |___/       \n"}
#define VOLTAGE_ASCII_TITLE_ROW 8
#define VOLTAGE_ASCII_TITLE_COL 37

#define SIGNAL_DETECT_ASCII_TITLE                       \
    {"  _____       _            _             ",       \
     " |  __ \\     | |          | |            ",      \
     " | |  | | ___| |_ ___  ___| |_ ___  _ __ ",       \
     " | |  | |/ _ \\ __/ _ \\/ __| __/ _ \\| '__|",    \
     " | |__| |  __/ ||  __/ (__| || (_) | |   ",       \
     " |_____/ \\___|\\__\\___|\\___|\\__\\___/|_|   ", \
     "                                         ",       \
     "                                         "}
#define SIGNAL_DETECT_ASCII_TITLE_ROW 8
#define SIGNAL_DETECT_ASCII_TITLE_COL 41

#define SIGNAL_GENERATOR_ASCII_TITLE                            \
    {"   _____                           _             ",       \
     "  / ____|                         | |            ",       \
     " | |  __  ___ _ __   ___ _ __ __ _| |_ ___  _ __ ",       \
     " | | |_ |/ _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__|",   \
     " | |__| |  __/ | | |  __/ | | (_| | || (_) | |   ",       \
     "  \\_____|\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_|   ", \
     "                                                 ",       \
     "                                                 "}
#define SIGNAL_GENERATOR_ASCII_TITLE_ROW 8
#define SIGNAL_GENERATOR_ASCII_TITLE_COL 49
typedef struct {
    size_t row;
    size_t col;
    char** ascii;
} ascii_text;

ascii_text* get_ascii_struct(const ascii_logo_type_t ascii);

ascii_text* allocate_ascii_struct(const size_t row, const size_t col);

void fill_ascii_struct(const ascii_text* ascii_struct, const char** ascii);

void free_ascii_text(ascii_text* ascii_struct);
#endif
