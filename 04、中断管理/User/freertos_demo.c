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
#include "./SYSTEM/delay/delay.h"
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

	//��ʼ����ִֻ��һ�Σ�ִ����ɾ���Լ����������ΪNULL������ɾ���Լ������뿪ʼ����������Ҳ����
    vTaskDelete(NULL);
    //�˳��ٽ�������ȥ����
	taskEXIT_CRITICAL();
}

//����1�����жϣ�ֹͣ��ӡ�����жϣ�������ӡ
void task1(void* pvParameters)
{
	uint8_t task1_num=0;
	while(1)
	{
		if(++task1_num==5)
		{
			task1_num=0;
			printf("���жϣ�\r\n");
			portDISABLE_INTERRUPTS();//���ж�
			delay_ms(5000);//��ʱ5�룬���ﲻ����ϵͳ�Դ�����ʱ�����������¿����ж�
			printf("���жϣ�\r\n");
			portENABLE_INTERRUPTS();//���ж�
		}
		vTaskDelay(1000);//ϵͳ�������ʱ�������ӳ�1��
	}
}
