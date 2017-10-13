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

/* Variables globales del programa */
extern st_Node *g_pLinkedList;

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
        Graphics_Rectangle m_stRect;
    private:
};


#endif /* PAINT_HPP_ */
