#ifndef __WTR_GEOMETRYPASS_H__
#define __WTR_GEOMETRYPASS_H__

#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	class MaterialProxy;

	class RHIPipeLine;
	class RHIRenderTarget;
};

namespace wtr
{
	class GeometryPass : public GraphicPass
	{
		GENERATE(GeometryPass);
	public :
		GeometryPass();
		virtual ~GeometryPass() = default;

	public :
		eResourceState GetResourceState() const override;
		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;

		void InitState() override;
		void Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList) override;
		
		bool SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand);
		void UnsetCommand(Memory::RefPtr<RHICommandList> cmdList);

		const RHIDrawIndexDesc GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand);
		Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy) override;

	private :
		bool SetMaterial(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MaterialProxy> material);

	private :
		Memory::RefPtr<RHIRenderTarget> m_target;
	};
}

#endif // __WTR_GEOMETRYPASS_H__