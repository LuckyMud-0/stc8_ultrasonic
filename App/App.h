#ifndef __APP_H__
#define __APP_H__

#include <STC8H.H>
#include <RTX51TNY.H>
#include "seet_eng.h"
#include "Ultrasonic.h"
#include "STC8G_H_Delay.h"
#include "STC8G_H_UART.h"
#include "STC8G_H_Delay.h"
#include "STC8G_H_NVIC.h"
#include "STC8G_H_Switch.h"
#include <stdlib.h>

#include "seet_eng.h"
#include "Ultrasonic.h"
#include "Uart.h"

#include "App_seet_eng.h"


#define UART_1          1
#define SEET_ENG        2
#define ULTRASONIC      3

extern float global_distance;
extern float ul_result_set;

// 

#endif