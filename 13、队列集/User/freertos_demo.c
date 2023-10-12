#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
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
QueueHandle_t queue_handle;
SemaphoreHandle_t semaphore_handle;
QueueSetHandle_t queue_set_handle;

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

//开始任务：创建一个队列、一个信号量、其它任务（只创建一次即可）
void start_task(void* pvParameters)
{
	//先进入临界区
	taskENTER_CRITICAL();
	
	queue_set_handle=xQueueCreateSet(2);
	
	if(semaphore_handle!=NULL&&queue_handle!=NULL&&queue_set_handle!=NULL)
	{
		printf("队列集创建成功\n");
	}
	
	queue_handle=xQueueCreate(1,sizeof(uint8_t));
	semaphore_handle=xSemaphoreCreateBinary();

	 if(xQueueAddToSet(queue_handle,queue_set_handle)==pdPASS)
		 printf("队列添加到队列集中！\n");
	 if(xQueueAddToSet(semaphore_handle,queue_set_handle)==pdPASS)
		 printf("信号量添加到队列集中！\n");	

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

//任务1：key0按下，往队列写数据，key1按下，释放二值信号量
void task1(void* pvParameters)
{
	uint8_t key=0;
	while(1)
	{
		key=key_scan();

		if(key==KEY0_PRES)
		{
			if(xQueueSend(queue_handle,&key,portMAX_DELAY)==pdPASS)
				printf("写队列成功！\n");
		}
		else if(key==KEY1_PRES)
		{
			if(xSemaphoreGive(semaphore_handle)==pdPASS)
				printf("释放信号量成功！\n");
		}
		
		vTaskDelay(10);//系统自身的延时函数
	}
}

//任务2：读取队列集消息，并打印
void task2(void* pvParameters)
{
	QueueSetMemberHandle_t queue_member_handle;
	uint8_t key;
	while(1)
	{
		queue_member_handle=xQueueSelectFromSet(queue_set_handle,portMAX_DELAY);
		if(queue_member_handle==queue_handle)
		{
			xQueueReceive(queue_member_handle,&key,portMAX_DELAY);
			printf("获取到的队列数据是：%d\n",key);
		}
		else if(queue_member_handle==semaphore_handle)
		{
			xSemaphoreTake(queue_member_handle,portMAX_DELAY);
			printf("信号量获取成功！\n");
		}
	}
}
