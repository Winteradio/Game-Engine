#ifndef __WTR_PIPELINE_H__
#define __WTR_PIPELINE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <ECS/include/Object/Data.h>
#include <Container/include/DynamicArray.h>
#include <Renderer/RenderResource.h>

namespace wtr
{
	class GlobalResource;
	struct MeshDrawCommand;

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
		using MeshDrawCommands = wtr::DynamicArray<Memory::RefPtr<const MeshDrawCommand>>;

		PipeLine();
		virtual ~PipeLine();

	public :
		void Execute(const MeshDrawCommands& meshDrawCommands, Memory::RefPtr<GlobalResource> globalResource, Memory::RefPtr<RHICommandList> cmdList);

		virtual void Init() = 0;
		
		virtual eResourceState GetShaderState() const = 0;

	protected :
		virtual void Prepare() = 0;
		virtual void Draw(const MeshDrawCommands& meshDrawCommands, Memory::RefPtr<GlobalResource> globalResource, Memory::RefPtr<RHICommandList> cmdList) = 0;

	protected :
		Memory::RefPtr<RHIPipeLine> m_pipeLine;

		bool m_prepared;
	};
};

#endif // __WTR_PIPELINE_H__