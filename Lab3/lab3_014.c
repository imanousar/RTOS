// Author: Ioannis Manousaridis
// Course: RTOS 
// AGH University, Krakow, Poland, May 2020

/* Libraries */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "supporting_functions.h"
#include <stdio.h>
#include <stdlib.h>

/* The periods assigned to the one-shot and auto-reload timers respectively. */
#define TIMER1_PERIOD		( pdMS_TO_TICKS( 1650UL ) )
#define TIMER2_PERIOD	    ( pdMS_TO_TICKS( 600UL ) )
#define TIMER3_PERIOD	    ( pdMS_TO_TICKS( 1000UL ) )

/*-----------------------------------------------------------*/

/* The callback functions that are used by timers. */

static void timer1_one_callback(TimerHandle_t xTimer);
static void timer2_one_callback(TimerHandle_t xTimer);
static void timer3_one_callback(TimerHandle_t xTimer);

/*-----------------------------------------------------------*/

/* Global variables */

int timer1_last_timestamp;
int timer2_last_timestamp;
int difference_in_timestamps;
TimerHandle_t timer1, timer2, timer3;
int flag_for_timer3 = 0;


int main( void )
{

	timer1 = xTimerCreate("Timer1", TIMER1_PERIOD, pdTRUE, (void *)0, timer1_one_callback);
	if (xTimerStart(timer1, 10) != pdPASS) {
		printf("Timer 1 start error");
	}
	
	timer2 = xTimerCreate("Timer2", TIMER2_PERIOD, pdTRUE, (void *)0, timer2_one_callback);
	if (xTimerStart(timer2, 10) != pdPASS) {
		printf("Timer 2 start error");
	}

	
	
	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If the scheduler was started then the following line should never be
	reached */
	while (1)
	{
		printf("Insufficient heap memory!");
	};
	return 0;
}
/*-----------------------------------------------------------*/

static void timer1_one_callback(TimerHandle_t xTimer)
{
	/* Obtain the current tick count. */
	timer1_last_timestamp = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	vPrintStringAndNumber("Task1: Timer1 callback executing at time:", timer1_last_timestamp);
}
/*-----------------------------------------------------------*/

static void timer2_one_callback(TimerHandle_t xTimer)
{
	/* Obtain the current tick count. */
	timer2_last_timestamp = xTaskGetTickCount();

	if (flag_for_timer3 == 0) {
		timer3 = xTimerCreate("Timer3", TIMER3_PERIOD, pdTRUE, (void *)0, timer3_one_callback);
		if (xTimerStart(timer3, 10) != pdPASS) {
			printf("Timer 3 start error\n");
		}
		flag_for_timer3++;
	}

	/* Output a string to show the time at which the callback was executed. */
	vPrintStringAndNumber("Task2: Timer2 callback executing at time:", timer2_last_timestamp);
}
/*-----------------------------------------------------------*/

static void timer3_one_callback(TimerHandle_t xTimer)
{
	
	printf("\nTask3: Difference between the last two timestamps of the timers is: %i\n\n", abs(timer1_last_timestamp - timer2_last_timestamp));

	}
/*-----------------------------------------------------------*/
