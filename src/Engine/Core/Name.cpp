#include "Name.h"

Name::Name()
{

}

Name::~Name()
{
    
}

std::string Name::Get( const std::type_info& Type )
{
    std::string Result = Type.name();

    auto ITR = Result.find(" ");
    if ( ITR == std::string::npos )
    {
        return Result;
    }

    Result.erase( 0, ITR + 1 );

    return Result;
}