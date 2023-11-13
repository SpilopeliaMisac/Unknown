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


short angle_num=838,speed=1800;
const short middle_angle_num=838,speed_defa=1800;        //angle1:1050;800;610   angle2:1040:833:?

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

//��ȡ������ֵ����Сֵ
void get_Adc_MaxMin()
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

unsigned short test1()   //ת��
{
    const unsigned short A=1200,B=200,C=200;
    static float temp1;

    temp1 = A*Adc.Err + B*(Adc.Err - Adc.Err_pre) + middle_angle_num;
    Adc.Err_pre = Adc.Err;

    temp1 = (temp1 > 1040)?1040:temp1;
    temp1 = (temp1 < 630)?630:temp1;

    return (short)temp1;
}

//void turn()
//{
//    if((Adc.RightHori_value < 0.9)&&(Adc.LeftHori_value >=0.99)){angle_num = 1040;}
//    else if ((Adc.LeftHori_value <0.9)&&(Adc.RightHori_value >=0.99)){angle_num = 630;}
//    else{angle_num=test1();}
//}

void speed_Cont()
{
    static float temp2;
    temp2 = 100000 * Adc.Err * Adc.Err;
    speed = (short)(speed_defa);
}

void ips_show()
{
    ips200_show_string(0,0,"LeftH");
    ips200_show_int(60,0,Adc.LeftHori,5);
    ips200_show_int(120,0,Adc.LeftHori_value*100,5);

    ips200_show_string(0,20,"RightH");
    ips200_show_int(60,20,Adc.RightHori,5);
    ips200_show_int(120,20,Adc.RightHori_value*100,5);

    ips200_show_string(0,40,"LeftV");
    ips200_show_int(60,40,Adc.LeftVert,5);
    ips200_show_int(120,40,Adc.LeftVert_Max,5);

    ips200_show_string(0,60,"RightV");
    ips200_show_int(60,60,Adc.RightVert,5);
    ips200_show_int(120,60,Adc.RightVert_Max,5);

    ips200_show_string(0,80,"angle");
    ips200_show_int(60,80,angle_num,5);

    ips200_show_string(0,100,"VertAdd");
    ips200_show_int(60,100,Adc.LeftVert + Adc.RightVert,5);

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

    Adc.LeftHori_Max = 255;
    Adc.RightHori_Max =255;
    Adc.LeftHori_Min = 5;
    Adc.RightHori_Min = 5;


    Adc.LeftVert_Max = 100;
    Adc.RightVert_Max = 100;
    Adc.LeftVert_Min = 1000;
    Adc.RightVert_Min = 1000;


    //gpio_init(P00_0, GPO, 1, GPO_PUSH_PULL);
    //gpio_init(P33_10, GPO, 0, GPO_PUSH_PULL);
    //gpio_init(P02_6, GPO, 1, GPO_PUSH_PULL);


    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���


        Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,10);  //���ҵ�вɼ�
        Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,10);
        Adc.LeftVert = adc_mean_filter_convert(ADC0_CH5_A5,10);  //181 147 38  76  179
        Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,10); //126 16  115 207 225

        get_Adc_MaxMin();
        get_Adc_Err();
        //turn();            //ת��
        angle_num = test1();

        //if((Adc.LeftHori < 20) && (Adc.RightHori < 20)){speed = 20;}

        pwm_set_duty(ATOM0_CH1_P33_9,angle_num);           //���

        pwm_set_duty(ATOM0_CH6_P02_6,1600);                //ת��

        ips_show();


        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore


// **************************** �������� ****************************
