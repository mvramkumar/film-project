#ifndef CONTROL_BOARD_RUN_STATUS_H
#define CONTROL_BOARD_RUN_STATUS_H

#include "Arduino.h"
//#include "control_board_common.h"
#include "PCF8575.h"
#include "Wire.h"

#define GPIO_RUN_STATUS_TRAIN_1   P0
#define GPIO_RUN_STATUS_TRAIN_2   P1
#define GPIO_RUN_STATUS_TRAIN_3   P2
#define GPIO_RUN_STATUS_TRAIN_4   P3
#define GPIO_RUN_STATUS_TRAIN_5   P4
#define GPIO_RUN_STATUS_TRAIN_6   P5
#define GPIO_RUN_STATUS_TRAIN_7   P6
#define GPIO_RUN_STATUS_TRAIN_8   P7
#define GPIO_RUN_STATUS_TRAIN_9   P8
#define GPIO_RUN_STATUS_TRAIN_10  P9
#define GPIO_RUN_STATUS_TRAIN_11  P10
#define GPIO_RUN_STATUS_TRAIN_12  P11
#define GPIO_RUN_STATUS_TRAIN_13  P12
#define GPIO_RUN_STATUS_TRAIN_14  P13
#define GPIO_RUN_STATUS_TRAIN_15  P14
#define GPIO_RUN_STATUS_TRAIN_16  P15

#define GPIO_RUN_STATUS_START   1
#define GPIO_RUN_STATUS_STOP    0

#define PCF_ADDRESS_RUN_STATUS   0x21
PCF8575 pcf_run(PCF_ADDRESS_RUN_STATUS);

#define NUM_OF_TRAINS 16

extern int gpio_run_status_arr[NUM_OF_TRAINS];
extern int gpio_run_status_train[NUM_OF_TRAINS];

void gpio_run_status_init();
void gpio_run_status_read();

#endif
