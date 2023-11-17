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
static short    dir_val,speed,
                
                state,              //状态

                delay,              //通用计数
                curve_delay,

                loop_ready;         //环岛预判断

const short     middle_dir_defa=390,                        //车1:1040:835:630 车2:600:390:180
                left_dir_limit= 600,right_dir_limit= 180,
                left_dir_defa = 510,right_dir_defa = 270,   //左右转默认中值,暂时没用到
                curve_delay_defa = 20,                      //转弯状态持续时间的延长,暂时没用到

                loop_delay_defa = 30,                       //环岛状态持续时间
                loop_ready_defa = 20,                       //环岛预判断持续时间

                crossing_exit = 1200,
                crossing_delay_defa = 30,


                speed_defa=1500;

const float     special_judge = 0.2,                        //Hori_Err绝对值小于该值时进行特殊状态判断
                curve_judge= 0.4;                           //Hori_Err绝对值大于该值时进入弯道状态

int             test[8];

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                     //电感值
                    LeftVert,RightVert,

                    LeftHori_value,RightHori_value;

    short           Vert_Err;

    float           Hori_Err,Hori_Err_pre,Hori_Err_sum;

}Adc;

struct Encoder_struct
{
    short       Val,Tar,

                Err,Err_pre;

}Encoder;


void vcan_sendware(void *wareaddr, uint32 waresize)
{
#define CMD_WARE     3
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_write_buffer(UART_2, cmdf, sizeof(cmdf));    //先发送前命令
    uart_write_buffer(UART_2, (uint8 *)wareaddr, waresize);    //发送数据
    uart_write_buffer(UART_2, cmdr, sizeof(cmdr));    //发送后命令

}


void ips_show()
{
    ips200_show_int(120,0,state,1);
    ips200_show_int(120,20,loop_ready,1);
    ips200_show_int(120,40,Encoder.Val,5);

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
    ips200_show_int(60,120,delay,10);

    ips200_show_string(0,160,"DirNum");
    ips200_show_int(60,160,dir_val,3);

    ips200_show_string(0,200,"Speed");
    ips200_show_int(60,200,speed,5);
}

//得到电感值,并求差和比
void Adc_handle()
{
    Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,8);
    Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,8);
    Adc.LeftVert  = adc_mean_filter_convert(ADC0_CH5_A5,8);
    Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,8);

    Adc.Hori_Err = 1.0 * (Adc.LeftHori - Adc.RightHori)/(Adc.LeftHori + Adc.RightHori);
}

void traction_control()
{
    const short P = 5, D = 2;

    Encoder.Val = encoder_get_count(TIM5_ENCODER);
    encoder_clear_count(TIM5_ENCODER);

    if(state ==3 || state ==4)
    {
        Encoder.Tar = 220;
    }
    else{Encoder.Tar = 240;}

    Encoder.Err = Encoder.Tar - Encoder.Val;

    speed += P * (Encoder.Err - Encoder.Err_pre) + D * Encoder.Err;

    Encoder.Err_pre = Encoder.Err;

    if(speed > 5000){speed = 5000;}
    else if(speed < 1000){speed = 1000;}

    if(Encoder.Val < 50){speed = 0;}
}

void dir_limit()
{
    if(dir_val >= left_dir_limit)       {dir_val = left_dir_limit;}
    else if(dir_val <= right_dir_limit) {dir_val = right_dir_limit;}
}

//状态判断
unsigned short state_judge()
{
    static short output;
    if((Adc.Hori_Err < special_judge) && (Adc.Hori_Err > -special_judge))
    {
        if((Adc.LeftVert > 2300) && (Adc.RightVert > 2300)){loop_ready = 0; output = 5;}
        else if((Adc.LeftVert < 650)&&(Adc.RightVert < 650))
            {loop_ready = 1; delay = loop_ready_defa;}
        else if((Adc.RightVert > 1300)&&(Adc.LeftVert - Adc.RightVert < -400)&&loop_ready)
            {loop_ready = 0; delay = loop_delay_defa; output = 3;}
        else if((Adc.LeftVert > 1300)&&(Adc.LeftVert - Adc.RightVert > 400)&&loop_ready)
            {loop_ready = 0; delay = loop_delay_defa; output = 4;}
        else if((Adc.RightVert > 800)&&(Adc.LeftVert - Adc.RightVert < -700)&&loop_ready)
            {loop_ready = 0; delay = loop_delay_defa; output = 3;}
        else if((Adc.LeftVert > 800)&&(Adc.LeftVert - Adc.RightVert > 700)&&loop_ready)
            {loop_ready = 0; delay = loop_delay_defa; output = 4;}
    }
    else if(Adc.Hori_Err >= curve_judge) {delay = curve_delay_defa; output = 1;}
    else if(Adc.Hori_Err <= -curve_judge){delay = curve_delay_defa; output = 2;}
    else                                {output = 0;}
    return output;
}

unsigned short state_judge_test1()
{
    static short output;

    Adc.Vert_Err = Adc.LeftVert - Adc.RightVert;

    if((Adc.Hori_Err < special_judge) && (Adc.Hori_Err > -special_judge))
    {
        if( (Adc.LeftVert >2000) && (Adc.RightVert >2000) )
            {loop_ready = 0; output = 5;}

        else if( (Adc.LeftVert <650) && (Adc.RightVert <650) )
            {loop_ready = 1; delay = loop_ready_defa;}

        else if( loop_ready && (Adc.RightVert >700) && (Adc.RightVert <1800) && (Adc.Vert_Err < -(920-0.4*Adc.RightVert)) )
            {loop_ready = 0; delay = loop_delay_defa; output = 3;}//左环岛

        else if( loop_ready && (Adc.LeftVert  >700) && (Adc.LeftVert  <1800) && (Adc.Vert_Err >  (920-0.4*Adc.LeftVert)) )
            {loop_ready = 0; delay = loop_delay_defa; output = 4;}//右环岛
    }
//    else if((Adc.Hori_Err > curve_judge) || (Adc.Hori_Err < -curve_judge))
//    {output = 6;}
    else output = 0;

    return output;
}

void loop_ready_control()
{
    if(loop_ready && (delay > 0)){delay--;}
    else if (loop_ready){loop_ready = 0;}
}

void general_control()
{
    const short P = 320, D = 250;           //车2：P = 320, D = 250;
    static float temp1;
    temp1 =P * Adc.Hori_Err + 1.0*D * (Adc.Hori_Err - Adc.Hori_Err_pre) + middle_dir_defa;
    dir_val = (short)temp1;
    Adc.Hori_Err_pre = Adc.Hori_Err;
}

void crossing_control()
{
    general_control();

    if((Adc.LeftVert > crossing_exit) && (Adc.RightVert > crossing_exit)) {delay = crossing_delay_defa;}
    if(delay > 0)   {delay--;}
    else            {state = 0;}
    //state = (delay-- > 0)?5:0;
}

void curve_contorl()
{
    general_control();

    if((Adc.Hori_Err > curve_judge) || (Adc.Hori_Err < -curve_judge)) {curve_delay = curve_delay_defa;}
    if(curve_delay > 0)   {curve_delay--;}
    else            {state = 0;}
}

void left_loop_control()
{
    dir_val = 550 + 150 * Adc.Hori_Err;
    if(delay > 0)   {delay--;}
    else            {state = 0;}
    //state = (delay-- > 0)?3:0;
}

void right_loop_control()
{
    dir_val = 230 +  150 * Adc.Hori_Err;
    if(delay > 0)   {delay--;}
    else            {state = 0;}
    //state = (delay-- > 0)?4:0;
}


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //舵机初始化
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);     //电机初始化
    

    // bluetooth_ch9141_init();                    //蓝牙初始化
    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);


    adc_init(ADC0_CH4_A4, ADC_12BIT);           //左水平电感初始化
    adc_init(ADC0_CH7_A7, ADC_12BIT);           //右水平
    adc_init(ADC0_CH5_A5, ADC_12BIT);           //左垂直
    adc_init(ADC0_CH6_A6, ADC_12BIT);           //右垂直


    ips200_init(IPS200_TYPE_PARALLEL8);         //屏幕初始化
    ips200_clear();


    encoder_dir_init(TIM5_ENCODER,TIM5_ENCODER_CH1_P10_3,TIM5_ENCODER_CH2_P10_1);

    Adc.Hori_Err_sum = 0;
    Adc.Hori_Err_pre = 0;

    dir_val = middle_dir_defa;
    speed = speed_defa;

    state = 0;

    Encoder.Tar = 240;
    Encoder.Err_pre = 0;

    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {

        Adc_handle();

        traction_control();

        if(!state)
        {
            general_control();
            state = state_judge_test1();
        }

        switch (state)
        {
            case 3: left_loop_control();    break;

            case 4: right_loop_control();   break;

            case 5: crossing_control();     break;

            case 6: curve_contorl();        break;

            default:                        break;
        }


        loop_ready_control();
        dir_limit();

        pwm_set_duty(ATOM0_CH1_P33_9,dir_val);          //舵机
        pwm_set_duty(ATOM0_CH7_P02_7,speed);            //电机

        ips_show();

        test[0] = speed;
        test[1] = 10*Encoder.Val;

        vcan_sendware(test,sizeof(test));
    }
}


#pragma section all restore


// **************************** 代码区域 ****************************
