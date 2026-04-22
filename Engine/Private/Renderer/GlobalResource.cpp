#include <Renderer/GlobalResource.h>

#include <Renderer/RenderView.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <Memory/include/Core.h>

namespace wtr
{
	GlobalResource::GlobalResource()
		: m_cameraResource{}
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

		if (!InitCamera(cmdList))
		{
			return false;
		}

		if (!InitTexture(cmdList))
		{
			return false;
		}

		if (!InitScreenQuad(cmdList))
		{
			return false;
		}

		return true;
	}
	
	void GlobalResource::Release(Memory::RefPtr<RHICommandList> cmdList)
	{
		ReleaseCamera(cmdList);
		ReleaseTexture(cmdList);
		ReleaseScreenQuad(cmdList);
	}

	void GlobalResource::UpdateCamera(const RenderView& renderView, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_cameraResource.buffer)
		{
			return;
		}

		m_cameraResource.camera.viewMatrix = renderView.camera.viewMatrix;
		m_cameraResource.camera.projectionMatrix = renderView.camera.projMatrix;
		m_cameraResource.camera.cameraPosition = renderView.camera.position;

		memcpy(m_cameraResource.rawBuffer->data.Data(), &m_cameraResource.camera, sizeof(CameraResource::CameraData));
		
		RHIBufferUpdateDesc updateDesc;
		updateDesc.bufferType = eBufferType::eConst;
		updateDesc.accessType = eDataAccess::eDynamic;
		updateDesc.componentType = eDataType::eNone;
		updateDesc.numComponents = 0;
		updateDesc.count = 1;
		updateDesc.stride = sizeof(CameraResource::CameraData);
		updateDesc.size = sizeof(CameraResource::CameraData);
		updateDesc.data = m_cameraResource.rawBuffer;
		updateDesc.mapAccess = eMapAccess::eNone;
		
		cmdList->UpdateBuffer(updateDesc, m_cameraResource.buffer);
	}

	const Memory::RefPtr<RHIBuffer> GlobalResource::GetCameraBuffer() const
	{
		return m_cameraResource.buffer;
	}

	const Memory::RefPtr<RHIVertexLayout> GlobalResource::GetScreenQuad() const
	{
		return m_screenQuadResource.vertexLayout;
	}

	bool GlobalResource::InitCamera(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		if (!m_cameraResource.rawBuffer)
		{
			m_cameraResource.rawBuffer = Memory::MakeRef<BulkData<uint8_t>>();
			m_cameraResource.rawBuffer->data.Resize(sizeof(CameraResource::CameraData));
		}

		if (!m_cameraResource.buffer)
		 {
			 RHIBufferCreateDesc desc;
			 desc.bufferType = eBufferType::eConst;
			 desc.accessType = eDataAccess::eDynamic;
			 desc.componentType = eDataType::eNone;
			 desc.numComponents = 0;
			 desc.count = 1;
			 desc.stride = sizeof(CameraResource::CameraData);
			 desc.size = sizeof(CameraResource::CameraData);
			 desc.data = m_cameraResource.rawBuffer;
			 
			 Memory::RefPtr<RHIBuffer> cameraBuffer = cmdList->CreateBuffer(desc);
			 if (!cameraBuffer)
			 {
				 return false;
			 }

			 m_cameraResource.buffer = cameraBuffer;
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

		Memory::RefPtr<BulkData<fvec2>> positionData = Memory::MakeRef<BulkData<fvec2>>();
		Memory::RefPtr<BulkData<fvec2>> uvData = Memory::MakeRef<BulkData<fvec2>>();
		Memory::RefPtr<BulkData<uint32_t>> indexData = Memory::MakeRef<BulkData<uint32_t>>();
		if (!positionData || !uvData || !indexData)
		{
			return false;
		}

		positionData->data.Resize(vertexCount);
		uvData->data.Resize(vertexCount);
		indexData->data.Resize(indexCount);

		positionData->data[0] = fvec2(-1.0, -1.0);
		positionData->data[1] = fvec2(1.0, -1.0);
		positionData->data[2] = fvec2(1.0, 1.0);
		positionData->data[3] = fvec2(-1.0, 1.0);

		uvData->data[0] = fvec2(0.0, 0.0);
		uvData->data[1] = fvec2(1.0, 0.0);
		uvData->data[2] = fvec2(1.0, 1.0);
		uvData->data[3] = fvec2(0.0, 1.0);

		indexData->data[0] = 0;
		indexData->data[1] = 1;
		indexData->data[2] = 2;
		indexData->data[3] = 0;
		indexData->data[4] = 2;
		indexData->data[5] = 3;

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

	void GlobalResource::ReleaseCamera(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_cameraResource.buffer)
		{
			cmdList->RemoveBuffer(m_cameraResource.buffer);
			m_cameraResource.buffer = nullptr;
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