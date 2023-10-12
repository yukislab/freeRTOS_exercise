/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS ��ֲʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO       1//��ʼ�����������ȼ�
#define START_TASK_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t start_task_handler;//������
StackType_t start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t start_task_tcb;
void start_task(void* pvParameters);

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO       2//��ʼ�����������ȼ�
#define TASK1_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task1_handler;//������
StackType_t task1_stack[TASK1_STACK_SIZE];
StaticTask_t task1_tcb;
void task1(void* pvParameters);

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO       3//��ʼ�����������ȼ�
#define TASK2_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task2_handler;//������
StackType_t task2_stack[TASK2_STACK_SIZE];
StaticTask_t task2_tcb;
void task2(void* pvParameters);

/* TASK3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK3_PRIO       4//��ʼ�����������ȼ�
#define TASK3_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task3_handler;//������
StackType_t task3_stack[TASK3_STACK_SIZE];
StaticTask_t task3_tcb;
void task3(void* pvParameters);
/******************************************************************************************************/

StaticTask_t idle_task_tcb;//�����������������ƿ� idle_task_stack;//�������������ջ
//�������������ջ,configMINIMAL_STACK_SIZEΪ���������ջ�ռ�
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];

//���������ڴ����
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
									StackType_t ** ppxIdleTaskStackBuffer,
									uint32_t * pulIdleTaskStackSize ) /*lint !e526 Symbol not defined as it is an application callback. */
{
	* ppxIdleTaskTCBBuffer=&idle_task_tcb;
	* ppxIdleTaskStackBuffer=idle_task_stack;//�������������ջ���Լ������������malloc����
	* pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

StaticTask_t timer_task_tcb;//���嶨ʱ��������ƿ�
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];


//�����ʱ���ڴ����
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
									 StackType_t ** ppxTimerTaskStackBuffer,
									 uint32_t * pulTimerTaskStackSize )
{
	* ppxTimerTaskTCBBuffer=&timer_task_tcb;
	* ppxTimerTaskStackBuffer=timer_task_stack;
	* pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
 
void freertos_demo(void)
{   
	//������̬��������������Ҫ��ȡ����������Ϊ����ɾ��������Ҫ�õ�
	start_task_handler = xTaskCreateStatic( (TaskFunction_t) start_task,
											(char *       ) "start_task",
											(uint32_t     ) START_TASK_STACK_SIZE,//����ջ��С
											(void *       ) NULL,
											(UBaseType_t  ) START_TASK_PRIO,//�������ȼ�
											(StackType_t *) start_task_stack,//�����ջ��Ҫ���ⶨ��
											(StaticTask_t *)&start_task_tcb );//��ʼ�����������ƿ�
	

    vTaskStartScheduler();//�������������
}

//start_task��������������������������ֻ����һ�μ��ɣ�
void start_task(void* pvParameters)
{
	//�Ƚ����ٽ���
	taskENTER_CRITICAL();

	task1_handler = xTaskCreateStatic( (TaskFunction_t) task1,
									   (char *        ) "task1",
									   (uint32_t      ) TASK1_STACK_SIZE,//����ջ��С
									   (void *        ) NULL,
									   (UBaseType_t   ) TASK1_PRIO,//�������ȼ�
									   (StackType_t  *) task1_stack,//�����ջ��Ҫ���ⶨ��
									   (StaticTask_t *) &task1_tcb );//��ʼ�����������ƿ�
	
	task2_handler = xTaskCreateStatic( (TaskFunction_t) task2,
									   (char *        ) "task2",
									   (uint32_t      ) TASK2_STACK_SIZE,//����ջ��С
									   (void *        ) NULL,
									   (UBaseType_t   ) TASK2_PRIO,//�������ȼ�
									   (StackType_t  *) task2_stack,//�����ջ��Ҫ���ⶨ��
									   (StaticTask_t *) &task2_tcb );//��ʼ�����������ƿ�

				
	task3_handler = xTaskCreateStatic( (TaskFunction_t) task3,
									   (char *        ) "task3",
									   (uint32_t      ) TASK3_STACK_SIZE,//����ջ��С
									   (void *        ) NULL,
									   (UBaseType_t   ) TASK3_PRIO,//�������ȼ�
									   (StackType_t  *) task3_stack,//�����ջ��Ҫ���ⶨ��
									   (StaticTask_t *) &task3_tcb );//��ʼ�����������ƿ�
					   
	//��ʼ����ִֻ��һ�Σ�ִ����ɾ���Լ����������ΪNULL������ɾ���Լ������뿪ʼ����������Ҳ����
    vTaskDelete(NULL);
    //�˳��ٽ�������ȥ����
	taskEXIT_CRITICAL();
}

//����1��ʵ��LED0ÿ500ms��תһ��
void task1(void* pvParameters)
{
	while(1)
	{
		printf("task1�������У�����\r\n");
		LED0_TOGGLE();
		vTaskDelay(500);//ϵͳ�������ʱ����
	}
}

//����2��ʵ��LED1ÿ500ms��תһ��
void task2(void* pvParameters)
{
	while(1)
	{
		printf("task2�������У�����\r\n");
		LED1_TOGGLE();
		vTaskDelay(500);//ϵͳ�������ʱ����
	}
}

//����3���жϰ���KEY0������KEY0ɾ��task1
void task3(void* pvParameters)
{
	uint8_t key=0;//�����ֵ
	while(1)
	{
		printf("task3�������У�����\r\n");
		key=key_scan(0);//ɨ�谴��
		if(key==KEY0_PRES)//KEY0����,ɾ��task1
		{	
			//ֻ�о����Ϊ�ղŻ�ɾ����ɾ��һ�β����ظ�ɾ��
			if(task1_handler!=NULL)
			{
				printf("ɾ��task3����\r\n");
				vTaskDelete(task1_handler);
				task1_handler=NULL;
			}
		}
		vTaskDelay(10);
	}
}

