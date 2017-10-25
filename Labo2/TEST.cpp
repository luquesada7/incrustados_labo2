#include "TEST.hpp"

TEST::TEST(uint8_t suma)
{
   m_bRunFlag = false;

};

uint8_t TEST::run()
{
    m_bsendTestMailbox = true;
    m_bMssgFlag = true;
    P5->OUT ^= BIT6;
    return(NO_ERR);
};

uint8_t TEST::setup()
{
    P5->DIR |= BIT6; //Red LED
    P5->OUT &= BIT6; // Initialize the LED Value
    return(NO_ERR);
};


uint8_t TEST::sendMessage(st_Message *l_stNewMessage)
{
    m_stMssg.std_bBoolData = m_bsendTestMailbox;
    *l_stNewMessage = m_stMssg;
    m_bMssgFlag = false;
    return(NO_ERR);
};

