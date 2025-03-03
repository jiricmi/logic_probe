#ifndef ANSI_PAGE_FREQUENCY_READER_H
#define ANSI_PAGE_FREQUENCY_READER_H

#include "ansi_abstraction_layer.h"
#include "signal_detector.h"

void ansi_render_frequency_reader_page(void);

void ansi_generate_frequency_reader(const sig_detector_t* detector);

void ansi_get_detector_mode(char* text,
                            ansi_text_config_t* text_conf,
                            const detector_mode_t mode);
void ansi_frequency_reader_generate_hint(void);
#endif
