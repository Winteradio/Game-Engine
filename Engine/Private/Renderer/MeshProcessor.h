#ifndef __WTR_MESHPROCESSOR_H__
#define __WTR_MESHPROCESSOR_H__

#include <Renderer/MeshBatch.h>
#include <Renderer/Proxy/MaterialProxy.h>
#include <Renderer/MeshDrawCommand.h>
#include <Renderer/ShaderState.h>
#include <RHI/RHIDescriptions.h>
#include <Reflection/include/Utils.h>

namespace wtr
{
	template<typename Tag, typename... Shaders>
	struct GetShaderState
	{
		using Type = void;
	};

	template<typename Tag, typename HeadShader, typename... TailShaders>
	struct GetShaderState<Tag, HeadShader, TailShaders...>
	{
		using Type = typename Reflection::Utils::Conditional_t<
			Reflection::Utils::IsBase<Tag, HeadShader>::value,
			HeadShader,
			typename GetShaderState<Tag, TailShaders...>::Type
		>;
	};

	template<typename... Shaders>
	class MeshProcessor
	{
	public:
		MeshProcessor() = delete;
		MeshProcessor(const RHIPipeLineDesc& desc);
		~MeshProcessor() = default;

		using VSType = typename GetShaderState<VSState, Shaders...>::Type;
		using PSType = typename GetShaderState<PSState, Shaders...>::Type;
		using GSType = typename GetShaderState<GSState, Shaders...>::Type;
		using HSType = typename GetShaderState<HSState, Shaders...>::Type;
		using DSType = typename GetShaderState<DSState, Shaders...>::Type;
		using CSType = typename GetShaderState<CSState, Shaders...>::Type;

	public:
		MeshDrawCommand CreateCommand(Memory::RefPtr<const MeshBatch> batch)
		{
			if (!batch)
			{
				LOGWARN() << "[MeshProcessor] Invalid mesh batch";
				return {};
			}

			auto mesh = batch->GetMesh();
			if (!mesh || mesh->GetState() != eResourceState::eReady)
			{
				LOGWARN() << "[MeshProcessor] Invalid mesh asset";
				return {};
			}

			auto material = batch->GetMaterial();
			if (!material || material->GetState() != eResourceState::eReady)
			{
				LOGWARN() << "[MeshProcessor] Invalid material proxy";
				return {};
			}

			auto vsState = material->GetShaderState<VSType>();
			auto psState = material->GetShaderState<PSType>();
			auto gsState = material->GetShaderState<GSType>();
			auto hsState = material->GetShaderState<HSType>();
			auto dsState = material->GetShaderState<DSType>();
			auto csState = material->GetShaderState<CSType>();

			RHIPipeLineCreateDesc desc = m_desc;

			MeshDrawCommand command;
		}

	private :
		const RHIPipeLineDesc m_desc;
	};
};

#endif // __WTR_MESHPROCESSOR_H__