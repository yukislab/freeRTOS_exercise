/**
 ****************************************************************************************************
 * @file        btim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ������ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211216
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "./SYSTEM/usart/usart.h"


TIM_HandleTypeDef g_timx_handle;  /* ��ʱ��6��� */
TIM_HandleTypeDef g_tim7_handle;  /* ��ʱ��7��� */


/**
 * @brief       ������ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
//��ʱ��6��ʼ������
void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
	//����ʵ��
    g_timx_handle.Instance = BTIM_TIMX_INT;  	/* ͨ�ö�ʱ��X */
    //��Ƶ����
	g_timx_handle.Init.Prescaler = psc;	/* ����Ԥ��Ƶϵ�� */
    //����ģʽ
	g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    g_timx_handle.Init.Period = arr;                             /* �Զ�װ��ֵ */
    //��ʱ����ʼ������
	HAL_TIM_Base_Init(&g_timx_handle);
	//��ʱ��ʹ��
    HAL_TIM_Base_Start_IT(&g_timx_handle);    /* ʹ�ܶ�ʱ��x��������ж� */
}

//��ʱ��7��ʼ������
void btim_tim7_int_init(uint16_t arr, uint16_t psc)
{
    g_tim7_handle.Instance = BTIM_TIM7_INT;                      /* ͨ�ö�ʱ��X */
    g_tim7_handle.Init.Prescaler = psc;                          /* ����Ԥ��Ƶϵ�� */
    g_tim7_handle.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    g_tim7_handle.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_tim7_handle);
	/* ʹ�ܶ�ʱ��7��������ж� */
    HAL_TIM_Base_Start_IT(&g_tim7_handle);    
}


/**
 * @brief       ��ʱ���ײ�����������ʱ��6��7�������ж����ȼ�
                �˺����ᱻHAL_TIM_Base_Init()��������
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//�ж��Ƿ�Ϊ��ʱ��6
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                     /* ʹ��TIMʱ�� */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 6, 0); /* ��ռ6�������ȼ�0����2 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);         /* ����ITM3�ж� */
    }
	
	//������ʱ��7
	if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIM7_INT_CLK_ENABLE();                     /* ʹ��TIMʱ�� */
        HAL_NVIC_SetPriority(BTIM_TIM7_INT_IRQn, 4, 0); /* ��ռ4�������ȼ�0����2 */
        HAL_NVIC_EnableIRQ(BTIM_TIM7_INT_IRQn);         /* ����ITM3�ж� */
    }

}

/**
 * @brief       ��ʱ��TIMX�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handle); /* ��ʱ���жϹ��������� */
}

void BTIM_TIM7_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim7_handle); /* ��ʱ���жϹ��������� */
}

/**
 * @brief       ��ʱ�������жϻص�����
 * @param       htim:��ʱ�����
 * @retval      ��
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//��ʱ��ÿ���ӡһ���ַ���
    if (htim->Instance == BTIM_TIMX_INT)
    {
		printf("TIM6���ȼ�Ϊ6���������У�\r\n");
    }else if(htim->Instance == BTIM_TIM7_INT)
	{
		printf("TIM7���ȼ�Ϊ4���������У�\r\n");
	}

}




