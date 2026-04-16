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
	{
	}
	
	bool GlobalResource::Init(Memory::RefPtr<RHICommandList> cmdList)
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

		return true;
	}
	
	void GlobalResource::Release(Memory::RefPtr<RHICommandList> cmdList)
	{
		ReleaseBuffer(cmdList);
		ReleaseTexture(cmdList);
		ReleaseScreenQuad(cmdList);
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

		cameraData->viewMatrix = renderView.camera.viewMatrix;
		cameraData->projectionMatrix = renderView.camera.projMatrix;
		cameraData->cameraPosition = renderView.camera.position;
		
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

	const Memory::RefPtr<RHIVertexLayout> GlobalResource::GetScreenQuad() const
	{
		return m_screenQuadResource.vertexLayout;
	}

	bool GlobalResource::InitBuffer(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		// Camera Buffer
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

		return true;
	}

	bool GlobalResource::InitTexture(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		// TODO : initialize the texture resource
		
		return true;
	}

	bool GlobalResource::InitScreenQuad(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		constexpr size_t vertexCount = 4;
		constexpr size_t indexCount = 6;
		fvec2* positionData = static_cast<fvec2*>(cmdList->Alloc<fvec2>(sizeof(fvec2) * vertexCount));
		fvec2* uvData = static_cast<fvec2*>(cmdList->Alloc<fvec2>(sizeof(fvec2) * vertexCount));
		uint32_t* indexData = static_cast<uint32_t*>(cmdList->Alloc<uint32_t>(sizeof(uint32_t) * indexCount));

		positionData[0] = fvec2(-1.0, -1.0);
		positionData[1] = fvec2(1.0, -1.0);
		positionData[2] = fvec2(1.0, 1.0);
		positionData[3] = fvec2(-1.0, 1.0);

		uvData[0] = fvec2(0.0, 0.0);
		uvData[1] = fvec2(1.0, 0.0);
		uvData[2] = fvec2(1.0, 1.0);
		uvData[3] = fvec2(0.0, 1.0);

		indexData[0] = 0;
		indexData[1] = 1;
		indexData[2] = 2;
		indexData[3] = 0;
		indexData[4] = 2;
		indexData[5] = 3;

		RHIBufferCreateDesc positionDesc;
		positionDesc.bufferType = eBufferType::eVertex;
		positionDesc.accessType = eDataAccess::eStatic;
		positionDesc.componentType = eDataType::eFloat;
		positionDesc.numComponents = 2;
		positionDesc.count = vertexCount;
		positionDesc.stride = sizeof(fvec2);
		positionDesc.size = sizeof(fvec2) * vertexCount;
		positionDesc.data = positionData;

		Memory::RefPtr<RHIBuffer> positionBuffer = cmdList->CreateBuffer(positionDesc);
		if (!positionBuffer)
		{
			return false;
		}

		RHIBufferCreateDesc uvDesc;
		uvDesc.bufferType = eBufferType::eVertex;
		uvDesc.accessType = eDataAccess::eStatic;
		uvDesc.componentType = eDataType::eFloat;
		uvDesc.numComponents = 2;
		uvDesc.count = vertexCount;
		uvDesc.stride = sizeof(fvec2);
		uvDesc.size = sizeof(fvec2) * vertexCount;
		uvDesc.data = uvData;

		Memory::RefPtr<RHIBuffer> uvBuffer = cmdList->CreateBuffer(uvDesc);
		if (!uvBuffer)
		{
			return false;
		}

		RHIBufferCreateDesc indexDesc;
		indexDesc.bufferType = eBufferType::eIndex;
		indexDesc.accessType = eDataAccess::eStatic;
		indexDesc.componentType = eDataType::eUInt;
		indexDesc.numComponents = 1;
		indexDesc.count = indexCount;
		indexDesc.stride = sizeof(uint32_t);
		indexDesc.size = sizeof(uint32_t) * indexCount;
		indexDesc.data = indexData;

		Memory::RefPtr<RHIBuffer> indexBuffer = cmdList->CreateBuffer(indexDesc);
		if (!indexBuffer)
		{
			return false;
		}

		RHIVertexLayoutCreateDesc layoutDesc;

		VertexKey positionKey{ eVertexSemantic::ePosition, 0 };
		RHIVertexStream positionStream;
		positionStream.buffer = positionBuffer;
		positionStream.attribute.semantic = eVertexSemantic::ePosition;
		positionStream.attribute.semanticIndex = 0;
		positionStream.attribute.componentType = eDataType::eFloat;
		positionStream.attribute.numComponents = 2;
		positionStream.attribute.normalized = false;
		positionStream.attribute.integer = false;
		positionStream.attribute.divisor = 0;
		positionStream.attribute.offset = 0;

		VertexKey uvKey{ eVertexSemantic::eTexCoord, 0 };
		RHIVertexStream uvStream;
		uvStream.buffer = uvBuffer;
		uvStream.attribute.semantic = eVertexSemantic::eTexCoord;
		uvStream.attribute.semanticIndex = 0;
		uvStream.attribute.componentType = eDataType::eFloat;
		uvStream.attribute.numComponents = 2;
		uvStream.attribute.normalized = false;
		uvStream.attribute.integer = false;
		uvStream.attribute.divisor = 0;
		uvStream.attribute.offset = 0;

		layoutDesc.vertexStreams.Insert(std::make_pair(positionKey, positionStream));
		layoutDesc.vertexStreams.Insert(std::make_pair(uvKey, uvStream));
		layoutDesc.indexBuffer = indexBuffer;

		Memory::RefPtr<RHIVertexLayout> vertexLayout = cmdList->CreateVertexLayout(layoutDesc);
		if (!vertexLayout)
		{
			return false;
		}

		m_screenQuadResource.positionBuffer = positionBuffer;
		m_screenQuadResource.uvBuffer = uvBuffer;
		m_screenQuadResource.indexBuffer = indexBuffer;
		m_screenQuadResource.vertexLayout = vertexLayout;

		return true;
	}

	void GlobalResource::ReleaseBuffer(Memory::RefPtr<RHICommandList> cmdList)
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
	}

	void GlobalResource::ReleaseTexture(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
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
	}

	void GlobalResource::ReleaseScreenQuad(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_screenQuadResource.positionBuffer)
		{
			cmdList->RemoveBuffer(m_screenQuadResource.positionBuffer);
			m_screenQuadResource.positionBuffer = nullptr;
		}

		if (m_screenQuadResource.uvBuffer)
		{
			cmdList->RemoveBuffer(m_screenQuadResource.uvBuffer);
			m_screenQuadResource.uvBuffer = nullptr;
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
}