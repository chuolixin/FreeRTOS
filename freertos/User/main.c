#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

/**************************************************************************
* 函数声明
***************************************************************************/
static void AppTaskCreate(void);/* 用于创建任务 */
static void LED1_Task(void* pvParameters);/* LED1_Task 任务实现 */
static void System_Init(void);/* 用于初始化板载相关资源 */

/**************************** 任务句柄 ********************************/
/*
* 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
* 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
* 这个句柄可以为 NULL。
*/
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED1 任务句柄 */
static TaskHandle_t LED1_Task_Handle;
/* LED2 任务句柄 */
static TaskHandle_t LED2_Task_Handle;
/* KEY 任务句柄 */
static TaskHandle_t KEY_Task_Handle;

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
	/* 按键初始化 */
	Key_GPIO_Config();
}

static void LED1_Task (void* parameter)
{
	while (1)
	{
		LED1_ON;
		printf("led 1 on\r\n");
		vTaskDelay(500); /* 延时 500 个 tick */
		LED1_OFF;
		printf("led 1 off\r\n");
		vTaskDelay(500);
	}
}

static void LED2_Task ( void* parameter )
{
	while (1)
	{
		LED2_ON;
		printf("led 2 on\r\n");
		vTaskDelay(100); /* 延时 500 个 tick */
		LED2_OFF;
		printf("led 2 off\r\n");
		vTaskDelay(100);
	}
}

static void KEY_Task ( void* parameter )
{
	while (1)
	{
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
		{
			printf("Suspend LED1 LED2\r\n");
			vTaskSuspend(LED1_Task_Handle);
			vTaskSuspend(LED2_Task_Handle);
		}
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
		{
			printf("Resume LED1 LED2\r\n");
			vTaskResume(LED1_Task_Handle);
			vTaskResume(LED2_Task_Handle);
		}
		vTaskDelay(20);		
	}
}

/***********************************************************************
132 * @ 函数名 ： AppTaskCreate
133 * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
134 * @ 参数 ： 无
135 * @ 返回值 ： 无
136 ***************************************************************/
static void AppTaskCreate( void )
{
	BaseType_t xReturn = pdPASS;

	taskENTER_CRITICAL(); //进入临界区
	/* 创建 LED1_Task 任务 */
	xReturn = xTaskCreate( (TaskFunction_t )LED1_Task, //任务函数
										(const char*)"LED1_Task",//任务名称
										(uint16_t)512, //任务堆栈大小
										(void* )NULL, //传递给任务函数的参数
										(UBaseType_t)2, //任务优先级
										(TaskHandle_t*)&LED1_Task_Handle );//任务控制块指针
	if (pdPASS == xReturn) /* 创建成功 */
		printf("LED1_Task 任务创建成功!\n");
	else
		printf("LED1_Task 任务创建失败!\n");
	/* 创建 LED1_Task 任务 */
	xReturn = xTaskCreate( (TaskFunction_t )LED2_Task, //任务函数
										(const char*)"LED2_Task",//任务名称
										(uint16_t)512, //任务堆栈大小
										(void* )NULL, //传递给任务函数的参数
										(UBaseType_t)3, //任务优先级
										(TaskHandle_t*)&LED2_Task_Handle );//任务控制块指针

	if (pdPASS == xReturn) /* 创建成功 */
		printf("LED2_Task 任务创建成功!\n");
	else
		printf("LED2_Task 任务创建失败!\n");
		/* 创建 LED1_Task 任务 */
	xReturn = xTaskCreate( (TaskFunction_t )KEY_Task, //任务函数
										(const char*)"KEY_Task",//任务名称
										(uint16_t)512, //任务堆栈大小
										(void* )NULL, //传递给任务函数的参数
										(UBaseType_t)4, //任务优先级
										(TaskHandle_t*)&KEY_Task_Handle );//任务控制块指针

	if (pdPASS == xReturn) /* 创建成功 */
		printf("KEY_Task 任务创建成功!\n");
	else
		printf("KEY_Task 任务创建失败!\n");
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

