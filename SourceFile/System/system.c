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
*�ļ���   : system.c
*����     : ϵͳӦ���ļ�
*����ʱ�� : 2013.7.21
********************************************************************************
*/

#include "system.h"

uint Empty;
void Dummy(void) {};

uint CriticalNesting = 0;

uint RomBase;
uint RamBase;
DataStruct * AppDataPointer;
MenuStruct * AppMenuPointer;

static const byte HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define DelayUsTick MainClock / 4000000

#define DelayMsTick MainClock / 4000

void Delay(int times)
{
    volatile int counter;
    counter = times;
	while(counter--);
}

void DelayUs(int times)
{
	while(times--)
		Delay(DelayUsTick);
}

void DelayMs(int times)
{
	while(times--)
		Delay(DelayMsTick);
}


byte HexToAscii(byte hex)
{
	Assert(hex < 16);

    if (hex < 16) 
        return HextoAscii[hex];
    else 
         return 0x30;
}

/*******************************************************************************
* ������	: PostMessage
* ����	    : ��LogicTask������Ϣ
* �������  : messageType: ��Ϣ����
*             data: ��Ϣֵ
* ���ز���  : ��
*******************************************************************************/
void PostMessage(MessageEnum message, uint data)
{
    data |= message << 24;
    System.OS.PostMessageQueue(data);	
}


extern void InitDevice(void);

static void Init(void)
{
    DisableIrq();
    
    RomBase = (uint)Init & 0xFF000000;
    RamBase = (uint)(&RamBase) & 0xFF000000;
    
    
	InitOs();
    InitDevice();
    InitGui();
}

SystemStruct System = 
{
	Init,
};


