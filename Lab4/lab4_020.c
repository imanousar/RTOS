// Author: Ioannis Manousaridis
// Course: RTOS 
// AGH University, Krakow, Poland, May 2020

/* Libraries */
#include "FreeRTOS.h"
#include "task.h"
#include "supporting_functions.h"
#include <stdio.h>
#include <stdio.h>
#include <conio.h>
#include "semphr.h"

/* The two task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask1_simple(void *pvParameters);
void vTask2_simple(void *pvParameters);

/* Used to hold the handle of Task2. */
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;
TaskHandle_t xTask1Handle_simple;
TaskHandle_t xTask2Handle_simple;

xSemaphoreHandle gatekeeper = 0;
int option;

int main( void )
{
	printf("Which example do you want to run:\r\n1.Run two continuous tasks without any synchronization.\r\n"
		"2.Run two continuous tasks with synchronization.");
	printf("\nEnter an integer [1/2]: ");
	scanf("%d", &option);
	printf("You selected case: %d\r\n\n", option);
   
	if(option == 1){
		/* Create the first task at priority 0.  */
		xTaskCreate(vTask1_simple, "Task 1 simple", 1000, NULL, tskIDLE_PRIORITY, &xTask1Handle_simple);
		xTaskCreate(vTask2_simple, "Task 2 simple", 1000, NULL, tskIDLE_PRIORITY, &xTask2Handle_simple);
	}
	else if (option == 2) {
		gatekeeper = xSemaphoreCreateMutex();

		/* Create the first task at priority 0.  */
		xTaskCreate(vTask1, "Task 1", 1000, NULL, tskIDLE_PRIORITY, &xTask1Handle);
		xTaskCreate(vTask2, "Task 2", 1000, NULL, tskIDLE_PRIORITY, &xTask2Handle);
	}

	
	vTaskStartScheduler();
	
	/* The following line should never be reached */
	while (1)
	{
		printf("Insufficient heap memory!");
	};
	return 0;
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	while (1) {
		if (xSemaphoreTake(gatekeeper, 1000)) {
			printf("Task1: I have stdout access!\r\n");
			vTaskDelay(250);
			xSemaphoreGive(gatekeeper);
	
		}
		else{
			printf("Task1: I was denied stdout access!\r\n");
	}
	}
}

void vTask2(void *pvParameters)
{
	while (1) {
		if (xSemaphoreTake(gatekeeper, 1000)) {
			printf("Task2: I have stdout access!\r\n");
			vTaskDelay(400);
			xSemaphoreGive(gatekeeper);
		}
		else{
			printf("Task2: I was denied stdout access!\r\n");
		}
		
	}
}

void vTask1_simple(void *pvParameters) {

	while (1)
	{
		printf("Task1: I have stdout access and there is no synchronization!\r\n");
		vTaskDelay(400);

	}
}
void vTask2_simple(void *pvParameters) {

	while (1)
	{
		printf("Task2: I have stdout access and there is no synchronization!\r\n");
		vTaskDelay(500);

	}
}
