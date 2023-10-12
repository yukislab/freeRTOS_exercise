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
#include "./SYSTEM/delay/delay.h"
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
/******************************************************************************************************/

//定义列表结构体、列表项
List_t     TestList;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;


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

//任务2：列表项插入和删除
void task2(void* pvParameters)
{
	//初始化列表和列表项
	vListInitialise(&TestList);
	vListInitialiseItem(&ListItem1);
	vListInitialiseItem(&ListItem2);
	vListInitialiseItem(&ListItem3);
	
	//升序插入
	ListItem1.xItemValue=40;
	ListItem2.xItemValue=60;
	ListItem3.xItemValue=50;
	
	//打印地址以验证实验结果
    printf("/**************第二步：打印列表和列表项的地址**************/\r\n");
    printf("项目\t\t\t地址\r\n");
    printf("TestList\t\t0x%p\t\r\n", &TestList);//列表地址
    printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);//列表项地址
    printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));//末尾列表项地址
    printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
    printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
    printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
    printf("/**************************结束***************************/\r\n");

	//升序插入列表项1
	printf("\r\n/*****************第三步：列表项1插入列表******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* 列表 */
                (ListItem_t*)&ListItem1);       /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//第一个的下一个
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    //最后一个的前一个
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//第一个的前一个
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************结束***************************/\r\n");

	//升序插入列表项2
	printf("\r\n/*****************第四步：列表项2插入列表******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* 列表 */
                (ListItem_t*)&ListItem2);       /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//第一个的下一个
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    //最后一个的前一个
	printf("ListItem1->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//第一个的前一个
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("/**************************结束***************************/\r\n");

	//尾插列表项3
	printf("\r\n/*****************第五步：列表项3插入列表******************/\r\n");
    vListInsertEnd((List_t*    )&TestList,         /* 列表 */
                (ListItem_t*)&ListItem3);       /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//第一个的下一个
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    //最后一个的前一个
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//第一个的前一个
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************结束***************************/\r\n");

	//移除列表项3
	printf("\r\n/*****************第六步：移除列表项3******************/\r\n");
	uxListRemove(&ListItem3);
	printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//第一个的下一个
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    //最后一个的前一个
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//第一个的前一个
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************结束***************************/\r\n");


	while(1)
	{
		vTaskDelay(1000);
	}
}
