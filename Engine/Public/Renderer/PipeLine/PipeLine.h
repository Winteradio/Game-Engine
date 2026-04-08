#ifndef __WTR_PIPELINE_H__
#define __WTR_PIPELINE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <ECS/include/Object/Data.h>

namespace wtr
{
	class RenderScene;
	struct RenderView;

	class RHIPipeLine;
	class RHICommandList;

	enum class eResourceState : uint8_t;
};

namespace wtr
{
	class PipeLine : public ECS::Object
	{
		GENERATE(PipeLine);

	public :
		PipeLine();
		virtual ~PipeLine();

	public :
		virtual void Draw(const RenderView& renderView, Memory::RefPtr<RenderScene> renderScene, Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual void Init(Memory::RefPtr<RHICommandList> commandList) = 0;
		virtual eResourceState GetResourceState() const = 0;

	protected :
		Memory::RefPtr<RHIPipeLine> m_pipeLine;
	};
};

#endif // __WTR_PIPELINE_H__