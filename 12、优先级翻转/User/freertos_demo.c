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

/* HIGH 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define HIGH_PRIO       4//高阶任务任务优先级
#define HIGH_TASK_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t high_task_handler;//任务句柄
void high_task(void* pvParameters);

/* MIDDLE 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define MIDDLE_PRIO       3//中间任务任务优先级
#define MIDDLE_TASK_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t middle_task_handler;//任务句柄
void middle_task(void* pvParameters);

/* LOW 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LOW_PRIO       2//低阶任务任务优先级
#define LOW_TASK_STACK_SIZE 128//单位是字，尽量大一点
TaskHandle_t low_task_handler;//任务句柄
void low_task(void* pvParameters);

/******************************************************************************************************/

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
SemaphoreHandle_t semphore_handle=NULL;

void freertos_demo(void)
{   
	semphore_handle=xSemaphoreCreateBinary();
	if(semphore_handle==NULL)
	{
		printf("信号量创建失败\n");
	}else printf("信号量创建成功\n");
	xSemaphoreGive(semphore_handle);                        
	
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

//开始任务：实现创建其它任务（只创建一次即可）
void start_task(void* pvParameters)
{
	//先进入临界区
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t        ) high_task,//指向任务函数的指针
				(char *                ) "high_task",//任务名字
			   (configSTACK_DEPTH_TYPE) HIGH_TASK_STACK_SIZE,//任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) HIGH_PRIO,//任务优先级
			   (TaskHandle_t *        ) &high_task_handler );//任务句柄，就是任务的任务控制块

			   
	xTaskCreate((TaskFunction_t        ) middle_task,//指向任务函数的指针
				(char *                ) "middle_task",//任务名字
			   (configSTACK_DEPTH_TYPE) MIDDLE_TASK_STACK_SIZE,//任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) MIDDLE_PRIO,//任务优先级
			   (TaskHandle_t *        ) &middle_task_handler );//任务句柄，就是任务的任务控制块

	xTaskCreate((TaskFunction_t        ) low_task,//指向开始任务函数的指针
				(char *                ) "low_task",//任务名字
			   (configSTACK_DEPTH_TYPE) LOW_TASK_STACK_SIZE,//开始任务堆栈大小
			   (void *                ) NULL,//入口参数
			   (UBaseType_t           ) LOW_PRIO,//任务优先级
			   (TaskHandle_t *        ) &low_task_handler );//任务句柄，就是任务的任务控制块

	//开始任务只执行一次，执行完删除自己，传入参数为NULL，代表删除自己，传入开始任务任务句柄也可以
    vTaskDelete(NULL);
    //退出临界区后再去调度
	taskEXIT_CRITICAL();
}

//高优先级任务：获取二值信号量，成功打印，处理完释放
void high_task(void* pvParameters)
{
	while(1)
	{
	 if( semphore_handle != NULL )
        if( xSemaphoreTake( semphore_handle, portMAX_DELAY) == pdTRUE )
		{   
			printf("高优先级任务获取成功！\n");
			printf("高优先级任务正在运行！\n");
			delay_ms(1000);//系统自身的延时函数
			printf("释放信号量！\n");
			xSemaphoreGive(semphore_handle);
			vTaskDelay(1000);//释放之后延时1秒
		}
	}
}

//中优先级任务：简单的应用任务
void middle_task(void* pvParameters)
{
	while(1)
	{
		printf("中优先级任务正在运行！\n");
		vTaskDelay(1000);//释放之后延时1秒
	}
}


//低优先级任务：同高优先级，但是占用时间更久
void low_task(void* pvParameters)
{
	while(1)
	{
	 if( semphore_handle != NULL )
        if( xSemaphoreTake( semphore_handle, portMAX_DELAY) == pdTRUE )
		{   
			printf("低优先级任务获取成功！\n");
			printf("低优先级任务正在运行！\n");
			delay_ms(3000);//系统自身的延时函数
			printf("释放信号量！\n");
			xSemaphoreGive(semphore_handle);
			vTaskDelay(1000);//释放之后延时1秒
		}
	}
}
