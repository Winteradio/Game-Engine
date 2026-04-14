#include <Renderer/GlobalResource.h>

#include <Renderer/RenderView.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>

namespace wtr
{
	GlobalResource::GlobalResource()
		: m_bufferResource{}
		, m_textureResource{}
		, m_screenQuadResource{}
		, m_ready(false)
	{
	}
	
	bool GlobalResource::IsReady() const
	{
		return m_ready;
	}

	bool GlobalResource::Setup(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		{
			CameraData* cameraData = static_cast<CameraData*>(cmdList->Alloc<CameraData>(sizeof(CameraData)));
			if (!cameraData)
			{
				return false;
			}

			RHIBufferCreateDesc desc;
			desc.bufferType = eBufferType::eConst;
			desc.accessType = eDataAccess::eDynamic;
			desc.componentType = eDataType::eNone;
			desc.numComponents = 0;
			desc.count = 1;
			desc.stride = sizeof(CameraData);
			desc.size = sizeof(CameraData);
			desc.data = cameraData;

			Memory::RefPtr<RHIBuffer> cameraBuffer = cmdList->CreateBuffer(desc);
			if (!cameraBuffer)
			{
				return false;
			}

			m_bufferResource.cameraBuffer = cameraBuffer;
		}

		// TODO : initialize the texture and quad resource

		m_ready = true;

		return true;
	}
	
	void GlobalResource::Release(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_bufferResource.cameraBuffer)
		{
			cmdList->RemoveBuffer(m_bufferResource.cameraBuffer);
			m_bufferResource.cameraBuffer = nullptr;
		}

		if (m_textureResource.position)
		{
			cmdList->RemoveTexture(m_textureResource.position);
			m_textureResource.position = nullptr;
		}

		if (m_textureResource.normal)
		{
			cmdList->RemoveTexture(m_textureResource.normal);
			m_textureResource.normal = nullptr;
		}

		if (m_textureResource.albedo)
		{
			cmdList->RemoveTexture(m_textureResource.albedo);
			m_textureResource.albedo = nullptr;
		}

		if (m_textureResource.depth)
		{
			cmdList->RemoveTexture(m_textureResource.depth);
			m_textureResource.depth = nullptr;
		}

		if (m_textureResource.stencil)
		{
			cmdList->RemoveTexture(m_textureResource.stencil);
			m_textureResource.stencil = nullptr;
		}

		if (m_screenQuadResource.vertexBuffer)
		{
			cmdList->RemoveBuffer(m_screenQuadResource.vertexBuffer);
			m_screenQuadResource.vertexBuffer = nullptr;
		}

		if (m_screenQuadResource.indexBuffer)
		{
			cmdList->RemoveBuffer(m_screenQuadResource.indexBuffer);
			m_screenQuadResource.indexBuffer = nullptr;
		}

		if (m_screenQuadResource.vertexLayout)
		{
			cmdList->RemoveVertexLayout(m_screenQuadResource.vertexLayout);
			m_screenQuadResource.vertexLayout = nullptr;
		}
	}

	void GlobalResource::UpdateCamera(const RenderView& renderView, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_bufferResource.cameraBuffer)
		{
			return;
		}

		CameraData* cameraData = static_cast<CameraData*>(cmdList->Alloc<CameraData>(sizeof(CameraData)));
		if (!cameraData)
		{
			return;
		}

		cameraData->viewMatrix = fmat4(1.0);
		cameraData->projectionMatrix = fmat4(1.0);
		cameraData->cameraPosition = renderView.camera.position;
		cameraData->nearPlane = renderView.camera.nearPlane;
		cameraData->farPlane = renderView.camera.farPlane;
		cameraData->fov = renderView.camera.fov;
		
		RHIBufferUpdateDesc updateDesc;
		updateDesc.bufferType = eBufferType::eConst;
		updateDesc.accessType = eDataAccess::eDynamic;
		updateDesc.componentType = eDataType::eNone;
		updateDesc.numComponents = 0;
		updateDesc.count = 1;
		updateDesc.stride = sizeof(CameraData);
		updateDesc.size = sizeof(CameraData);
		updateDesc.data = cameraData;
		updateDesc.dataSize = sizeof(CameraData);
		updateDesc.mapAccess = eMapAccess::eNone;
		
		cmdList->UpdateBuffer(updateDesc, m_bufferResource.cameraBuffer);
	}

	const Memory::RefPtr<RHIBuffer> GlobalResource::GetCameraBuffer() const
	{
		return m_bufferResource.cameraBuffer;
	}
}