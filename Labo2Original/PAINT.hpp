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

#define NUMBER_OF_SLOTS 255
#define NULL            0

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern Task * g_aTaskPointers[NUMBER_OF_SLOTS];

class PAINT : public Task
{
  public:
    PAINT(void);
    virtual uint8_t run(void);
    virtual uint8_t setup(void);
    virtual uint8_t readMessage(st_Message * l_stNewMessage);
    virtual uint8_t sendMessage(st_Message * l_stNewMessage);
  protected:
    int m_iLastLine;
    int m_iLines;
    int m_iNewLine;
    bool m_bColor;
    Graphics_Rectangle m_stRect;
  private:
};

#endif /* PAINT_HPP_ */
