#ifndef __WTR_PIPELINE_H__
#define __WTR_PIPELINE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Reflection/include/Type/TypeMacro.h>

namespace wtr
{
	class RenderScene;
	class RenderView;
	class RHIPipeLine;
	class RHICommandList;
};

namespace wtr
{
	class PipeLine
	{
		GENERATE(PipeLine);

	public :
		PipeLine();
		virtual ~PipeLine();

	public :
		virtual void Draw(const RenderView& renderView, Memory::RefPtr<RenderScene> renderScene, Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual void Init(Memory::RefPtr<RHICommandList> commandList) = 0;

	protected :
		Memory::RefPtr<RHIPipeLine> m_pipeLine;
	};
};

#endif // __WTR_PIPELINE_H__