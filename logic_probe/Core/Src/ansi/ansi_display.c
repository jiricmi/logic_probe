#include "ansi_display.h"
#include "ansi_abstraction_layer.h"
#include <string.h>
#include <stdio.h>

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24

void ansi_set_default_cursor(void) {
  send_uart_string("\033[2J");
  send_uart_string("\033[H");

}

void ansi_set_blue_background(void) {
  send_uart_string("\033[44m");
  ansi_set_default_cursor();
}

// Funkce pro nastavení pozice kurzoru
void set_cursor_position(int row, int col) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "\033[%d;%dH", row, col); // Nastavení pozice kurzoru
    send_uart_string(buffer);
}

// Funkce pro vybarvení čtverce 80x24
void ansi_set_red_rectangle(void) {
    const char *red_background = "\033[41m"; // Červené pozadí
    const char *reset_color = "\033[0m";      // Reset barev

    // Naplnění čtverce
    for (int i = 1; i <= TERMINAL_HEIGHT; i++) { // Pozice řádku od 1 do 24
        set_cursor_position(i, 1); // Nastavení pozice kurzoru na (řádek i, sloupec 1)
        
        // Odeslání červeného pozadí a prázdných znaků
        send_uart_string(red_background); // Nastavení červeného pozadí
        
        // Odeslání 80 prázdných znaků pro vybarvení řádku
        for (int j = 0; j < TERMINAL_WIDTH; j++) {
            send_uart_string(" "); // Odeslání prázdného znaku
        }
        
        send_uart_string(reset_color); // Resetování barev
    }
}

void ansi_reset(void) {
    send_uart_string("\033[0m");  // Resetuje barvy a formátování
}
