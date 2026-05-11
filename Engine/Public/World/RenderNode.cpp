#include <World/RenderNode.h>

#include <Renderer/RenderTask.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderScene.h>
#include <Renderer/Proxy/PrimitiveProxy.h>
#include <Renderer/Proxy/LightProxy.h>

namespace wtr
{
	RenderTask ProxyNode::RemoveProxy() const
	{
		RenderTask task;
		task.func = [id = GetID()](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->RemoveProxy(id);
			};

		return task;
	}

	RenderTask StaticMeshNode::CreateProxy() const
	{
		Memory::RefPtr<StaticPrimitiveProxy> proxy = Memory::MakeRef<StaticPrimitiveProxy>(GetID());
		if (!proxy)
		{
			return {};
		}

		if (transform)
		{
			proxy->UpdatePosition(transform->GetPosition());
			proxy->UpdateRotation(transform->GetRotation());
			proxy->UpdateScale(transform->GetScale());
		}

		if (mesh)
		{
			proxy->SetMesh(mesh->GetMeshAsset());
		}

		if (overrideMaterial)
		{
			proxy->SetOverrideMaterial(overrideMaterial->GetMaterialAsset());
		}

		RenderTask task;
		task.func = [proxy](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer || !proxy)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->AddProxy(proxy);
			};

		return task;
	}

	RenderTaskList StaticMeshNode::UpdateProxy() const
	{
		RenderTaskList taskList;

		if (transform && transform->IsDirty())
		{
			RenderTask transformTask;
			transformTask.func = [id = GetID(),
				position = transform->GetPosition(),
				rotation = transform->GetRotation(),
				scale = transform->GetScale()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<StaticPrimitiveProxy> proxy = renderScene->GetPrimitiveProxy<StaticPrimitiveProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->UpdatePosition(position);
					proxy->UpdateRotation(rotation);
					proxy->UpdateScale(scale);
				};

			taskList.PushBack(transformTask);
		}

		// The mesh is constant value in the static mesh node

		return taskList;
	}

	void StaticMeshNode::ClearDirty()
	{
		if (transform)
		{
			transform->ClearDirty();
		}

		if (mesh)
		{
			mesh->ClearDirty();
		}
	}

	RenderTask InstancedStaticMeshNode::CreateProxy() const
	{
		Memory::RefPtr<InstancedStaticPrimitiveProxy> proxy = Memory::MakeRef<InstancedStaticPrimitiveProxy>(GetID());
		if (!proxy)
		{
			return {};
		}

		if (transform)
		{
			proxy->UpdatePosition(transform->GetPosition());
			proxy->UpdateRotation(transform->GetRotation());
			proxy->UpdateScale(transform->GetScale());

			auto instanceTransforms = transform->GetTransforms();
			proxy->UpdateTransforms(std::move(instanceTransforms));
		}

		if (mesh)
		{
			proxy->SetMesh(mesh->GetMeshAsset());
		}

		if (overrideMaterial)
		{
			proxy->SetOverrideMaterial(overrideMaterial->GetMaterialAsset());
		}

		RenderTask task;
		task.func = [proxy](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (!renderer || !renderer->GetScene())
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				renderScene->AddProxy(proxy);
			};

		return task;
	}

	RenderTaskList InstancedStaticMeshNode::UpdateProxy() const
	{
		RenderTaskList taskList;
		
		if (transform && transform->IsDirty())
		{
			RenderTask transformTask;
			transformTask.func = [id = GetID(),
				position = transform->GetPosition(),
				rotation = transform->GetRotation(),
				scale = transform->GetScale(),
				instanceTransforms = transform->GetTransforms()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdLIst) mutable
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<InstancedStaticPrimitiveProxy> proxy = renderScene->GetPrimitiveProxy<InstancedStaticPrimitiveProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->UpdatePosition(position);
					proxy->UpdateRotation(rotation);
					proxy->UpdateScale(scale);

					proxy->UpdateTransforms(std::move(instanceTransforms));
				};

			taskList.PushBack(transformTask);
		}

		return taskList;
	}

	void InstancedStaticMeshNode::ClearDirty()
	{
		if (transform)
		{
			transform->ClearDirty();
		}

		if (mesh)
		{
			mesh->ClearDirty();
		}
	}

	RenderTask DynamicMeshNode::CreateProxy() const
	{
		RenderTask task;

		return task;
	}

	RenderTaskList DynamicMeshNode::UpdateProxy() const
	{
		return {};

		// TODO
	}

	void DynamicMeshNode::ClearDirty()
	{
		if (transform)
		{
			transform->ClearDirty();
		}

		if (mesh)
		{
			mesh->ClearDirty();
		}
	}

	RenderTask DirectionalLightNode::CreateProxy() const
	{
		Memory::RefPtr<DirectionalLightProxy> proxy = Memory::MakeRef<DirectionalLightProxy>(GetID());
		if (!proxy)
		{
			return {};
		}

		if (transform)
		{
			proxy->UpdatePosition(transform->GetPosition());
			proxy->UpdateRotation(transform->GetRotation());
		}

		if (light)
		{
			proxy->SetLightType(light->GetLightType());
			proxy->SetShadowType(light->GetShadowType());
			proxy->SetColor(light->GetColor());
			proxy->SetDirection(light->GetDirection());
			proxy->SetIntensity(light->GetIntensity());
		}

		RenderTask task;
		task.func = [proxy](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (!renderer || !renderer->GetScene())
				{
					return;
				}

				auto renderScene = renderer->GetScene();

				renderScene->AddProxy(proxy);
			};

		return task;
	}

	RenderTaskList DirectionalLightNode::UpdateProxy() const
	{
		RenderTaskList taskList;

		if (transform && transform->IsDirty())
		{
			RenderTask transformTask;
			transformTask.func = [id = GetID(),
				position = transform->GetPosition(),
				rotation = transform->GetRotation()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<DirectionalLightProxy> proxy = renderScene->GetLightProxy<DirectionalLightProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->UpdatePosition(position);
					proxy->UpdateRotation(rotation);
				};

			taskList.PushBack(transformTask);
		}

		if (light && light->IsDirty())
		{
			RenderTask lightTask;
			lightTask.func = [id = GetID(),
				lightType = light->GetLightType(),
				shadowType = light->GetShadowType(),
				color = light->GetColor(),
				direction = light->GetDirection(),
				intensity = light->GetIntensity()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<DirectionalLightProxy> proxy = renderScene->GetLightProxy<DirectionalLightProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->SetLightType(lightType);
					proxy->SetShadowType(shadowType);
					proxy->SetColor(color);
					proxy->SetDirection(direction);
					proxy->SetIntensity(intensity);
				};

			taskList.PushBack(lightTask);
		}

		return taskList;
	}

	void DirectionalLightNode::ClearDirty()
	{
		if (transform)
		{
			transform->ClearDirty();
		}

		if (light)
		{
			light->ClearDirty();
		}
	}

	RenderTask PointLightNode::CreateProxy() const
	{
		Memory::RefPtr<PointLightProxy> proxy = Memory::MakeRef<PointLightProxy>(GetID());
		if (!proxy)
		{
			return {};
		}

		if (transform)
		{
			proxy->UpdatePosition(transform->GetPosition());
			proxy->UpdateRotation(transform->GetRotation());
		}

		if (light)
		{
			proxy->SetLightType(light->GetLightType());
			proxy->SetShadowType(light->GetShadowType());
			proxy->SetColor(light->GetColor());
			proxy->SetDirection(light->GetDirection());
			proxy->SetIntensity(light->GetIntensity());
			proxy->SetRange(light->GetRange());
		}

		RenderTask task;
		task.func = [proxy](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (!renderer || !renderer->GetScene())
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				renderScene->AddProxy(proxy);
			};

		return task;
	}

	RenderTaskList PointLightNode::UpdateProxy() const
	{
		RenderTaskList taskList;

		if (transform && transform->IsDirty())
		{
			RenderTask transformTask;
			transformTask.func = [id = GetID(),
				position = transform->GetPosition(),
				rotation = transform->GetRotation()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<PointLightProxy> proxy = renderScene->GetLightProxy<PointLightProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->UpdatePosition(position);
					proxy->UpdateRotation(rotation);
				};

			taskList.PushBack(transformTask);
		}

		if (light && light->IsDirty())
		{
			RenderTask lightTask;
			lightTask.func = [id = GetID(),
				lightType = light->GetLightType(),
				shadowType = light->GetShadowType(),
				color = light->GetColor(),
				direction = light->GetDirection(),
				intensity = light->GetIntensity(),
				range = light->GetRange()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<PointLightProxy> proxy = renderScene->GetLightProxy<PointLightProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->SetLightType(lightType);
					proxy->SetShadowType(shadowType);
					proxy->SetColor(color);
					proxy->SetDirection(direction);
					proxy->SetIntensity(intensity);
					proxy->SetRange(range);
				};

			taskList.PushBack(lightTask);
		}

		return taskList;
	}

	void PointLightNode::ClearDirty()
	{
		if (transform)
		{
			transform->ClearDirty();
		}

		if (light)
		{
			light->ClearDirty();
		}
	}

	RenderTask SpotLightNode::CreateProxy() const
	{
		Memory::RefPtr<SpotLightProxy> proxy = Memory::MakeRef<SpotLightProxy>(GetID());
		if (!proxy)
		{
			return {};
		}

		if (transform)
		{
			proxy->UpdatePosition(transform->GetPosition());
			proxy->UpdateRotation(transform->GetRotation());
		}

		if (light)
		{
			proxy->SetColor(light->GetColor());
			proxy->SetDirection(light->GetDirection());
			proxy->SetIntensity(light->GetIntensity());
			proxy->SetRange(light->GetRange());
			proxy->SetInnerAngle(light->GetInnerAngle());
			proxy->SetOuterAngle(light->GetOuterAngle());
		}

		RenderTask task;
		task.func = [proxy](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (!renderer || !renderer->GetScene())
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				renderScene->AddProxy(proxy);
			};

		return task;
	}

	RenderTaskList SpotLightNode::UpdateProxy() const
	{
		RenderTaskList taskList;

		if (transform && transform->IsDirty())
		{
			RenderTask transformTask;
			transformTask.func = [id = GetID(),
				position = transform->GetPosition(),
				rotation = transform->GetRotation()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<SpotLightProxy> proxy = renderScene->GetLightProxy<SpotLightProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->UpdatePosition(position);
					proxy->UpdateRotation(rotation);
				};

			taskList.PushBack(transformTask);
		}

		if (light && light->IsDirty())
		{
			RenderTask lightTask;
			lightTask.func = [id = GetID(),
				lightType = light->GetLightType(),
				shadowType = light->GetShadowType(),
				color = light->GetColor(),
				direction = light->GetDirection(),
				intensity = light->GetIntensity(),
				range = light->GetRange(),
				innerAngle = light->GetInnerAngle(),
				outerAngle = light->GetOuterAngle()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
				{
					if (!renderer || !renderer->GetScene())
					{
						return;
					}

					auto renderScene = renderer->GetScene();

					Memory::RefPtr<SpotLightProxy> proxy = renderScene->GetLightProxy<SpotLightProxy>(id);
					if (!proxy)
					{
						return;
					}

					proxy->SetLightType(lightType);
					proxy->SetShadowType(shadowType);
					proxy->SetColor(color);
					proxy->SetDirection(direction);
					proxy->SetIntensity(intensity);
					proxy->SetRange(range);
					proxy->SetInnerAngle(innerAngle);
					proxy->SetOuterAngle(outerAngle);
				};

			taskList.PushBack(lightTask);
		}

		return taskList;
	}

	void SpotLightNode::ClearDirty()
	{
		if (transform)
		{
			transform->ClearDirty();
		}

		if (light)
		{
			light->ClearDirty();
		}
	}
}