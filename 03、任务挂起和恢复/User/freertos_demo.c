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
void start_task(void* pvParameters);

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO       2//��ʼ�����������ȼ�
#define TASK1_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task1_handler;//������
void task1(void* pvParameters);

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO       3//��ʼ�����������ȼ�
#define TASK2_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task2_handler;//������
void task2(void* pvParameters);

/* TASK3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK3_PRIO       4//��ʼ�����������ȼ�
#define TASK3_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task3_handler;//������
void task3(void* pvParameters);
/******************************************************************************************************/


/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{   
//���񴴽�����
	//ǿ��ת������ֹ����
	xTaskCreate((TaskFunction_t        ) start_task,//ָ��ʼ��������ָ��
				(char *                ) "start_task",//��������
			   (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) START_TASK_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &start_task_handler );//�����������������������ƿ�
    vTaskStartScheduler();//�������������
}

//��ʼ����ʵ�ִ���������������ֻ����һ�μ��ɣ�
void start_task(void* pvParameters)
{
	//�Ƚ����ٽ���
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t        ) task1,//ָ��ʼ��������ָ��
				(char *                ) "task1",//��������
			   (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) TASK1_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &task1_handler );//�����������������������ƿ�

	xTaskCreate((TaskFunction_t        ) task2,//ָ��ʼ��������ָ��
				(char *                ) "task2",//��������
			   (configSTACK_DEPTH_TYPE) TASK2_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) TASK2_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &task2_handler );//�����������������������ƿ�
	
	xTaskCreate((TaskFunction_t        ) task3,//ָ��ʼ��������ָ��
				(char *                ) "task3",//��������
			   (configSTACK_DEPTH_TYPE) TASK3_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) TASK3_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &task3_handler );//�����������������������ƿ�

	//��ʼ����ִֻ��һ�Σ�ִ����ɾ���Լ����������ΪNULL������ɾ���Լ������뿪ʼ����������Ҳ����
    vTaskDelete(NULL);
    //�˳��ٽ�������ȥ����
	taskEXIT_CRITICAL();
}

//����1��ʵ��LED0ÿ500ms��תһ��
void task1(void* pvParameters)
{
	uint32_t task1_num=0;
	while(1)
	{
		printf("task1_num%d\r\n",++task1_num);//����һ������һ��
		LED0_TOGGLE();
		vTaskDelay(500);//ϵͳ�������ʱ����
	}
}
//����2��ʵ��LED1ÿ500ms��תһ��
void task2(void* pvParameters)
{
	uint32_t task2_num=0;
	while(1)
	{
		printf("task2_num%d\r\n",++task2_num);
		LED1_TOGGLE();
		vTaskDelay(500);//ϵͳ�������ʱ�����������ʱ������������������
	}
}
//����3���жϰ���KEY0������KEY0����task1������KEY1�ָ�task1
void task3(void* pvParameters)
{
	uint8_t key=0;//�����ֵ
	while(1)
	{
		key=key_scan(0);//ɨ�谴��
		if(key==KEY0_PRES)//KEY0����,����task1
		{	
			vTaskSuspend(task1_handler);
		}else if(key==KEY1_PRES)
		{
			vTaskResume(task1_handler);
		}
		vTaskDelay(10);
	}
}

