#ifndef CONTROL_BOARD_SPEED_H
#define CONTROL_BOARD_SPEED_H

//#include "control_board_common.h"

#define GPIO_SPEED_SIG    25
#define GPIO_SPEED_SEL_0  26
#define GPIO_SPEED_SEL_1  27
#define GPIO_SPEED_SEL_2  32
#define GPIO_SPEED_SEL_3  33

#define GPIO_SPEED_CHAN_0   0
#define GPIO_SPEED_CHAN_1   1
#define GPIO_SPEED_CHAN_2   2
#define GPIO_SPEED_CHAN_3   3
#define GPIO_SPEED_CHAN_4   4
#define GPIO_SPEED_CHAN_5   5
#define GPIO_SPEED_CHAN_6   6
#define GPIO_SPEED_CHAN_7   7
#define GPIO_SPEED_CHAN_8   8
#define GPIO_SPEED_CHAN_9   9
#define GPIO_SPEED_CHAN_10  10
#define GPIO_SPEED_CHAN_11  11
#define GPIO_SPEED_CHAN_12  12
#define GPIO_SPEED_CHAN_13  13
#define GPIO_SPEED_CHAN_14  14


#define GPIO_SPEED_CHAN_PINS  NUM_OF_TRAINS
#define GPIO_SPEED_SEL_PINS   4

#define GPIO_SPEED_SAMPLES  30
#define TRAIN_SPEED_MIN  3000
#define TRAIN_SPEED_MAX  100


void gpio_speed_channel_sel(int channel);
void gpio_speed_set_mode();
void gpio_calculate_speed(int channel);
void process_speed();
#define NUM_OF_TRAINS 16


extern int train_speed[NUM_OF_TRAINS];
extern int gpio_speed_sel_arr[GPIO_SPEED_SEL_PINS];
extern int gpio_speed_chan_arr[NUM_OF_TRAINS];

#endif
