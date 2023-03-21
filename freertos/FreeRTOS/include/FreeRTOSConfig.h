#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f10x.h"
#include "bsp_usart.h"

//针对不同的编译器调用不同的 stdint.h 文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__) 
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif
//断言
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__) 

#define configUSE_PREEMPTION		1//置 1： RTOS 使用抢占式调度器；置 0： RTOS 使用协作式调度器（时间片）
#define configUSE_IDLE_HOOK			0//空闲钩子函数
#define configUSE_TICK_HOOK			0//时间片钩子函数
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )//RTOS 系统节拍中断的频率 即一秒中断的次数
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )//空闲任务默认使用的堆栈大小
#define configMAX_TASK_NAME_LEN		( 16 )//任务名字字符长度
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0//系统节拍计数器变量数据类型
#define configIDLE_SHOULD_YIELD		1//控制任务在空闲优先级中的行为， 空闲任务放弃 CPU 使用权给其他同优先级的用户任务。
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15
#define configUSE_TIME_SLICING 1 					//0:使能时间片调度 1:失能时间片调度
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1	//0:通用 1:特殊（前导零）
#define configUSE_TICKLESS_IDLE 0 					//0：保持系统节拍（tick）中断一直运行 1：使能低功耗 tickless 模式
#define configCPU_CLOCK_HZ (SystemCoreClock) 		
#define configMAX_PRIORITIES (32) 					//可使用的最大优先级
#define configUSE_QUEUE_SETS 1 						//启用队列
#define configUSE_TASK_NOTIFICATIONS 1 				//开启任务通知功能
#define configUSE_MUTEXES 1 						//使用互斥信号量
#define configUSE_RECURSIVE_MUTEXES 1 				//使用递归互斥信号量
#define configUSE_COUNTING_SEMAPHORES 1				//使用计数信号量 
#define configQUEUE_REGISTRY_SIZE 10 				//设置可以注册的信号量和消息队列个数
#define configUSE_APPLICATION_TASK_TAG 0			//使用跟踪宏?
#define configSUPPORT_DYNAMIC_ALLOCATION 1 			//支持动态申请
#define configSUPPORT_STATIC_ALLOCATION 1			//支持静态内存
#define configTOTAL_HEAP_SIZE ((size_t)(36*1024)) 	//系统所有总的堆大小
#define configUSE_MALLOC_FAILED_HOOK 0 				//内存申请失败钩子函数
#define configCHECK_FOR_STACK_OVERFLOW 0 			//这个宏定义大于 0 时启用堆栈溢出检测功能
#define configGENERATE_RUN_TIME_STATS 0 			//运行时间统计功能
#define configUSE_TRACE_FACILITY 0 					//启用可视化跟踪调试
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configUSE_CO_ROUTINES 0 					//启用协程
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 ) 		//协程的有效优先级数目
#define configUSE_TIMERS 1							//启用软件定时器 
#define configTIMER_TASK_PRIORITY ( configMAX_PRIORITIES - 1 ) //配置软件定时器任务优先级为最高优先级
#define configTIMER_QUEUE_LENGTH 10 				//允许配置多少个软件定时器
#define configTIMER_TASK_STACK_DEPTH ( configMINIMAL_STACK_SIZE * 2 ) //配置软件定时器任务堆栈大小
#define INCLUDE_xTaskGetSchedulerState 1 
#define INCLUDE_vTaskPrioritySet 1 
#define INCLUDE_uxTaskPriorityGet 1 
#define INCLUDE_vTaskDelete 1 
#define INCLUDE_vTaskCleanUpResources 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1

#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS 
#else
#define configPRIO_BITS 4 
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15 //配置中断最低优先级是15 
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5 //系统不响应比该优先级低的中断

#define configKERNEL_INTERRUPT_PRIORITY ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//0xF0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//0x50

#define xPortPendSVHandler PendSV_Handler
#define vPortSVCHandler SVC_Handler

#if ( configUSE_TRACE_FACILITY == 1 )
#include "trcRecorder.h"
#define INCLUDE_xTaskGetCurrentTaskHandle 0
#endif

#endif /* FREERTOS_CONFIG_H */

