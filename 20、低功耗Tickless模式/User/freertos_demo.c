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
 SemaphoreHandle_t semaphore;
 //进入低功耗前
 void PRE_SLEEP_PROCESSING(void)
 {
	 __HAL_RCC_GPIOA_CLK_DISABLE();
	 __HAL_RCC_GPIOB_CLK_DISABLE();
	 __HAL_RCC_GPIOC_CLK_DISABLE();
	 __HAL_RCC_GPIOD_CLK_DISABLE();
	 __HAL_RCC_GPIOE_CLK_DISABLE();
	 __HAL_RCC_GPIOF_CLK_DISABLE();
	 __HAL_RCC_GPIOG_CLK_DISABLE();	 
 }
 //退出低功耗后
 void POST_SLEEP_PROCESSING(void)
 {
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 __HAL_RCC_GPIOD_CLK_ENABLE();
	 __HAL_RCC_GPIOE_CLK_ENABLE();
	 __HAL_RCC_GPIOF_CLK_ENABLE();
	 __HAL_RCC_GPIOG_CLK_ENABLE();
 }

void freertos_demo(void)
{   
	semaphore=xSemaphoreCreateBinary();
	if(semaphore==NULL)
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

	//开始任务只执行一次，执行完删除自己，传入参数为NULL，代表删除自己，传入开始任务任务句柄也可以
    vTaskDelete(NULL);
    //退出临界区后再去调度
	taskEXIT_CRITICAL();
}

//任务1：key0按下，释放二值信号量
void task1(void* pvParameters)
{
	uint8_t key=0;
	BaseType_t status1;
	while(1)
	{
		key=key_scan();

		if(key==KEY0_PRES)
		{
			status1=xSemaphoreGive(semaphore);
			if(status1==pdTRUE)
				printf("信号量释放成功！\n");
		}
		vTaskDelay(200);//系统自身的延时函数

	}
}

//任务2：获取二值信号量，成功获取打印信息
void task2(void* pvParameters)
{
	BaseType_t status2=0;
	while(1)
	{
		 status2=xSemaphoreTake(semaphore,portMAX_DELAY);
		 if(status2==pdTRUE)
			printf("信号量获取成功！\n");
	
	}
}
