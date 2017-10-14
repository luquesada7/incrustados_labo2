/*
 * *******
 * ADC 1: (.h)
 * *******
 * Message received: none
 * Activates ADC to read and convert accelerometer values
 * Message sent: none
 * ADC interrupt: Send a message to Task 2 with ADCResultx, ADCResulty, ADCResultz
 *
 * Created on: Oct 6, 2017
 * Authors: Luisa Fernanda Quesada Camacho &
 *          Jose Pablo Martinez Hernandez
 */


#ifndef ADC_HPP_
#define ADC_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "Task.hpp"
#include "Strct.hpp"

#define NUMBER_OF_SLOTS 255
#define NULL            0

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern uint8_t g_aSendMessageFlag[NUMBER_OF_SLOTS];
extern uint8_t g_aExecuteNextFrame[NUMBER_OF_SLOTS];
extern Task * g_aTaskPointers[NUMBER_OF_SLOTS];

class ADC: public Task
{
    public:
    ADC(void);
    uint8_t m_u8MyTaskID; // - My TaskID
    uint8_t m_u8NextTaskID; // - Next Task to be executed
    uint16_t m_fADC14Resultx;
    uint16_t m_fADC14Resulty;
    uint16_t m_fADC14Resultz;
    uint16_t m_fPastADC14Resultx;
    uint16_t m_fPastADC14Resulty;
    uint16_t m_fPastADC14Resultz;
    virtual uint8_t run(void);
    virtual uint8_t setup(void);
	virtual uint8_t sendMessage(st_Message * l_stNewMessage);
  protected:
  private:
};

#endif /* ADC_HPP_ */
