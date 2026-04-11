#ifndef __WTR_PIPELINE_H__
#define __WTR_PIPELINE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <ECS/include/Object/Data.h>
#include <Container/include/HashMap.h>
#include <Renderer/RenderResource.h>

namespace wtr
{
	class RenderScene;
	struct RenderView;

	class RHIShader;
	class RHIPipeLine;
	class RHICommandList;

	enum class eResourceState : uint8_t;
	enum class eShaderType : uint8_t;
};

namespace wtr
{
	class PipeLine : public ECS::Object, public RenderResource
	{
		GENERATE(PipeLine);

	public :
		PipeLine();
		virtual ~PipeLine();

	public :
		virtual void Draw(const RenderView& renderView, Memory::RefPtr<RenderScene> renderScene, Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual void Init() = 0;

		virtual eResourceState GetShaderState() const = 0;

	protected :
		Memory::RefPtr<RHIPipeLine> m_pipeLine;

	};
};

#endif // __WTR_PIPELINE_H__