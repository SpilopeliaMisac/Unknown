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
* �ļ�����          cpu0_main
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
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************
static short    dir_num,speed,
                
                state = 0,                 //״̬

                loop_delay;

const short     middle_dir_defa=390,                        //dir1:1040:835:630 dir2:600:390:180    .       
                left_dir_limit= 600,right_dir_limit= 180,   
                left_dir_defa = 510,right_dir_defa = 270,   //����תĬ����ֵ

                loop_delay_defa = 40,

                speed_defa=1900;

const float     loop_dete = 0.3;

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                     //���ֵ
                    LeftVert,RightVert,

                    LeftHori_value,RightHori_value;         //��ʱû�õ�

    float           Hori_Err,Hori_Err_pre,Hori_Err_sum;


}Adc;

void ips_show()
{
    ips200_show_int(120,0,state,1);

    ips200_show_string(0,0,"LH");
    ips200_show_int(60,0,Adc.LeftHori,5);

    ips200_show_string(0,20,"RH");
    ips200_show_int(60,20,Adc.RightHori,5);

    ips200_show_string(0,40,"HoriErr");
    ips200_show_int(60,40,Adc.Hori_Err*100,5);

    ips200_show_string(0,60,"LV");
    ips200_show_int(60,60,Adc.LeftVert,5);

    ips200_show_string(0,80,"RV");
    ips200_show_int(60,80,Adc.RightVert,5);

    ips200_show_string(0,120,"CurveD");
    ips200_show_int(60,120,loop_delay,10);

    ips200_show_string(0,160,"DirNum");
    ips200_show_int(60,160,dir_num,3);




}

//�õ����ֵ
void get_Adc()
{
    Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,8);
    Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,8);
    Adc.LeftVert  = adc_mean_filter_convert(ADC0_CH5_A5,8);
    Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,8);

    // Adc.LeftHori_value  = Adc.LeftHori / 4095.0;
    // Adc.RightHori_value = Adc.RightHori / 4095.0;
}

//ˮƽ��в�ͱ�
void get_Adc_Err()
{
    Adc.Hori_Err = 1.0 * (Adc.LeftHori - Adc.RightHori)/(Adc.LeftHori + Adc.RightHori); //һ�������<0.4
}

//״̬�ж�
unsigned short state_judge()
{
    static short output;
    if((Adc.Hori_Err < loop_dete) && (Adc.Hori_Err > -loop_dete))
    {
        if((Adc.LeftVert > 200)&&(Adc.LeftVert < 500)&&(Adc.RightVert > 1200)&&(Adc.RightVert < 1500))
            {loop_delay = loop_delay_defa;output = 3;}
        else if((Adc.LeftVert > 1200)&&(Adc.LeftVert < 1500)&&(Adc.RightVert > 200)&&(Adc.RightVert < 500))
            {loop_delay = loop_delay_defa;output = 4;}
    }
//    else if(Adc.Hori_Err > 0.3) {output = 1;}
//    else if(Adc.Hori_Err < -0.3){output = 2;}
    else{output = 0;}
    return output;
}

void dir_limit()
{
    dir_num = (dir_num >= left_dir_limit)?  left_dir_limit:dir_num;
    dir_num = (dir_num <= right_dir_limit)? right_dir_limit:dir_num;
}

void straight_way_control()
{
    const short P = 300, I = 0, D = 250;
    static float temp1;
    Adc.Hori_Err_sum += Adc.Hori_Err;
    temp1 =P * Adc.Hori_Err + 1.0*I * Adc.Hori_Err_sum + 1.0*D * (Adc.Hori_Err - Adc.Hori_Err_pre) + middle_dir_defa;
    dir_num = (short)temp1;
    Adc.Hori_Err_pre = Adc.Hori_Err;
}

void left_curve_control()
{
    

}

void left_loop_control()
{
    dir_num = 550;
    if(loop_delay >0) {loop_delay--;state = 3;}
    else               {state = 0;}
}

void right_loop_control()
{
    dir_num = 230;
    if(loop_delay >0) {loop_delay--;state = 4;}
    else               {state = 0;}
}


int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //�����ʼ��
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);     //�����ʼ��


    adc_init(ADC0_CH4_A4, ADC_12BIT);            //���г�ʼ��
    adc_init(ADC0_CH7_A7, ADC_12BIT);            //��
    adc_init(ADC0_CH5_A5, ADC_12BIT);
    adc_init(ADC0_CH6_A6, ADC_12BIT);


    ips200_init(IPS200_TYPE_PARALLEL8);
    ips200_clear();

    Adc.Hori_Err_sum = 0;
    Adc.Hori_Err_pre = 0;
    dir_num = middle_dir_defa;
    speed   = speed_defa;

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        
        get_Adc();
        get_Adc_Err();

        if(!state)
        {
            straight_way_control();
            state = state_judge();
        }

        switch (state)
        {
            case 3: left_loop_control();   break;

            case 4: right_loop_control();  break;

            default:                        break;
        }

        dir_limit();
//        if((Adc.LeftHori < 150) && (Adc.RightHori < 150)){speed = 20;}
        pwm_set_duty(ATOM0_CH1_P33_9,dir_num);          //���
        pwm_set_duty(ATOM0_CH7_P02_7,1800);            //ת��
        ips_show();

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore


// **************************** �������� ****************************
