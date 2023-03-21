#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_usart.h"


void system_init(void)
{
	LED_GPIO_Config();
	USART_Config();
}

int main(void)
{	
	system_init();
	LED1_ON;
	Usart_SendString( DEBUG_USARTx,"test usart send\n");
	printf("xxx\n\n\n\n");
	
	while(1)
	{	

	}
}
