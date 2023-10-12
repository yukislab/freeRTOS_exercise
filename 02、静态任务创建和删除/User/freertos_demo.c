/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 移植实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO       1//开始任务任务优先级
#define START_TASK_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t start_task_handler;//任务句柄
StackType_t start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t start_task_tcb;
void start_task(void* pvParameters);

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO       2//开始任务任务优先级
#define TASK1_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t task1_handler;//任务句柄
StackType_t task1_stack[TASK1_STACK_SIZE];
StaticTask_t task1_tcb;
void task1(void* pvParameters);

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO       3//开始任务任务优先级
#define TASK2_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t task2_handler;//任务句柄
StackType_t task2_stack[TASK2_STACK_SIZE];
StaticTask_t task2_tcb;
void task2(void* pvParameters);

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO       4//开始任务任务优先级
#define TASK3_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t task3_handler;//任务句柄
StackType_t task3_stack[TASK3_STACK_SIZE];
StaticTask_t task3_tcb;
void task3(void* pvParameters);
/******************************************************************************************************/

StaticTask_t idle_task_tcb;//定义空闲任务任务控制块 idle_task_stack;//空闲任务任务堆栈
//空闲任务任务堆栈,configMINIMAL_STACK_SIZE为空闲任务堆栈空间
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];

//空闲任务内存分配
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
									StackType_t ** ppxIdleTaskStackBuffer,
									uint32_t * pulIdleTaskStackSize ) /*lint !e526 Symbol not defined as it is an application callback. */
{
	* ppxIdleTaskTCBBuffer=&idle_task_tcb;
	* ppxIdleTaskStackBuffer=idle_task_stack;//空闲任务任务堆栈，自己定义数组或者malloc分配
	* pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

StaticTask_t timer_task_tcb;//定义定时器任务控制块
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];


//软件定时器内存分配
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
									 StackType_t ** ppxTimerTaskStackBuffer,
									 uint32_t * pulTimerTaskStackSize )
{
	* ppxTimerTaskTCBBuffer=&timer_task_tcb;
	* ppxTimerTaskStackBuffer=timer_task_stack;
	* pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
 
void freertos_demo(void)
{   
	//创建静态任务函数，这里需要获取任务句柄，因为后面删除任务需要用到
	start_task_handler = xTaskCreateStatic( (TaskFunction_t) start_task,
											(char *       ) "start_task",
											(uint32_t     ) START_TASK_STACK_SIZE,//任务栈大小
											(void *       ) NULL,
											(UBaseType_t  ) START_TASK_PRIO,//任务优先级
											(StackType_t *) start_task_stack,//任务堆栈需要另外定义
											(StaticTask_t *)&start_task_tcb );//开始任务的任务控制块
	

    vTaskStartScheduler();//开启任务调度器
}

//start_task的任务函数：创建其它三个任务（只创建一次即可）
void start_task(void* pvParameters)
{
	//先进入临界区
	taskENTER_CRITICAL();

	task1_handler = xTaskCreateStatic( (TaskFunction_t) task1,
									   (char *        ) "task1",
									   (uint32_t      ) TASK1_STACK_SIZE,//任务栈大小
									   (void *        ) NULL,
									   (UBaseType_t   ) TASK1_PRIO,//任务优先级
									   (StackType_t  *) task1_stack,//任务堆栈需要另外定义
									   (StaticTask_t *) &task1_tcb );//开始任务的任务控制块
	
	task2_handler = xTaskCreateStatic( (TaskFunction_t) task2,
									   (char *        ) "task2",
									   (uint32_t      ) TASK2_STACK_SIZE,//任务栈大小
									   (void *        ) NULL,
									   (UBaseType_t   ) TASK2_PRIO,//任务优先级
									   (StackType_t  *) task2_stack,//任务堆栈需要另外定义
									   (StaticTask_t *) &task2_tcb );//开始任务的任务控制块

				
	task3_handler = xTaskCreateStatic( (TaskFunction_t) task3,
									   (char *        ) "task3",
									   (uint32_t      ) TASK3_STACK_SIZE,//任务栈大小
									   (void *        ) NULL,
									   (UBaseType_t   ) TASK3_PRIO,//任务优先级
									   (StackType_t  *) task3_stack,//任务堆栈需要另外定义
									   (StaticTask_t *) &task3_tcb );//开始任务的任务控制块
					   
	//开始任务只执行一次，执行完删除自己，传入参数为NULL，代表删除自己，传入开始任务任务句柄也可以
    vTaskDelete(NULL);
    //退出临界区后再去调度
	taskEXIT_CRITICAL();
}

//任务1：实现LED0每500ms翻转一次
void task1(void* pvParameters)
{
	while(1)
	{
		printf("task1正在运行！！！\r\n");
		LED0_TOGGLE();
		vTaskDelay(500);//系统自身的延时函数
	}
}

//任务2：实现LED1每500ms翻转一次
void task2(void* pvParameters)
{
	while(1)
	{
		printf("task2正在运行！！！\r\n");
		LED1_TOGGLE();
		vTaskDelay(500);//系统自身的延时函数
	}
}

//任务3：判断按键KEY0，按下KEY0删除task1
void task3(void* pvParameters)
{
	uint8_t key=0;//保存键值
	while(1)
	{
		printf("task3正在运行！！！\r\n");
		key=key_scan(0);//扫描按键
		if(key==KEY0_PRES)//KEY0按下,删除task1
		{	
			//只有句柄不为空才会删除，删除一次不会重复删除
			if(task1_handler!=NULL)
			{
				printf("删除task3任务\r\n");
				vTaskDelete(task1_handler);
				task1_handler=NULL;
			}
		}
		vTaskDelay(10);
	}
}

