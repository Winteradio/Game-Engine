#ifndef __CORE_H__
#define __CORE_H__

#include "DirectXAPI.h"
#include "WinAPI.h"
#include "ImGuiAPI.h"

#include "Manager.h"
#include "Handler.h"

class Core
{
	private :
		Core();
		~Core();

	public :
		static Core& GetHandle();
		void Start();
		void Update();

		void Init();
		void InitAPI();
		void InitHandler();
		void InitManager();

		void Destroy();

	private :
		static Core m_Core;
};

#endif // __CORE_H__