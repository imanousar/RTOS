// Author: Ioannis Manousaridis
// Course: RTOS 
// AGH University, Krakow, Poland, May 2020


// Libraries 

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>												/* System headers */
#include "posix_demo.h"											/* demo include */
#define mainPOSIX_DEMO_PRIORITY    ( tskIDLE_PRIORITY + 4 )     /* Demo task priority */


// Global variables

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;



/*-----------------------------------------------------------*/

// Function for first task
void vStartTask_1(void *pvParameters)
{
	int counter = 1;
	int iStatus = 0;

	// convert int long to ticks
	const uint32_t ulLongTime_ms = pdMS_TO_TICKS(1000UL);

	/* Remove warnings about unused parameters. */
	(void)pvParameters;


	// Print for 30 times and then terminate
	printf("Starting Task 1 \r\n");
	for (;; ) {
		Sleep(ulLongTime_ms);
		printf("Task 1: I was executed %d times\r\n", counter++);
		if (counter == 30) {
			printf("Task 1: I was executed %d times. I will go to rest for now.\r\n", counter++);
			vTaskDelete(task1Handle);
		}
	}
}

// Task 2. The same approach but for 20 repeations
void vStartTask_2(void *pvParameters)
{
	int counter = 1;
	int iStatus = 0;

	const uint32_t ulLongTime_ms = pdMS_TO_TICKS(2000UL);

	/* Remove warnings about unused parameters. */
	(void)pvParameters;

	printf("Starting Task 2 \r\n");
	for (;; ) {
		Sleep(ulLongTime_ms);
		printf("Task 2: I was executed %d times \r\n", counter++);
		if (counter == 20) {
			printf("Task 2: I was executed %d times. I will go to rest for now.\r\n", counter++);
			vTaskDelete(task2Handle);
		}
	}
}


int main(void)
{
	configASSERT((mainPOSIX_DEMO_PRIORITY < configMAX_PRIORITIES));

	const uint32_t ulLongTime_ms = pdMS_TO_TICKS(1000UL);

	printf("FreeRTOS POSIX demo starting\n");

	// Create task 1 and assign handler 1
	xTaskCreate(vStartTask_1,
		"task_1",
		configMINIMAL_STACK_SIZE,
		NULL,
		1,
		&task1Handle);

	// Create task 2 and assign handler 2
	xTaskCreate(vStartTask_2,
		"task_2",
		configMINIMAL_STACK_SIZE,
		NULL,
		1,
		&task2Handle);

	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	 * line will never be reached.  If the following line does execute, then
	 * there was insufficient FreeRTOS heap memory available for the idle and/or
	 * timer tasks to be created.  See the memory management section on the
	 * FreeRTOS web site for more details (this is standard text that is not
	 * really applicable to the Win32 simulator port). */
	for (; ; )
	{
		printf("insufficient FreeRTOS heap memory\n");
		Sleep(ulLongTime_ms);
	}

	return 0;
}



