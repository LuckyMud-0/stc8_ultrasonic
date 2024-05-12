#ifndef __SEET_ENG_H__
#define __SEET_ENG_H__

#include "STC8G_H_GPIO.h"
#include "STC8G_H_Switch.h"
#include "STC8H_PWM.h"

#define PERIOD	(MAIN_Fosc / 1000)
#define Prescaler	20

void seet_eng_init();

void set_seet_eng(float angle);

#endif
