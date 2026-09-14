#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project.h"
#include "general.h"
#include "Sys.h"
#include "SysTimer.h"

#include "TCS34725.h"

#define PRESSED 0
#define RELEASED 1
//To use double on sprintf:
//.../Linker/Command_line: -u_printf_float

static _Bool err;

float r = -1;
float g = -1;
float b = -1;
float r2 = -1;
float g2 = -1;
float b2 = -1;
char rStr[6];
char gStr[6];
char bStr[6];

volatile _Bool sw_mero_Triggered = 0;
volatile _Bool sw_modvalto_Triggered = 0;
int mod = 0;

CY_ISR(MeroSwitchInterrupt){
    if (Pin_sw_mero_Read() == PRESSED){
        sw_mero_Triggered = 1;
        //Pin_sw_mero_ClearInterrupt();
        Isr_sw_mero_ClearPending();
    }
}

CY_ISR(ModvaltoSwitchInterrupt){
    if (Pin_sw_modvalto_Read() == PRESSED){
        mod = !mod;
        sw_modvalto_Triggered = 1;
        //Pin_sw_mero_ClearInterrupt();
        Isr_sw_modvalto_ClearPending();
    }
}

void Init(void)
{
    UART_Start();
    CyDelay(100);

    I2C_Start();
    CyDelay(100);
    
    err = init_TCS34725();
    CyDelay(500);
    
    Isr_sw_mero_StartEx(MeroSwitchInterrupt);
    Isr_sw_modvalto_StartEx(ModvaltoSwitchInterrupt);
    
    UART_PutString("Uart ready.\r\n");
    if (err == 0) {
        UART_PutString("TCS34725 is not ready.\r\n");
    } 
    else if (err == 1){
        UART_PutString("TCS34725 is ready.\r\n");
    }
}

int main(void)
{
    CyGlobalIntEnable;
    Init();

    for(;;)
    {
        if (sw_modvalto_Triggered){
            if (mod == 0){
                UART_PutString("---------- Szin mero mod ----------\r\n");
            }
            else if (mod == 1){
                UART_PutString("----- Szin osszehasonlito mod -----\r\n");
            }
            sw_modvalto_Triggered = 0;
        }
        if (sw_mero_Triggered){
            if (mod == 0){
                getRGB(&r, &g, &b);
                sprintf(rStr, "%d", (int) r);
                sprintf(gStr, "%d", (int) g);
                sprintf(bStr, "%d", (int) b);
                UART_PutString("Red: ");
                UART_PutString(rStr);
                UART_PutString(", ");
                UART_PutString("Green: ");
                UART_PutString(gStr);
                UART_PutString(", ");
                UART_PutString("Blue: ");
                UART_PutString(bStr);
                UART_PutString("\r\n");
            }
            else if (mod == 1){
                if ( r == -1 && g == -1 && b == -1){
                    getRGB(&r, &g, &b);
                    sprintf(rStr, "%d", (int) r);
                    sprintf(gStr, "%d", (int) g);
                    sprintf(bStr, "%d", (int) b);
                    UART_PutString("Red: ");
                    UART_PutString(rStr);
                    UART_PutString(", ");
                    UART_PutString("Green: ");
                    UART_PutString(gStr);
                    UART_PutString(", ");
                    UART_PutString("Blue: ");
                    UART_PutString(bStr);
                    UART_PutString("\r\n");
                }
                else if (r2 == -1 && g2 == -1 && b2 == -1){
                    getRGB(&r2, &g2, &b2);
                    sprintf(rStr, "%d", (int) r2);
                    sprintf(gStr, "%d", (int) g2);
                    sprintf(bStr, "%d", (int) b2);
                    UART_PutString("Red: ");
                    UART_PutString(rStr);
                    UART_PutString(", ");
                    UART_PutString("Green: ");
                    UART_PutString(gStr);
                    UART_PutString(", ");
                    UART_PutString("Blue: ");
                    UART_PutString(bStr);
                    UART_PutString("\r\n");
                    
                    int redDif = (float) (abs((int)(r - r2))) / 255 * 100;
                    int greenDif = (float) abs((int)(g - g2)) / 255 * 100;
                    int blueDif = (float) abs((int)(b - b2)) / 255 * 100;
                    
                    sprintf(rStr, "%d", (int) redDif);
                    sprintf(gStr, "%d", (int) greenDif);
                    sprintf(bStr, "%d", (int) blueDif);
                    UART_PutString("A ket szin kozotti kulonbseg: ");
                    UART_PutString("Red: ");
                    UART_PutString(rStr);
                    UART_PutString("%, ");
                    UART_PutString("Green: ");
                    UART_PutString(gStr);
                    UART_PutString("%, ");
                    UART_PutString("Blue: ");
                    UART_PutString(bStr);
                    UART_PutString("%\r\n");
                    
                    r = r2;
                    g = g2;
                    b = b2;
                    r2 = -1;
                    g2 = -1;
                    b2 = -1;
                }
            }
            sw_mero_Triggered = 0;
        }
    }
}

/* [] END OF FILE */
