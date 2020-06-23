/* Libraries */
#include "FreeRTOS.h"
#include "task.h"
#include "supporting_functions.h"
#include <stdio.h>

/* The two task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Used to hold the handle of Tasks. */
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;
int testInteger;

/*-----------------------------------------------------------*/

int main( void )
{

	printf("Which example do you want to run:\r\n1.Run two continuous tasks with the same priorities.\r\n" 
			"2.Run two continuous tasks with the same priorities, and then change the priority of one task after some time.\r\n"
		    "3.Run two continuous tasks with different priorities.\r\nSelect an integer: ");
	scanf("%d", &testInteger);
	printf("You selected case: %d\r\n", testInteger);

	/* Create the first task at priority 1.  */
	xTaskCreate( vTask1, "Task 1", 1000, NULL, tskIDLE_PRIORITY, &xTask1Handle);
    
	/* Create the second task at priority 1 */
	xTaskCreate( vTask2, "Task 2", 1000, NULL, tskIDLE_PRIORITY, &xTask2Handle );
	
	
	/* Start the scheduler to start the tasks executing. */
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

/* Query the priority at which this task is running - passing in NULL means
	"return our own priority". */
	if (testInteger == 3) {
		vTaskPrioritySet(xTask1Handle, tskIDLE_PRIORITY + 2);
	}
	uxPriority = uxTaskPriorityGet(NULL);

	while(1)
	{
	
		/* Print out the name of this task. */
		printf("Task1: I run at priority %d. \r\n", uxPriority);
		vTaskDelay(1000);


	}
}

/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
UBaseType_t uxPriority;
int counter = 0;
if (testInteger == 3) {
	vTaskPrioritySet(xTask2Handle, tskIDLE_PRIORITY + 3 );
}
	while(1){

		
		uxPriority = uxTaskPriorityGet(NULL);
		
		/* Print out the name of this task. */
		printf("Task2: I run at priority %d. \r\n", uxPriority);
		

		counter++;
		if (counter == 5 && testInteger == 2) {
			// Use the handle to raise the priority of the created task.
			vTaskPrioritySet(xTask2Handle, tskIDLE_PRIORITY + 1);
			printf("Task2: Now I am more important than task1\r\n");
		}

		vTaskDelay(1000);
	}

}
/*-----------------------------------------------------------*/




