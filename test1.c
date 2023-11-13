/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.4
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************


short angle_num=838,speed=1800;
const short middle_angle_num=838,speed_defa=1800;        //angle1:1050;800;610   angle2:1040:833:?

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                 //电感值
                    LeftVert,RightVert,

                    HoriMax,HoriMin,
                    VertMax,VertMin,

                    LeftHori_Max,RightHori_Max,
                    LeftVert_Max,RightVert_Max,

                    LeftHori_Min,RightHori_Min,
                    LeftVert_Min,RightVert_Min;

    float           Err,Err_pre,
                    LeftHori_value,RightHori_value,     //二值化的值
                    LeftVert_value,RightVert_value;

    bool            temp1,temp2;
}Adc;

//获取电感最大值和最小值
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

//二值化 以及 求差比和
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

unsigned short test1()   //转弯
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
    clock_init();                               // 获取时钟频率<务必保留>
    debug_init();                               // 初始化默认调试串口


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //舵机初始化
    pwm_init(ATOM0_CH6_P02_6, 17000, 1000);     //电机初始化

    //gpio_init(P02_6, GPO, 0, GPO_PUSH_PULL);

    adc_init(ADC0_CH4_A4, ADC_8BIT);            //左电感初始化
    adc_init(ADC0_CH7_A7, ADC_8BIT);            //右
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


    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码


        Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,10);  //左右电感采集
        Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,10);
        Adc.LeftVert = adc_mean_filter_convert(ADC0_CH5_A5,10);  //181 147 38  76  179
        Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,10); //126 16  115 207 225

        get_Adc_MaxMin();
        get_Adc_Err();
        //turn();            //转弯
        angle_num = test1();

        //if((Adc.LeftHori < 20) && (Adc.RightHori < 20)){speed = 20;}

        pwm_set_duty(ATOM0_CH1_P33_9,angle_num);           //舵机

        pwm_set_duty(ATOM0_CH6_P02_6,1600);                //转速

        ips_show();


        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore


// **************************** 代码区域 ****************************
