#include <Log/include/Log.h>
#include <Log/include/LogPlatform.h>

int MAIN()
{
	Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type);

	LOGINFO() << "Hello World";

	system("pause");
	return 0;
}
