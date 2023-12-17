/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� enableInterrupts(); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� disableInterrupts(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� enableInterrupts(); �������жϵ���Ӧ��


// **************************** �������� ****************************

#define steering_mid_defa (610) //:690:610:520




void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������

    ips200_init(IPS200_TYPE_PARALLEL8);         //��Ļ��ʼ��
    ips200_clear();

    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //�����ʼ��

    pwm_init(ATOM0_CH6_P02_6, 17000, 1000);     //�����ʼ��
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);
    pwm_init(ATOM0_CH4_P02_4, 17000, 1000);
    pwm_init(ATOM0_CH5_P02_5, 17000, 1000);

    mt9v03x_init();

    int i=50,t=1;
    unsigned char   threshold,
                    column_white_mid;
    unsigned char   image_nor[ImageNor_H][ImageNor_W];

    short Steering_val;

    // system_delay_ms(100);
    // threshold = Ostu_find();


    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        
        //Image_Nor(image_nor,100);
        column_white_mid = Image_handle(image_nor,100);

        ips200_show_gray_image(1,1,mt9v03x_image,MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
        ips200_show_int(1,123,column_white_mid,3);
        ips200_show_gray_image(1,141,image_nor,ImageNor_W,ImageNor_H,ImageNor_W,ImageNor_H,0);

        if(!column_white_mid){column_white_mid = 46;}
        Steering_val = -(3*(column_white_mid - 46)) + steering_mid_defa;

        if(Steering_val > 680){Steering_val = 680;}
        if(Steering_val < 530){Steering_val = 530;}

        pwm_set_duty(ATOM0_CH1_P33_9,Steering_val);

        pwm_set_duty(ATOM0_CH7_P02_7,2500);
        pwm_set_duty(ATOM0_CH5_P02_5,2500);

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
#pragma section all restore
