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
SemaphoreHandle_t count_semphore_handle;

void freertos_demo(void)
{   
	/*计数型信号量初始值为0，没有资源，死等阻塞
	按下key0，释放信号量资源，资源为1，这时再获取，信号量-1，打印结果为0
	*/
	count_semphore_handle=xSemaphoreCreateCounting(10,0);
	if(count_semphore_handle==NULL)
	{
		printf("信号量创建失败\n");
	}else printf("信号量创建成功\n");
	                        
	
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

//任务1：key0按下，释放计数型信号量，按键按下加资源
void task1(void* pvParameters)
{
	uint8_t key=0;
	while(1)
	{
		key=key_scan();

		if(key==KEY0_PRES)
		{
			//计数型信号量不为空则释放
			if(count_semphore_handle!=NULL)
				xSemaphoreGive(count_semphore_handle);
		}
		vTaskDelay(10);//系统自身的延时函数

	}
}

//任务2：每过一秒获取一次计数型信号量，不按按键每隔一秒加资源，当成功获取后打印信号量计数值
void task2(void* pvParameters)
{
	BaseType_t status2=0;
	while(1)
	{
		 status2=xSemaphoreTake(count_semphore_handle,portMAX_DELAY);
		 if(status2==pdTRUE)
			printf("信号量获取成功！数值为%d\n",(int)uxSemaphoreGetCount(count_semphore_handle));
		 vTaskDelay(1000);//系统自身的延时函数
	}
}
