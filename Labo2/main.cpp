#include <ti/devices/msp432p4xx/inc/msp.h>
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "ADC.hpp"
#include "Strct.hpp"

uint16_t ADC14Resultx = 0U;
uint16_t ADC14Resulty = 0U;
uint16_t ADC14Resultz = 0U;

// ##########################
// Global/Static declarations
// ##########################
uint8_t Task::m_u8NextTaskID = 0; // - Init task ID
volatile static uint64_t g_SystemTicks = 0; // - The system counter.
Scheduler g_MainScheduler; // - Instantiate a Scheduler
st_Node *g_pLinkedList = NULL;
Task *g_aTaskPointers[NUMBER_OF_SLOTS] = {NULL};

// #########################
//          MAIN
// #########################
void main(void)
{

    // - Instantiate two new Tasks
    ADC TestADC();
    // - Run the overall setup function for the system
    Setup();
    // - Attach the Tasks to the Scheduler;
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
	
	return;
}

extern "C"
{
  // - Handle the Timer32 Interrupt
  void T32_INT1_IRQHandler(void)
  {
    __disable_irq();
    TIMER32_1->INTCLR = 0U;
    P1->OUT ^= BIT0; // - Toggle the heart beat indicator (1ms)
    g_SystemTicks++;
    __enable_irq();
    return;
  }

  // - Handles new data in the ADC
  void ADC14_IRQHandler(void)
  {
    __disable_irq();
    P5->OUT ^= BIT6;
    m_fADC14Resultz = ADC14->MEM[1];
    m_fADC14Resulty = ADC14->MEM[2];
    m_fADC14Resultx = ADC14->MEM[3];
    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG1 | ADC14_CLRIFGR0_CLRIFG2
                    | ADC14_CLRIFGR0_CLRIFG3;

    if (abs(m_fPastADC14Resultz-m_fADC14Resultz)>= 50.0) {
      //- Flag of Next Task that should be executed if dADCz is big enough
      g_aExecuteNextFrame[m_u8NextTaskID] = 1;

      //- Flag to tell the Scheduler the Task wants to send a message
      g_aSendMessageFlag[m_u8MyTaskID] = 1;
    }
    __enable_irq();
    return;
    }

}
