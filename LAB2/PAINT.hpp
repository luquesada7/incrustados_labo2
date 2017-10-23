/*
 * PAINT.hpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Jose Pablo Martinez &
                 Luisa Fernanda Quesada
 */

#ifndef PAINT_HPP_
#define PAINT_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "Task.hpp"
#include "Strct.hpp"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/Crystalfontz128x128_ST7735.h"


#define NUMBER_OF_SLOTS 255
#define NULL            0

class PAINT : public Task
{
  public:
    PAINT(uint16_t);
    virtual uint8_t run(void);
    virtual uint8_t setup(void);
    virtual uint8_t readMessage(st_Message * l_stNewMessage);
    int getNewLine(void){return m_iNewLine;};
    int getLastLine(void){return m_iLastLine;};
    bool getMinLimit(void){return m_bMinLimit;};
    bool getMaxLimit(void){return m_bMaxLimit;};
  protected:
    int m_iLastLine; //- Limit between sky and ground
    int m_iPaintLines; //- Amount of lines to be painted on frame 
    int m_iLines; //- Amount of total lines to be painted
    int m_iNewLine; //- New limit between sky and ground
    bool m_bColor; //- Paint Blue or Brown
    bool m_bMaxLimit;
    bool m_bMinLimit;
    Graphics_Rectangle m_stRect;
  private:
};

#endif /* PAINT_HPP_ */
