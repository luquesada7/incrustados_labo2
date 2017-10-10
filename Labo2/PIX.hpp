/*
 * PIXELS.hpp
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

class PIX : public Task
{
    public:
        PIX(void);
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
        float m_fPastZ;
        float m_fNewZ;
        float m_fDelta;
        int m_iLines;
        bool m_bColor;
        const float l_fPixDelta ;
        const int l_iPixLine = 128;
    private:
};


#endif /* PIX_HPP_ */
