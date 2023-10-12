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

/* HIGH ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define HIGH_PRIO       4//�߽������������ȼ�
#define HIGH_TASK_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t high_task_handler;//������
void high_task(void* pvParameters);

/* MIDDLE ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define MIDDLE_PRIO       3//�м������������ȼ�
#define MIDDLE_TASK_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t middle_task_handler;//������
void middle_task(void* pvParameters);

/* LOW ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LOW_PRIO       2//�ͽ������������ȼ�
#define LOW_TASK_STACK_SIZE 128//��λ���֣�������һ��
TaskHandle_t low_task_handler;//������
void low_task(void* pvParameters);

/******************************************************************************************************/

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
SemaphoreHandle_t semphore_handle=NULL;

void freertos_demo(void)
{   
	semphore_handle=xSemaphoreCreateBinary();
	if(semphore_handle==NULL)
	{
		printf("�ź�������ʧ��\n");
	}else printf("�ź��������ɹ�\n");
	xSemaphoreGive(semphore_handle);                        
	
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

//��ʼ����ʵ�ִ�����������ֻ����һ�μ��ɣ�
void start_task(void* pvParameters)
{
	//�Ƚ����ٽ���
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t        ) high_task,//ָ����������ָ��
				(char *                ) "high_task",//��������
			   (configSTACK_DEPTH_TYPE) HIGH_TASK_STACK_SIZE,//�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) HIGH_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &high_task_handler );//�����������������������ƿ�

			   
	xTaskCreate((TaskFunction_t        ) middle_task,//ָ����������ָ��
				(char *                ) "middle_task",//��������
			   (configSTACK_DEPTH_TYPE) MIDDLE_TASK_STACK_SIZE,//�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) MIDDLE_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &middle_task_handler );//�����������������������ƿ�

	xTaskCreate((TaskFunction_t        ) low_task,//ָ��ʼ��������ָ��
				(char *                ) "low_task",//��������
			   (configSTACK_DEPTH_TYPE) LOW_TASK_STACK_SIZE,//��ʼ�����ջ��С
			   (void *                ) NULL,//��ڲ���
			   (UBaseType_t           ) LOW_PRIO,//�������ȼ�
			   (TaskHandle_t *        ) &low_task_handler );//�����������������������ƿ�

	//��ʼ����ִֻ��һ�Σ�ִ����ɾ���Լ����������ΪNULL������ɾ���Լ������뿪ʼ����������Ҳ����
    vTaskDelete(NULL);
    //�˳��ٽ�������ȥ����
	taskEXIT_CRITICAL();
}

//�����ȼ����񣺻�ȡ��ֵ�ź������ɹ���ӡ���������ͷ�
void high_task(void* pvParameters)
{
	while(1)
	{
	 if( semphore_handle != NULL )
        if( xSemaphoreTake( semphore_handle, portMAX_DELAY) == pdTRUE )
		{   
			printf("�����ȼ������ȡ�ɹ���\n");
			printf("�����ȼ������������У�\n");
			delay_ms(1000);//ϵͳ�������ʱ����
			printf("�ͷ��ź�����\n");
			xSemaphoreGive(semphore_handle);
			vTaskDelay(1000);//�ͷ�֮����ʱ1��
		}
	}
}

//�����ȼ����񣺼򵥵�Ӧ������
void middle_task(void* pvParameters)
{
	while(1)
	{
		printf("�����ȼ������������У�\n");
		vTaskDelay(1000);//�ͷ�֮����ʱ1��
	}
}


//�����ȼ�����ͬ�����ȼ�������ռ��ʱ�����
void low_task(void* pvParameters)
{
	while(1)
	{
	 if( semphore_handle != NULL )
        if( xSemaphoreTake( semphore_handle, portMAX_DELAY) == pdTRUE )
		{   
			printf("�����ȼ������ȡ�ɹ���\n");
			printf("�����ȼ������������У�\n");
			delay_ms(3000);//ϵͳ�������ʱ����
			printf("�ͷ��ź�����\n");
			xSemaphoreGive(semphore_handle);
			vTaskDelay(1000);//�ͷ�֮����ʱ1��
		}
	}
}
