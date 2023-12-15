/*********************************************************************************************************************
* TC264 Opensourec Library ï¿½ï¿½ï¿½ï¿½TC264 ï¿½ï¿½Ô´ï¿½â£©ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ï¿½ï¿½Ú¹Ù·ï¿½ SDK ï¿½Ó¿ÚµÄµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½
* Copyright (c) 2022 SEEKFREE ï¿½ï¿½É¿Æ¼ï¿?
*
* ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ï¿½ TC264 ï¿½ï¿½Ô´ï¿½ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ï¿?
*
* TC264 ï¿½ï¿½Ô´ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
* ï¿½ï¿½ï¿½ï¿½ï¿½Ô¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½á·?ï¿½ï¿½ï¿½ï¿½ GPLï¿½ï¿½GNU General Public Licenseï¿½ï¿½ï¿½ï¿½ GNUÍ¨ï¿½Ã¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¤ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
* ï¿½ï¿½ GPL ï¿½Äµï¿½3ï¿½æ£¨ï¿½ï¿½ GPL3.0ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ñ¡ï¿½ï¿½Ä£ï¿½ï¿½ÎºÎºï¿½ï¿½ï¿½ï¿½Ä°æ±¾ï¿½ï¿½ï¿½ï¿½ï¿½Â·ï¿½ï¿½ï¿½ï¿½ï¿?/ï¿½ï¿½ï¿½Þ¸ï¿½ï¿½ï¿½
*
* ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½Ä·ï¿½ï¿½ï¿½ï¿½ï¿½Ï£ï¿½ï¿½ï¿½ï¿½ï¿½Ü·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã£ï¿½ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÎºÎµÄ±ï¿½Ö?
* ï¿½ï¿½ï¿½ï¿½Ã»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô»ï¿½ï¿½Êºï¿½ï¿½Ø¶ï¿½ï¿½ï¿½Í¾ï¿½Ä±ï¿½Ö¤
* ï¿½ï¿½ï¿½ï¿½Ï¸ï¿½ï¿½ï¿½ï¿½Î¼ï¿? GPL
*
* ï¿½ï¿½Ó¦ï¿½ï¿½ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½ï¿½Í?Ê±ï¿½Õµï¿½Ò»ï¿½ï¿½ GPL ï¿½Ä¸ï¿½ï¿½ï¿½
* ï¿½ï¿½ï¿½Ã»ï¿½Ð£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½<https://www.gnu.org/licenses/>
*
* ï¿½ï¿½ï¿½ï¿½×¢ï¿½ï¿½ï¿½ï¿½
* ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½Ê¹ï¿½ï¿½ GPL3.0 ï¿½ï¿½Ô´ï¿½ï¿½ï¿½ï¿½Ö¤Ð­ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½ï¿½Ä°æ±¾
* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¢ï¿½Ä°ï¿½ï¿½ï¿½ libraries/doc ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½Âµï¿½ GPL3_permission_statement.txt ï¿½Ä¼ï¿½ï¿½ï¿½
* ï¿½ï¿½ï¿½ï¿½Ö¤ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ libraries ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½Âµï¿½ LICENSE ï¿½Ä¼ï¿½
* ï¿½ï¿½Ó­ï¿½ï¿½Î»Ê¹ï¿½Ã²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Þ¸ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ë±£ï¿½ï¿½ï¿½ï¿½É¿Æ¼ï¿½ï¿½Ä°ï¿½È?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
*
* ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½          cpu0_main
* ï¿½ï¿½Ë¾ï¿½ï¿½ï¿½ï¿½          ï¿½É¶ï¿½ï¿½ï¿½É¿Æ¼ï¿½ï¿½ï¿½ï¿½Þ¹ï¿½Ë?
* ï¿½æ±¾ï¿½ï¿½Ï¢          ï¿½é¿´ libraries/doc ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ version ï¿½Ä¼ï¿½ ï¿½æ±¾Ëµï¿½ï¿½
* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½          ADS v1.9.4
* ï¿½ï¿½ï¿½ï¿½Æ½Ì¨          TC264D
* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½          https://seekfree.taobao.com/
*
* ï¿½Þ¸Ä¼ï¿½Â¼
* ï¿½ï¿½ï¿½ï¿½              ï¿½ï¿½ï¿½ï¿½                ï¿½ï¿½×¢
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?#pragma section all restoreï¿½ï¿½ï¿½Ö?ï¿½ï¿½ï¿½È?ï¿½Ö±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½CPU0ï¿½ï¿½RAMï¿½ï¿½

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç¿ï¿½Ô´ï¿½ï¿½Õ¹ï¿½ï¿½ï¿? ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö²ï¿½ï¿½ï¿½ß²ï¿½ï¿½Ô¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç¿ï¿½Ô´ï¿½ï¿½Õ¹ï¿½ï¿½ï¿? ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö²ï¿½ï¿½ï¿½ß²ï¿½ï¿½Ô¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç¿ï¿½Ô´ï¿½ï¿½Õ¹ï¿½ï¿½ï¿? ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö²ï¿½ï¿½ï¿½ß²ï¿½ï¿½Ô¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

// **************************** ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ****************************

static short    Steering_val,           //¶æ»úpwmÖµ
                ServoL_val,ServoR_val,  //×óÓÒµç»úpwmÖµ
                
                STATE,                  //×´Ì¬
                state_ready,            //Ô¤±¸×´Ì¬

                annulus_cooling_delay,
                annulus_ste_A,
                delay;                  //Í¨ÓÃ¼ÆÊý

const short     steering_mid_defa=610,          //³µ1:1040:835:630 ³µ2:600:390:180 ³µ3:690:610:520
                steering_left_limit= 680,steering_right_limit= 530,
                ste_A = 1,                      //×ªÏò·½Ïò²ÎÊý[0,1]

                annulus_ready_defa = 20,        //»·µºÔ¤±¸³ÖÐøÖÜÆÚ
                annulus_delay_defa = 30,        //»·µº×´Ì¬³ÖÐøÖÜÆÚ
                annulus_cooling_defa = 40,

                crossing_entr_val = 1800,
                crossing_exit_val = 1000,       //ÍË³öÂ·¿Ú×´Ì¬µÄµç¸ÐÖµ
                crossing_delay_defa = 5,       //Â·¿Ú×´Ì¬³ÖÐøÖÜÆÚµÄÑÓ³¤

                blind_delay_defa = 10,

                data_list_rng = 4,              //´«¸ÐÆ÷ÀúÊ·¼ÇÂ¼µÄ·¶Î§

                speed_defa=3300;                //Ä¬ÈÏËÙ¶ÈÖµ

const float     special_rng = 0.15,             //Hori_AbsErrÐ¡ÓÚ¸ÃÖµÊ±½øÐÐÌØÊâ×´Ì¬ÅÐ¶Ï
                curve_exit_rng = 0.15;          //Hori_AbsErrÐ¡ÓÚÊ±ÍË³öÍäµÀ×´Ì¬

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
    short   LeftVert,RightVert;     //¹ýÈ¥µÄ´¹Ö±µç¸ÐÖµ

    float   Hori_Err,Hori_AbsErr;   //¹ýÈ¥µÄË®Æ½Err

    struct data_node *next;
};
typedef struct data_node    Data_Node;

Data_Node* DataList;   //×îÔ¶µÄÀúÊ·¼ÇÂ¼

//´«¸ÐÆ÷ÀúÊ·¼ÇÂ¼
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
    for(i=0;i<num-1;i++)    //Í·²å
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
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //´®¿Úµ÷ÊÔ Ê¹ÓÃµÄÇ°ÃüÁî
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //´®¿Úµ÷ÊÔ Ê¹ÓÃµÄºóÃüÁî

    uart_write_buffer(UART_2, cmdf, sizeof(cmdf));    //ÏÈ·¢ËÍÇ°ÃüÁî
    uart_write_buffer(UART_2, (uint8 *)wareaddr, waresize);    //·¢ËÍÊý¾Ý
    uart_write_buffer(UART_2, cmdr, sizeof(cmdr));    //·¢ËÍºóÃüÁî

}


void IPS_show()
{
    ips200_show_string(0,0,"LH");
    ips200_show_int(60,0,Adc.LeftHori,4);
    ips200_show_string(120,0,"State");//×´Ì¬
    ips200_show_int(180,0,STATE,1);

    ips200_show_string(0,20,"RH");
    ips200_show_int(60,20,Adc.RightHori,4);
    ips200_show_string(120,20,"Ready?");//Ô¤±¸
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

//»ñÈ¡Óë´¦Àíµç¸ÐÖµ
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

//×´Ì¬ÅÐ¶Ï
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

//Í¨ÓÃ¶æ»úPID
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

void Crossing_ctrl() //Â·¿Ú
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
    clock_init();                   // »ñÈ¡Ê±ÖÓÆµÂÊ<Îñ±Ø±£Áô>
    debug_init();                   // ³õÊ¼»¯Ä¬ÈÏµ÷ÊÔ´®¿Ú


    pwm_init(ATOM0_CH1_P33_9, 50, 1000);        //¶æ»ú³õÊ¼»¯

    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);
    

    pwm_init(ATOM0_CH6_P02_6, 17000, 1000);     //µç»ú³õÊ¼»¯
    pwm_init(ATOM0_CH7_P02_7, 17000, 1000);
    pwm_init(ATOM0_CH4_P02_4, 17000, 1000);
    pwm_init(ATOM0_CH5_P02_5, 17000, 1000);

    adc_init(ADC0_CH4_A4, ADC_10BIT);           //×óË®Æ½µç¸Ð³õÊ¼»¯
    adc_init(ADC0_CH7_A7, ADC_10BIT);           //ÓÒË®Æ½
    adc_init(ADC0_CH5_A5, ADC_12BIT);           //×ó´¹Ö±
    adc_init(ADC0_CH6_A6, ADC_12BIT);           //ÓÒ´¹Ö±


    ips200_init(IPS200_TYPE_PARALLEL8);         //ÆÁÄ»³õÊ¼»¯
    ips200_clear();

    //encoder_dir_init(TIM5_ENCODER,TIM5_ENCODER_CH1_P10_3,TIM5_ENCODER_CH2_P10_1);//±àÂëÆ÷³õÊ¼»¯

    //³õÊ¼»¯Ò»Ð©±äÁ¿
    Adc.Hori_Err_pre = 0;

    Steering_val = steering_mid_defa;
    ServoL_val = speed_defa;
    ServoR_val = speed_defa;

    STATE = 0;

    DataList = InitList(data_list_rng);

    cpu_wait_event_ready();         // µÈ´ýËùÓÐºËÐÄ³õÊ¼»¯Íê±Ï

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

        //ÔÓÆßÔÓ°Ë
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


// **************************** ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ****************************
