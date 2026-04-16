#ifndef __WTR_WORLDCOMMANDLIST_H__
#define __WTR_WORLDCOMMANDLIST_H__

#include <Framework/CommandList.h>

namespace wtr
{
    struct WorldCommand
    {};

    class WorldCommandList : public CommandList<WorldCommand>
    {};
};

#endif // __WTR_WORLDCOMMANDLIST_H__