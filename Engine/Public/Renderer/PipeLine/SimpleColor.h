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
		virtual void Draw(const RenderView& renderView, Memory::RefPtr<RenderScene> renderScene, Memory::RefPtr<RHICommandList> commandList) override;
		virtual void Init(Memory::RefPtr<RHICommandList> commandList);
		virtual eResourceState GetResourceState() const override;

	private :
		Memory::RefPtr<ShaderAsset> m_vertexShader;
		Memory::RefPtr<ShaderAsset> m_pixelShader;
	};
};

#endif // __WTR_SIMPLECOLOR_H__