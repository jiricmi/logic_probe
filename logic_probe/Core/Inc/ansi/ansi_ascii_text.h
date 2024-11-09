#ifndef ANSI_ASCII_TEXT_H
#define ANSI_ASCII_TEXT_H

#include <stdio.h>

#define ASCII_MAIN_LOGO 0
#define ASCII_VOLTAGE 1

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

typedef struct {
    size_t row;
    size_t col;
    char** ascii;
} ascii_text;

ascii_text* get_ascii_struct(const unsigned short int ascii);

ascii_text* allocate_ascii_struct(const size_t row, const size_t col);

void fill_ascii_struct(const ascii_text* ascii_struct, const char** ascii);

void free_ascii_text(ascii_text* ascii_struct);
#endif
