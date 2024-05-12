#include "App.h"

#define RANGE       180
#define STEP        5
#define LEN         (RANGE / STEP + 1)

static u16 angle_arr[LEN];//  = {0, 30, 45, 60, 90, 120, 135, 150, 180, 150, 135, 120, 90, 60, 45, 30};
static u16 distance_arr[LEN];//  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void print_info(int i) {
    os_send_signal(ULTRASONIC);
	os_wait2(K_TMO, 10);
    while (!ul_result_set)
	    os_wait2(K_TMO, 2);
    ul_result_set = 0;
    if (global_distance > 0) {
        printf("angle = %d, distance: %.2f\n", angle_arr[i], global_distance);
        distance_arr[i] = global_distance;
    } else {
        printf("angle = %d, check fail...", angle_arr[i]);
        distance_arr[i] = 0;
    }
    printf("Data %d %d;", angle_arr[i], distance_arr[i]);
}

void set_init_dat() {
    u8 i;
    for(i = 0; i < LEN; i++) {
        angle_arr[i] = i * STEP;
        distance_arr[i] = 0;
    }
}

void seet_eng_task() _task_ SEET_ENG
{

    u16 ang = 0;
    char dir = 1;
    u8 idx = 0;

	EAXSFR();
	seet_eng_init();
    // 
    set_init_dat();
    // 
    set_seet_eng(0);
    os_wait2(K_TMO, 100);
    print_info(idx);
    os_wait2(K_TMO, 100);
    while(1) {
        // printf("seet...\n");
        // 
        ang += STEP * dir;
        idx = ang / STEP;

        set_seet_eng(ang);
        print_info(idx);

        if (ang >= RANGE) {
            dir = -1;
        } else if (ang <= 0) {
            dir = 1;
        }
        
    }
    
}
