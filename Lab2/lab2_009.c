// Author: Ioannis Manousaridis
// Course: RTOS 
// AGH University, Krakow, Poland, May 2020

/* Libraries */
#include "FreeRTOS.h"
#include "task.h"
#include "supporting_functions.h"
#include <stdio.h>

/* The two task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Used to hold the handle of Task2. */
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;

int periodic = 1;
TickType_t xLastWakeTime;
const TickType_t xFrequency = 100;
/*-----------------------------------------------------------*/

int main( void )
{
	printf("Which example do you want to run:\r\n1.Run a continuous task and then run the next higher priority task from this task.\r\n"
		"2.Run a periodical task with a delay in the loop, and then from this task run another\r\n"
		" such periodical task with a higher priority with a delay in the loop\r\n");
	printf("Enter an integer [1/2]: ");
	scanf("%d", &periodic);
	printf("You selected case: %d\r\n\n", periodic);

	/* Create the first task at priority 0.  */
	xTaskCreate( vTask1, "Task 1", 1000, NULL, tskIDLE_PRIORITY, &xTask1Handle );
          
	/* Start the scheduler to start the task executing. */
	vTaskStartScheduler();	

	/* The following line should never be reached */
	while (1)
	{
		printf("Insufficient heap memory!");
	};
	return 0;
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
UBaseType_t uxPriority;
int check = 0;
const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );

while (1){

		uxPriority = uxTaskPriorityGet(NULL);
		/* Print out the name of this task. */

		if (periodic == 2) {
			printf("Task1: I run at priority %d and periodically.\r\n", uxPriority);
			vTaskDelay(xDelay500ms);
		}
		else {
			printf("Task1: I run at priority %d.\r\n", uxPriority);
		}

		if (check == 0) {
			check++;
			vPrintString("Task1: I created task2.\r\n");
			xTaskCreate(vTask2, "Task 2", 1000, NULL, tskIDLE_PRIORITY + 1, &xTask2Handle);

		};
	}
}

/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
UBaseType_t uxPriority;
const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);


	while (1)
	{
		uxPriority = uxTaskPriorityGet(NULL);

		/* Print out the name of this task. */
		
		if (periodic == 2) {
			printf("Task2: I run at priority %d and periodically.\r\n", uxPriority);
			vTaskDelay(xDelay500ms);
		}
		else {
			printf("Task2: I run at priority %d.\r\n", uxPriority);
			vTaskEndScheduler();
		}
	}
}
/*-----------------------------------------------------------*/




