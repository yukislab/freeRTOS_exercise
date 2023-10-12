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

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
QueueHandle_t queue_handle;
SemaphoreHandle_t semaphore_handle;
QueueSetHandle_t queue_set_handle;

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

//��ʼ���񣺴���һ�����С�һ���ź�������������ֻ����һ�μ��ɣ�
void start_task(void* pvParameters)
{
	//�Ƚ����ٽ���
	taskENTER_CRITICAL();
	
	queue_set_handle=xQueueCreateSet(2);
	
	if(semaphore_handle!=NULL&&queue_handle!=NULL&&queue_set_handle!=NULL)
	{
		printf("���м������ɹ�\n");
	}
	
	queue_handle=xQueueCreate(1,sizeof(uint8_t));
	semaphore_handle=xSemaphoreCreateBinary();

	 if(xQueueAddToSet(queue_handle,queue_set_handle)==pdPASS)
		 printf("������ӵ����м��У�\n");
	 if(xQueueAddToSet(semaphore_handle,queue_set_handle)==pdPASS)
		 printf("�ź�����ӵ����м��У�\n");	

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

//����1��key0���£�������д���ݣ�key1���£��ͷŶ�ֵ�ź���
void task1(void* pvParameters)
{
	uint8_t key=0;
	while(1)
	{
		key=key_scan();

		if(key==KEY0_PRES)
		{
			if(xQueueSend(queue_handle,&key,portMAX_DELAY)==pdPASS)
				printf("д���гɹ���\n");
		}
		else if(key==KEY1_PRES)
		{
			if(xSemaphoreGive(semaphore_handle)==pdPASS)
				printf("�ͷ��ź����ɹ���\n");
		}
		
		vTaskDelay(10);//ϵͳ�������ʱ����
	}
}

//����2����ȡ���м���Ϣ������ӡ
void task2(void* pvParameters)
{
	QueueSetMemberHandle_t queue_member_handle;
	uint8_t key;
	while(1)
	{
		queue_member_handle=xQueueSelectFromSet(queue_set_handle,portMAX_DELAY);
		if(queue_member_handle==queue_handle)
		{
			xQueueReceive(queue_member_handle,&key,portMAX_DELAY);
			printf("��ȡ���Ķ��������ǣ�%d\n",key);
		}
		else if(queue_member_handle==semaphore_handle)
		{
			xSemaphoreTake(queue_member_handle,portMAX_DELAY);
			printf("�ź�����ȡ�ɹ���\n");
		}
	}
}
