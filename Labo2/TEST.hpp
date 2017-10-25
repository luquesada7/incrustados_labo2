/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef TEST_HPP_
#define TEST_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"


class TEST : public Task
{
    public:
        TEST(uint8_t);
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
        virtual uint8_t sendMessage(st_Message *l_stNewMessage);
        bool m_bsendTestMailbox;

    protected:


    private:

};

#endif /* TEST_HPP_ */
