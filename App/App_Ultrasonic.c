#include "App.h"

volatile float global_distance = 0;
volatile float ul_result_set = 0;
void ultrasonic_task() _task_ ULTRASONIC
{
	Ultrasonic_init();
	while(1){
		char ret;
		// printf("ul...\n");
		os_wait1(K_SIG);
		ret = Ultrasonic_get_distance(&global_distance);
		// 
		if (ret != SUCCESS)
		{
			global_distance = -1;
		}
		ul_result_set = 1;
		
		// os_wait2(K_TMO, 2);
	}
}
