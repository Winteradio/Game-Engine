#ifndef __NAME_H__
#define __NAME_H__

#include "Utils.h"

class Name
{
    private :
        Name();
        ~Name();

    public :
        static std::string Get( const std::type_info& Type );

    private :
        static Name m_Name;
};

#endif // __NAME_H__