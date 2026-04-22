#include <Asset/AssetCommander.h>

#include <Asset/AssetTypes.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>

#include <Memory/include/Core.h>

namespace wtr
{
	void AssetCommander::Load(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (asset->type == eAsset::eMesh)
		{
			Memory::RefPtr<MeshAsset> meshAsset = Memory::Cast<MeshAsset>(asset);
			onLoad(meshAsset, cmdList);
		}
		else if (asset->type == eAsset::eTexture)
		{
			Memory::RefPtr<TextureAsset> textureAsset = Memory::Cast<TextureAsset>(asset);
			onLoad(textureAsset, cmdList);
		}
		else if (asset->type == eAsset::eShader)
		{
			Memory::RefPtr<ShaderAsset> shaderAsset = Memory::Cast<ShaderAsset>(asset);
			onLoad(shaderAsset, cmdList);
		}
		else
		{
			// The invalid asset type
			return;
		}
	}

	void AssetCommander::Update(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (asset->type == eAsset::eMesh)
		{
			Memory::RefPtr<MeshAsset> meshAsset = Memory::Cast<MeshAsset>(asset);
			onUpdate(meshAsset, cmdList);
		}
		else if (asset->type == eAsset::eTexture)
		{
			Memory::RefPtr<TextureAsset> textureAsset = Memory::Cast<TextureAsset>(asset);
			onUpdate(textureAsset, cmdList);
		}
		else
		{
			// The invalid asset type
			return;
		}
	}

	void AssetCommander::Unload(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (asset->type == eAsset::eMesh)
		{
			Memory::RefPtr<MeshAsset> meshAsset = Memory::Cast<MeshAsset>(asset);
			onUnload(meshAsset, cmdList);
		}
		else if (asset->type == eAsset::eTexture)
		{
			Memory::RefPtr<TextureAsset> textureAsset = Memory::Cast<TextureAsset>(asset);
			onUnload(textureAsset, cmdList);
		}
		else if (asset->type == eAsset::eShader)
		{
			Memory::RefPtr<ShaderAsset> shaderAsset = Memory::Cast<ShaderAsset>(asset);
			onUnload(shaderAsset, cmdList);
		}
		else
		{
			// The invalid asset type
			return;
		}
	}

	void AssetCommander::onLoad(Memory::RefPtr<MeshAsset> meshAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!meshAsset || !cmdList)
		{
			return;
		}

		for (const auto& [vertexKey, rawBuffer] : meshAsset->rawBuffers)
		{
			if (!rawBuffer.bulkData || rawBuffer.bulkData->IsEmpty())
			{
				continue;
			}

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = eBufferType::eVertex;
			bufferDesc.accessType = eDataAccess::eStatic;
			bufferDesc.componentType = rawBuffer.componentType;
			bufferDesc.numComponents = rawBuffer.numComponents;
			bufferDesc.count = rawBuffer.count;
			bufferDesc.size = static_cast<uint32_t>(rawBuffer.bulkData->GetSize());
			bufferDesc.stride = rawBuffer.numComponents * GetDataTypeSize(rawBuffer.componentType);
			bufferDesc.data = rawBuffer.bulkData;

			Memory::RefPtr<RHIBuffer> vertexBuffer = cmdList->CreateBuffer(bufferDesc);
			if (vertexBuffer)
			{
				meshAsset->buffers[vertexKey] = vertexBuffer;
			}
		}

		if (meshAsset->rawIndex.bulkData && !meshAsset->rawIndex.bulkData->IsEmpty())
		{
			const auto& rawIndex = meshAsset->rawIndex;

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = eBufferType::eIndex;
			bufferDesc.accessType = eDataAccess::eStatic;
			bufferDesc.componentType = rawIndex.componentType;
			bufferDesc.numComponents = rawIndex.numComponents;
			bufferDesc.count = rawIndex.count;
			bufferDesc.size = static_cast<uint32_t>(rawIndex.bulkData->GetSize());
			bufferDesc.stride = rawIndex.numComponents * GetDataTypeSize(rawIndex.componentType);
			bufferDesc.data = rawIndex.bulkData;

			Memory::RefPtr<RHIBuffer> indexBuffer = cmdList->CreateBuffer(bufferDesc);
			if (indexBuffer)
			{
				meshAsset->index = indexBuffer;
			}
		}
	}

	void AssetCommander::onLoad(Memory::RefPtr<TextureAsset> textureAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!textureAsset || !cmdList)
		{
			return;
		}

		auto& rawTexture = textureAsset->rawTexture;
		if (rawTexture.faces.Empty())
		{
			return;
		}

		RHITextureCreateDesc textureDesc;
		textureDesc.width = textureAsset->width;
		textureDesc.height = textureAsset->height;
		textureDesc.depth = textureAsset->depth;
		textureDesc.mipLevels = textureAsset->mipLevels;
		textureDesc.sampleCount = textureAsset->sampleCount;
		textureDesc.format = textureAsset->pixelFormat;
		textureDesc.dataType = rawTexture.componentType;

		textureDesc.faces.Resize(rawTexture.faces.Size());

		for (size_t faceIndex = 0; faceIndex < textureDesc.faces.Size(); ++faceIndex)
		{
			const auto& rawFaceBuffer = rawTexture.faces[faceIndex];
			auto& faceDesc = textureDesc.faces[faceIndex];

			faceDesc.mipMaps.Resize(rawFaceBuffer.mipMaps.Size());

			for (size_t mipIndex = 0; mipIndex < rawFaceBuffer.mipMaps.Size(); ++mipIndex)
			{
				const auto& rawMipBuffer = rawFaceBuffer.mipMaps[mipIndex];
				auto& mipMap = faceDesc.mipMaps[mipIndex];

				mipMap.level = rawMipBuffer.level;
				mipMap.data = rawMipBuffer.bulkData;
			}
		}

		// TODO : Determine the texture type based on the texture asset's properties
		if (textureAsset->depth == 1)
		{
			textureDesc.textureType = (textureAsset->isCubemap) ? eTextureType::eTextureCube : eTextureType::eTexture2D;
		}
		else
		{
			textureDesc.textureType = (textureAsset->isCubemap) ? eTextureType::eTextureCubeArray : eTextureType::eTexture3D;
		}

		textureDesc.usage = eTextureUsage::eSampled;
		textureDesc.generateMips = textureAsset->isGenerateMips;
		textureDesc.compressed = false;

		Memory::RefPtr<RHITexture> texture = cmdList->CreateTexture(textureDesc);
		if (!texture)
		{
			return;
		}

		textureAsset->texture = texture;
	}

	void AssetCommander::onLoad(Memory::RefPtr<ShaderAsset> shaderAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !shaderAsset)
		{
			return;
		}

		auto& rawBuffer = shaderAsset->rawBuffer;
		if (!rawBuffer.bulkData || rawBuffer.bulkData->IsEmpty())
		{
			return;
		}

		RHIShaderCreateDesc shaderDesc;
		shaderDesc.shaderType = shaderAsset->GetShaderType();
		shaderDesc.data = rawBuffer.bulkData;

		if (shaderDesc.shaderType == eShaderType::eNone)
		{
			return;
		}
		else if (shaderDesc.shaderType == eShaderType::eVertex)
		{
			Memory::RefPtr<RHIShader> vertexShader = cmdList->CreateVertexShader(shaderDesc);
			if (vertexShader)
			{
				shaderAsset->shader = vertexShader;
			}
		}
		else if (shaderDesc.shaderType == eShaderType::eGeometry)
		{
			Memory::RefPtr<RHIShader> geometryShader = cmdList->CreateGeometryShader(shaderDesc);
			if (geometryShader)
			{
				shaderAsset->shader = geometryShader;
			}
		}
		else if (shaderDesc.shaderType == eShaderType::eHull)
		{
			Memory::RefPtr<RHIShader> hullShader = cmdList->CreateHullShader(shaderDesc);
			if (hullShader)
			{
				shaderAsset->shader = hullShader;
			}
		}
		else if (shaderDesc.shaderType == eShaderType::ePixel)
		{
			Memory::RefPtr<RHIShader> pixelShader = cmdList->CreatePixelShader(shaderDesc);
			if (pixelShader)
			{
				shaderAsset->shader = pixelShader;
			}
		}
		else if (shaderDesc.shaderType == eShaderType::eCompute)
		{
			Memory::RefPtr<RHIShader> computeShader = cmdList->CreateComputeShader(shaderDesc);
			if (computeShader)
			{
				shaderAsset->shader = computeShader;
			}
		}
		else
		{
			return;
		}
	}

	void AssetCommander::onUpdate(Memory::RefPtr<MeshAsset> meshAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!meshAsset || !cmdList)
		{
			return;
		}
	}

	void AssetCommander::onUpdate(Memory::RefPtr<TextureAsset> textureAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!textureAsset || !cmdList)
		{
			return;
		}
	}

	void AssetCommander::onUnload(Memory::RefPtr<MeshAsset> meshAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!meshAsset || !cmdList)
		{
			return;
		}

		for (const auto& [vertexKey, vertexBuffer] : meshAsset->buffers)
		{
			if (!vertexBuffer)
			{
				continue;
			}

			cmdList->RemoveBuffer(vertexBuffer);
		}

		if (meshAsset->index)
		{
			cmdList->RemoveBuffer(meshAsset->index);
		}

		meshAsset->rawBuffers.Clear();
		meshAsset->buffers.Clear();
		meshAsset->materials.Clear();
		meshAsset->index = nullptr;
	}

	void AssetCommander::onUnload(Memory::RefPtr<TextureAsset> textureAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!textureAsset || !cmdList)
		{
			return;
		}

		if (textureAsset->texture)
		{
			cmdList->RemoveTexture(textureAsset->texture);
		}

		textureAsset->texture = nullptr;
	}

	void AssetCommander::onUnload(Memory::RefPtr<ShaderAsset> shaderAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!shaderAsset || !cmdList)
		{
			return;
		}

		if (shaderAsset->shader)
		{
			cmdList->RemoveShader(shaderAsset->shader);
		}

		shaderAsset->shader = nullptr;
	}
}