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
*�ļ���   : os.c
*����ʱ�� : 2014.9.9
********************************************************************************
*/





#include "system.h"



typedef struct                              // �����������
{
    uint    *   Start;                      // ָ����п�ʼ
    uint    *   End;                        // ָ����н���
    uint    *   In;                         // ����һ����Ϣ
    uint    *   Out;                        // ȡ��һ����Ϣ
    ushort      Entries;                    // ��Ϣ����
} QueueStruct;

typedef struct 
{
    uint *          StackPointer;           // ջ��ָ��
    uint            Message;                // ��Ϣ
    uint            Delay;                  // ������ʱ������
} TaskStruct;

#define LogicTaskStackSum   400             // ҵ���߼�����ջ���
#define MenuTaskStackSum    400             // �˵���������ջ���
#define QueueBufferSum      40              // ��Ϣ�������

#define PendSVRegister *((volatile  uint *)0xE000ED04)
#define PendSVSet 0x10000000

#define SwitchContext() PendSVRegister = PendSVSet

static QueueStruct MessageQueue;
static uint QueueBuffer[QueueBufferSum];                // ҵ���߼���Ϣ����
#define Align __align(8)
Align static uint LogicTaskStack[LogicTaskStackSum];    //ҵ���߼�����ջ����
Align static uint MenuTaskStack[MenuTaskStackSum];      //�˵���������ջ����

static TaskStruct LogicTask;
static TaskStruct MenuTask;

TaskStruct * CurrentTaskPointer;
TaskStruct * NextTaskPointer;

/*******************************************************************************
* ����	    : ��ʼ������ջ��xPSR��PC��R14��R12��R3��R2��R1��R0��ջ˳������Cortex
*           : оƬ�����ġ��ж���ջ��оƬӲ���Զ���ɣ�R11~R4˳������OS�Լ������ġ�
* �������  : taskPointer: ������ڵ�ַ, stackRamTopPointer: ջ�ڴ���ߵ�ַ
* ���ز���  : ջ��
*******************************************************************************/
static uint * InitStack(void (*taskPointer)(void), uint * stackPointer)
{
    *(stackPointer)   = (uint)0x01000000L;       // xPSR
    *(--stackPointer) = (uint)taskPointer;       // ������ڵ�ַ
    *(--stackPointer) = (uint)0xFFFFFFFEL;       // R14 (LR)
                                                                           
    *(--stackPointer) = (uint)0x12121212L;       // R12
    *(--stackPointer) = (uint)0x03030303L;       // R3
    *(--stackPointer) = (uint)0x02020202L;       // R2
    *(--stackPointer) = (uint)0x01010101L;       // R1
    *(--stackPointer) = (uint)0x00000000L;       // R0 

    *(--stackPointer) = (uint)0x11111111L;       // R11
    *(--stackPointer) = (uint)0x10101010L;       // R10
    *(--stackPointer) = (uint)0x09090909L;       // R9
    *(--stackPointer) = (uint)0x08080808L;       // R8
    *(--stackPointer) = (uint)0x07070707L;       // R7
    *(--stackPointer) = (uint)0x06060606L;       // R6
    *(--stackPointer) = (uint)0x05050505L;       // R5
    *(--stackPointer) = (uint)0x04040404L;       // R4

    return(stackPointer);
}


/*******************************************************************************
* ����	    : ��ʼ�������
* �������  : task: �����, sp:ջ��
*******************************************************************************/
static void InitTask(TaskStruct * task, uint * sp)
{
    task->StackPointer = sp;
    task->Message = 0;
    task->Delay = 0;
}


/*******************************************************************************
* ����	    : ����ҵ���߼�����
* �������  : ҵ���߼�������ڵ�ַ
*******************************************************************************/
static void CreateLogicTask(function taskPointer)
{
    uint * sp;	
    sp = InitStack(taskPointer, &LogicTaskStack[LogicTaskStackSum - 1]);
    InitTask(&LogicTask, sp);
}

/*******************************************************************************
* ����	    : �����˵���������
* �������  : �˵�����������ڵ�ַ
*******************************************************************************/
static void CreateMenuTask(function taskPointer)
{
    uint * sp;	
    sp = InitStack(taskPointer, &MenuTaskStack[MenuTaskStackSum - 1]);
    InitTask(&MenuTask, sp);
}

/*******************************************************************************
* ����	    : �����л�
* �������  : �л���ָ������,����PendSV�ж�ʵ���л�
*******************************************************************************/
static void Schedule(TaskStruct *taskPointer)
{
    if (taskPointer != CurrentTaskPointer)
    {
        NextTaskPointer = taskPointer;
        SwitchContext();    //����PendSV�жϣ����ж���ʵ������������л�
    }
}

/*******************************************************************************
* ����	    : �������У���Ҫ�������������ʱ����
*******************************************************************************/
static void SystickRoutine(void)
{       
    if (LogicTask.Delay > 0) 
    {
        LogicTask.Delay--;
        if (LogicTask.Delay == 0)            
            Schedule(&LogicTask);
    }
}

/*******************************************************************************
* ����	    : OS����
*******************************************************************************/
static void Start(void)
{
    System.Device.Systick.Register(Systick1000, SystickRoutine); //ע��OS���ĺ���
    CurrentTaskPointer = &LogicTask;
    NextTaskPointer = CurrentTaskPointer;
    
    SwitchContext();
    EnableIrq();
    while(1);
}

/*******************************************************************************
* ����	    : ����һ����Ϣ����Ϣ�����У����ڵȴ���������Զ�����
*******************************************************************************/
static bool PostMessageQueue(uint message)
{	
    EnterCritical();
    // �˵����������£�������Ϣ���������ԱMessage���ݣ�������Ϣ����
    if (CurrentTaskPointer == &MenuTask && LogicTask.Message == 0)
    {
        LogicTask.Delay = 0;
        LogicTask.Message = message;    	
        ExitCritical();
        Schedule(&LogicTask);
        return (true);
    }

    if (MessageQueue.Entries >= QueueBufferSum)
    {                                                              
        ExitCritical();
        return (false);
    }

    MessageQueue.Entries++;
    
    *MessageQueue.In++ = message;
    if (MessageQueue.In > MessageQueue.End)
       	MessageQueue.In = MessageQueue.Start;

    ExitCritical();

    return (true);
}

/*******************************************************************************
* ����	    : �ȴ���Ϣ���У�����Ϣ����Ϊ��ʱ�������������
*******************************************************************************/
static uint PendMessageQueue(void)
{
    uint message;
    EnterCritical();

    if (LogicTask.Message != 0) goto ReadMessage;
		
    if (MessageQueue.Entries > 0)
    {                    
       	message = *MessageQueue.Out++;
       	MessageQueue.Entries--;
       	if (MessageQueue.Out > MessageQueue.End) 
            MessageQueue.Out = MessageQueue.Start;
        
       	ExitCritical();
       	return (message);
    }
    LogicTask.Delay = 0;
    ExitCritical();
    
    Schedule(&MenuTask); 
    
    EnterCritical();
ReadMessage:
    message = LogicTask.Message; 	
    LogicTask.Message = 0; 	
    ExitCritical();
    return (message);
}

/*******************************************************************************
* ����	    : �������ȴ�ʱ�䣬��λΪ1��OS���ģ�Ĭ����1mS.
* �������  : times�������������Ĭ��һ������Ϊ1mS������1000,����1����
*******************************************************************************/
static void DelayMs(int times)
{
    if (times > 0) 
    {          
       	LogicTask.Delay = times;
        
       	Schedule(&MenuTask); 
    }
}

/*******************************************************************************
* ����	    : �����л�����PendSV�ж���ʵ��
*******************************************************************************/     
__asm void PendSV_Handler(void)
{
  IMPORT  CurrentTaskPointer
  IMPORT  NextTaskPointer
    CPSID   I                           // �ر��ж�
    MRS     R0, PSP                     // ��ȡPSP��R0
    LDR     R2, =CurrentTaskPointer     // ��ȡָ��ǰ����ָ��ĵ�ַ
    LDR     R3, =NextTaskPointer        // ��ȡָ��������ָ��ĵ�ַ
    CBZ     R0, NextTask                // ��λ����������ʱ�����ж�ģʽ��ת��������
    // ��ջ                                    
    SUBS    R0, R0, #32                 // ջ����32����8Word
    STM     R0, {R4-R11}                // ѹջR4~R11,8���Ĵ���
    LDR     R1, [R2]                    // ��ȡָ��ǰ�����ָ�룬Ҳָ���һ��ԱStackPointer
    STR     R0, [R1]                    // �ѵ�ǰջ��������е�һ����Ա
    // ��ȡ��ջ��
NextTask       
    LDR     R0, [R3]                    // ��ȡ������ָ�����ֵ
    STR     R0, [R2]                    // ��ֵ����ǰ����ָ�����
    // ��ջ
    LDR     R0, [R0]                    // ����������ջ����R0
    LDM     R0, {R4-R11}                // ����ջ���ݵ�R4~R11
    ADDS    R0, R0, #32                 // ��ջ����32����8Word
    MSR     PSP, R0                     // ��ջ��д��PSP
    ORR     LR, LR, #0x04               // LR��bit2д��1���ж��˳����������ģʽ��ʹ��PSPջ
    CPSIE   I                           // ���ж�
    BX      LR                          // ����
    NOP
}


/*******************************************************************************
* ����	    : ��ʼ���û�ջΪ0������OS��һ�������л��ж��ã����忴�л����ֻ�ຯ��
*******************************************************************************/
__asm void InitPsp(uint psp)
{
    MSR     PSP, R0
    BX      LR
}

void InitOs(void)
{    
    MessageQueue.Start        = QueueBuffer;
    MessageQueue.End          = QueueBuffer + QueueBufferSum - 1;
    MessageQueue.In           = MessageQueue.Start;
    MessageQueue.Out          = MessageQueue.Start;
    MessageQueue.Entries      = 0;	
    
    InitPsp(0);     // PSP���㣬PSP�����жϴ���ģʽ�ж�
    
    System.OS.CreateLogicTask = CreateLogicTask;
    System.OS.CreateMenuTask = CreateMenuTask;
    System.OS.Start = Start;
    System.OS.PendMessageQueue = PendMessageQueue;
    System.OS.PostMessageQueue = PostMessageQueue;
    System.OS.DelayMs = DelayMs;
}

