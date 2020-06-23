/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "stdlib.h"
#include "queue.h"
#include "semphr.h"

#include "supporting_functions.h"

#define N 5

#define PRODUCER_DELAY 700
#define CONSUMER_DELAY 400


xSemaphoreHandle empty, full;
xSemaphoreHandle mutex;
xTaskHandle  producerHandler;
xTaskHandle consumerHandler;
int buffer[N];
int in = 0, out = 0;

void producer(void *pvParameters);
void consumer(void *pvParameters);



int main(int argc, char *argv[])
{
	printf("Author: Ioannis Manousaridis \n\n");
	full = xSemaphoreCreateCounting(N, 0);
	empty = xSemaphoreCreateCounting(N, N);
	mutex = xSemaphoreCreateMutex();
	if ((empty == NULL) || (full == NULL))
		printf("Error while creating counting Semaphores\n");
	else if (mutex == NULL)
		printf("Error while creating mutex\n");
	else
	{
		xTaskCreate(producer, "producer", 1000, NULL, tskIDLE_PRIORITY, &producerHandler);
		xTaskCreate(consumer, "consumer", 1000, NULL, tskIDLE_PRIORITY, &consumerHandler);

	}

	vTaskStartScheduler();

	/* The following line should never be reached */
	while (1)
	{
		printf("Insufficient heap memory!\n");
	};
	return 0;
}



void producer(void *pvParameters)
{
	int count = 0;
	int item;
	while (1)
	{
		item = rand() % 10;
		if (xSemaphoreTake(empty, PRODUCER_DELAY) != pdTRUE)
			printf("Producer unable to take empty, buffer is full\n");
		else if (xSemaphoreTake(mutex, PRODUCER_DELAY) != pdTRUE)
			printf("Producer unable to take mutex\n");
		else
		{
			printf("Producer sending item no %d with value: %6d\n", count, item);
			buffer[in] = item;
			in = (in + 1) % N;
			count++;
			if (xSemaphoreGive(mutex) != pdTRUE)
				printf("Producer unable to give mutex\n");
			if (xSemaphoreGive(full) != pdTRUE)
				printf("Producer unable to give full\n");
			vTaskDelay(PRODUCER_DELAY);
		}

	}

}

void consumer(void *pvParameters)
{
	int item;
	while (1)
	{
		if (xSemaphoreTake(full, CONSUMER_DELAY) != pdTRUE)
			printf("Consumer unable to take full, buffer is empty\n");
		else if (xSemaphoreTake(mutex, CONSUMER_DELAY) != pdTRUE)
			printf("Consumer unable to take mutex\n");
		else
		{
			item = buffer[out];
			printf("Consumer received item: %6d\n", item);
			out = (out + 1) % N;
			if (xSemaphoreGive(mutex) != pdTRUE)
				printf("consumer unable to give mutex\n");
			if (xSemaphoreGive(empty) != pdTRUE)
				printf("Consumer unable to give empty\n");
			vTaskDelay(CONSUMER_DELAY);
		}
	}
}