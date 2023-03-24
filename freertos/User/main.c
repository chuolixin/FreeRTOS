#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_usart.h"

/**************************************************************************
* 函数声明
***************************************************************************/
static void AppTaskCreate(void);/* 用于创建任务 */
static void LED_Task(void* pvParameters);/* LED_Task 任务实现 */
static void System_Init(void);/* 用于初始化板载相关资源 */

/**************************** 任务句柄 ********************************/
/*
* 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
* 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
* 这个句柄可以为 NULL。
*/
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED 任务句柄 */
static TaskHandle_t LED_Task_Handle;

static void System_Init(void)
{
	/*
	* STM32 中断优先级分组为 4，即 4bit 都用来表示抢占优先级，范围为： 0~15
	* 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	* 都统一用这个优先级分组，千万不要再分组，切忌。
	*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	/* LED 初始化 */
	LED_GPIO_Config();
	/* 串口初始化 */
	USART_Config();
}

static void LED_Task (void* parameter)
{
	while (1)
	{
		LED1_ON;
		printf("led on\r\n");
		vTaskDelay(500); /* 延时 500 个 tick */
		LED1_OFF;
		printf("led off\r\n");
		vTaskDelay(500);
	}
}

/***********************************************************************
132 * @ 函数名 ： AppTaskCreate
133 * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
134 * @ 参数 ： 无
135 * @ 返回值 ： 无
136 ***************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

	taskENTER_CRITICAL(); //进入临界区
	/* 创建 LED_Task 任务 */
	xReturn = xTaskCreate( (TaskFunction_t )LED_Task, //任务函数
										(const char*)"LED_Task",//任务名称
										(uint16_t)512, //任务堆栈大小
										(void* )NULL, //传递给任务函数的参数
										(UBaseType_t)2, //任务优先级
										(TaskHandle_t*)&LED_Task_Handle );//任务控制块指针

	if (pdPASS == xReturn) /* 创建成功 */
		printf("LED_Task 任务创建成功!\n");
	else
		printf("LED_Task 任务创建失败!\n");
	vTaskDelete(AppTaskCreate_Handle); //删除 AppTaskCreate 任务
	taskEXIT_CRITICAL(); //退出临界区
}

int main(void)
{
	BaseType_t xReturn = pdPASS;
	
	System_Init();
	
	/* 创建 AppTaskCreate 任务 */
	xReturn = xTaskCreate( (TaskFunction_t)AppTaskCreate, 
							(const char*)"AppTaskCreate",//任务名称
							(uint16_t)512, //任务堆栈大小
							(void*)NULL, //传递给任务函数的参数
							(UBaseType_t)1, //任务优先级
							(TaskHandle_t*)&AppTaskCreate_Handle ); //任务控制块指针
	if (pdPASS == xReturn) /* 创建成功 */
	{
		vTaskStartScheduler(); /* 启动任务，开启调度 */
	}
	else
	{
		return -1;
	}
	while(1);
}

