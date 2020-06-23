// Author: Ioannis Manousaridis
// Course: RTOS 
// AGH University, Krakow, Poland, May 2020

/* Libraries */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "supporting_functions.h"
#include "stdio.h"

/*  Once started, a one-shot timer will execute its callback function only once.
 It can be manually re-started, but will not automatically re-start itself.
 Conversely, once started, an auto-reload timer will automatically re-start
 itself after each execution of its callback function, resulting in periodic
 callback execution.*/

// The periods assigned to the one-shot and auto-reload timers respectively.
#define ONE_SHOT_TIMER_PERIOD		( pdMS_TO_TICKS( 1000UL ) )
#define AUTO_RELOAD_TIMER_PERIOD	( pdMS_TO_TICKS( 500UL ) )

 /*-----------------------------------------------------------*/

 /*
  * The callback functions used by the one-shot and auto-reload timers
  * respectively.
  */
static void timer1_one_callback(TimerHandle_t xTimer);
static void timer2_one_callback(TimerHandle_t xTimer);
static void autoreload_timer_callback(TimerHandle_t xTimer);

/*-----------------------------------------------------------*/

/* Global variables*/
int input = 0;
int main(void)
{

	printf("Which example do you want to run:\r\n\n1.Two one shot timers with the same trigger time.\r\n"
		"2.Two one shot timers with different trigger time.\r\n"
		"3.One one shot timer and one auto reload timer.\r\n\n");

	printf("Enter an integer [1-3]: ");
	scanf("%d", &input);


	printf("\nYou selected case: %d\r\n\n", input);


	TimerHandle_t xAutoReloadTimer, timer1_one, timer2_one;

	// Case one was selected
	if (input == 1) {
		timer1_one = xTimerCreate("Timer1", ONE_SHOT_TIMER_PERIOD, pdFALSE, (void *)0, timer1_one_callback);
		if (xTimerStart(timer1_one, 10) != pdPASS) {
			printf("Timer start error");
		}

		timer2_one = xTimerCreate("Timer2", ONE_SHOT_TIMER_PERIOD, pdFALSE, (void *)0, timer2_one_callback);
		if (xTimerStart(timer2_one, 10) != pdPASS) {
			printf("Timer start error");
		}
	}

	// Case two was selected
	else if (input == 2) {
		timer1_one = xTimerCreate("Timer1", ONE_SHOT_TIMER_PERIOD, pdFALSE, (void *)0, timer1_one_callback);
		if (xTimerStart(timer1_one, 10) != pdPASS) {
			printf("Timer start error");
		}

		timer2_one = xTimerCreate("Timer2", pdMS_TO_TICKS(1500UL), pdFALSE, (void *)0, timer2_one_callback);
		if (xTimerStart(timer2_one, 10) != pdPASS) {
			printf("Timer start error");
		}
	}

	// Case three was selected
	else {
		timer1_one = xTimerCreate("Timer1", ONE_SHOT_TIMER_PERIOD, pdFALSE, (void *)0, timer1_one_callback);
		if (xTimerStart(timer1_one, 10) != pdPASS) {
			printf("Timer start error");
		}
		timer1_one = xTimerCreate("Timer1", AUTO_RELOAD_TIMER_PERIOD, pdTRUE, (void *)0, autoreload_timer_callback);
		if (xTimerStart(timer1_one, 10) != pdPASS) {
			printf("Timer start error");
		}
	}


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
	static TickType_t xTimeNow;
	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	vPrintStringAndNumber("One-shot timer callback executing at time:", xTimeNow);
}
/*-----------------------------------------------------------*/

static void timer2_one_callback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	vPrintStringAndNumber("One-shot timer callback executing at time:", xTimeNow);
}
/*-----------------------------------------------------------*/


static void autoreload_timer_callback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	vPrintStringAndNumber("Auto-reload timer callback executing at time:", xTimeNow);
}
/*-----------------------------------------------------------*/