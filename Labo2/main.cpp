#include <ti/devices/msp432p4xx/inc/msp.h>
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "ADC.hpp"


uint16_t ADC14Resultx = 0U;
uint16_t ADC14Resulty = 0U;
uint16_t ADC14Resultz = 0U;

// ##########################
// Global/Static declarations
// ##########################
uint8_t Task::m_u8NextTaskID = 0; // - Init task ID
volatile static uint64_t g_SystemTicks = 0; // - The system counter.
Scheduler g_MainScheduler; // - Instantiate a Scheduler

// #########################
//          MAIN
// #########################
void main(void)
{

    // - Instantiate two new Tasks
    LED BlueLED(BIT2);
    LED GreenLED(BIT1);
    ADC TestADC(1,2,3);
    // - Run the overall setup function for the system
    Setup();
    // - Attach the Tasks to the Scheduler;
    g_MainScheduler.attach(&BlueLED, 500);
    g_MainScheduler.attach(&GreenLED, 300);
    g_MainScheduler.attach(&TestADC, 10);
    // - Run the Setup for the scheduler and all tasks
    g_MainScheduler.setup();
    // - Main Loop
    while(1)
    {
    	__wfe(); // Wait for Event
        if(g_SystemTicks != g_MainScheduler.m_u64ticks)
        {
            //- Only execute the tasks if one tick has passed.
            g_MainScheduler.m_u64ticks = g_SystemTicks;
            g_MainScheduler.run();
        }
    }
}

// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void)
{
    P5->DIR = BIT6;
    P5->OUT = BIT6;
	// ****************************
	//         DEVICE CONFIG
	// ****************************
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	// - This is the heart beat indicator.
	P1->DIR |= BIT0; //Red LED

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	TIMER32_1->LOAD = TIMER32_COUNT; //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);

	NVIC_SetPriority(ADC14_IRQn, 1);
	NVIC_EnableIRQ(ADC14_IRQn);
	__enable_irq();

    /*    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

        P1->DIR = BIT0;
        P1->OUT = BIT0;
    // Set P4.3 for Analog input, disabling the I/O circuit.
        P4->SEL0 = BIT3;
        P4->SEL1 = BIT3;
        P4->DIR &= ~BIT3;

        // Configures Pin 4.0, 4.2, and 6.1 as ADC input
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

        // Initializes display
        Crystalfontz128x128_Init();

        // Set default screen orientation
        Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

        // Initializes graphics context
        Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                             &g_sCrystalfontz128x128_funcs);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

        // Paint initial display

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
                                        l_uint16tValue); */

	return;
}

extern "C"
{
    // - Handle the Timer32 Interrupt
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0; // - Toggle the heart beat indicator (1ms)
		g_SystemTicks++;
		return;
	}

	void ADC14_IRQHandler(void)
    {
        __disable_irq();
        P5->OUT ^= BIT6;
        ADC14Resultz = ADC14->MEM[1];
        ADC14Resulty = ADC14->MEM[2];
        ADC14Resultx = ADC14->MEM[3];
        ADC14->CLRIFGR0 =  ADC14_CLRIFGR0_CLRIFG1
                | ADC14_CLRIFGR0_CLRIFG2 | ADC14_CLRIFGR0_CLRIFG3;
        __enable_irq();
        return;
    }

}
