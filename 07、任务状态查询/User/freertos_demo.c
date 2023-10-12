#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
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
		LED0_TOGGLE();
		vTaskDelay(500);//系统自身的延时函数
	}
}

//任务2：实现状态查询API函数的使用
void task2(void* pvParameters)
{
	UBaseType_t priority_num=0;
	UBaseType_t task_num=0;
	UBaseType_t task_num1=0;
	TaskStatus_t * status_array=0;
	//指向数组，数组元素为任务状态结构体
	uint8_t i=0;

	priority_num=uxTaskPriorityGet(task1_handler);
	printf("task1任务优先级为%1d\n",priority_num);
	
	vTaskPrioritySet(task1_handler,3);
	priority_num=uxTaskPriorityGet(task1_handler);
	printf("task1新设置的任务优先级为%1d\n",priority_num);

	task_num=uxTaskGetNumberOfTasks();
	printf("当前系统任务数目为%1d\n",task_num);
	
//获取所有任务状态信息
	//动态申请数组，参数1存储位置，参数2数组大小
	status_array=mymalloc(SRAMIN,sizeof(TaskStatus_t)*task_num);
	task_num1=uxTaskGetSystemState(status_array,task_num,NULL);
	printf("任务名\t\t任务优先级\t\t任务编号\r\n");

	//任务很多，需要循环打印
	for(i=0;i<task_num1;i++)
	{
		printf("%s\t\t%1d\t\t%1d\r\n",
				status_array[i].pcTaskName,
				status_array[i].uxCurrentPriority,
				status_array[i].xTaskNumber);//任务编号：第几个创建的
	}
//获取单个任务状态信息
	TaskStatus_t task1_info;
	vTaskGetInfo(task1_handler,&task1_info,pdTRUE,eInvalid);
	printf("任务名\t\t任务优先级\t\t任务编号\r\n");
	printf("%s\t\t%1d\t\t%1d\r\n",
			task1_info.pcTaskName,
			task1_info.uxCurrentPriority,
			task1_info.xTaskNumber);//任务编号：第几个创建的

//获取任务句柄
	TaskHandle_t task1_handle;
	task1_handle=xTaskGetHandle("task1");
	printf("获取到的任务句柄地址为：%#x\r\n",(int)task1_handle);
	printf("真实的任务句柄地址为：%#x\r\n",(int)task1_handler);
	
//任务高水位标记，输出值为0说明溢出
	UBaseType_t water_mark=0;
	water_mark=uxTaskGetStackHighWaterMark(task1_handler);
	printf("当前任务的高水位标记为：%#x\r\n",water_mark);




	while(1)
	{
	vTaskDelay(1000);
	}
}
