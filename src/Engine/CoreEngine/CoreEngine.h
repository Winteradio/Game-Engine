#ifndef __COREENGINE_H__
#define __COREENGINE_H__

#include "DirectXAPI.h"
#include "WinAPI.h"
#include "ImGuiAPI.h"

class RenderingEngine;
class PhysicsEngine;

class CoreEngine
{
	private :
		CoreEngine();
		~CoreEngine();

	public :
		static CoreEngine& Get();
		void Start();
		void Update();

		void Init();
		void InitOtherEngine();
		void InitManager();
		void InitAPI();
		void Destroy();

	private :
		static CoreEngine m_CoreEngine;
};

#endif // __COREENGINE_H__