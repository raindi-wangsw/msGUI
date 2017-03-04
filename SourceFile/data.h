/*
********************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*
*                            Ӳ��ƽ̨:msPLC DEMO
*                          ��оƬ:STM32F103R8T6/RBT6
*                           ��������οƼ����޹�˾
*
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:Wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        ��̳:http://bbs.huayusoft.com
*                        �Ա���:http://52edk.taobao.com
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���     : data.h
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2015/08/29       ��ʼ�汾
********************************************************************************
*/


#ifndef __DATA_H
#define __DATA_H



typedef struct
{
    int Frequency;                 // Frq
    int PowerPercent;
    int Power;
    int Voltage;
    int Current;
    int Temperature;  
    int OnOff;    

    int MaxPower;
    int MaxTemperature;
    int MaxFrequency;
    int MaxFrequencyOffset;
    float MaxPress;
    int State;

// ����Ϊϵͳ�������ݣ�ϵͳ��ֱ��ʹ��
    uint Systick1000;
    RtcStruct Rtc;

    uint SerialNumber;              // SN
    int Year;
    int Month;
    int Day;
    uint IdentifyNumber0;           // ID0
    uint IdentifyNumber1;           // ID1
    uint IdentifyNumber2;           // ID2
    
    DiStruct DI;
    DoStruct DO;
    AdcStruct Adc;

}DataStruct;

typedef struct
{
    Form LogoForm;
    Form CheckForm;
    Form WorkForm;
    Form SetupForm;
    Form ServiceForm;
    Form TimeForm;
    Form PortForm;
    Form * FocusFormPointer;
}MenuStruct;

#endif /*__Data_H*/

