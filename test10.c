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
static short    dir_val,Speed,
                
                state,              //状态
                state_ready,

                delay;              //通用计数

const short     middle_dir_defa=390,                        //车1:1040:835:630 车2:600:390:180
                left_dir_limit= 600,right_dir_limit= 180,

                annulus_delay_defa = 30,                    //环岛状态持续时间
                annulus_ready_defa = 10,                    //环岛预判断持续时间

                crossing_exit = 700,                        //退出路口状态的电感
                crossing_delay_defa = 60,

                ramp_exit_ready_defa = 20,
                ramp_exit_delay_defa = 20,

                starting_delay_defa = 20,

                speed_defa=1800;

const float     special_judge = 0.2,                        //Hori_Err绝对值小于该值时进行特殊状态判断
                curve_judge= 0.4;                           //Hori_Err绝对值大于该值时进入弯道状态

static int      test[8];

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
    short   Val,Tar,

            Err,Err_pre,Err_sum;

}Encoder;

struct data_node
{
    short   Speed,              //过去的电机施力值
            Enc_Val,            //过去的编码器输出
            LeftVert,RightVert; //过去的垂直电感值

    float   Hori_Err;

    struct data_node *next;
};
typedef struct data_node        Data_Node;

Data_Node* DataList;   //最远的历史记录

//记录过去的变量值
Data_Node* InitList(short num)
{
    Data_Node* head = (Data_Node*)malloc(sizeof(Data_Node));
    Data_Node* node;

    head->Speed    = 0;
    head->Enc_Val  = 0;
    head->Hori_Err = 0;
    head->LeftVert = 0;
    head->RightVert= 0;

    head->next = head;

    short i;
    for(i=0;i<num-1;i++)
    {
        node = (Data_Node*)malloc(sizeof(Data_Node));
        node->Speed    = 0;
        node->Enc_Val  = 0;
        node->Hori_Err = 0;
        head->LeftVert = 0;
        head->RightVert= 0;

        node->next = head->next;
        head->next = node;
    }

    return node;
}

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
    ips200_show_string(0,0,"LH");
    ips200_show_int(60,0,Adc.LeftHori,4);
    ips200_show_string(120,0,"State");//状态
    ips200_show_int(180,0,state,1);

    ips200_show_string(0,20,"RH");
    ips200_show_int(60,20,Adc.RightHori,4);
    ips200_show_string(120,20,"Ready?");//预备
    ips200_show_int(180,20,state_ready,1);

    ips200_show_string(0,40,"HoriErr");
    ips200_show_int(60,40,Adc.Hori_Err*100,2);

    ips200_show_string(0,60,"LV");
    ips200_show_int(40,60,Adc.LeftVert,4);
    ips200_show_int(80,60,DataList->LeftVert,4);
    ips200_show_string(120,60,"Enc_Val");//编码器输出
    ips200_show_int(180,60,Encoder.Val,3);

    ips200_show_string(0,80,"RV");
    ips200_show_int(40,80,Adc.RightVert,4);
    ips200_show_int(80,80,DataList->RightVert,4);

    ips200_show_string(0,120,"Delay");
    ips200_show_int(60,120,delay,2);

    ips200_show_string(0,160,"DirNum");
    ips200_show_int(60,160,dir_val,3);

    ips200_show_string(0,200,"Speed");
    ips200_show_int(60,200,Speed,5);
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
    //const float P = 3, I = 2, D = 0;
    const float P = 4, I = 0, D = 5;

    //static float Err;
    //Err = Adc.Hori_Err - DataList->Hori_Err;
    //*((int*)&Err) &= 0x7fffffff;

    //Encoder.Tar = 310;
    if(!state)  {Encoder.Tar = (Adc.Hori_Err < 0.2 && Adc.Hori_Err > -0.2)?330:290;}
    if(state_ready == 3){Encoder.Tar = 200;}


    Encoder.Val = encoder_get_count(TIM5_ENCODER);
    encoder_clear_count(TIM5_ENCODER);

    Encoder.Err = Encoder.Tar - Encoder.Val;

    Speed += P * Encoder.Err + I * Encoder.Err_sum + D * (Encoder.Err - Encoder.Err_pre);

    Encoder.Err_sum += Encoder.Err;
    Encoder.Err_pre = Encoder.Err;

    if(Speed < 1000)    {Speed = 1000;}
    if(Speed > 9000)    {Speed = 9000;}

    //const short temp = 200;
    // if(Adc.LeftHori < temp && Adc.RightHori < temp && Adc.LeftVert < temp && Adc.RightVert < temp)
    //     {Speed = 0;}
}

void safety_control()
{
    if(dir_val >= left_dir_limit)       {dir_val = left_dir_limit;}
    else if(dir_val <= right_dir_limit) {dir_val = right_dir_limit;}
}

//状态判断
bool ramp_exit_judge()//待优化
{
    bool output;
    static short Err;
    Err = Encoder.Val - DataList->Enc_Val;
    if((Err > 50)&&(DataList->Enc_Val < 250))
        {output = true;}
    else{output = false;}

    return output;
}

bool annulus_ready_judge()
{
    bool output;
    if((Adc.LeftVert >650) && (Adc.RightVert >650) )
        {output = false;}
    else if((DataList->LeftVert - Adc.LeftVert > 500) || (DataList->RightVert - Adc.RightVert > 500))
        {output = true;}
    else{output = false;}
    return output;
}

void state_judge()
{
    Adc.Vert_Err = Adc.LeftVert - Adc.RightVert;

    if((Adc.Hori_Err >= special_judge) || (Adc.Hori_Err <= -special_judge))
        {state = 0;state_ready = 0;}

    else if(  (Adc.LeftVert >1500) &&
              (Adc.RightVert >1500) )
        {state_ready = 0; state = 5;}//路口

    else if( annulus_ready_judge() )
        {state_ready = 3; delay = annulus_ready_defa;}

    else if(  (state_ready == 3)    &&
              (Adc.RightVert >700)  && 
              (Adc.RightVert <1800) && 
              (Adc.Vert_Err < -(920-0.4*Adc.RightVert)) )
        {state = 3; state_ready = 0; delay = annulus_delay_defa;}

    else if(  (state_ready == 3)    &&
              (Adc.LeftVert  >700)  && 
              (Adc.LeftVert  <1800) && 
              (Adc.Vert_Err >  (920-0.4*Adc.LeftVert)) )
        {state = 4; state_ready = 0; delay = annulus_delay_defa;}

//    else if(Speed > 4000 && Encoder.Val < 280)
//        {state_ready = 7; delay = ramp_exit_ready_defa;}//下坡预备
//
//    else if ( state_ready == 7 && ramp_exit_judge() )
//        {state = 7; state_ready = 0;}//下坡

    else {state = 0;}
}

void ready_handle()
{
    if((state_ready == 3 || state_ready == 7)&&(delay <= 0))
        {state_ready = 0;}
}

//通用舵机PID
void general_control(short P,short I,short D)
{
    static float temp1;
    temp1 =P * Adc.Hori_Err + 1.0*D * (Adc.Hori_Err - Adc.Hori_Err_pre) + middle_dir_defa;
    dir_val = (short)temp1;
    Adc.Hori_Err_pre = Adc.Hori_Err;
}

void crossing_control() //路口
{
    general_control(200,0,150);
    Encoder.Tar = 200;

    if((Adc.LeftVert > crossing_exit) && (Adc.RightVert > crossing_exit)) 
        {delay = crossing_delay_defa;}
}

void starting_control() //起步
{
    general_control(320,0,250);
    Encoder.Tar = Encoder.Val;

    if(Encoder.Val < 20)
        {delay = starting_delay_defa;}
    Speed = speed_defa;
}

void ramp_exit_control()    //下坡
{
    general_control(100,0,40);
    Encoder.Tar = 0;
    Speed = 0;
}

void left_annulus_control()
{
    Encoder.Tar = 280;
    dir_val = 600 + 150 * Adc.Hori_Err;
}

void right_annulus_control()
{
    Encoder.Tar = 280;
    dir_val = 180 +  120 * Adc.Hori_Err;//240
}


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //舵机初始化
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);     //电机初始化
    
    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);

    adc_init(ADC0_CH4_A4, ADC_12BIT);           //左水平电感初始化
    adc_init(ADC0_CH7_A7, ADC_12BIT);           //右水平
    adc_init(ADC0_CH5_A5, ADC_12BIT);           //左垂直
    adc_init(ADC0_CH6_A6, ADC_12BIT);           //右垂直


    ips200_init(IPS200_TYPE_PARALLEL8);         //屏幕初始化
    ips200_clear();

    encoder_dir_init(TIM5_ENCODER,TIM5_ENCODER_CH1_P10_3,TIM5_ENCODER_CH2_P10_1);//编码器初始化

    //初始化一些变量
    Adc.Hori_Err_sum = 0;
    Adc.Hori_Err_pre = 0;

    dir_val = middle_dir_defa;
    Speed = speed_defa;

    state = 8;
    delay = starting_delay_defa;

    Encoder.Err_pre = 0;
    Encoder.Err_sum = 0;

    DataList = InitList(3);

    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {

        Adc_handle();

        traction_control();

        switch (state)
        {
            case 0: general_control(320,0,250);
                    state_judge();          break;

            case 3: left_annulus_control(); break;

            case 4: right_annulus_control();break;

            case 5: crossing_control();     break;

            case 7: ramp_exit_control();    break;

            case 8: starting_control();     break;

            default:                        break;
        }

        if(delay > 0)   {delay--;}
        else            {state = 0;}

        ready_handle();
        safety_control();

        pwm_set_duty(ATOM0_CH1_P33_9,dir_val);          //舵机
        pwm_set_duty(ATOM0_CH7_P02_7,Speed);            //电机

        //杂七杂八
        DataList->Speed = Speed;
        DataList->Enc_Val = Encoder.Val;
        DataList->Hori_Err = Adc.Hori_Err;
        DataList->LeftVert = Adc.LeftVert;
        DataList->RightVert = Adc.RightVert;

        DataList = DataList->next;

        ips_show();

        test[0] = Speed;
        test[1] = 10*Encoder.Val;
        test[3] = 100*state;

        vcan_sendware(test,sizeof(test));
    }
}


#pragma section all restore


// **************************** 代码区域 ****************************
