#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

#define PENDIENTE 2.482424242

/* Graphic library context */
Graphics_Context g_sContext;

// Holalalallal
//
//
// :)
//

//COMENTARIOOOOOOOOOOOOOOOOOOOOOOOO

// Hola Jose!

// CONFLICTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!! :-(

//

//


// Me llamo Luisai

// Hola Luluisaaa, qué tal te va!? 
// Nuevo
// Comentario
uint16_t ADC14Result = 0U;
uint16_t ADC14Resultx = 0U;
uint16_t ADC14Resulty = 0U;
uint16_t ADC14Resultz = 0U;
int numlines = 0U;
double linesLCD = 0U;
void Setup(void);
void drawAccelData(int num);
int main(void)
{

    Setup();
    while (1)
    {
        linesLCD = (ADC14Resultz*PENDIENTE - 4800*PENDIENTE)/128;
        numlines = static_cast<int>(linesLCD);

        drawAccelData(numlines);
    }
   // return 0;
}

void Setup(void)
{
    WDTCTL = WDTPW | WDTHOLD; /* Stop watchdog timer */

    P1->DIR = BIT0;
    P1->OUT = BIT0;
    // Set P4.3 for Analog input, disabling the I/O circuit.
    P4->SEL0 = BIT3;
    P4->SEL1 = BIT3;
    P4->DIR &= ~BIT3;


    /* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
    P4->SEL0 = BIT0;
    P4->SEL1 = BIT0;
    P4->DIR &= ~BIT0;
    P4->SEL0 = BIT2;
    P4->SEL1 = BIT2;
    P4->DIR &= ~BIT2;
    P6->SEL0 = BIT1;
    P6->SEL1 = BIT1;
    P6->DIR &= ~BIT1;

    //TIMER32_1->LOAD = 0x00B71B00; //~0.5s ---> a 48Mhz
    TIMER32_1->LOAD = 0x0000BB80; //~0.5s ---> a 48Mhz
    TIMER32_1->CONTROL =
    TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE
            | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT1_IRQn, 1);
    NVIC_EnableIRQ(T32_INT1_IRQn);

    ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7 |
    ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON | ADC14_CTL0_SHP
            | ADC14_CTL0_CONSEQ_3;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_14 | ADC14_MCTLN_VRSEL_0;
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC;
    ADC14->IER0 = ADC14_IER0_IE0 | ADC14_IER0_IE1 | ADC14_IER0_IE2
            | ADC14_IER0_IE3;
    NVIC_SetPriority(ADC14_IRQn, 1);
    NVIC_EnableIRQ(ADC14_IRQn);

    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

}

void drawAccelData(int num)
{
    HAL_LCD_writeCommand(CM_RAMWR);
    int i;
    for (i = 0; i < num*128 ; i++)
    {
        HAL_LCD_writeData(0x36);
        //light blue
        HAL_LCD_writeData(0x36);
    }
    int j;
    for (j = num*128 ; j < 16384; j++ )
    {
        HAL_LCD_writeData(0XAA);
//brown
        HAL_LCD_writeData(0XAA);
    }

    HAL_LCD_delay(10);
    HAL_LCD_writeCommand(CM_DISPON);
}

extern "C"
{
    void T32_INT1_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_1->INTCLR = 0U;
        P1->OUT ^= BIT0;
        ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
        __enable_irq();
        return;
    }

    void ADC14_IRQHandler(void)
    {
        __disable_irq();
        ADC14Result = ADC14->MEM[0];
        ADC14Resultz = ADC14->MEM[1];
        ADC14Resulty = ADC14->MEM[2];
        ADC14Resultx = ADC14->MEM[3];
        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0 | ADC14_CLRIFGR0_CLRIFG1
                | ADC14_CLRIFGR0_CLRIFG2 | ADC14_CLRIFGR0_CLRIFG3;
        __enable_irq();
        return;
    }
}
