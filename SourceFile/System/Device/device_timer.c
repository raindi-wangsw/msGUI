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
*�ļ���     : device_timer.c
*����       : �����ʱ���豸
*ԭ��       : ����ϵͳ����1000/S����1mSһ������������ʱ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/



#include "system.h"


typedef struct 
{
    int  Times;
    function RegisterFunction;
}TimerStruct;

#define TimerSum 8    //����ܳ���32

static uint State = 0;
static uint Mode;
static TimerStruct TimerBlock[TimerSum];




/*******************************************************************************
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ã�ʵ�����ⶨʱ����
*******************************************************************************/
void TimerSystick1000Routine(void)
{
    int i = 0;
    uint stateBackup;
    
    if(State == 0x00) return;

    stateBackup = State;
    
    while(stateBackup)	
    {
        if((stateBackup & 0x01) == 1) 
        {
            if((--TimerBlock[i].Times) == 0) 
            {
                ResetBit(State, i);
                if(GetBit(Mode, i)) 
                    PostMessage(MessageTimer, (uint)(TimerBlock[i].RegisterFunction) - RomBase);
                else 
                    TimerBlock[i].RegisterFunction(); 
            }
        }
		
        stateBackup = stateBackup >> 1;
        i++;
    }
}

/*******************************************************************************
* ����	    : �����ʱ��
* �������  : id:��ʱ��ID��
*           : handleMode: ��ʱ��ģʽ��
*           :             TimerSystickHandleϵͳ�����д���
*           :             TimerMessageHandle�߼������д���
*           : times:��ʱ����������ϵͳ����Ϊ��λ
*           : registerFunction: ע��ص�ִ�к�������ʱ��ʱ��ִ�д˺�����
* ���ز���  : bool����
*******************************************************************************/
static void Start(int id, TimerModeEnum mode, int times, function functionPointer)
{
    TimerBlock[id].Times  = times;
    TimerBlock[id].RegisterFunction = functionPointer;
           
    if(mode) 
        SetBit(Mode, id);
    else 
        ResetBit(Mode, id);
    			
    SetBit(State, id); 
}

/*******************************************************************************
* ����	    : ֹͣĳһ·�������ʱ��
* �������  : idΪ0��1��2...
* ����ֵ    : bool���ͣ�trueΪ�رղ�û�лص�������false���������������Ѿ��ر�
*******************************************************************************/
static void Stop(int id) 
{
    ResetBit(State, id);
}

void InitTimer(void)
{
    System.Device.Timer.Start = Start;
    System.Device.Timer.Stop = Stop; 
}
