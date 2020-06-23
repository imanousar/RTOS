// Author: Ioannis Manousaridis
// Course: RTOS 
// AGH University, Krakow, Poland, May 2020


// Libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" //lib for handling queues
#include <stdio.h>
#include <stdlib.h>

#include "posix_demo.h"

/* Demo task priority */
#define mainPOSIX_DEMO_PRIORITY    ( tskIDLE_PRIORITY + 4 )

/*-----------------------------------------------------------*/

// Create handlers for the queues
xQueueHandle  Queue_Handle_var = 0;
xQueueHandle  Queue_Handle_array = 0;
xQueueHandle  Queue_Handle_string = 0;


static const char ucase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const size_t ucase_count = sizeof(ucase) - 1; // ucase includes terminating '\0'


// Sender task
void sender_task(void *pvParameters)
{
	//  Memory allocation and initializations for the integer, array and string to send
	int i = 0;
	int arr[5]; 
	char str[10];
	
	while (1) {

		// Assign random integers in the array 
		for (int k = 0; k < 5; k++)
		{
			arr[k] = rand() % 9;
		}

		// Create random string 
		int random_index;
		for (int k = 0; k < 9; k++) {
			random_index = rand() % ucase_count;

			str[k] = ucase[random_index];
		}
		str[9] = '\0';



		printf("\n Sending data. %i attempts took place before.\n\n", i);

		// Send to the variable queue the integer
		if (!xQueueSend(Queue_Handle_var, &i, 1000))
		{
			printf("Failed to send to queue var\n");
		}

		// Send to the array queue the array
		if (!xQueueSend(Queue_Handle_array, &arr, 1500))
		{
			printf("Failed to send to queue array\n");
		}

		// Send to the string queue the string
		if (!xQueueSend(Queue_Handle_string, &str, 2000))
		{
			printf("Failed to send to queue string\n");
		}
		
		++i;
		vTaskDelay(2000);

	}
}


// Receiver task
void receiver_task(void *pvParameters)
{
	// Initialize variables to receive data
	int i;
	int arr[5];
	char str[10];


	while (1) {
		
		// Check var queue for 1500 ticks
		if (xQueueReceive(Queue_Handle_var, &i, 1500))
		{
			printf("Received from queue var: %i \n", i);
		}
		else
		{
			puts("Failed to receive data from queue var\n");
		}

		// Check array queue for 1000 ticks
		if (xQueueReceive(Queue_Handle_array, &arr, 1000))
		{
			printf("Received from queue array: ");
			for (int j = 0; j < 5; j++) {
				printf("%i", arr[j]);
			}
			printf("\n");
		}
		else
		{
			puts("Failed to receive data from queue array\n");
		}

		// Check string queue for 2000 ticks
		if (xQueueReceive(Queue_Handle_string, &str, 2000))
		{
			printf("Received from string queue: %s\n", str);

		}
		else
		{
			puts("Failed to receive data from queue string\n");
		}

		vTaskDelay(2000);

	}
}

// main function to create the tasks.
int main(void)
{
	configASSERT((mainPOSIX_DEMO_PRIORITY < configMAX_PRIORITIES));

	const uint32_t ulLongTime_ms = pdMS_TO_TICKS(1000UL);
	int array[5];
	char str[10];

	// Create variable, array and string queues
	Queue_Handle_var = xQueueCreate(3, sizeof(int));
	Queue_Handle_array = xQueueCreate(3, sizeof(array));
	Queue_Handle_string = xQueueCreate(3, sizeof(str));

	printf("Lab8 - Ioannis Manousaridis \n");

	// Create sender and receiver tasks
	xTaskCreate(sender_task, "tx", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(receiver_task, "rx", configMINIMAL_STACK_SIZE, NULL, 1, NULL);


	// Start the scheduler
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	 * line will never be reached.  If the following line does execute, then
	 * there was insufficient FreeRTOS heap memory available for the idle and/or
	 * timer tasks to be created.  See the memory management section on the
	 * FreeRTOS web site for more details (this is standard text that is not
	 * really applicable to the Win32 simulator port). */
	for (; ; )
	{
		printf("Insufficient memory on the heap.\n");
		Sleep(ulLongTime_ms);
	}

	return 0;
}



