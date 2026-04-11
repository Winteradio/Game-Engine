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
			if (!rawBuffer || rawBuffer->data.Empty())
			{
				continue;
			}

			const auto& desc = rawBuffer->desc;

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = eBufferType::eVertex;
			bufferDesc.accessType = eDataAccess::eStatic;
			bufferDesc.componentType = desc.componentType;
			bufferDesc.numComponents = desc.numComponents;
			bufferDesc.count = desc.count;
			bufferDesc.size = static_cast<uint32_t>(rawBuffer->data.Size());
			bufferDesc.stride = desc.numComponents * GetDataTypeSize(desc.componentType);
			bufferDesc.data = rawBuffer->data.Data();

			Memory::RefPtr<RHIBuffer> vertexBuffer = cmdList->CreateBuffer(bufferDesc);
			if (vertexBuffer)
			{
				meshAsset->buffers[vertexKey] = vertexBuffer;
			}
		}

		if (meshAsset->rawIndex && !meshAsset->rawIndex->data.Empty())
		{
			const auto& desc = meshAsset->rawIndex->desc;

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = eBufferType::eIndex;
			bufferDesc.accessType = eDataAccess::eStatic;
			bufferDesc.componentType = desc.componentType;
			bufferDesc.numComponents = desc.numComponents;
			bufferDesc.count = desc.count;
			bufferDesc.size = static_cast<uint32_t>(meshAsset->rawIndex->data.Size());
			bufferDesc.stride = desc.numComponents * GetDataTypeSize(desc.componentType);
			bufferDesc.data = meshAsset->rawIndex->data.Data();

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

		// TODO : Load the texture asset
	}

	void AssetCommander::onLoad(Memory::RefPtr<ShaderAsset> shaderAsset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !shaderAsset || !shaderAsset->rawBuffer || shaderAsset->rawBuffer->data.Empty())
		{
			return;
		}

		const eShaderType shaderType = shaderAsset->GetShaderType();
		if (shaderType == eShaderType::eNone)
		{
			return;
		}
		else if (shaderType == eShaderType::eVertex)
		{
			RHIVertexShaderCreateDesc shaderDesc;
			shaderDesc.shaderType = eShaderType::eVertex;
			shaderDesc.data = shaderAsset->rawBuffer->data.Data();

			Memory::RefPtr<RHIVertexShader> vertexShader = cmdList->CreateVertexShader(shaderDesc);
			if (vertexShader)
			{
				shaderAsset->shader = vertexShader;
			}
		}
		else if (shaderType == eShaderType::eGeometry)
		{
			RHIGeometryShaderCreateDesc shaderDesc;
			shaderDesc.shaderType = eShaderType::eGeometry;
			shaderDesc.data = shaderAsset->rawBuffer->data.Data();

			Memory::RefPtr<RHIGeometryShader> geometryShader = cmdList->CreateGeometryShader(shaderDesc);
			if (geometryShader)
			{
				shaderAsset->shader = geometryShader;
			}
		}
		else if (shaderType == eShaderType::eHull)
		{
			RHIHullShaderCreateDesc shaderDesc;
			shaderDesc.shaderType = eShaderType::eHull;
			shaderDesc.data = shaderAsset->rawBuffer->data.Data();

			Memory::RefPtr<RHIHullShader> hullShader = cmdList->CreateHullShader(shaderDesc);
			if (hullShader)
			{
				shaderAsset->shader = hullShader;
			}
		}
		else if (shaderType == eShaderType::ePixel)
		{
			RHIPixelShaderCreateDesc shaderDesc;
			shaderDesc.shaderType = eShaderType::ePixel;
			shaderDesc.data = shaderAsset->rawBuffer->data.Data();

			Memory::RefPtr<RHIPixelShader> pixelShader = cmdList->CreatePixelShader(shaderDesc);
			if (pixelShader)
			{
				shaderAsset->shader = pixelShader;
			}
		}
		else if (shaderType == eShaderType::eCompute)
		{
			RHIComputeShaderCreateDesc shaderDesc;
			shaderDesc.shaderType = eShaderType::eCompute;
			shaderDesc.data = shaderAsset->rawBuffer->data.Data();

			Memory::RefPtr<RHIComputeShader> computeShader = cmdList->CreateComputeShader(shaderDesc);
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