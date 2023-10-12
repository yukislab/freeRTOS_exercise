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
void start_task(void* pvParameters);

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO       2//开始任务任务优先级
#define TASK1_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t task1_handler;//任务句柄
void task1(void* pvParameters);

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO       3//开始任务任务优先级
#define TASK2_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t task2_handler;//任务句柄
void task2(void* pvParameters);

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO       4//开始任务任务优先级
#define TASK3_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t task3_handler;//任务句柄
void task3(void* pvParameters);
/******************************************************************************************************/


/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{   
//任务创建函数
	//强制转换，防止警报
	xTaskCreate((TaskFunction_t        ) start_task,//指向开始任务函数的指针
				(char *                ) "start_task",//任务名字
			   (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,//开始任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) START_TASK_PRIO,//任务优先级
			   (TaskHandle_t *        ) &start_task_handler );//任务句柄，就是任务的任务控制块
    vTaskStartScheduler();//开启任务调度器
}

//开始任务：实现创建其它三个任务（只创建一次即可）
void start_task(void* pvParameters)
{
	//先进入临界区
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t        ) task1,//指向开始任务函数的指针
				(char *                ) "task1",//任务名字
			   (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//开始任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) TASK1_PRIO,//任务优先级
			   (TaskHandle_t *        ) &task1_handler );//任务句柄，就是任务的任务控制块

	xTaskCreate((TaskFunction_t        ) task2,//指向开始任务函数的指针
				(char *                ) "task2",//任务名字
			   (configSTACK_DEPTH_TYPE) TASK2_STACK_SIZE,//开始任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) TASK2_PRIO,//任务优先级
			   (TaskHandle_t *        ) &task2_handler );//任务句柄，就是任务的任务控制块
	
	xTaskCreate((TaskFunction_t        ) task3,//指向开始任务函数的指针
				(char *                ) "task3",//任务名字
			   (configSTACK_DEPTH_TYPE) TASK3_STACK_SIZE,//开始任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) TASK3_PRIO,//任务优先级
			   (TaskHandle_t *        ) &task3_handler );//任务句柄，就是任务的任务控制块

	//开始任务只执行一次，执行完删除自己，传入参数为NULL，代表删除自己，传入开始任务任务句柄也可以
    vTaskDelete(NULL);
    //退出临界区后再去调度
	taskEXIT_CRITICAL();
}

//任务1：实现LED0每500ms翻转一次
void task1(void* pvParameters)
{
	uint32_t task1_num=0;
	while(1)
	{
		printf("task1_num%d\r\n",++task1_num);//进来一次自增一次
		LED0_TOGGLE();
		vTaskDelay(500);//系统自身的延时函数
	}
}
//任务2：实现LED1每500ms翻转一次
void task2(void* pvParameters)
{
	uint32_t task2_num=0;
	while(1)
	{
		printf("task2_num%d\r\n",++task2_num);
		LED1_TOGGLE();
		vTaskDelay(500);//系统自身的延时函数（相对延时，调用则任务阻塞）
	}
}
//任务3：判断按键KEY0，按下KEY0挂起task1，按下KEY1恢复task1
void task3(void* pvParameters)
{
	uint8_t key=0;//保存键值
	while(1)
	{
		key=key_scan(0);//扫描按键
		if(key==KEY0_PRES)//KEY0按下,挂起task1
		{	
			vTaskSuspend(task1_handler);
		}else if(key==KEY1_PRES)
		{
			vTaskResume(task1_handler);
		}
		vTaskDelay(10);
	}
}

