#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMMANAGER_H__

#include "ISystem.h"

class SystemManager
{
    using Data = std::map< const type_info*, std::any >;

	private :
        SystemManager() {};
        ~SystemManager() {};
};

#endif // __SYSTEMMANAGER_H__