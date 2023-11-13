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
                
                NEXT_MUST_BE = 0,state = 0;

static short    curve_count_defa = 24000;

const short     middle_dir_defa=835,                    //angle1:1050;800;610   angle2:1040:835:?
                left_dir_defa = 985,right_dir_defa = 715,

                speed_defa=1800;

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                 //���ֵ
                    LeftVert,RightVert,

                    LeftHori_value,RightHori_value;

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


}

//ˮƽ��в�ͱ�
void get_Adc_Err()
{
    Adc.Hori_Err = 1.0 * (Adc.LeftHori - Adc.RightHori)/(Adc.LeftHori + Adc.RightHori); //һ�������<0.5
}

//�õ����ֵ
void get_Adc()
{
    Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,8);
    Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,8);
    Adc.LeftVert  = adc_mean_filter_convert(ADC0_CH5_A5,8);
    Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,8);

    Adc.LeftHori_value  = Adc.LeftHori / 4095.0;
    Adc.RightHori_value = Adc.RightHori / 4095.0;
}

unsigned short state_judge()
{
    static float output;
    if(Adc.Hori_Err > 0.1)      {output = 1;}
    else if(Adc.Hori_Err < -0.1){output = 2;}
    else                        {output = 0;}
    return output;
}

void dir_limit()
{
    dir_num = (dir_num > 1040)?1040:dir_num;
    dir_num = (dir_num < 630)? 630:dir_num;
}

void straight_way_control()
{
    const short P = 500, I = 100, D = 100;
    Adc.Hori_Err_sum += Adc.Hori_Err;
    dir_num = P * Adc.Hori_Err + I * Adc.Hori_Err + D * (Adc.Hori_Err - Adc.Hori_Err_pre) + middle_dir_defa;
    Adc.Hori_Err_pre = Adc.Hori_Err;
}

unsigned short left_curve_control()
{
    const short P = 2000, D = 100;
    static short curve_count,output;

    dir_num = P * Adc.Hori_Err + D * (Adc.Hori_Err - Adc.Hori_Err_pre) + left_dir_defa;
    Adc.Hori_Err_pre = Adc.Hori_Err;

    if((Adc.Hori_Err > 0.1) || (curve_count >= 0))
    {
        curve_count = (Adc.Hori_Err > 0.1)?curve_count_defa:--curve_count;
        output = 1;
    }
    else output = 0;
    return output;
}

unsigned short right_curve_control()
{
    const short P = 2000, D = 100;
    static short curve_count,output;

    dir_num = P * Adc.Hori_Err + D * (Adc.Hori_Err - Adc.Hori_Err_pre) + right_dir_defa;
    Adc.Hori_Err_pre = Adc.Hori_Err;

    if((Adc.Hori_Err < -0.1) || (curve_count >= 0))
    {
        curve_count = (Adc.Hori_Err < -0.1)?curve_count_defa:--curve_count;
        output = 2;
    }
    else output = 0;
    return output;
}


int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //�����ʼ��
    pwm_init(ATOM0_CH6_P02_6, 17000, 1000);     //�����ʼ��


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

        if(!NEXT_MUST_BE)
        {
            straight_way_control();
            state = state_judge();
        }
        else
        {
            state = NEXT_MUST_BE;
        }
        
        switch (state)
        {
            case 1: NEXT_MUST_BE = left_curve_control();   break;

            case 2: NEXT_MUST_BE = right_curve_control();  break;
        
            default:                        break;
        }
        
        dir_limit();
        pwm_set_duty(ATOM0_CH1_P33_9,dir_num);          //���
        pwm_set_duty(ATOM0_CH6_P02_6,speed);            //ת��
        ips_show();

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore


// **************************** �������� ****************************
