#ifndef ANSI_PAGE_FREQUENCY_READER_H
#define ANSI_PAGE_FREQUENCY_READER_H

#define FREQ_READER_ROW_MODE 9
#define FREQ_READER_COL_TEXT 10
#define FREQ_READER_ROW_TEXT 11
#define DETECTOR_MODE_BUFF_SIZE 50

#include "signal_detector.h"

void ansi_render_frequency_reader_page(void);

void ansi_generate_frequency_reader(sig_detector_t* detector);

void ansi_get_detector_mode(detector_mode_t mode);
void ansi_help_reader(void);
#endif
