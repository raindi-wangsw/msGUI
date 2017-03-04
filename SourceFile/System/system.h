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
*                                ��ʶ:wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        ��̳:http://bbs.huayusoft.com
*                        �Ա���:http://52edk.taobao.com
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���   : system.h
*����     : ϵͳ��ȫ��ͷ�ļ�
*����ʱ�� : 2013.7.21
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



/*
1�����Ͷ������C#����������C#��׼�����õ�Ϊint
2��char��C#��Ϊ16bit��unicode��keil C��Ϊ8bit
*/
//8bit
//typedef char                sbyte;	    /*char Keil4Ĭ�϶���Ϊbyte����Ҫ�����ţ���Ҫ����keil����*/
typedef unsigned char       byte;       
//16bit
//typedef short             short
typedef unsigned short      ushort;
//32bit
//typedef int               int;
typedef unsigned int        uint;

//�ַ���
typedef char *              string;

typedef unsigned char       bool;

typedef union
{
    byte    Byte[2];
    ushort  Ushort;
}Union16;

typedef union
{
    byte    Byte[4];
    ushort  Ushort[2];
    uint    Uint;
}Union32;

typedef void (*function)(void);
#define Function(address)   ((function)(address))()

typedef enum
{
    MessageKey      = 0x01,     // ��Ϣ���Ͳ���Ϊ0x00
    MessageTimer    = 0x02
    /*���������*/
}MessageEnum;

#include "os.h"
#include "gui.h"
#include "device.h"  
#include "data.h"




#define true                        1
#define false                       0

#define on                          1
#define off                         0

#define ok                          1
#define yes                         1
#define no                          0

#define null                        ((void *) 0)                    // ��ָ��
#define invalid                     0xFF                            // ��Чֵ
#define MainClock                   168000000                       // ϵͳʱ��Ƶ��
#define IdentifyNumberAddress       0x1FFFF7E8                      // оƬID��

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // ��λ
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // ��λ
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // ��ȡλ

#define Byte0(data)                 ((byte *)(&(data)))[0]
#define Byte1(data)                 ((byte *)(&(data)))[1]
#define Byte2(data)                 ((byte *)(&(data)))[2]
#define Byte3(data)                 ((byte volatile *)(&(data)))[3]

#define Ushort0(data)               ((ushort *)(&(data)))[0]
#define Ushort1(data)               ((ushort *)(&(data)))[1]

#define Byte(data)                  *((byte *)(&data)) 
#define Ushort(data)                *((ushort *)(&data)) 
#define Uint(data)                  *((uint *)(&data)) 
#define Float(data)                 *(float *)(&data)

#define pByte(address)              *((byte *)(address)) 
#define pUshort(address)            *((ushort *)(address)) 
#define pUint(address)              *((uint *)(address)) 
#define pFloat(address)             *(float *)(address)

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0
 				  
extern uint Empty;
extern void Dummy(void);
extern uint CriticalNesting;

#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}

#define EnableIrq()         __enable_irq();
#define DisableIrq()        __disable_irq();

extern uint RomBase;
extern uint RamBase;
extern DataStruct * AppDataPointer;
extern MenuStruct * AppMenuPointer;

extern void Delay(int times);

extern void DelayUs(int times);

extern void DelayMs(int times);

extern byte HexToAscii(byte hex);

extern void PostMessage(MessageEnum message, uint data);

/*******************************************************************************
* �ṹ����	: SystemStruct
* ����	    : msOS������Ҫ��һ���ṹ�壬��װ��System������нӿ�
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
typedef struct
{
    void (*Init)(void);
    
    struct Device
    {     
        struct Adc
        {
            void (*Register)(AdcChannelEnum channel, ushort * dataPointer);
        }Adc;

        struct DI
        {
            void (*Config)(DiModeEnum mode);
            void (*Register)(int * frequencyPointer, short *dutyRatioPointer);
           
            struct Capture
            {
                void (*SetParameter)(PwmEnum channel);
            }Capture;
        }DI;
        
        struct DO
        {
            void (*Config)(DoModeEnum mode);
            struct Pwm
            {
                void (*Open)(PwmEnum channel);
                void (*Close)(PwmEnum channel); 
                void (*SetParameter)(PwmEnum channel, int prescaler, int period, int dutyRatio);
                void (*SetDutyRatio)(PwmEnum channel, int dutyRatio);
            }Pwm;
        }DO;

        struct Key
        {
            void (*Enable)(bool status);
        }Key;
        
        struct Misc
        {
            void (*SetBeep)(bool status);
        }Misc;

        struct Sdram
        {
            void  (*Init)(void);
            void  (*Write)(uint * dataPointer, uint address, uint sum);
            void  (*Read)(uint * dataPointer, uint address, uint sum);
        }Sdram;
        
        struct Lcd
        {
            void (* DisplayString)(byte y, string string);
        }Lcd;
        
        struct Usart1
        {
            void (*Open)(void);
            void (*Close)(void);
            bool (*WriteByte)(byte data);
            void (*Write)(byte * dataPointer, int sum);
        }Usart1;

        struct Usart2
        {
            void (*Register)(uint rxdFucntion);
            void (*Write)(byte * dataPointer, int sum);
        }Usart2;
        
        struct Usart3
        {
            void (*Register)(uint rxdFucntion);
            void (*Write)(byte * dataPointer, int sum);
        }Usart3;		
		
        struct Timer
        {
            void (*Start)(int id, TimerModeEnum mode, int times, function functionPointer);
            void (*Stop)(int id); 
        }Timer;

        struct Storage
        {
            struct Parameter
            {
                void (*ReadAll)(void);
                bool (*Write)(void * dataPointer);  
            }Parameter;
            struct Log
            {
                void (*Erase)(void);
                char * (*Read)(int sequence);
                bool (*Write)(char *fmt, ...);
            }Log;
        }Storage;

        struct Systick
        {
            bool (*Register)(SystickEnum type, function functionPointer);
        }Systick;
        
    }Device;

    struct OS
    {
        void (* CreateLogicTask)(function taskPointer);
        void (* CreateMenuTask)(function taskPointer);
        void (* Start)(void);
        bool (* PostMessageQueue)(uint message);
        uint (* PendMessageQueue)(void);
        void (* DelayMs)(int times); 
    }OS;

    struct Gui
    {
        void (*Parse)(Form * formPointer);
        struct
        {
            void (* Text)(void);
            void (* Image)(const uint * imagePointer);
        }Function;
        struct
        {
            void (* AddLabel)(Form * formPointer, Label *labelPointer);
            void (* AddTextBox)(Form * formPointer, TextBox *textBoxPointer);
            void (* Init)(Form *formPointer);
            void (*SwitchTextBoxFocus)(void);
            void (*ModifyTextBoxData)(KeyEnum key);
            void (*AddMessage)(int id, int x, int y, char *fmt, ...);
            void (*DeleteMessage)(int id);
        }Form;
    }Gui;
}SystemStruct;

extern SystemStruct System;

#endif 
