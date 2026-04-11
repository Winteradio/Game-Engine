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
		virtual void Draw(const RenderView& renderView, Memory::RefPtr<RenderScene> renderScene, Memory::RefPtr<RHICommandList> cmdList) override;
		virtual void Init();
		virtual void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		virtual void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
		virtual eResourceState GetResourceState() const override;
		virtual eResourceState GetShaderState() const override;

	private :
		Memory::RefPtr<const ShaderAsset> m_vertexShader;
		Memory::RefPtr<const ShaderAsset> m_pixelShader;
	};
};

#endif // __WTR_SIMPLECOLOR_H__