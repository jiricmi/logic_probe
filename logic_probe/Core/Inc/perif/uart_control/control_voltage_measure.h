#ifndef CONTROL_VOLTAGE_MEASURE_H
#define CONTROL_VOLTAGE_MEASURE_H

void control_voltage_page(const unsigned char received_char);

void control_voltage_base(const unsigned char received_char);

void control_voltage_measures(const unsigned char received_char);

void control_voltage_resistance(const unsigned char received_char);
#endif
