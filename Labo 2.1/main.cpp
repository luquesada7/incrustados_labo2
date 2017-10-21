#include "main.hpp"
#include "Task.hpp"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdlib.h>
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "ADC.hpp"
#include "TEST.hpp"
#include "Strct.hpp"
#include "PAINT.hpp"
#include "PIX.hpp"
#include <stdio.h>

/* Graphic library context */
Graphics_Context g_sContext;

//Task *g_aTaskPointers[NUMBER_OF_SLOTS] = {NULL};

uint16_t ADC14Resultx = 0U;
uint16_t ADC14Resulty = 0U;
uint16_t ADC14Resultz = 0U;

uint16_t *pToAx = &ADC14Resultx;
uint16_t datapToAx = *pToAx;

// ################################################################################
// Global/Static declarations
// ################################################################################

uint8_t Task::m_u8NextTaskID = 0; // - Init task ID
volatile static uint64_t g_SystemTicks = 0; // - The system counter.
Scheduler g_MainScheduler; // - Instantiate a Scheduler
ADC TestADC(0);

// ################################################################################
//                                       MAIN
// ################################################################################

void main(void)
{

    // ################################################################################
    //                                 TEST ZONE
    // ################################################################################

    //P2->DIR |= BIT0 + BIT1 + BIT2; //Red LED

    // - Instantiate two new Tasks
    LED BlueLED(BIT2);
    LED GreenLED(BIT1);
    LED RedLED(BIT0);
    PIX Pixels(0);
    PAINT LCD(0);
    TEST Suma(0);


    BlueLED.setKey("BLUE");
    GreenLED.setKey("GREEN");
    //Suma.setKey("SUMA");
    //Suma.setDestKey("GREEN");
    TestADC.setKey("ADC");
    TestADC.setDestKey("PIXELS");
    Pixels.setKey("PIXELS");
    Pixels.setDestKey("LCD");
    LCD.setKey("LCD");




    /*if ((&BlueLED)->getKey() == (&Suma)->m_stMssg.std_pDestKey)
    {
        P2->OUT = BIT0+BIT2;
    }*/

    /*__delay_cycles(1000000);
    //Task *pointer = (&GreenLED);

    if ("TAREA1" == (&BlueLED)->getKey())
    {
        P2->OUT = BIT0;
    }*/

    // - Run the overall setup function for the system
    Setup();

    //- Attach the Tasks to the Scheduler;
    //g_MainScheduler.attach(&RedLED, 80);
    //g_MainScheduler.attach(&GreenLED, 40);
    //g_MainScheduler.attach(&BlueLED, 200);
    //g_MainScheduler.attach(&Suma, 50);
    g_MainScheduler.attach(&TestADC, 0);
    g_MainScheduler.attach(&Pixels, 100);
    g_MainScheduler.attach(&LCD, 100);


    // ################################################################################
    //                                 END TEST ZONE
    // ################################################################################


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
            int pixel_Lines = Pixels.getPixelLines();
            int newLine = LCD.getNewLine();
            int LastLine = LCD.getLastLine();
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

	NVIC_SetPriority(ADC14_IRQn, 2);
	NVIC_EnableIRQ(ADC14_IRQn);

	__enable_irq();

	/* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

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

	// - Handle the ADC14 Interrupt
	void ADC14_IRQHandler(void)
    {
        //__disable_irq();
        ADC14Resultz = ADC14->MEM[1];
        TestADC.m_stMssg.std_u16IntData = ADC14->MEM[1];
        //TestADC.m_bMssgFlag = true;
        //ADC14Resulty = ADC14->MEM[2];
        //ADC14Resultx = ADC14->MEM[3];
        //TestADC.setMssgFlag(true);
        ADC14->CLRIFGR0 =  ADC14_CLRIFGR0_CLRIFG1
                | ADC14_CLRIFGR0_CLRIFG2 | ADC14_CLRIFGR0_CLRIFG3;
        //__enable_irq();
        return;
    }

}
