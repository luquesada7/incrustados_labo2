/*
 * HOR.hpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Jose Pablo Martinez &
                 Luisa Fernanda Quesada
 */

#ifndef HOR_HPP_
#define HOR_HPP_
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
#include <math.h>


#define NUMBER_OF_SLOTS 255
#define NULL            0

class HOR : public Task
{
  public:
    HOR(uint16_t);
    virtual uint8_t run(void);
    virtual uint8_t setup(void);
    virtual uint8_t readMessage(st_Message * l_stNewMessage);
  protected:
    int m_iZ;
    int m_iX;
    int m_iHip;
    uint16_t m_uint16ADC14Rx;
    uint16_t m_uint16ADC14Rz;
    uint16_t  m_uint16tulValue;
    Graphics_Rectangle m_stRect;
    Graphics_Rectangle m_stRect1;
  private:
};

#endif /* PAINT_HPP_ */
