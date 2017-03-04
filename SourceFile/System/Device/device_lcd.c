/**
  ******************************************************************************
  * @file    stm32f429i_discovery_lcd.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    28-October-2013
  * @brief   This file includes the LCD driver for ILI9341 Liquid Crystal 
  *          Display Modules of STM32F429I-DISCO kit (MB1075).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drive.h"
#include "system.h"


#define PinCs   PcOut->Bit2
#define PinRs   PdOut->Bit13

static void InitSpi(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef    SPI_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;   //SCK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOF, &GPIO_InitStructure);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOF, &GPIO_InitStructure);      //MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOF, &GPIO_InitStructure);      // MOSI

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);
  
    SPI_I2S_DeInit(SPI5);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI5, &SPI_InitStructure);
    SPI_Cmd(SPI5, ENABLE);
}

static void SendInstruct(byte instruct)
{
    PinCs = 0;
    PinRs = 0;
    
    SPI_I2S_SendData(SPI5, instruct);
    while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET) ;
    while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_BSY) != RESET);
    
    PinCs = 1;
}

static void SendData(byte data)
{
    PinCs = 0;
    PinRs = 1;
    
    SPI_I2S_SendData(SPI5, data);  
    while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET) ;
    while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_BSY) != RESET);
    
    PinCs = 1;
}

static void InitRegister(void)
{
    SendInstruct(0xCA);
    SendData(0xC3);
    SendData(0x08);
    SendData(0x50);
    SendInstruct(0xCF);
    SendData(0x00);
    SendData(0xC1);
    SendData(0x30);
    SendInstruct(0xED);
    SendData(0x64);
    SendData(0x03);
    SendData(0x12);
    SendData(0x81);
    SendInstruct(0xE8);
    SendData(0x85);
    SendData(0x00);
    SendData(0x78);
    SendInstruct(0xCB);
    SendData(0x39);
    SendData(0x2C);
    SendData(0x00);
    SendData(0x34);
    SendData(0x02);
    SendInstruct(0xF7);
    SendData(0x20);
    SendInstruct(0xEA);
    SendData(0x00);
    SendData(0x00);
    SendInstruct(0xB1);
    SendData(0x00);
    SendData(0x1B);
    SendInstruct(0xB6);
    SendData(0x0A);
    SendData(0xA2);
    SendInstruct(0xC0);
    SendData(0x10);
    SendInstruct(0xC1);
    SendData(0x10);
    SendInstruct(0xC5);
    SendData(0x45);
    SendData(0x15);
    SendInstruct(0xC7);
    SendData(0x90);
    SendInstruct(0x36);
    SendData(0xC8);
    SendInstruct(0xF2);
    SendData(0x00);
    SendInstruct(0xB0);
    SendData(0xC2);
    SendInstruct(0xB6);
    SendData(0x0A);
    SendData(0xA7);
    SendData(0x27);
    SendData(0x04);

    /* colomn address set */
    SendInstruct(0x2A);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0xEF);
    /* Page Address Set */
    SendInstruct(0x2B);
    SendData(0x00);
    SendData(0x00);
    SendData(0x01);
    SendData(0x3F);
    SendInstruct(0xF6);
    SendData(0x01);
    SendData(0x00);
    SendData(0x06);
  
    SendInstruct(0x2C);
    Delay(200);
  
    SendInstruct(0x26);
    SendData(0x01);
  
    SendInstruct(0xE0);
    SendData(0x0F);
    SendData(0x29);
    SendData(0x24);
    SendData(0x0C);
    SendData(0x0E);
    SendData(0x09);
    SendData(0x4E);
    SendData(0x78);
    SendData(0x3C);
    SendData(0x09);
    SendData(0x13);
    SendData(0x05);
    SendData(0x17);
    SendData(0x11);
    SendData(0x00);
    SendInstruct(0xE1);
    SendData(0x00);
    SendData(0x16);
    SendData(0x1B);
    SendData(0x04);
    SendData(0x11);
    SendData(0x07);
    SendData(0x31);
    SendData(0x33);
    SendData(0x42);
    SendData(0x05);
    SendData(0x0C);
    SendData(0x0A);
    SendData(0x28);
    SendData(0x2F);
    SendData(0x0F);
  
    SendInstruct(0x11);
    Delay(200);
    SendInstruct(0x29);
    /* GRAM start writing */
    SendInstruct(0x2C);
 }



static void RgbPortConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                           RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

/*|  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
  |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
  |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
  |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
  |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
  |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
  |           LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04                |              
  |           LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10                |*/

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | \
                               GPIO_Pin_11 | GPIO_Pin_12;
                             
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, 0x09);     // ≈‰÷√Ãÿ ‚
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, 0x09);     // ≈‰÷√Ãÿ ‚
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | \
                               GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LTDC);
  
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);
  
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
  
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
  
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOF, &GPIO_InitStruct);     

    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, 0x09);    // ≈‰÷√Ãÿ ‚
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, 0x09);    // ≈‰÷√Ãÿ ‚

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | \
                               GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void InitLayer(void)
{
    LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct; 
  
    /* In this case all the active display area is used to display a picture then :
    Horizontal start = horizontal synchronization + Horizontal back porch = 30 
    Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
    Vertical start   = vertical synchronization + vertical back porch     = 4
    Vertical stop   = Vertical start + window height -1  = 4 + 320 -1      */      
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 30 - 1); 
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 4 - 1);
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3);
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;
    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
    
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
    LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
    
    LTDC_LayerCmd(LTDC_Layer1, ENABLE); 
    LTDC_LayerCmd(LTDC_Layer2, ENABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
    LTDC_DitherCmd(ENABLE);
}


void InitLcd(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    LTDC_InitTypeDef       LTDC_InitStruct;
    

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    PinCs = 1;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  // CS
      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  // RS
    
    InitSpi();
    InitRegister();
    
    RgbPortConfig();  

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); 
  
    LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
    LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
    LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
    LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
  
    LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
    LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;  
  
    RCC_PLLSAIConfig(192, 7, 4);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
    RCC_PLLSAICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
  
    LTDC_InitStruct.LTDC_HorizontalSync = 9;
    LTDC_InitStruct.LTDC_VerticalSync = 1;
    LTDC_InitStruct.LTDC_AccumulatedHBP = 29; 
    LTDC_InitStruct.LTDC_AccumulatedVBP = 3;  
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
    LTDC_InitStruct.LTDC_TotalWidth = 279; 
    LTDC_InitStruct.LTDC_TotalHeigh = 327;
    LTDC_Init(&LTDC_InitStruct);  
    InitLayer();
  
    LTDC_LayerAlpha(LTDC_Layer2, 0);
    LTDC_ReloadConfig(LTDC_IMReload);
  
    LTDC_Cmd(ENABLE);
#if 1
    int i;
    for (i = 0; i < 320 * 240; i++)
        *(volatile ushort*)(LCD_FRAME_BUFFER + (2*i)) = 0x00FF;
#endif
} 
