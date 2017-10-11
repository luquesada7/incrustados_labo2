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
#include "Task.hpp"

class PAINT : public Task
{
    public:
        PAINT(void);
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
        int m_iLastLine;
        int m_iLastPixel;
        int m_iPixels;
        int m_iLines;
        bool m_bColor;
        struct m_stRectInfo{
            uint16_t sXMin;
            uint16_t sXMax;
            uint16_t sYMin;
            uint16_t sYMax;
        }
    private:
};


#endif /* PAINT_HPP_ */
