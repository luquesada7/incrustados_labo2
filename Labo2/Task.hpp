#ifndef TASK_HPP_
#define TASK_HPP_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Strct.hpp"

#define NO_ERR 0
#define RET_ERR 1
#define NUMBER_OF_SLOTS 255
#define NULL            0

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern uint8_t g_aSendMessageFlag[NUMBER_OF_SLOTS];
extern uint8_t g_aExecuteNextFrame[NUMBER_OF_SLOTS];
extern Task * g_aTaskPointers[NUMBER_OF_SLOTS];

class Task
{
	public:
		Task();
		uint8_t m_u8TaskID;
		virtual uint8_t     run(void){return(0);};
		virtual uint8_t     setup(void){return(0);};
		virtual uint8_t     readMessage(st_Message * l_stNewMessage){return(0);};
		virtual uint8_t sendMessage(st_Message * l_stNewMessage){return l_stNewMessage;};
		bool                IsTaskFinished(void){return m_bIsFinished;};
		//uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		//void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
	private:
	   static uint8_t m_u8NextTaskID;
	   //uint8_t m_u8Priority;
	protected:
	   bool m_bIsFinished;
};

#endif /* TASK_HPP_ */
