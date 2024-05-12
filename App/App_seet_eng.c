#include "App.h"
// #include "App_seet_eng.h"

// #define RANGE       180
// #define step        5
// #define LEN         (range_max / step + 1)

static int range_min = 0;
static int range_max = 180;
static u8 step   = 5;
static u16 g_len;

void set_seet_init_val(int r_min, int r_max, u8 stp) {
    range_min = r_min;
    range_max = r_max;
    step = stp;
}

// static int * angle_arr;// [LEN];//  = {0, 30, 45, 60, 90, 120, 135, 150, 180, 150, 135, 120, 90, 60, 45, 30};
// static int * distance_arr;// [LEN];//  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int angle_arr[512];// [LEN];//  = {0, 30, 45, 60, 90, 120, 135, 150, 180, 150, 135, 120, 90, 60, 45, 30};
static int distance_arr[512];// [LEN];//  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

// void set_init_dat() {
//     u8 i;
//     u16 len = (range_max - range_min) / step + 1;
//     g_len = len;
//     printf("len = %d\n", len);
//     printf("%p - %p = %p\n", distance_arr, angle_arr, distance_arr - angle_arr);
//     angle_arr = (int*)malloc(len * sizeof(int));
//     distance_arr = (int*)malloc(len * sizeof(int));
//     printf("%p - %p = %p\n", distance_arr, angle_arr, distance_arr - angle_arr);
//     for(i = 0; i < len; i++) {
//         angle_arr[i] = i * step + range_min;
//         distance_arr[i] = 0;
//         printf("%d, %d;\n", angle_arr[i], distance_arr[i]);
//     }
// }


void set_init_dat() {
    u8 i;
    // u16 len = (range_max - range_min) / step + 1;
    u16 len = (range_max - range_min) / step + 1;
    g_len = len;
    // printf("len = %d\n", len);
    // printf("%p - %p = %p\n", distance_arr, angle_arr, distance_arr - angle_arr);
    // angle_arr = (int*)malloc(len * sizeof(int));
    // distance_arr = (int*)malloc(len * sizeof(int));
    // printf("%p - %p = %p\n", distance_arr, angle_arr, distance_arr - angle_arr);
    for(i = 0; i < len; i++) {
        angle_arr[i] = i * step + range_min;
        distance_arr[i] = 0;
        // printf("%d, %d;\n", angle_arr[i], distance_arr[i]);
    }
}

void seet_eng_task() _task_ SEET_ENG
{

    int ang = range_min;
    char dir = 1;
    u8 idx = 0;

	EAXSFR();
    // 
    set_init_dat();
    // printf("%p - %p = %p\n", distance_arr, angle_arr, distance_arr - angle_arr);
	seet_eng_init();
    set_seet_eng(range_min);
    os_wait2(K_TMO, 100);
    print_info(idx);
    os_wait2(K_TMO, 100);
    while(1) {
        // printf("seet... ang = %d\n", ang);
        // 
        ang += step * dir;
        if (ang >= range_max) {
            dir = -1;
        } else if (ang <= range_min) {
            dir = 1;
        }

        idx = (ang - range_min) / step;
        if (idx >= g_len) 
            continue;

        set_seet_eng(ang);
        print_info(idx);
        
    }
    
}
