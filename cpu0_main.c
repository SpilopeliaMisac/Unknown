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
static short    dir_num,speed,
                
                state = 0,                 //状态

                loop_delay;

const short     middle_dir_defa=390,                        //dir1:1040:835:630 dir2:600:390:180    .       
                left_dir_limit= 600,right_dir_limit= 180,   
                left_dir_defa = 510,right_dir_defa = 270,   //左右转默认中值

                loop_delay_defa = 40,

                speed_defa=1900;

const float     loop_dete = 0.3;

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                     //电感值
                    LeftVert,RightVert,

                    LeftHori_value,RightHori_value;         //暂时没用到

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

//得到电感值
void get_Adc()
{
    Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,8);
    Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,8);
    Adc.LeftVert  = adc_mean_filter_convert(ADC0_CH5_A5,8);
    Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,8);

    // Adc.LeftHori_value  = Adc.LeftHori / 4095.0;
    // Adc.RightHori_value = Adc.RightHori / 4095.0;
}

//水平电感差和比
void get_Adc_Err()
{
    Adc.Hori_Err = 1.0 * (Adc.LeftHori - Adc.RightHori)/(Adc.LeftHori + Adc.RightHori); //一般情况下<0.4
}

//状态判断
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
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //舵机初始化
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);     //电机初始化


    adc_init(ADC0_CH4_A4, ADC_12BIT);            //左电感初始化
    adc_init(ADC0_CH7_A7, ADC_12BIT);            //右
    adc_init(ADC0_CH5_A5, ADC_12BIT);
    adc_init(ADC0_CH6_A6, ADC_12BIT);


    ips200_init(IPS200_TYPE_PARALLEL8);
    ips200_clear();

    Adc.Hori_Err_sum = 0;
    Adc.Hori_Err_pre = 0;
    dir_num = middle_dir_defa;
    speed   = speed_defa;

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        
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
        pwm_set_duty(ATOM0_CH1_P33_9,dir_num);          //舵机
        pwm_set_duty(ATOM0_CH7_P02_7,1800);            //转速
        ips_show();

        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore


// **************************** 代码区域 ****************************
