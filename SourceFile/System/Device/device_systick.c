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
*�ļ���     : device_systick.c
*����       : ϵͳ����
*ԭ��       : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/12/16       ��ʼ�汾
********************************************************************************
*/



#include "drive.h"
#include "system.h"

extern void KeySystick100Routine(void);

extern void RtcSystick100Routine(void);

extern void TimerSystick1000Routine(void);

extern void Usart1Systick1000Routine(void);

extern void AdcSystick10000Routine(void);

extern void DiSystick10000Routine(void);

extern void DoSystick10000Routine(void);

extern void SysTick_Handler(void);

#define Systicks 10000     // ÿ���ֲ����жϵĴ�����Ҳ���ǽ�����

#define Systick10000Sum 2
#define Systick1000Sum  4
#define Systick100Sum   10

static function Systick10000RegisterPointerBlock[Systick10000Sum] =
{
    Dummy, Dummy
};

static function Systick1000RegisterPointerBlock[Systick1000Sum] =
{
    Dummy, Dummy, Dummy, Dummy
};

static function Systick100RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy
};

/*******************************************************************************
* ����	    : ϵͳ����ע�ắ��������ע��ص���������ͬ�ٶȵ�ϵͳ���������С�
* �������  : systemTick ϵͳ�����ٶ����� registerFunction ע��ĺ���ָ��
* ���ز���  : bool���������������Ƿ�ɹ�
*******************************************************************************/
static bool RegisterSystick(SystickEnum type, function functionPointer)
{
    static byte Systick100Counter = 0;
    static byte Systick1000Counter = 0;
    static byte Systick10000Counter = 0;

    switch(type)
    {
        case Systick100:
            if (Systick100Counter == Systick100Sum) return(false);
            Systick100RegisterPointerBlock[Systick100Counter++] = functionPointer;  
            return(true);
        case Systick1000:
            if (Systick1000Counter == Systick1000Sum) return(false);
            Systick1000RegisterPointerBlock[Systick1000Counter++] = functionPointer;   
            return(true);
        case Systick10000:
            if (Systick10000Counter == Systick10000Sum) return(false);
            Systick10000RegisterPointerBlock[Systick10000Counter++] = functionPointer;   
            return(true);
        default:
            return(false);
    }
}

/*******************************************************************************
* ����	    : ϵͳ�����ж���ڣ��ǳ���Ҫ��ÿ��10000�Σ���0.1mSһ��
*           : ϵͳ����ͨ��switchϸ��Ϊÿ��1000�Σ�ÿ��100�ε����н���
*           : ͬ��ֱ�ӵ��ã��߲����ע�����
*******************************************************************************/
void SysTick_Handler(void)
{
    byte div;
    static byte Counter = 0;
    if(++Counter == 100) Counter = 0;
    Systick10000RegisterPointerBlock[0]();
    Systick10000RegisterPointerBlock[1]();
    //AdcSystick10000Routine();
    //DiSystick10000Routine();
    //DoSystick10000Routine();
    div = Counter / 10;
    switch(Counter % 10)
    {
        case 0: Systick1000RegisterPointerBlock[0](); break;
        case 1: Systick1000RegisterPointerBlock[1](); break;
        case 2: Systick1000RegisterPointerBlock[2](); break;
        case 3: Systick1000RegisterPointerBlock[3](); break;
        case 4: Systick100RegisterPointerBlock[div]();break;
        case 5: AppDataPointer->Systick1000++;        break;
        //case 6: Usart1Systick1000Routine();           break;
        case 7: TimerSystick1000Routine();            break;
        case 8:                                       break;
        case 9:
            switch(div)
            {
                //case 0: RtcSystick100Routine();       break;
                case 1: KeySystick100Routine();       break;
                default:                              break;
            }
            break;
    }    

} 

void InitSystick(void)
{
    SysTick_Config(SystemCoreClock / Systicks);             // ���ý����ٶ�
    System.Device.Systick.Register = RegisterSystick;       // �����ӿں���ָ��
}





