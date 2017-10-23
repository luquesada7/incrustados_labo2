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
#include "Task.hpp"
#include <stdlib.h>

#define NUMBER_OF_SLOTS 255
#define NULL            0


class PIX : public Task
{
  public:
    PIX(uint16_t);
    virtual uint8_t run(void);
    virtual uint8_t setup(void);
    virtual uint8_t readMessage(st_Message * l_stNewMessage);
    virtual uint8_t sendMessage(st_Message * l_stNewMessage);
    int getPixelLines(void){return m_iLines;};
  protected:
    uint16_t m_fPastZ;
    uint16_t m_fNewZ;
    float m_fDelta;
    int m_iLines;
    bool m_bColor;
    bool m_breceivedTestMailbox;
  private:
    float l_fPixDelta;
    int l_iPixLine;
};

#endif /* PIX_HPP_ */
