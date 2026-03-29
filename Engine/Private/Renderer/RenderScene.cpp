#include <Renderer/RenderScene.h>

#include <Renderer/Proxy/PrimitiveProxy.h>
#include <Renderer/Proxy/LightProxy.h>

#include <Memory/include/Core.h>

namespace wtr
{
	RenderScene::RenderScene()
		: m_primitives()
		, m_lights()
		, m_addable()
		, m_removable()
	{}

	RenderScene::~RenderScene()
	{}

	void RenderScene::FlushPending()
	{
		FlushAddable();
		FlushRemovable();
	}

	void RenderScene::AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive)
	{
		if (!primitive)
		{
			return;
		}

		m_addable.PushBack(primitive);
	}

	void RenderScene::RemovePrimitive(const ECS::UUID& id)
	{
		auto itr = m_primitives.Find(id);
		if (itr != m_primitives.End())
		{
			auto& primitiveProxy = itr->second;

			m_removable.PushBack(primitiveProxy);
			m_primitives.Erase(itr);
		}
	}

	Memory::RefPtr<PrimitiveProxy> RenderScene::GetPrimitive(const ECS::UUID& id)
	{
		auto itr = m_primitives.Find(id);
		if (itr != m_primitives.End())
		{
			return itr->second;
		}
		else
		{
			return {};
		}
	}

	void RenderScene::AddLight(Memory::RefPtr<LightProxy> light)
	{
		if (!light)
		{
			return;
		}

		m_addable.PushBack(light);
	}

	void RenderScene::RemoveLight(const ECS::UUID& id)
	{
		auto itr = m_lights.Find(id);
		if (itr != m_lights.End())
		{
			auto& lightProxy = itr->second;

			m_removable.PushBack(lightProxy);
			m_lights.Erase(itr);
		}
	}

	Memory::RefPtr<LightProxy> RenderScene::GetLight(const ECS::UUID& id)
	{
		auto itr = m_lights.Find(id);
		if (itr != m_lights.End())
		{
			return itr->second;
		}
		else
		{
			return {};
		}
	}

	RenderScene::PendingProxy& RenderScene::GetAddable()
	{
		return m_addable;
	}

	RenderScene::PendingProxy& RenderScene::GetRemovable()
	{
		return m_removable;
	}

	void RenderScene::FlushAddable()
	{
		auto itr = m_addable.begin();
		while (itr != m_addable.end())
		{
			auto& sceneProxy = *itr;
			if (sceneProxy->GetState() != eProxyState::eReady)
			{
				itr++;

				continue;
			}

			const Reflection::TypeInfo* typeInfo = sceneProxy->GetTypeInfo();
			if (typeInfo == Reflection::TypeInfo::Get<PrimitiveProxy>())
			{
				auto primitiveProxy = Memory::Cast<PrimitiveProxy>(sceneProxy);
				if (primitiveProxy)
				{
					m_primitives[primitiveProxy->GetID()] = primitiveProxy;
				}
			}
			else if (typeInfo == Reflection::TypeInfo::Get<LightProxy>())
			{
				auto lightProxy = Memory::Cast<LightProxy>(sceneProxy);
				if (lightProxy)
				{
					m_lights[lightProxy->GetID()] = lightProxy;
				}
			}
			else
			{
				// nothing
			}

			itr = m_addable.Erase(itr);
		}
	}

	void RenderScene::FlushRemovable()
	{
		auto itr = m_removable.begin();
		while (itr != m_removable.end())
		{
			auto& sceneProxy = *itr;
			if (sceneProxy->GetState() == eProxyState::eRemoved)
			{
				itr = m_removable.Erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}