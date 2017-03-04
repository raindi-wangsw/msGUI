/*
******************************************************************************
*                                嵌入式微系统
*                                   msOS
*
*                            硬件平台:msPLC DEMO
*                          主芯片:STM32F103R8T6/RBT6
*                           深圳市雨滴科技有限公司
*
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        论坛:http://bbs.huayusoft.com
*                        淘宝店:http://52edk.taobao.com
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
******************************************************************************
*文件名   : drive.h
*作用     : 驱动库头文件
*创建时间 : 2013.7.21
******************************************************************************
*/

#ifndef __DRIVE_H
#define __DRIVE_H
#include "stm32f4xx.h"

typedef struct                                  
{
    volatile unsigned int Bit0;                                   
    volatile unsigned int Bit1;                                  
    volatile unsigned int Bit2;                                
    volatile unsigned int Bit3;                                 
    volatile unsigned int Bit4;  
    volatile unsigned int Bit5;                                   
    volatile unsigned int Bit6;                                   
    volatile unsigned int Bit7;                                  
    volatile unsigned int Bit8;                                
    volatile unsigned int Bit9;                                 
    volatile unsigned int Bit10;                               
    volatile unsigned int Bit11;                                  
    volatile unsigned int Bit12;                                 
    volatile unsigned int Bit13;                                  
    volatile unsigned int Bit14;
    volatile unsigned int Bit15;    
}BitBandStruct;

//IO口地址映射
#define GPIOA_IDR_ADDR    (GPIOA_BASE+0x10) // 0x40000000 + 0x00020000 + 0x10
#define GPIOB_IDR_ADDR    (GPIOB_BASE+0x10)
#define GPIOC_IDR_ADDR    (GPIOC_BASE+0x10)
#define GPIOD_IDR_ADDR    (GPIOD_BASE+0x10)
#define GPIOE_IDR_ADDR    (GPIOE_BASE+0x10)
#define GPIOF_IDR_ADDR    (GPIOF_BASE+0x10)
#define GPIOG_IDR_ADDR    (GPIOG_BASE+0x10)
#define GPIOH_IDR_ADDR    (GPIOH_BASE+0x10)
#define GPIOI_IDR_ADDR    (GPIOI_BASE+0x10)
#define GPIOJ_IDR_ADDR    (GPIOJ_BASE+0x10)
#define GPIOK_IDR_ADDR    (GPIOK_BASE+0x10)

#define GPIOA_ODR_ADDR    (GPIOA_BASE+0x14)
#define GPIOB_ODR_ADDR    (GPIOB_BASE+0x14)
#define GPIOC_ODR_ADDR    (GPIOC_BASE+0x14)
#define GPIOD_ODR_ADDR    (GPIOD_BASE+0x14)
#define GPIOE_ODR_ADDR    (GPIOE_BASE+0x14)
#define GPIOF_ODR_ADDR    (GPIOF_BASE+0x14) 
#define GPIOG_ODR_ADDR    (GPIOG_BASE+0x14) 
#define GPIOH_ODR_ADDR    (GPIOH_BASE+0x14) 
#define GPIOI_ODR_ADDR    (GPIOI_BASE+0x14) 
#define GPIOJ_ODR_ADDR    (GPIOJ_BASE+0x14) 
#define GPIOK_ODR_ADDR    (GPIOK_BASE+0x14) 

//IO口操作,只对单一的IO口!确保n的值小于16!
#define BitBand(ioAddress, pinNumber) ((ioAddress & 0xF0000000)+0x2000000+((ioAddress&0xFFFFF)<<5)+(pinNumber<<2)) 

#define PaIn ((BitBandStruct *)(BitBand(GPIOA_IDR_ADDR, 0)))
#define PbIn ((BitBandStruct *)(BitBand(GPIOB_IDR_ADDR, 0)))
#define PcIn ((BitBandStruct *)(BitBand(GPIOC_IDR_ADDR, 0)))
#define PdIn ((BitBandStruct *)(BitBand(GPIOD_IDR_ADDR, 0)))
#define PeIn ((BitBandStruct *)(BitBand(GPIOE_IDR_ADDR, 0)))
#define PfIn ((BitBandStruct *)(BitBand(GPIOF_IDR_ADDR, 0)))
#define PgIn ((BitBandStruct *)(BitBand(GPIOG_IDR_ADDR, 0)))
#define PhIn ((BitBandStruct *)(BitBand(GPIOH_IDR_ADDR, 0)))
#define PiIn ((BitBandStruct *)(BitBand(GPIOI_IDR_ADDR, 0)))
#define PjIn ((BitBandStruct *)(BitBand(GPIOJ_IDR_ADDR, 0)))
#define PkIn ((BitBandStruct *)(BitBand(GPIOK_IDR_ADDR, 0)))

#define PaOut ((BitBandStruct *)(BitBand(GPIOA_ODR_ADDR, 0)))
#define PbOut ((BitBandStruct *)(BitBand(GPIOB_ODR_ADDR, 0)))
#define PcOut ((BitBandStruct *)(BitBand(GPIOC_ODR_ADDR, 0)))
#define PdOut ((BitBandStruct *)(BitBand(GPIOD_ODR_ADDR, 0)))
#define PeOut ((BitBandStruct *)(BitBand(GPIOE_ODR_ADDR, 0)))
#define PfOut ((BitBandStruct *)(BitBand(GPIOF_ODR_ADDR, 0)))
#define PgOut ((BitBandStruct *)(BitBand(GPIOG_ODR_ADDR, 0)))
#define PhOut ((BitBandStruct *)(BitBand(GPIOH_ODR_ADDR, 0)))
#define PiOut ((BitBandStruct *)(BitBand(GPIOI_ODR_ADDR, 0)))
#define PjOut ((BitBandStruct *)(BitBand(GPIOJ_ODR_ADDR, 0)))
#define PkOut ((BitBandStruct *)(BitBand(GPIOK_ODR_ADDR, 0)))



#endif

