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
static short    dir_val,Speed,
                
                state,              //״̬
                state_ready,

                delay;              //ͨ�ü���

const short     middle_dir_defa=390,                        //��1:1040:835:630 ��2:600:390:180
                left_dir_limit= 600,right_dir_limit= 180,

                annulus_delay_defa = 30,                    //����״̬����ʱ��
                annulus_ready_defa = 10,                    //����Ԥ�жϳ���ʱ��

                crossing_exit = 700,                        //�˳�·��״̬�ĵ��
                crossing_delay_defa = 60,

                ramp_exit_ready_defa = 20,
                ramp_exit_delay_defa = 20,

                starting_delay_defa = 20,

                speed_defa=1800;

const float     special_judge = 0.2,                        //Hori_Err����ֵС�ڸ�ֵʱ��������״̬�ж�
                curve_judge= 0.4;                           //Hori_Err����ֵ���ڸ�ֵʱ�������״̬

static int      test[8];

struct Adc_struct
{
    unsigned short  LeftHori,RightHori,                     //���ֵ
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
    short   Speed,              //��ȥ�ĵ��ʩ��ֵ
            Enc_Val,            //��ȥ�ı��������
            LeftVert,RightVert; //��ȥ�Ĵ�ֱ���ֵ

    float   Hori_Err;

    struct data_node *next;
};
typedef struct data_node        Data_Node;

Data_Node* DataList;   //��Զ����ʷ��¼

//��¼��ȥ�ı���ֵ
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
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_write_buffer(UART_2, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_write_buffer(UART_2, (uint8 *)wareaddr, waresize);    //��������
    uart_write_buffer(UART_2, cmdr, sizeof(cmdr));    //���ͺ�����

}


void ips_show()
{
    ips200_show_string(0,0,"LH");
    ips200_show_int(60,0,Adc.LeftHori,4);
    ips200_show_string(120,0,"State");//״̬
    ips200_show_int(180,0,state,1);

    ips200_show_string(0,20,"RH");
    ips200_show_int(60,20,Adc.RightHori,4);
    ips200_show_string(120,20,"Ready?");//Ԥ��
    ips200_show_int(180,20,state_ready,1);

    ips200_show_string(0,40,"HoriErr");
    ips200_show_int(60,40,Adc.Hori_Err*100,2);

    ips200_show_string(0,60,"LV");
    ips200_show_int(40,60,Adc.LeftVert,4);
    ips200_show_int(80,60,DataList->LeftVert,4);
    ips200_show_string(120,60,"Enc_Val");//���������
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

//�õ����ֵ,�����ͱ�
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

//״̬�ж�
bool ramp_exit_judge()//���Ż�
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
        {state_ready = 0; state = 5;}//·��

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
//        {state_ready = 7; delay = ramp_exit_ready_defa;}//����Ԥ��
//
//    else if ( state_ready == 7 && ramp_exit_judge() )
//        {state = 7; state_ready = 0;}//����

    else {state = 0;}
}

void ready_handle()
{
    if((state_ready == 3 || state_ready == 7)&&(delay <= 0))
        {state_ready = 0;}
}

//ͨ�ö��PID
void general_control(short P,short I,short D)
{
    static float temp1;
    temp1 =P * Adc.Hori_Err + 1.0*D * (Adc.Hori_Err - Adc.Hori_Err_pre) + middle_dir_defa;
    dir_val = (short)temp1;
    Adc.Hori_Err_pre = Adc.Hori_Err;
}

void crossing_control() //·��
{
    general_control(200,0,150);
    Encoder.Tar = 200;

    if((Adc.LeftVert > crossing_exit) && (Adc.RightVert > crossing_exit)) 
        {delay = crossing_delay_defa;}
}

void starting_control() //��
{
    general_control(320,0,250);
    Encoder.Tar = Encoder.Val;

    if(Encoder.Val < 20)
        {delay = starting_delay_defa;}
    Speed = speed_defa;
}

void ramp_exit_control()    //����
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
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //�����ʼ��
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);     //�����ʼ��
    
    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);

    adc_init(ADC0_CH4_A4, ADC_12BIT);           //��ˮƽ��г�ʼ��
    adc_init(ADC0_CH7_A7, ADC_12BIT);           //��ˮƽ
    adc_init(ADC0_CH5_A5, ADC_12BIT);           //��ֱ
    adc_init(ADC0_CH6_A6, ADC_12BIT);           //�Ҵ�ֱ


    ips200_init(IPS200_TYPE_PARALLEL8);         //��Ļ��ʼ��
    ips200_clear();

    encoder_dir_init(TIM5_ENCODER,TIM5_ENCODER_CH1_P10_3,TIM5_ENCODER_CH2_P10_1);//��������ʼ��

    //��ʼ��һЩ����
    Adc.Hori_Err_sum = 0;
    Adc.Hori_Err_pre = 0;

    dir_val = middle_dir_defa;
    Speed = speed_defa;

    state = 8;
    delay = starting_delay_defa;

    Encoder.Err_pre = 0;
    Encoder.Err_sum = 0;

    DataList = InitList(3);

    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
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

        pwm_set_duty(ATOM0_CH1_P33_9,dir_val);          //���
        pwm_set_duty(ATOM0_CH7_P02_7,Speed);            //���

        //�����Ӱ�
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


// **************************** �������� ****************************
