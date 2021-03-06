/*
 * PIX.hpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Jose Pablo Martinez &
                 Luisa Fernanda Quesada
 */

#ifndef PIX_HPP_
#define PIX_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"
#include "Strct.hpp"

#define NUMBER_OF_SLOTS 255
#define NULL            0

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern Task * g_aTaskPointers[NUMBER_OF_SLOTS];

class PIX : public Task
{
  public:
    PIX(void);
    virtual uint8_t run(void);
    virtual uint8_t setup(void);
    virtual uint8_t readMessage(st_Message * l_stNewMessage);
    virtual uint8_t sendMessage(st_Message * l_stNewMessage);
  protected:
    float m_fPastZ;
    float m_fNewZ;
    float m_fDelta;
    int m_iLines;
    bool m_bColor;
  private:
    const float l_fPixDelta;
    const int l_iPixLine;
};

#endif /* PIX_HPP_ */
