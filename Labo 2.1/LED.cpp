#include "LED.hpp"

LED::LED(uint16_t i_BITN)
{
	m_u16BITN = i_BITN;
	m_bRunFlag = false;
	//this->m_pKey = "LED";
	//this->m_stMssg.std_pDestKey = "DEFAULT";
	//setKey("LED");
	//setDestKey("DELFAULT");
	//m_breceivedTestMailbox = false;
}

uint8_t LED::run()
{
    //#########################
    // Blink code Assuming PORT2
    //P2->OUT ^= m_u16BITN;
    /*if (m_breceivedTestMailbox){
        P2->OUT = m_u16BITN;
        m_bsendTestMailbox = false;
    }else{
        P2->OUT ^= m_u16BITN;
        m_bsendTestMailbox = true;
    }*/

    if (m_breceivedTestMailbox){
        P2->OUT ^= m_u16BITN;
    }
    m_bRunFlag = false;
    //#########################
    return(NO_ERR);
}

uint8_t LED::setup()
{
    //LED Setup, assuming PORT2
    // - P2.0 is connected to the RGB LED
    //m_bRunFlag = false;
    P2->DIR |= m_u16BITN; //Red LED
    P2->OUT &= m_u16BITN; // Initialize the LED Value
    return(NO_ERR);
}

uint8_t LED::readMessage(st_Message *l_stNewMessage)
{
    m_breceivedTestMailbox = l_stNewMessage->std_bBoolData;
    m_bRunFlag = true;
    return(NO_ERR);
}

/*uint8_t LED::sendMessage(st_Message *l_stNewMessage)
{
    m_stMssg.std_bBoolData = m_bsendTestMailbox;
    *l_stNewMessage = m_stMssg;
    m_bMssgFlag = false;
    return(NO_ERR);
}*/

