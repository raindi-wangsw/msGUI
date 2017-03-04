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
*�ļ���     : gui.c
*����       : ͼ�ο��ļ�
*ԭ��       : ���������ƣ���һ��Menu��Ϊ���Form,һ��Form��������ؼ�����Chart��
*           : Label��TextBox�ȣ�����һ��������ȼ�����MenuTaskȥ����ִ����Щ�ؼ���Ϣ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/
#include "fonts.h"
#include "drive.h"
#include "system.h"

#define LCD_DEFAULT_FONT         Font16x24

static sFONT *LCD_Currentfonts;
/* Global variables to set the written text color */
static uint16_t CurrentTextColor   = 0x0000;
static uint16_t CurrentBackColor   = 0xFFFF;
/* Default LCD configuration with LCD Layer 1 */
static uint32_t CurrentFrameBuffer = LCD_FRAME_BUFFER;
//static uint32_t CurrentLayer = LCD_BACKGROUND_LAYER;


void LCD_SetLayer(uint32_t Layerx)
{
  if (Layerx == LCD_BACKGROUND_LAYER)
  {
    CurrentFrameBuffer = LCD_FRAME_BUFFER; 
//    CurrentLayer = LCD_BACKGROUND_LAYER;
  }
  else
  {
    CurrentFrameBuffer = LCD_FRAME_BUFFER + BUFFER_OFFSET;
//    CurrentLayer = LCD_FOREGROUND_LAYER;
  }
} 

void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}


void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

void LCD_Clear(uint16_t Color)
{
  uint32_t index = 0;
  
  for (index = 0; index < BUFFER_OFFSET; index++)
  {
    *(volatile uint16_t*)(CurrentFrameBuffer + (2*index)) = Color;
  } 
}

/*
    �˺���д��̫�����ӣ��ڱ������Ż�Ϊ0�ȼ�ʱ���޷�������1��2��3�ȼ�������������Ҫ�Ż���д������������
*/
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0;
  uint32_t counter = 0;
  uint32_t xpos =0;
  uint32_t Xaddress = 0;
  uint32_t width;
  int t0;
  int t1;

  width = LCD_PIXEL_WIDTH - LCD_Currentfonts->Width;
  xpos = Xpos*LCD_PIXEL_WIDTH*2;
  Xaddress += Ypos;
  t0 = 0x80 << ((LCD_Currentfonts->Width / 12 ) << 3) ;
  
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
    {
      t1 = CurrentFrameBuffer + (2*Xaddress) + xpos;
      if((((c[index] & (t0 >> counter)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
      {
         *(volatile uint16_t*)t1 = CurrentBackColor;
      }
      else
      {
         *(volatile uint16_t*)t1 = CurrentTextColor;         
      }
      Xaddress++;
    }     
      Xaddress += width;
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..29
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;

  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{  
  uint16_t refcolumn = 0;
  /* Send the string character by character on lCD */
  while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & (((refcolumn + LCD_Currentfonts->Width) & 0xFFFF) >= LCD_Currentfonts->Width)))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by width */
    refcolumn += LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;
  }
}

void Text(void)
{ 
  LCD_Clear(LCD_COLOR_RED);
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LCD_LINE_1, (byte*)"    Display    ");
  LCD_DisplayStringLine(LCD_LINE_5, (byte*)"               ");
  LCD_DisplayStringLine(LCD_LINE_9, (byte*)"     Test      ");
}

void Image(const uint * imagePointer)
{ 
    DMA2D_InitTypeDef              DMA2D_InitStruct;
    DMA2D_FG_InitTypeDef           DMA2D_FG_InitStruct;
    DMA2D_DeInit();
    DMA2D_InitStruct.DMA2D_Mode = DMA2D_M2M;       
    DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
    DMA2D_InitStruct.DMA2D_OutputGreen = 0x00;      
    DMA2D_InitStruct.DMA2D_OutputBlue = 0x00;     
    DMA2D_InitStruct.DMA2D_OutputRed = 0x00;                
    DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0;                  
    DMA2D_InitStruct.DMA2D_OutputMemoryAdd = 0xD0000000;                
    DMA2D_InitStruct.DMA2D_OutputOffset = 0x0;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = 320;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = 240;
    DMA2D_Init(&DMA2D_InitStruct);   
    DMA2D_FG_InitStruct.DMA2D_FGMA = (uint)imagePointer;
    DMA2D_FG_InitStruct.DMA2D_FGO = 0x00;
    DMA2D_FG_InitStruct.DMA2D_FGCM = CM_RGB565;
    DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_VALUE = 0x00;
    DMA2D_FG_InitStruct.DMA2D_FGC_BLUE = 0x00;
    DMA2D_FG_InitStruct.DMA2D_FGC_GREEN = 0x00;
    DMA2D_FG_InitStruct.DMA2D_FGC_RED = 0x00;
    DMA2D_FGConfig(&DMA2D_FG_InitStruct);
    Delay(100);
    DMA2D_StartTransfer();  
    while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET){}
}

/*******************************************************************************
* ����	    : ����ҳ��
*******************************************************************************/
static void Parse(Form * formPointer)
{  

}




void InitGui(void)
{
    System.Gui.Function.Image = Image;
    System.Gui.Function.Text = Text;
    System.Gui.Parse = Parse;
    
    LCD_SetFont(&LCD_DEFAULT_FONT); 
    LCD_SetLayer(LCD_BACKGROUND_LAYER);
    LCD_Clear(LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_RED); 
    LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"     msGUI     ");
    LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*)"    V1.0.1     ");
    LCD_DisplayStringLine(LCD_LINE_9,(uint8_t*)"    Wangsw     ");
}



