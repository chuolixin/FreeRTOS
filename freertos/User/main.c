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

/* AppTaskCreate 任务任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];
/* LED 任务堆栈 */
static StackType_t LED_Task_Stack[128];
/* AppTaskCreate 任务控制块 */
static StaticTask_t AppTaskCreate_TCB;
/* AppTaskCreate 任务控制块 */
static StaticTask_t LED_Task_TCB;

/* 空闲任务任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
									StackType_t **ppxIdleTaskStackBuffer, 
									uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
 									StackType_t **ppxTimerTaskStackBuffer,
 									uint32_t *pulTimerTaskStackSize);

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
	taskENTER_CRITICAL(); //进入临界区
	/* 创建 LED_Task 任务 */
	LED_Task_Handle = xTaskCreateStatic((TaskFunction_t )LED_Task, //任务函数
										(const char*)"LED_Task",//任务名称
										(uint32_t)128, //任务堆栈大小
										(void* )NULL, //传递给任务函数的参数
										(UBaseType_t)4, //任务优先级
										(StackType_t*)LED_Task_Stack,//任务堆栈
										(StaticTask_t*)&LED_Task_TCB);//任务控制块

	if (NULL != LED_Task_Handle) /* 创建成功 */
		printf("LED_Task 任务创建成功!\n");
	else
		printf("LED_Task 任务创建失败!\n");
	vTaskDelete(AppTaskCreate_Handle); //删除 AppTaskCreate 任务
	taskEXIT_CRITICAL(); //退出临界区
}

int main(void)
{	
	System_Init();
	
	/* 创建 AppTaskCreate 任务 */
	AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate, 
											(const char* )"AppTaskCreate",//任务名称
											(uint32_t )128, //任务堆栈大小
											(void* )NULL, //传递给任务函数的参数
											(UBaseType_t )3, //任务优先级
											(StackType_t* )AppTaskCreate_Stack, //任务堆栈
											(StaticTask_t* )&AppTaskCreate_TCB); //任务控制块
	if (NULL != AppTaskCreate_Handle) /* 创建成功 */
	{
		vTaskStartScheduler(); /* 启动任务，开启调度 */
	}
	else
	{
		LED1_ON;
	}
	while(1)
	{	

	}
}

 /**
 *******************************************************************
 * @brief 获取空闲任务的任务堆栈和任务控制块内存
 * ppxTimerTaskTCBBuffer : 任务控制块内存
 * ppxTimerTaskStackBuffer : 任务堆栈内存
 * pulTimerTaskStackSize : 任务堆栈大小
 * @author fire
 * @version V1.0
 * @date 2018-xx-xx
 **********************************************************************
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* 任务控制块内存 */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* 任务堆栈内存 */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* 任务堆栈大小 */
}

 /**
 *********************************************************************
 * @brief 获取定时器任务的任务堆栈和任务控制块内存
 * ppxTimerTaskTCBBuffer : 任务控制块内存
 * ppxTimerTaskStackBuffer : 任务堆栈内存
 * pulTimerTaskStackSize : 任务堆栈大小
 * @author fire
 * @version V1.0
 * @date 2018-xx-xx
 **********************************************************************
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* 任务控制块内存 */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* 任务堆栈内存 */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* 任务堆栈大小 */
}

