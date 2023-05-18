#ifndef __MEMORYMANAGER_H__
#define __MEMORYMANAGER_H__

#include "Log.h"

class MemoryPool
{

};

class MemoryManager
{
    private :
        MemoryManager();
        ~MemoryManager();

    public :
        static MemoryManager& GetHandle();

    private :
        static MemoryManager m_MemoryManager;
};

#endif // __MEMORYMANAGER_H__