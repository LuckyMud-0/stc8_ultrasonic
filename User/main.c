#include "App.h"

void main_task() _task_ 0 {

	// ��������
	os_create_task(UART_1);
	os_create_task(SEET_ENG);
	os_create_task(ULTRASONIC);
	
	// ɾ������
	os_delete_task(0);
	
}