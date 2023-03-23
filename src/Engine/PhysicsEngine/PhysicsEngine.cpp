#include "PhysicsEngine.h"

#include "CoreEngine.h"
#include "RenderingEngine.h"

#include "ResourceManager.h"

PhysicsEngine::PhysicsEngine() {}

PhysicsEngine::~PhysicsEngine()
{
	Destroy();
}

PhysicsEngine& PhysicsEngine::Get()
{
	return m_PhysicsEngine;
}

void PhysicsEngine::Init()
{

}

void PhysicsEngine::Update()
{

}

void PhysicsEngine::Destroy()
{

}

PhysicsEngine PhysicsEngine::m_PhysicsEngine;