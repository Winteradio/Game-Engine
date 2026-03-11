#ifndef __WTR_SIMPLECOLOR_H__
#define __WTR_SIMPLECOLOR_H__

#include <Renderer/PipeLine/PipeLine.h>

namespace wtr
{
	class RHIShader;
};

namespace wtr
{
	class SimpleColor : public PipeLine
	{
	public :
		SimpleColor();
		virtual ~SimpleColor();

	public :
		virtual void Draw(const SceneView& scene, Memory::RefPtr<RHICommandList> commandList);
		virtual void Init(Memory::RefPtr<RHICommandList> commandList);

	private :
		Memory::RefPtr<RHIShader> m_vertexShader;
		Memory::RefPtr<RHIShader> m_pixelShader;
	};
};

#endif // __WTR_SIMPLECOLOR_H__