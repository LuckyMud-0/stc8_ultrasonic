#include "App.h"

void uart1_rec() {
    // set_seet_init_val(0, 180, 5);
    int range_min;
    int range_max;
    u8 step;// , i;
    // for (i = 0; i < COM1.RX_Cnt; i++)
    //      TX1_write2buff(RX1_Buffer[i]);
    if (   RX1_Buffer[0] == 0x00
        && RX1_Buffer[1] == 0x00
        && RX1_Buffer[2] == 0x3A) {
        // 
        range_min = RX1_Buffer[3];
        range_max = RX1_Buffer[4];
        step = RX1_Buffer[5];
        // 
        // if (range_min < 0 || range_max > 180){
        //     printf("range_min < 0 || range_max > 180");
        //     return;
        // }
        // if ((range_max - range_min) % step != 0){
        //     printf("(range_max - range_min) \% step != 0");
        //     return;
        // }
        // 
        set_seet_init_val(range_min, range_max, step);
        // 
        os_delete_task(SEET_ENG);
        os_create_task(SEET_ENG);
        // 
        printf("set_seet_init_val(%d, %d, %d)\n", range_min, range_max, (int)step);
    }
    
}

void uart1_task() _task_ UART_1 {
    // 
    UART1_init();
	printf("uart1...\n");
    // 
    while(1) {
        // printf("uart...\n");
        // 
        if (COM1.RX_TimeOut > 0) // 超时计数
		{
			if (--COM1.RX_TimeOut == 0)
			{
				// uart1接收到数据
                if (COM1.RX_Cnt > 0)
				    uart1_rec();
				COM1.RX_Cnt = 0;
			}
		}

        os_wait2(K_TMO, 1);
    }
}