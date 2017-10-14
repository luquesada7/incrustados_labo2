#include "Scheduler.hpp"

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    m_u8TaskCounter = 0;
    for(int index1 = 0; index1 < NUMBER_OF_SLOTS; index1++)
    {
        m_aSchedule[index1].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }
    return;
}

// ##############################################
// Function that attaches functions, inserts the
// task into the schedule slots
// ##############################################

uint8_t Scheduler::attach(Task * i_ToAttach, uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    st_TaskInfo l_st_StructToAttach;

    l_st_StructToAttach.pToAttach = i_ToAttach;
    l_st_StructToAttach.u64ticks = this->m_u64ticks;
    l_st_StructToAttach.u64TickInterval = 0;
	l_st_StructToAttach.u64TickIntervalInitValue = i_u64TickInterval;

    if((m_u8OpenSlots>0) && (m_u8NextSlot < NUMBER_OF_SLOTS))
    {
        m_aSchedule[m_u8NextSlot] =  l_st_StructToAttach;
        m_u8OpenSlots--;
        m_u8NextSlot++;
        g_aTaskPointers[m_u8TaskCounter] = i_ToAttach;
        m_aTaskInfoStructs[m_u8TaskCounter] = l_st_StructToAttach;
        m_u8TaskCounter++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}

// ##########################################
// Function that executes current schedule
// ##########################################

uint8_t Scheduler::run(void)
{
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
    	l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
		if(l_pNextTask != ((uintptr_t) 0))
		{
			if(m_aSchedule[l_iNextTaskSlot].u64TickInterval == 0){
				l_pNextTask->run();
			}
			m_aSchedule[l_iNextTaskSlot].u64TickInterval ++;

			if(m_aSchedule[l_iNextTaskSlot].u64TickInterval > m_aSchedule[l_iNextTaskSlot].u64TickIntervalInitValue) {
				m_aSchedule[l_iNextTaskSlot].u64TickInterval = 0;
			}
		}
		l_iNextTaskSlot++;
    }
    CalculateNextSchedule(); // TODO

    return l_u8ReturnCode;
}

// ##############################################
// Function that executes setup for all the tasks
// ##############################################

uint8_t Scheduler::setup(void)
{
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask->setup();
        }
        l_iNextTaskSlot++;
    }
    return l_u8ReturnCode;
}

uint8_t Scheduler::CalculateNextSchedule(void)
{
    Task * l_pTaskPointer = (unintptr_t)0;
    uint8_t l_u8Schedule = 0;

    for(int index1 = 0; index1 < NUMBER_OF_SLOTS; index1++)
    {
        m_aNextSchedule[index1].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }

    for(int l_iCounter; l_iCounter < NUMBER_OF_SLOTS; l_iCounter++)
    {
        if(g_aExecuteNextFrame[l_iCounter] == 1)
        {
            l_st_StructToAttach = m_aTaskInfoStructs[l_iCounter];
            m_aNextSchedule[l_u8Schedule] = l_st_StructToAttach;
            l_u8Schedule++;
        }
    }
    return(NO_ERR);
}

uint8_t Scheduler::CollectMessages(void)
{
    Task * l_pNextSender = (uintptr_t)0;

    for(int l_iCounter; l_iCounter < NUMBER_OF_SLOTS; l_iCounter++)
    {
        if(g_aSendMessageFlag[l_iCounter] == 1){
            st_Message l_stNewMessage;
            l_pNextSender = g_aTaskPointers[l_iCounter];
            l_pNextSender->sendMessage(&l_stNewMessage);
            InsertNode(g_pLinkedList, l_stNewMessage);
        }
    }
    return(NO_ERR);
}

uint8_t Scheduler::DistributeMessages(void)
{
    st_Message l_stNewMessage;
    while(g_pLinkedList != NULL)
    {
        DistributeEraseFirstNode(g_pLinkedList, l_stNewMessage);
    }
    return(NO_ERR);
}

// ##############################################
// Function that inserts new node on linked list
// - creates new node
// - saves message structure to node
// - links node to list ordering them in ascending
//   order with the DestTaskID of the message
// ##############################################

uint8_t Scheduler::InsertNode(st_Node *&l_pLinkedList, st_Message l_stNewMessage)
{
    st_Node *l_stNewNode = new st_Node();
    l_stNewNode->std_stMssg = l_stNewMessage;

    st_Node *l_stAux1 = l_pLinkedList;
    st_Node *l_stAux2;

    // -while list is not empty and DestTaskID of first node is smaller than the new message DestTaskID
    while((l_stAux1 != NULL) && (l_stAux1->std_stMssg.std_u8DestTaskID < l_stNewMessage.std_u8DestTaskID))
    {
        l_stAux2 = l_stAux1; //- points to first node
        l_stAux1 = l_stAux1->std_pnext; //- new position
    }

    if(l_pLinkedList == l_stAux1)
    {
        l_pLinkedList = l_stNewNode; //- adding node at the beggining of the list
    }else
    {
        l_stAux2->std_pnext = l_stNewNode; // -adding node in the middle or back of the list
    }

    l_stNewNode->std_pnext = l_stAux1;

    return(NO_ERR);
}

// ##############################################
// Function that erases a node on a linked list
// - Creates a temporary node to store the node
//   on the list that will be deleted
// - Current message is distributed to Task
// - Pointer to list then points to the second
//   node
// - Temporary node is deleted
// ##############################################

uint8_t Scheduler::DistributeEraseFirstNode(st_Node *&l_pLinkedList, st_Message &l_stNewMessage)
{
    st_Node *l_stAux = l_pLinkedList;
    l_stMessage = l_stAux->std_stMssg;
    Task * l_pTaskPointer = (uintptr_t)0;
    l_pTaskPointer = g_aTaskPointers[l_stMessage.std_u8DestTaskID];
    l_pTaskPointer->readMessage(l_stMessage);
    l_pLinkedList = l_stAux->std_pnext;
    delete l_stAux;
    return(NO_ERR);
}
