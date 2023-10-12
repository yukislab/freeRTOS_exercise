#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "timers.h"
#include "./MALLOC/malloc.h"
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
TimerHandle_t timer1_handle;
TimerHandle_t timer2_handle;
void timer1_callback();
void timer2_callback();


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


//��ʼ���񣺴������񣬴���������ʱ�������κ����ڣ�
void start_task(void* pvParameters)
{
	//�Ƚ����ٽ���
	taskENTER_CRITICAL();
	
	timer1_handle=xTimerCreate("timer1",500,pdFALSE,(void*)1,timer1_callback);
	timer2_handle=xTimerCreate("timer2",500,pdTRUE,(void*)1,timer2_callback);

	if(timer1_handle!=NULL&&timer2_handle!=NULL)
		printf("��ʱ�������ɹ���\n");
	
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

//����1������ɨ�裬������ֹͣ�����ʱ��
void task1(void* pvParameters)
{
	uint8_t key=0;
	while(1)
	{
		key=key_scan();

		if(key==KEY0_PRES)
		{
			 xTimerStart(timer1_handle,portMAX_DELAY);
			 xTimerStart(timer2_handle,portMAX_DELAY);
		}
		else if(key==KEY1_PRES)
		{
			 xTimerStop(timer1_handle,portMAX_DELAY);
			 xTimerStop(timer2_handle,portMAX_DELAY);
		}
		vTaskDelay(150);//ϵͳ�������ʱ����
	}
}

void timer1_callback()
{
	uint8_t count1=1;
	printf("���ζ�ʱ������%d��\n",count1);
	count1++;
}
void timer2_callback()
{
	uint8_t count2=1;
	printf("���ڶ�ʱ������%d��\n",count2);
	count2++;

}
