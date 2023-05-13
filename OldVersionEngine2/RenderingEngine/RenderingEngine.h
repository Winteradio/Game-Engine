#ifndef __RENDERINGENGINE_H__
#define __RENDERINGENGINE_H__

#include "DirectXAPI.h"
#include "ImGuiAPI.h"
#include "RenderingStatus.h"

class CoreEngine;
class PhysicsEngine;

class RenderingEngine
{
	private :
		RenderingEngine();
		~RenderingEngine();

	public :
		static RenderingEngine& Get();
		void Init();
		void Begin();
		void Render();
		void End();
		void Destroy();

	private :
		static RenderingEngine m_RenderingEngine;
};

#endif // __RENDERINGENGINE_H__