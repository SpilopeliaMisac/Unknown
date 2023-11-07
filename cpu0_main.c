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

short angle_num=800,speed=2000,temp;
const short middle_angle_num=833,def_speed=2000;        //1050;800;610

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                 //���ֵ
                    LeftVert,RightVert,

                    HoriMax,HoriMin,
                    VertMax,VertMin,

                    LeftHori_Max,RightHori_Max,
                    LeftVert_Max,RightVert_Max,

                    LeftHori_Min,RightHori_Min,
                    LeftVert_Min,RightVert_Min;

    float           Err,Err_pre,
                    LeftHori_value,RightHori_value,     //��ֵ����ֵ
                    LeftVert_value,RightVert_value;

    bool            temp1,temp2;
}Adc;

unsigned short test1()   //ת�䣬��ʱ
{
    const unsigned short A=1200,B=1000,C=200;
    static float temp1;

    temp1 = (A*Adc.Err + B*(Adc.Err - Adc.Err_pre)) + middle_angle_num;
    Adc.Err_pre = Adc.Err;

    temp1 = (temp1 > 1040)?1040:temp1;
    temp1 = (temp1 < 630)?630:temp1;

    //speed = (Adc.Err > 0)?(C*Adc.Err + def_speed):(-C*Adc.Err + def_speed);

    return (short)temp1;
}

//��ȡ������ֵ����Сֵ
void get_MaxMin_Adc()
{
    Adc.LeftHori_Max = (Adc.LeftHori >= Adc.LeftHori_Max)?Adc.LeftHori:Adc.LeftHori_Max;
    Adc.LeftHori_Min = (Adc.LeftHori <= Adc.LeftHori_Min)?Adc.LeftHori:Adc.LeftHori_Min;

    Adc.LeftVert_Max = (Adc.LeftVert >= Adc.LeftVert_Max)?Adc.LeftVert:Adc.LeftVert_Max;
    Adc.LeftVert_Min = (Adc.LeftVert <= Adc.LeftVert_Min)?Adc.LeftVert:Adc.LeftVert_Min;

    Adc.RightHori_Max = (Adc.RightHori >= Adc.RightHori_Max)?Adc.RightHori:Adc.RightHori_Max;
    Adc.RightHori_Min = (Adc.RightHori <= Adc.RightHori_Min)?Adc.RightHori:Adc.RightHori_Min;

    Adc.RightVert_Max = (Adc.RightVert >= Adc.RightVert_Max)?Adc.RightVert:Adc.RightVert_Max;
    Adc.RightVert_Min = (Adc.RightVert <= Adc.RightVert_Min)?Adc.RightVert:Adc.RightVert_Min;

}

//��ֵ�� �Լ� ���Ⱥ�
void get_Adc_Err()
{
    Adc.LeftHori_value =    1.0*(Adc.LeftHori - Adc.LeftHori_Min)/(Adc.LeftHori_Max - Adc.LeftHori_Min);
    Adc.RightHori_value =   1.0*(Adc.RightHori - Adc.RightHori_Min)/( Adc.RightHori_Max - Adc.RightHori_Min);

    Adc.LeftVert_value =    1.0*(Adc.LeftVert - Adc.LeftVert_Min)/(Adc.LeftVert_Max - Adc.LeftVert_Min);
    Adc.RightVert_value =   1.0*(Adc.RightVert - Adc.RightVert_Min)/(Adc.RightVert_Max - Adc.RightVert_Min);

    Adc.temp1 = Adc.LeftVert_value;
    Adc.temp2 = Adc.RightVert_value;

    Adc.Err = (Adc.LeftHori_value - Adc.RightHori_value)/(Adc.LeftHori_value + Adc.RightHori_value);
}


void Speed_Cont(unsigned short speed_targ)
{
    if(speed_targ > speed){speed++;}
    else{speed--;}
    speed = (speed > 3000)?3000:speed;
}


int core0_main(void)
{
    clock_init();                               // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                               // ��ʼ��Ĭ�ϵ��Դ���


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //�����ʼ��
    pwm_init(ATOM0_CH6_P02_6, 17000, 1000);     //�����ʼ��

    //gpio_init(P02_6, GPO, 0, GPO_PUSH_PULL);

    adc_init(ADC0_CH4_A4, ADC_8BIT);            //���г�ʼ��
    adc_init(ADC0_CH7_A7, ADC_8BIT);            //��
    adc_init(ADC0_CH5_A5, ADC_8BIT);
    adc_init(ADC0_CH6_A6, ADC_8BIT);


    ips200_init(IPS200_TYPE_PARALLEL8);
    ips200_clear();

    Adc.LeftHori_Max = 200;
    Adc.RightHori_Max =200;
    Adc.LeftHori_Min = 50;
    Adc.RightHori_Min = 50;


    Adc.LeftVert_Max = 100;
    Adc.RightVert_Max = 100;
    Adc.LeftVert_Min = 100;
    Adc.RightVert_Min = 100;


    //gpio_init(P00_0, GPO, 1, GPO_PUSH_PULL);
    //gpio_init(P33_10, GPO, 0, GPO_PUSH_PULL);
    //gpio_init(P02_6, GPO, 1, GPO_PUSH_PULL);


    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���


        Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,5);  //���ҵ�вɼ�
        Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,5);
        Adc.LeftVert = adc_mean_filter_convert(ADC0_CH5_A5,5);  //181 147 38  76  179
        Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,5); //126 16  115 207 225



        get_MaxMin_Adc();
        get_Adc_Err();
        angle_num =test1();            //ת��

        if((Adc.LeftHori < 20) && (Adc.RightHori < 20)){speed = 20;}

        pwm_set_duty(ATOM0_CH1_P33_9, angle_num);           //���


        pwm_set_duty(ATOM0_CH6_P02_6, speed);                //ת��

        //gpio_set_level(P02_6, 1);

        //if((!Adc.temp1)*Adc.temp2 + Adc.temp1*(!Adc.temp2))


        ips200_show_string(0,0,"LeftH");
        ips200_show_int(60,0,Adc.LeftHori,5);
        ips200_show_int(120,0,Adc.LeftHori_value*100,5);

        ips200_show_string(0,20,"RightH");
        ips200_show_int(60,20,Adc.RightHori,5);
        ips200_show_int(120,20,Adc.RightHori_value*100,5);

        ips200_show_string(0,40,"LeftV");
        ips200_show_int(60,40,Adc.LeftVert,5);
        ips200_show_int(120,40,Adc.LeftVert_value*100,5);

        ips200_show_string(0,60,"RightV");
        ips200_show_int(60,60,Adc.RightVert,5);
        ips200_show_int(120,60,Adc.RightVert_value*100,5);

        ips200_show_string(0,80,"temp2");
        ips200_show_int(60,80,(!Adc.temp1)*Adc.temp2 + Adc.temp1*(!Adc.temp2),5);



        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore


// **************************** �������� ****************************
