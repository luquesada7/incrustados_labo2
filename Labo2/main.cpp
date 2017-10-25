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
#include "HOR.hpp"
#include <stdio.h>

/* Graphic library context */
Graphics_Context g_sContext;



// ################################################################################
// Global/Static declarations
// ################################################################################

uint16_t ADC14Resultz = 0U;
uint16_t ADC14Resultx = 0U;
uint16_t PastADC14Resultz = 0U;
uint8_t Task::m_u8NextTaskID = 0; // - Init task ID
volatile static uint64_t g_SystemTicks = 0; // - The system counter.
Scheduler g_MainScheduler; // - Instantiate a Scheduler
ADC TaskADC(0);
//int pixel_Lines;
//int newLine;
//int LastLine;
//uint16_t PastZ;
//uint16_t NewZ;
//bool Color;
//bool maxLimit;
//bool minLimit;

// ################################################################################
//                                       MAIN
// ################################################################################

void main(void)
{

    // ################################################################################
    //                                 TEST ZONE
    // ################################################################################

    // ###################################
    // - Initiating new tasks	
    // - Setting Task Keys
    // - Setting Message Destination Keys
    // ###################################
	
    PIX Pixels(0);
    PAINT LCD(0);
    HOR HV(0);
    ;
    LED BlueLED(BIT2);
    LED GreenLED(BIT1);
    LED RedLED(BIT0);

    TaskADC.setKey("ADC");
    TaskADC.setDestKey("HORIZONTE");
    Pixels.setKey("PIXELS");
    Pixels.setDestKey("LCD");
    LCD.setKey("LCD");
    HV.setKey("HORIZONTE");


    // - Run the overall setup function for the system
    Setup();

    //- Attach the Tasks to the Scheduler;
    g_MainScheduler.attach(&TaskADC, 0); // - TickInitValue = 0 for continuous or oneShot tasks
    g_MainScheduler.attach(&RedLED, 40); // - TickInitValue = 0 for continuous or oneShot tasks
    g_MainScheduler.attach(&GreenLED, 80);
    g_MainScheduler.attach(&BlueLED, 160);
    //g_MainScheduler.attach(&Pixels, 0);
    //g_MainScheduler.attach(&LCD, 0);
    g_MainScheduler.attach(&HV, 0);


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
            //pixel_Lines = static_cast<int>(Pixels.getPixelLines());
            //newLine = static_cast<int>(LCD.getNewLine());
            //LastLine = static_cast<int>(LCD.getLastLine());
            //Color = static_cast<bool>(Pixels.getColor());
            //minLimit = static_cast<bool>(LCD.getMinLimit());
            // maxLimit = static_cast<bool>(LCD.getMaxLimit());
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
        __disable_irq();
        ADC14Resultz = ADC14->MEM[1];
        ADC14Resultx = ADC14->MEM[3];
        PastADC14Resultz = TaskADC.m_ADCResult;
        TaskADC.m_ADCResult = ADC14Resultz;

        if(abs(PastADC14Resultz-ADC14Resultz)>30)
        {
        TaskADC.m_stMssg.std_u16IntData = ADC14Resultz;
        TaskADC.m_stMssg.std_u16IntData2 = ADC14Resultx;
        TaskADC.m_bMssgFlag = true;
        }

        ADC14->CLRIFGR0 =  ADC14_CLRIFGR0_CLRIFG1
                | ADC14_CLRIFGR0_CLRIFG2 | ADC14_CLRIFGR0_CLRIFG3;
        __enable_irq();
        return;
    }

}
