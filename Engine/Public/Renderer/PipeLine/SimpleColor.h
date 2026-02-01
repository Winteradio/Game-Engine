#ifndef __WTR_SIMPLECOLOR_H__
#define __WTR_SIMPLECOLOR_H__

#include <Renderer/PipeLine/Base.h>

namespace wtr
{
	class RHIShader;
};

namespace wtr
{
	namespace PipeLine
	{
		class SimpleColor : public Base
		{
		public :
			SimpleColor(Memory::RefPtr<RHISystem> system);
			virtual ~SimpleColor();

		public :
			virtual void Draw(const SceneView& scene) override;
			virtual void Init() override;

		private :
			Memory::RefPtr<RHIShader> m_vertexShader;
			Memory::RefPtr<RHIShader> m_pixelShader;
		};
	};
};

#endif // __WTR_SIMPLECOLOR_H__