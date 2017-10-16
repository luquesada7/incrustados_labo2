#include "Task.hpp"

Task::Task()
{
  m_u8TaskID = m_u8NextTaskID;
  m_u8NextTaskID++;
  m_bIsFinished = false;
}

Task::getKey()
{
    return m_pKey;
}

Task::setKey(char *l_pKey)
{
    m_pKey = l_pKey;
}

Task::getMssgFlag()
{
    return m_bMssgFlag;
}

Task::getRunFlag()
{
    return m_bRunFlag;
}
