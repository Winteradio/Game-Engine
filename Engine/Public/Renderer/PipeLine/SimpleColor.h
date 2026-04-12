#ifndef __WTR_SIMPLECOLOR_H__
#define __WTR_SIMPLECOLOR_H__

#include <Renderer/PipeLine/PipeLine.h>

namespace wtr
{
	class ShaderAsset;
};

namespace wtr
{
	class SimpleColor : public PipeLine
	{
		GENERATE(SimpleColor);
	public :
		SimpleColor();
		virtual ~SimpleColor();

	public :
		void Init() override;
		void Prepare() override;
		void Draw(const RenderView& renderView, const MeshDrawCommands& meshDrawCommands, Memory::RefPtr<RHICommandList> cmdList) override;

		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
		eResourceState GetResourceState() const override;
		eResourceState GetShaderState() const override;

	private :
		Memory::RefPtr<const ShaderAsset> m_vertexShader;
		Memory::RefPtr<const ShaderAsset> m_pixelShader;

		uint32_t m_cameraSlot;
		uint32_t m_instanceSlot;
	};
};

#endif // __WTR_SIMPLECOLOR_H__