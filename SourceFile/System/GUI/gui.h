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
*�ļ���     : gui.h
*����       : 
*ԭ��       : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2014/06/21       ��ʼ�汾
********************************************************************************
*/



#ifndef __GUI_H
#define __GUI_H

#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0


#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)
#define LCD_LINE_30              LINE(30)
#define LCD_LINE_31              LINE(31)
#define LCD_LINE_32              LINE(32)
#define LCD_LINE_33              LINE(33)
#define LCD_LINE_34              LINE(34)
#define LCD_LINE_35              LINE(35)
#define LCD_LINE_36              LINE(36)
#define LCD_LINE_37              LINE(37)
#define LCD_LINE_38              LINE(38)
#define LCD_LINE_39              LINE(39)




typedef enum 
{
    GuiDataTypeByteDec,
    GuiDataTypeShortDec,
    GuiDataTypeUshortDec,
    GuiDataTypeIntDec,
    GuiDataTypeUintDec,
    GuiDataTypeFloatDec,
    GuiDataTypeUshortHex,
    GuiDataTypeUintHex,
    GuiDataTypeString,              // ֱ����ʾ�ַ���
    GuiDataTypeSnString             // �����ʾ�ַ���(���ַ�����������)
}GuiDataType;

typedef enum
{
	GuiDataAlignLeft,
	GuiDataAlignRight
}GuiDataAlign;

typedef struct
{
    char Character;
    byte Column[16];
}Chart;

typedef struct LabelSelf
{
    byte X;
    byte Y;
    
    GuiDataType Type;
    GuiDataAlign Align;
    ushort Digits;
    
    float Offset;
    float Coefficient;
    
    void * DataPointer;
    const string * StringBlockPointer;
    struct LabelSelf * NextLabelPointer;
}Label;

typedef struct TextBoxSelf
{
    byte X;
    byte Y;
    
    GuiDataType Type;
    GuiDataAlign Align;
    ushort Digits;
    
    float Offset;
    float Coefficient;
    
    void * DataPointer;
    const string * StringBlockPointer;
    struct TextBoxSelf * NextTextBoxPointer;

    int DataMax;
    int DataMin;
    int DataStep;
    int DataBigStep;
}TextBox;

typedef struct MessageSelf
{
    bool Valid;
    byte X;
    byte Y;
    byte Len;
    char Buffer[16];
}Message;

typedef struct
{
    const byte* BackTextPointer;
    Chart     * ChartPointer;
    Label     * LabelPointer;
    TextBox   * TextBoxPointer;
    TextBox   * FocusTextBoxPointer;
}Form;




extern void InitGui(void);
    
#endif 




