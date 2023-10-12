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
	xTaskCreate((TaskFunction_t        ) start_task,//ָ����������ָ��
				(char *                ) "start_task",//��������
			   (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,//��ʼ�����ջ��С����λΪ��
			   (void *                ) NULL,//����������
			   (UBaseType_t           ) START_TASK_PRIO,//�������ȼ�����ֵԽ�����ȼ�Խ��
			   //��������ָ�������������ƿ飬���񴴽��ɹ����ظþ��
			   (TaskHandle_t *        ) &start_task_handler );
    vTaskStartScheduler();//�������������
}

/*��������д
1����ʼ���񣺴�����������ֻ����һ�μ��ɣ�
*/
void start_task(void* pvParameters)
{
	//�Ƚ����ٽ���
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t        ) task1,//ָ����������ָ��
				(char *                ) "task1",//��������
			   (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//����������
			   (UBaseType_t           ) TASK1_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &task1_handler );//��������ָ�������������ƿ�

	xTaskCreate((TaskFunction_t        ) task2,//ָ����������ָ��
				(char *                ) "task2",//��������
			   (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//����������
			   (UBaseType_t           ) TASK2_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &task2_handler );//��������ָ�������������ƿ�

	xTaskCreate((TaskFunction_t        ) task3,//ָ��ʼ��������ָ��
				(char *                ) "task3",//��������
			   (configSTACK_DEPTH_TYPE) TASK3_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//����������
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
		if(key==KEY0_PRES)//key0���£�ɾ��task1
		{	
			//ֻ�о����Ϊ�ղŻ�ɾ����ɾ��һ�β����ظ�ɾ��
			if(task1_handler!=NULL)
			{
				printf("ɾ��task1����\r\n");
				vTaskDelete(task1_handler);
				task1_handler=NULL;
			}
		}
		vTaskDelay(10);
	}
}

