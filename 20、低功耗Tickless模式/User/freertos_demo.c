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
 SemaphoreHandle_t semaphore;
 //����͹���ǰ
 void PRE_SLEEP_PROCESSING(void)
 {
	 __HAL_RCC_GPIOA_CLK_DISABLE();
	 __HAL_RCC_GPIOB_CLK_DISABLE();
	 __HAL_RCC_GPIOC_CLK_DISABLE();
	 __HAL_RCC_GPIOD_CLK_DISABLE();
	 __HAL_RCC_GPIOE_CLK_DISABLE();
	 __HAL_RCC_GPIOF_CLK_DISABLE();
	 __HAL_RCC_GPIOG_CLK_DISABLE();	 
 }
 //�˳��͹��ĺ�
 void POST_SLEEP_PROCESSING(void)
 {
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 __HAL_RCC_GPIOD_CLK_ENABLE();
	 __HAL_RCC_GPIOE_CLK_ENABLE();
	 __HAL_RCC_GPIOF_CLK_ENABLE();
	 __HAL_RCC_GPIOG_CLK_ENABLE();
 }

void freertos_demo(void)
{   
	semaphore=xSemaphoreCreateBinary();
	if(semaphore==NULL)
	{
		printf("�ź�������ʧ��\n");
	}else printf("�ź��������ɹ�\n");
	                        
	
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

//����1��key0���£��ͷŶ�ֵ�ź���
void task1(void* pvParameters)
{
	uint8_t key=0;
	BaseType_t status1;
	while(1)
	{
		key=key_scan();

		if(key==KEY0_PRES)
		{
			status1=xSemaphoreGive(semaphore);
			if(status1==pdTRUE)
				printf("�ź����ͷųɹ���\n");
		}
		vTaskDelay(200);//ϵͳ�������ʱ����

	}
}

//����2����ȡ��ֵ�ź������ɹ���ȡ��ӡ��Ϣ
void task2(void* pvParameters)
{
	BaseType_t status2=0;
	while(1)
	{
		 status2=xSemaphoreTake(semaphore,portMAX_DELAY);
		 if(status2==pdTRUE)
			printf("�ź�����ȡ�ɹ���\n");
	
	}
}
