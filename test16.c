/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�?
*
* ���ļ��� TC264 ��Դ���һ����?
*
* TC264 ��Դ�� ���������?
* �����Ը����������������?���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����?/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı��?
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ�? GPL
*
* ��Ӧ�����յ�����Դ����?ʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ��?����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹��?
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
// ���������?#pragma section all restore����?����?�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ���? ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ���? ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ���? ��������ֲ���߲��Ը���������

// **************************** �������� ****************************

static short    Steering_val,           //���pwmֵ
                ServoL_val,ServoR_val,  //���ҵ��pwmֵ
                
                STATE,                  //״̬
                state_ready,            //Ԥ��״̬

                annulus_cooling_delay,
                annulus_ste_A,
                delay;                  //ͨ�ü���

const short     steering_mid_defa=610,          //��1:1040:835:630 ��2:600:390:180 ��3:690:610:520
                steering_left_limit= 680,steering_right_limit= 530,
                ste_A = 1,                      //ת�������[0,1]

                annulus_ready_defa = 20,        //����Ԥ����������
                annulus_delay_defa = 30,        //����״̬��������
                annulus_cooling_defa = 40,

                crossing_entr_val = 1800,
                crossing_exit_val = 1000,       //�˳�·��״̬�ĵ��ֵ
                crossing_delay_defa = 5,       //·��״̬�������ڵ��ӳ�

                blind_delay_defa = 10,

                data_list_rng = 4,              //��������ʷ��¼�ķ�Χ

                speed_defa=3300;                //Ĭ���ٶ�ֵ

const float     special_rng = 0.15,             //Hori_AbsErrС�ڸ�ֵʱ��������״̬�ж�
                curve_exit_rng = 0.15;          //Hori_AbsErrС��ʱ�˳����״̬

static int      test[8];

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,
                    LeftVert,RightVert;

    short           Vert_Err;

    float           Hori_Err,Hori_AbsErr,
                    Hori_Err_pre;

}Adc;

struct data_node
{
    short   LeftVert,RightVert;     //��ȥ�Ĵ�ֱ���ֵ

    float   Hori_Err,Hori_AbsErr;   //��ȥ��ˮƽErr

    struct data_node *next;
};
typedef struct data_node    Data_Node;

Data_Node* DataList;   //��Զ����ʷ��¼

//��������ʷ��¼
Data_Node* InitList(short num)
{
    Data_Node* head = (Data_Node*)malloc(sizeof(Data_Node));
    Data_Node* node;

    head->Hori_Err = 0;
    head->LeftVert = 0;
    head->RightVert= 0;
    head->Hori_AbsErr = 0;

    head->next = head;

    short i;
    for(i=0;i<num-1;i++)    //ͷ��
    {
        node = (Data_Node*)malloc(sizeof(Data_Node));
        node->Hori_Err = 0;
        node->LeftVert = 0;
        node->RightVert= 0;
        node->Hori_AbsErr = 0;

        node->next = head->next;
        head->next = node;
    }

    return head;
}

void vcan_sendware(void *wareaddr, uint32 waresize)
{
#define CMD_WARE     3
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_write_buffer(UART_2, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_write_buffer(UART_2, (uint8 *)wareaddr, waresize);    //��������
    uart_write_buffer(UART_2, cmdr, sizeof(cmdr));    //���ͺ�����

}


void IPS_show()
{
    ips200_show_string(0,0,"LH");
    ips200_show_int(60,0,Adc.LeftHori,4);
    ips200_show_string(120,0,"State");//״̬
    ips200_show_int(180,0,STATE,1);

    ips200_show_string(0,20,"RH");
    ips200_show_int(60,20,Adc.RightHori,4);
    ips200_show_string(120,20,"Ready?");//Ԥ��
    ips200_show_int(180,20,state_ready,1);

    ips200_show_string(0,40,"HErr");
    ips200_show_int(40,40,Adc.Hori_Err*100,2);
    ips200_show_int(80,40,(DataList->Hori_Err)*100,2);

    ips200_show_string(0,60,"LV");
    ips200_show_int(40,60,Adc.LeftVert,4);
    ips200_show_int(80,60,DataList->LeftVert,4);

    ips200_show_string(0,80,"RV");
    ips200_show_int(40,80,Adc.RightVert,4);
    ips200_show_int(80,80,DataList->RightVert,4);

    ips200_show_string(0,120,"Delay");
    ips200_show_int(60,120,delay,2);

    ips200_show_string(0,160,"DirNum");
    ips200_show_int(60,160,Steering_val,3);

    ips200_show_string(0,180,"Lspeed");
    ips200_show_int(60,180,ServoL_val,4);
    ips200_show_string(0,200,"Rspeed");
    ips200_show_int(60,200,ServoR_val,4);


}

//��ȡ�봦����ֵ
void Adc_handle()
{
    Adc.LeftHori  = adc_mean_filter_convert(ADC0_CH4_A4,8);
    Adc.RightHori = adc_mean_filter_convert(ADC0_CH7_A7,8);
    Adc.LeftVert  = adc_mean_filter_convert(ADC0_CH5_A5,8);
    Adc.RightVert = adc_mean_filter_convert(ADC0_CH6_A6,8);

    Adc.Hori_Err = 1.0*ste_A * (Adc.LeftHori - Adc.RightHori )/(Adc.LeftHori + Adc.RightHori);
    Adc.Vert_Err = ste_A * (Adc.LeftVert - Adc.RightVert);
    *((int*)&Adc.Hori_AbsErr) = *((int*)&Adc.Hori_Err) & 0x7fffffff;
}

void Safety_handle()
{
    if(Steering_val >= steering_left_limit)
        {Steering_val = steering_left_limit;}
    else if(Steering_val <= steering_right_limit)
        {Steering_val = steering_right_limit;}

    if(ServoL_val <200){ServoL_val = 200;}
    if(ServoR_val <200){ServoR_val = 200;}

    if(Adc.LeftHori < 30)
        {ServoL_val=0;ServoR_val=0;}
}

//״̬�ж�
void STATE_judge()
{

    // if( (Adc.Hori_AbsErr <= curve_exit_rng) && 
    //     (DataList->Hori_AbsErr - Adc.Hori_AbsErr > 0.08) )
    //     {state_ready = 0; STATE = 6;}

    if( Adc.Hori_AbsErr >= special_rng )
        {state_ready = 0; STATE = 0;}

    else if((annulus_cooling_delay == 0) &&
            (Adc.LeftVert >  crossing_entr_val) &&
            (Adc.RightVert > crossing_entr_val) )
        {state_ready = 0; STATE = 5; delay = crossing_delay_defa;}

    else if((state_ready==3 || state_ready==4) &&
            (Adc.LeftVert < 1000 && Adc.RightVert < 1000) )
        {STATE = state_ready; state_ready = 0;delay = annulus_delay_defa; annulus_ste_A = 6;}

    else if((state_ready == 0) &&
            (annulus_cooling_delay == 0) &&
            (DataList->LeftVert > 1500)  &&
            (DataList->LeftVert - Adc.LeftVert> 400) &&
            (DataList->RightVert < 1000) )
        {state_ready = 3; STATE = 0; delay = annulus_ready_defa;}

    else if((state_ready == 0) &&
            (annulus_cooling_delay == 0) &&
            (DataList->RightVert > 1500) &&
            (DataList->RightVert - Adc.RightVert> 400) &&
            (DataList->LeftVert < 1000) )
        {state_ready = 4; STATE = 0; delay = annulus_ready_defa;}

    else{STATE = 0;}
}

void Ready_handle()
{
    if((state_ready == 3 || state_ready == 4))
    {
        if(delay>0) {delay--;}
        else        {state_ready = 0;}
    }
    if(annulus_cooling_delay > 0)   {annulus_cooling_delay--;}
}

//ͨ�ö��PID
void General_ctrl(short P,short I,short D)
{
    static float temp1;
    if(Adc.Hori_AbsErr < 0.08 && DataList->Hori_AbsErr < 0.08)
        {P>>=1; D>>=1;}

    temp1 = P * Adc.Hori_Err + D * (Adc.Hori_Err - Adc.Hori_Err_pre) + steering_mid_defa;
    Adc.Hori_Err_pre = Adc.Hori_Err;

    Steering_val = (short)temp1;
}

void Beeline_ctrl()
{
    General_ctrl(40,0,20);

    if(delay > 0)   {delay--;}
    else            {STATE = 0;}
}

void Crossing_ctrl() //·��
{
    General_ctrl(60,0,30);

    if((Adc.LeftVert > crossing_exit_val) && (Adc.RightVert > crossing_exit_val)) 
        {delay = crossing_delay_defa;}

    if(delay > 0)   {delay--;}
    else            {STATE = 0;}
}

void AnnulusL_ctrl()
{
    Steering_val = 0*Adc.Hori_Err + steering_mid_defa + 65 - 3*annulus_ste_A;
    ServoL_val = speed_defa -200;

    if(annulus_ste_A > 0){annulus_ste_A--;}
    if(delay > 0)   {delay--;}
    else            {STATE = 0; annulus_cooling_delay = annulus_cooling_defa;}
}

void AnnulusR_ctrl()
{
    Steering_val = 0*Adc.Hori_Err + steering_mid_defa - 70 + 3*annulus_ste_A;
    ServoR_val = speed_defa -200;

    if(annulus_ste_A > 0){annulus_ste_A--;}
    if(delay > 0)   {delay--;}
    else            {STATE = 0; annulus_cooling_delay = annulus_cooling_defa;}
}

void Servo_ctrl(float temp1)
{
    if(Adc.Hori_AbsErr > 0.15)
    {
        if(Adc.Hori_Err > 0)
            {ServoL_val = speed_defa*(temp1-Adc.Hori_AbsErr); ServoR_val = speed_defa - 100;}
        else{ServoR_val = speed_defa*(temp1-Adc.Hori_AbsErr); ServoL_val = speed_defa - 100;}
    }
}

void RampExit_ctrl()
{
    
}


int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //�����ʼ��

    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);
    

    pwm_init(ATOM0_CH6_P02_6, 17000, 1000);     //�����ʼ��
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);
    pwm_init(ATOM0_CH4_P02_4, 17000, 1000);
    pwm_init(ATOM0_CH5_P02_5, 17000, 1000);

    adc_init(ADC0_CH4_A4, ADC_10BIT);           //��ˮƽ��г�ʼ��
    adc_init(ADC0_CH7_A7, ADC_10BIT);           //��ˮƽ
    adc_init(ADC0_CH5_A5, ADC_12BIT);           //��ֱ
    adc_init(ADC0_CH6_A6, ADC_12BIT);           //�Ҵ�ֱ


    ips200_init(IPS200_TYPE_PARALLEL8);         //��Ļ��ʼ��
    ips200_clear();

    //encoder_dir_init(TIM5_ENCODER,TIM5_ENCODER_CH1_P10_3,TIM5_ENCODER_CH2_P10_1);//��������ʼ��

    //��ʼ��һЩ����
    Adc.Hori_Err_pre = 0;

    Steering_val = steering_mid_defa;
    ServoL_val = speed_defa;
    ServoR_val = speed_defa;

    STATE = 0;

    DataList = InitList(data_list_rng);

    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

    goto TheMain;   //TheMain,TheDebug

TheMain:
    while (TRUE)
    {

        Adc_handle();
        ServoL_val = speed_defa;
        ServoR_val = speed_defa;

        switch (STATE)
        {
            case 0: General_ctrl(130,0,90);
                    STATE_judge();
                    Servo_ctrl(0.8);        break;

            case 3: AnnulusL_ctrl();        break;

            case 4: AnnulusR_ctrl();        break;

            case 5: Crossing_ctrl();        break;

            case 6: Beeline_ctrl();
                    Servo_ctrl(0.7);        break;

            default:                        break;
        }

        Ready_handle();
        Safety_handle();

        pwm_set_duty(ATOM0_CH1_P33_9,Steering_val);

        pwm_set_duty(ATOM0_CH7_P02_7,ServoL_val);
        pwm_set_duty(ATOM0_CH5_P02_5,ServoR_val);

        //�����Ӱ�
        DataList->Hori_Err = Adc.Hori_Err;
        DataList->Hori_AbsErr = Adc.Hori_AbsErr;
        DataList->LeftVert = Adc.LeftVert;
        DataList->RightVert = Adc.RightVert;

        DataList = DataList->next;

        IPS_show();

        test[0] = ServoL_val;
        test[2] = Adc.LeftVert;
        test[3] = 100*STATE;

        vcan_sendware(test,sizeof(test));

    }

TheDebug:
    while (TRUE)
    {
        pwm_set_duty(ATOM0_CH1_P33_9,steering_mid_defa);

        pwm_set_duty(ATOM0_CH7_P02_7,0);
        pwm_set_duty(ATOM0_CH5_P02_5,0);

//        pwm_set_duty(ATOM0_CH6_P02_6,2000);
//        pwm_set_duty(ATOM0_CH4_P02_4,2000);
    }
}


#pragma section all restore


// **************************** �������� ****************************
