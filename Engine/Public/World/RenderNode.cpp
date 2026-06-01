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

	void StaticMeshNode::OnAttached(Scene* scene)
	{
		if (transform)
		{
			transform->OnAttached(scene);
		}

		if (mesh)
		{
			mesh->OnAttached(scene);
		}
	}

	void StaticMeshNode::OnDetached()
	{
		if (transform)
		{
			transform->OnDetached();
		}

		if (mesh)
		{
			mesh->OnDetached();
		}
	}

	void StaticMeshNode::OnUpdated()
	{
		if (transform)
		{
			OnSynced(transform);
			transform->OnFlushed();
		}

		if (mesh)
		{
			OnSynced(mesh);
			mesh->OnFlushed();
		}
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

		if (IsDirty(mesh))
		{
			RenderTask meshTask;
			meshTask.func = [id = GetID(),
				meshAsset = mesh->GetMeshAsset()
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

					if (proxy->GetMesh() != meshAsset)
					{
						proxy->SetMesh(meshAsset);
						renderScene->RemoveProxy(proxy);
						renderScene->AddProxy(proxy);
					}
				};

			taskList.PushBack(meshTask);
		}

		if (IsDirty(transform))
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

		return taskList;
	}

	void InstancedStaticMeshNode::OnAttached(Scene* scene)
	{
		if (transform)
		{
			transform->OnAttached(scene);
		}

		if (mesh)
		{
			mesh->OnAttached(scene);
		}
	}

	void InstancedStaticMeshNode::OnDetached()
	{
		if (transform)
		{
			transform->OnDetached();
		}

		if (mesh)
		{
			mesh->OnDetached();
		}
	}

	void InstancedStaticMeshNode::OnUpdated()
	{
		if (transform)
		{
			OnSynced(transform);
			transform->OnFlushed();
		}

		if (mesh)
		{
			OnSynced(mesh);
			mesh->OnFlushed();
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

			auto& instanceTransforms = transform->GetTransforms();
			
			proxy->SetInstanceCount(instanceTransforms.Size());
			for (size_t index = 0; index < instanceTransforms.Size(); index++)
			{
				proxy->UpdateTransform(index, instanceTransforms[index]);
			}
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

		if (IsDirty(mesh))
		{
			RenderTask meshTask;
			meshTask.func = [id = GetID(),
				meshAsset = mesh->GetMeshAsset()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
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

					if (proxy->GetMesh() != meshAsset)
					{
						proxy->SetMesh(meshAsset);
						renderScene->RemoveProxy(proxy);
						renderScene->AddProxy(proxy);
					}
				};

			taskList.PushBack(meshTask);
		}
		
		if (IsDirty(transform))
		{
			RenderTask transformTask;
			transformTask.func = [id = GetID(),
				position = transform->GetPosition(),
				rotation = transform->GetRotation(),
				scale = transform->GetScale(),
				instanceTransforms = transform->GetTransforms()
			] (Renderer* renderer, Memory::RefPtr<RHICommandList> cmdLIst)
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

					proxy->SetInstanceCount(instanceTransforms.Size());
					for (size_t index = 0; index < instanceTransforms.Size(); index++)
					{
						proxy->UpdateTransform(index, instanceTransforms[index]);
					}
				};

			taskList.PushBack(transformTask);
		}

		return taskList;
	}

	void DynamicMeshNode::OnAttached(Scene* scene)
	{
		if (transform)
		{
			transform->OnAttached(scene);
		}

		if (mesh)
		{
			mesh->OnAttached(scene);
		}
	}

	void DynamicMeshNode::OnDetached()
	{
		if (transform)
		{
			transform->OnDetached();
		}

		if (mesh)
		{
			mesh->OnDetached();
		}
	}

	void DynamicMeshNode::OnUpdated()
	{
		if (transform)
		{
			OnSynced(transform);
			transform->OnFlushed();
		}

		if (mesh)
		{
			OnSynced(mesh);
			mesh->OnFlushed();
		}
	}

	RenderTask DynamicMeshNode::CreateProxy() const
	{
		RenderTask task;

		return task;

		// TODO
	}

	RenderTaskList DynamicMeshNode::UpdateProxy() const
	{
		return {};

		// TODO
	}

	void DirectionalLightNode::OnAttached(Scene* scene)
	{
		if (transform)
		{
			transform->OnAttached(scene);
		}

		if (light)
		{
			light->OnAttached(scene);
		}
	}

	void DirectionalLightNode::OnDetached()
	{
		if (transform)
		{
			transform->OnDetached();
		}

		if (light)
		{
			light->OnDetached();
		}
	}

	void DirectionalLightNode::OnUpdated()
	{
		if (transform)
		{
			OnSynced(transform);
			transform->OnFlushed();
		}

		if (light)
		{
			OnSynced(light);
			light->OnFlushed();
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

		if (IsDirty(transform))
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

		if (IsDirty(light))
		{
			RenderTask lightTask;
			lightTask.func = [id = GetID(),
				lightType = light->GetLightType(),
				shadowType = light->GetShadowType(),
				color = light->GetColor(),
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
					proxy->SetIntensity(intensity);
				};

			taskList.PushBack(lightTask);
		}

		return taskList;
	}

	void PointLightNode::OnAttached(Scene* scene)
	{
		if (transform)
		{
			transform->OnAttached(scene);
		}

		if (light)
		{
			light->OnAttached(scene);
		}
	}

	void PointLightNode::OnDetached()
	{
		if (transform)
		{
			transform->OnDetached();
		}

		if (light)
		{
			light->OnDetached();
		}
	}

	void PointLightNode::OnUpdated()
	{
		if (transform)
		{
			OnSynced(transform);
			transform->OnFlushed();
		}

		if (light)
		{
			OnSynced(light);
			light->OnFlushed();
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

		if (IsDirty(transform))
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

		if (IsDirty(light))
		{
			RenderTask lightTask;
			lightTask.func = [id = GetID(),
				lightType = light->GetLightType(),
				shadowType = light->GetShadowType(),
				color = light->GetColor(),
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
					proxy->SetIntensity(intensity);
					proxy->SetRange(range);
				};

			taskList.PushBack(lightTask);
		}

		return taskList;
	}

	void SpotLightNode::OnAttached(Scene* scene)
	{
		if (transform)
		{
			transform->OnAttached(scene);
		}

		if (light)
		{
			light->OnAttached(scene);
		}
	}

	void SpotLightNode::OnDetached()
	{
		if (transform)
		{
			transform->OnDetached();
		}

		if (light)
		{
			light->OnDetached();
		}
	}

	void SpotLightNode::OnUpdated()
	{
		if (transform)
		{
			OnSynced(transform);
			transform->OnFlushed();
		}

		if (light)
		{
			OnSynced(light);
			light->OnFlushed();
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

		if (IsDirty(transform))
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

		if (IsDirty(light))
		{
			RenderTask lightTask;
			lightTask.func = [id = GetID(),
				lightType = light->GetLightType(),
				shadowType = light->GetShadowType(),
				color = light->GetColor(),
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
					proxy->SetIntensity(intensity);
					proxy->SetRange(range);
					proxy->SetInnerAngle(innerAngle);
					proxy->SetOuterAngle(outerAngle);
				};

			taskList.PushBack(lightTask);
		}

		return taskList;
	}
}