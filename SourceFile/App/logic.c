/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main program body
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

#include <stddef.h>
#include "system.h"

#include "image.h"








static void InitLogic(void)
{
}
void LogicTask(void)
{ 
    uint message;
    uint data; 
    int i;


    System.Device.Key.Enable(true);

    InitLogic();
    while(true)
    {     
        message = System.OS.PendMessageQueue();
        data = message & 0x00FFFFFF;

        switch(message >> 24)
        {
            case MessageKey:                    //°´¼üÏûÏ¢
                if (++i >= 2) i = 0;
                switch (i)
                {
                    case 0:
                        System.Gui.Function.Text();
                        break;
                    case 1:
                        System.Gui.Function.Image(ImageBlock);
                        break;
                }
                break;
            case MessageTimer:
                Function(data + RomBase);
                break;
            default:
                break;
        }
    }
    
}

