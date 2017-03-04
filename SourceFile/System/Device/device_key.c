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
*�ļ���     : device_key.c
*����       : �����豸
*ԭ��       : ����SystemTick 100/�� 10mSɨ���ȡ����ֵ������ֵ��Ϊ������̰�����
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/



#include "drive.h"
#include "system.h"

#if 0
static uint * pPinBeep;
static uint * pPinX0;
static uint * pPinX1;
static uint * pPinX2;
static uint * pPinX3;
static uint * pPinY0;
static uint * pPinY1;
#endif
//static uint * pPinKey;

#define ShortInterval       2		// 
#define LongInterval        20		// ??????
#define InvalidInterval     2       // ??????
#define DoubleHitInterval   10		// ?????????
#define KeyBeepInterval     10      // ?????

static byte Scan = invalid;
static byte ScanData;

static byte ValidCounter = 0;
static byte InvalidCounter = 0;
static byte DoubleHitCounter = 0;
//static byte KeyBeepCounter = 0;
static bool Enable = false;

#if 0
static byte RemapKey(byte scan) 
{
    switch(scan)
    {
        case 0xEF:  return(0);           
        case 0xDF:  return(1);         
        case 0xBF:  return(2);            
        case 0x7F:  return(3);       
        case 0xFE:  return(4);         
        case 0xFD:  return(5);       
        case 0xFB:  return(6);   
        case 0xF7:  return(7);
        default:    return(invalid);  
    }
}

static byte RemapLongKey(byte scan) 
{
    switch(scan)
    {
        case 0xEF:  return(0x30);
        case 0xDF:  return(0x31);
        case 0xBF:  return(0x32);
        case 0x7F:  return(0x33);
        case 0xFE:  return(0x34);
        case 0xFD:  return(0x35);
        case 0xFB:  return(0x36);
        case 0xF7:  return(0x37);
        default:    return(invalid);
    }	
}
#endif
static void EnableKey(bool status)
{
    Enable = status;
}

void KeySystick100Routine(void) 
{
    byte key;
    static bool Switch = false;
    Switch = ~Switch;
    if (Switch)
    {
#if 0
        key = invalid;

        if(*pPinX3 == 0)  key &= 0x7F;
        if(*pPinX2 == 0)  key &= 0xBF;
        if(*pPinX1 == 0)  key &= 0xDF;
        if(*pPinX0 == 0)  key &= 0xEF;
        
        *pPinY0 = 0;
        *pPinY1 = 1;
        
        Delay(1);
        if(*pPinX3 == 0)  key &= 0xF7;
        if(*pPinX2 == 0)  key &= 0xFB;
        if(*pPinX1 == 0)  key &= 0xFD;
        if(*pPinX0 == 0)  key &= 0xFE;
        
        *pPinY1 = 0;
        *pPinY0 = 1;
#endif
        //if (*pPinKey & 0x01 == 0)
        //if(PaIn(0) == 0)
        //key = GPIO_ReadInputData(GPIOA) & 0x01;
        key = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
        if (!key)
            key = invalid;
        else
            key = 0;
        
        if (Enable)
            Scan = key;
        else
            Scan = invalid;
    }
    else
    {
        //if (KeyBeepCounter == 1) *pPinBeep = 0;
            
        //if (KeyBeepCounter > 0) KeyBeepCounter--;
    	
        if (Scan != invalid) 
        {
            ScanData = Scan;
            InvalidCounter = 0;
    		
            if (++ValidCounter > LongInterval) 
                ValidCounter = LongInterval;
        }
        else
    	{
            InvalidCounter++;
            if (InvalidCounter >= InvalidInterval)
            {   
                InvalidCounter = InvalidInterval;
           
                if(DoubleHitCounter)
                {
                    DoubleHitCounter--;
                    ValidCounter = 0;
                    return;
                }

                if (ValidCounter < ShortInterval) return;

                if (ValidCounter == LongInterval) 
                    //key = RemapLongKey(ScanData);
                    key = ScanData;
                else if (ValidCounter >= ShortInterval) 
                    //key = RemapKey(ScanData);
                    key = ScanData;
                else
                    key = invalid;
                
                if (key != invalid)
                {
                    PostMessage(MessageKey, key);  
                    //*pPinBeep = 1;
                    //KeyBeepCounter = KeyBeepInterval;
                    DoubleHitCounter = DoubleHitInterval;
                }
                ValidCounter = 0;
        	} 
        }
    }
}

void InitKey(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    
    /* Enable the BUTTON Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
         
    System.Device.Key.Enable = EnableKey;
}

