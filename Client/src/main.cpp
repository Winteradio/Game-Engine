#include <Log/include/Log.h>
#include <Log/include/LogPlatform.h>

#include <Framework/Engine.h>
#include <Framework/Window.h>

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Memory.h>
#include <Container/include/DynamicArray.h>
#include <thread>

class RHIBuffer : public Memory::RefCounted
{
public :
	RHIBuffer()
	{
		LOGINFO() << "[REF] Construct : ";
	}

	~RHIBuffer()
	{
		LOGINFO() << "[REF] Destruct : ";
	}
};

void CopyRHI(Memory::RefPtr<RHIBuffer> refBuffer)
{
	wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> refPool;
	refPool.Resize(100);

	for (auto& ref : refPool)
	{
		ref = refBuffer;

		LOGINFO() << "[REF]" << static_cast<uint32_t>(GetCurrentThreadId()) << " , Count " << ref->GetRefCount();
	}
}

void Thread()
{
	wtr::DynamicArray<std::thread> vThreadPool;
	vThreadPool.Resize(10);

	// Construct RHI
	Memory::RefPtr<RHIBuffer> refBuffer = Memory::MakeRef<RHIBuffer>();

	// Copy RHI
	for (auto& thread : vThreadPool)
	{
		thread = std::thread(CopyRHI, refBuffer);
	}

	// Wait RHI
	for (auto& thread : vThreadPool)
	{
		thread.join();
	}
}

int MAIN()
{
	Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type);

	wtr::Engine engine;
	wtr::WindowDesc mainWindowDesc;
	mainWindowDesc.Type = wtr::eWindowType::eWin32;

	engine.Init(mainWindowDesc);
	engine.Run();
	engine.Shutdown();

	system("pause");
	return 0;
}
