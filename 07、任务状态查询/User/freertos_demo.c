#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
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

//����2��ʵ��״̬��ѯAPI������ʹ��
void task2(void* pvParameters)
{
	UBaseType_t priority_num=0;
	UBaseType_t task_num=0;
	UBaseType_t task_num1=0;
	TaskStatus_t * status_array=0;
	//ָ�����飬����Ԫ��Ϊ����״̬�ṹ��
	uint8_t i=0;

	priority_num=uxTaskPriorityGet(task1_handler);
	printf("task1�������ȼ�Ϊ%1d\n",priority_num);
	
	vTaskPrioritySet(task1_handler,3);
	priority_num=uxTaskPriorityGet(task1_handler);
	printf("task1�����õ��������ȼ�Ϊ%1d\n",priority_num);

	task_num=uxTaskGetNumberOfTasks();
	printf("��ǰϵͳ������ĿΪ%1d\n",task_num);
	
//��ȡ��������״̬��Ϣ
	//��̬�������飬����1�洢λ�ã�����2�����С
	status_array=mymalloc(SRAMIN,sizeof(TaskStatus_t)*task_num);
	task_num1=uxTaskGetSystemState(status_array,task_num,NULL);
	printf("������\t\t�������ȼ�\t\t������\r\n");

	//����ܶ࣬��Ҫѭ����ӡ
	for(i=0;i<task_num1;i++)
	{
		printf("%s\t\t%1d\t\t%1d\r\n",
				status_array[i].pcTaskName,
				status_array[i].uxCurrentPriority,
				status_array[i].xTaskNumber);//�����ţ��ڼ���������
	}
//��ȡ��������״̬��Ϣ
	TaskStatus_t task1_info;
	vTaskGetInfo(task1_handler,&task1_info,pdTRUE,eInvalid);
	printf("������\t\t�������ȼ�\t\t������\r\n");
	printf("%s\t\t%1d\t\t%1d\r\n",
			task1_info.pcTaskName,
			task1_info.uxCurrentPriority,
			task1_info.xTaskNumber);//�����ţ��ڼ���������

//��ȡ������
	TaskHandle_t task1_handle;
	task1_handle=xTaskGetHandle("task1");
	printf("��ȡ������������ַΪ��%#x\r\n",(int)task1_handle);
	printf("��ʵ����������ַΪ��%#x\r\n",(int)task1_handler);
	
//�����ˮλ��ǣ����ֵΪ0˵�����
	UBaseType_t water_mark=0;
	water_mark=uxTaskGetStackHighWaterMark(task1_handler);
	printf("��ǰ����ĸ�ˮλ���Ϊ��%#x\r\n",water_mark);




	while(1)
	{
	vTaskDelay(1000);
	}
}
