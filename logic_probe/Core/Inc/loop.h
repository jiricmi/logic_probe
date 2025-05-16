#ifndef LOOP_H
#define LOOP_H
#include "global_vars.h"

dev_setup_t dev_mode_get_dev_setup(void);
void dev_mode_package_version(void);
void dev_mode_check_update(void);
void dev_mode_run_with_uart(void);
void dev_mode_change_mode(dev_state_t mode);

void dev_mode_request_frontend_change(void);

void dev_mode_request_perif_change(void);

void dev_mode_request_update(void);
void dev_mode_perif_turn_off(sig_detector_t* sig_det, adc_vars_t* adc_vars);
void dev_mode_update_perif(void);
void dev_mode_run(void);
#endif
