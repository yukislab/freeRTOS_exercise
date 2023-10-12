#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "./MALLOC/malloc.h"
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
/******************************************************************************************************/

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
EventGroupHandle_t event_group_handle;
#define EVENTBIT_0  (1 << 0)
#define EVENTBIT_1  (1 << 1)


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

//开始任务：创建任务
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

	//开始任务只执行一次，执行完删除自己，传入参数为NULL，代表删除自己，传入开始任务任务句柄也可以
    vTaskDelete(NULL);
    //退出临界区后再去调度
	taskEXIT_CRITICAL();
}

//任务1：读按键键值、key0按下发送0，key1按下发送1
void task1(void* pvParameters)
{
	uint8_t key=0;
	while(1)
	{
		key=key_scan();

		if(key!=0)
		{
			printf("模拟邮箱发送\n");
			//带通知值发送消息
			xTaskNotify(task2_handler,key,eSetValueWithOverwrite);
		}
		
		vTaskDelay(150);//系统自身的延时函数
	}
}

//任务2：接收任务通知，根据接收到的数据做出相应的动作
void task2(void* pvParameters)
{
	uint32_t rev=0;
	while(1)
	{
		//参数含义：0进入函数不清除任何位；退出时清零；通知值地址
		xTaskNotifyWait(0,0xffffffff,&rev,portMAX_DELAY);
		printf("收到的通知值为%d\n",rev);
	}
}
