#ifndef LOOP_H
#define LOOP_H

#include "adc_control.h"
#include "global_structs.h"

void dev_mode_check_update(void);

void dev_mode_perif_turn_off(adc_vars_t* adc_perif);
void dev_mode_change_mode(dev_state_t mode);
void dev_mode_request_frontend_change(void);
void dev_mode_request_update(void);

void dev_mode_update_perif(void);

void dev_mode_run(void);

#endif
