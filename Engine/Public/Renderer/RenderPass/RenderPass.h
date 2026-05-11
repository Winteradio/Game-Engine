#ifndef __WTR_RENDERPASS_H__
#define __WTR_RENDERPASS_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

namespace wtr
{
	struct MeshDrawCommand;
	class LightProxy;
	class GlobalResource;

	struct RHIDrawIndexDesc;
	class RHICommandList;
};

namespace wtr
{
	using MeshDrawCommands = wtr::DynamicArray<Memory::RefPtr<MeshDrawCommand>>;
	using LightProxies = wtr::DynamicArray<Memory::RefPtr<LightProxy>>;

	class RenderPass
	{
	public :
		RenderPass();
		virtual ~RenderPass();

	public :
		virtual void Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<GlobalResource> globalResource, Memory::RefPtr<RHICommandList> cmdList) = 0;

	protected :
		const RHIDrawIndexDesc GetDrawIndexDesc(Memory::RefPtr<MeshDrawCommand> drawCommand);
	};
};

#endif // __WTR_RENDERPASS_H__