#ifndef __PHYSICSENGINE_H__
#define __PHYSICSENGINE_H__

#include "DirectXAPI.h"

class CoreEngine;
class RenderingEngine;

class PhysicsEngine
{
	private :
		PhysicsEngine();
		~PhysicsEngine();

	public :
		static PhysicsEngine& Get();
		void Init();
		void Update();
		void Destroy();

	private :
		static PhysicsEngine m_PhysicsEngine;
};

#endif // __PHYSICSENGINE_H__