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

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO       3//��ʼ�����������ȼ�
#define TASK2_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t task2_handler;//������
void task2(void* pvParameters);
/******************************************************************************************************/

//�����б�ṹ�塢�б���
List_t     TestList;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;


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
		LED0_TOGGLE();
		vTaskDelay(500);//ϵͳ�������ʱ����
	}
}

//����2���б�������ɾ��
void task2(void* pvParameters)
{
	//��ʼ���б���б���
	vListInitialise(&TestList);
	vListInitialiseItem(&ListItem1);
	vListInitialiseItem(&ListItem2);
	vListInitialiseItem(&ListItem3);
	
	//�������
	ListItem1.xItemValue=40;
	ListItem2.xItemValue=60;
	ListItem3.xItemValue=50;
	
	//��ӡ��ַ����֤ʵ����
    printf("/**************�ڶ�������ӡ�б���б���ĵ�ַ**************/\r\n");
    printf("��Ŀ\t\t\t��ַ\r\n");
    printf("TestList\t\t0x%p\t\r\n", &TestList);//�б��ַ
    printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);//�б����ַ
    printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));//ĩβ�б����ַ
    printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
    printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
    printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
    printf("/**************************����***************************/\r\n");

	//��������б���1
	printf("\r\n/*****************���������б���1�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem1);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//��һ������һ��
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    //���һ����ǰһ��
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//��һ����ǰһ��
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************����***************************/\r\n");

	//��������б���2
	printf("\r\n/*****************���Ĳ����б���2�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem2);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//��һ������һ��
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    //���һ����ǰһ��
	printf("ListItem1->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//��һ����ǰһ��
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("/**************************����***************************/\r\n");

	//β���б���3
	printf("\r\n/*****************���岽���б���3�����б�******************/\r\n");
    vListInsertEnd((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem3);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//��һ������һ��
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    //���һ����ǰһ��
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//��һ����ǰһ��
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************����***************************/\r\n");

	//�Ƴ��б���3
	printf("\r\n/*****************���������Ƴ��б���3******************/\r\n");
	uxListRemove(&ListItem3);
	printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	//��һ������һ��
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    //���һ����ǰһ��
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	//��һ����ǰһ��
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************����***************************/\r\n");


	while(1)
	{
		vTaskDelay(1000);
	}
}
