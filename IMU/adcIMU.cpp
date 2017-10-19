#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdlib.h>

/* Graphic library context */
Graphics_Context g_sContext;

//uint16_t ADC14Result = 0U;
uint16_t ADC14Resultx = 0U;
uint16_t ADC14Resulty = 0U;
uint16_t ADC14Resultz = 0U;

uint16_t  m_fPastZ = 8085;
uint16_t  m_fNewZ = 8085;
float m_fDelta;
int m_iLines;
int m_iNewLine;
bool m_bColor;
const float l_fPixDelta =  128.0 / 6400.0;
const int l_iPixLine = 128;

int m_iLastLine = 64;
int m_iPixels;
uint16_t l_uint16tulValue;
Graphics_Rectangle l_stRect;

void Setup(void);

int main(void)
{
    Setup();
    while (1)
    {
        //linesLCD = (ADC14Resultz*PENDIENTE - 4800*PENDIENTE)/128;
        //numlines = static_cast<int>(linesLCD);
        //drawAccelData(numlines);

        if (m_fNewZ > m_fPastZ)
        {
            m_bColor = true;
        }
        else
        {
            m_bColor = false;
        }

        //################################
        // Calculating number of pixels
        // that need to be painted
        //################################
        m_fDelta = abs(m_fNewZ - m_fPastZ);
        m_iLines = 1 + ((m_fDelta * l_fPixDelta * l_iPixLine - l_iPixLine) / l_iPixLine);
        //m_iLines = 1 + ((m_fDelta*l_fPixDelta - l_iPixLine)/l_iPixLine);

        l_stRect.xMin = 0;
        l_stRect.xMax = 127;

        if (m_bColor)
        {
            m_iNewLine = m_iLastLine - m_iLines;
            if (m_iNewLine < 0)
            {
                l_stRect.yMin = 0;
                l_stRect.yMax = m_iLastLine;
            }
            else
            {
                l_stRect.yMin = m_iNewLine;
                l_stRect.yMax = m_iLastLine;
            }

            l_uint16tulValue = 0XAA00; //brown

            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRect,
                                            l_uint16tulValue);
        }
        else
        {
            m_iNewLine = m_iLastLine + m_iLines;

            if (m_iNewLine > 128)
            {
                l_stRect.yMin = m_iLastLine;
                l_stRect.yMax = 127;
            }
            else
            {
                l_stRect.yMin = m_iLastLine;
                l_stRect.yMax = m_iNewLine;
            }

            l_uint16tulValue = 0X00FF; //blue

            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRect,
                                            l_uint16tulValue);
        }
        m_iLastLine = m_iNewLine;
        m_fPastZ = m_fNewZ;
        m_fNewZ = ADC14Resultz;
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

//ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_14 | ADC14_MCTLN_VRSEL_0;

    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC;
    ADC14->IER0 = ADC14_IER0_IE1 | ADC14_IER0_IE2 | ADC14_IER0_IE3;
    NVIC_SetPriority(ADC14_IRQn, 1);
    NVIC_EnableIRQ(ADC14_IRQn);

    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    /*
     * Paint initial display
     */

    Graphics_Rectangle l_stRectInit;

    uint16_t l_uint16tValue;

    l_stRectInit.xMin = 0;
    l_stRectInit.xMax = 127;
    l_stRectInit.yMin = 0;
    l_stRectInit.yMax = 63;

    l_uint16tValue = 0X00FF; //blue

    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRectInit,
                                    l_uint16tValue);
    l_stRectInit.xMin = 0;
    l_stRectInit.xMax = 127;
    l_stRectInit.yMin = 64;
    l_stRectInit.yMax = 127;

    l_uint16tValue = 0XAA00; //brown

    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRectInit,
                                    l_uint16tValue);

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
//ADC14Result = ADC14->MEM[0];
    ADC14Resultz = ADC14->MEM[1];
    ADC14Resulty = ADC14->MEM[2];
    ADC14Resultx = ADC14->MEM[3];
    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG1 | ADC14_CLRIFGR0_CLRIFG2
            | ADC14_CLRIFGR0_CLRIFG3;
    __enable_irq();
    return;
}
}
