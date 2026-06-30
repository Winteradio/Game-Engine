#include <RHI/Vulkan/VkSystem.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

#include <RHI/RHIStates.h>
#include <RHI/RHIResources.h>
#include <RHI/Vulkan/VkContext.h>
#include <RHI/Vulkan/VkResources.h>

#include <cmath>

namespace wtr
{
	VkSystem::VkSystem()
		: m_context()
	{
	}

	VkSystem::~VkSystem()
	{
	}

	bool VkSystem::Init(void* nativeHandle)
	{
		if (!m_context)
		{
			m_context = Memory::MakeRef<VkContext>();
		}

		if (!m_context)
		{
			LOGERROR() << "[Vk] Failed to create the vkContext instance";
			return false;
		}

		if (!m_context->Init(nativeHandle))
		{
			LOGERROR() << "[VK] Failed to initialize the vulkan system, cause to failed to initialize the vulkan context";
			return false;
		}

		return true;
	}

	void VkSystem::FlushPending()
	{
	}

	void VkSystem::InitializeState()
	{
	}

	void VkSystem::Clear(const RHIClearState& state)
	{
	}

	void VkSystem::Flush()
	{
	}

	void VkSystem::MakeCurrent()
	{
	}

	void VkSystem::ReleaseCurrent()
	{
	}

	void VkSystem::Present()
	{
	}

	void VkSystem::Resize(const uint32_t width, const uint32_t height)
	{
	}

	void VkSystem::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
	}

	void VkSystem::SetColorState(const RHIColorState& state)
	{
	}

	void VkSystem::SetDepthState(const RHIDepthState& state)
	{
	}

	void VkSystem::SetStencilState(const RHIStencilState& state)
	{
	}

	void VkSystem::SetBlendState(const RHIBlendState& state)
	{
	}

	void VkSystem::SetRasterizerState(const RHIRasterizerState& state)
	{
	}

	Memory::RefPtr<RHIBuffer> VkSystem::CreateBuffer(const RHIBufferDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIVertexLayout> VkSystem::CreateVertexLayout(const RHIVertexLayoutDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHITexture> VkSystem::CreateTexture(const RHITextureDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHISampler> VkSystem::CreateSampler(const RHISamplerDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIShader> VkSystem::CreateVertexShader(const RHIShaderDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIShader> VkSystem::CreateGeometryShader(const RHIShaderDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIShader> VkSystem::CreateHullShader(const RHIShaderDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIShader> VkSystem::CreatePixelShader(const RHIShaderDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIShader> VkSystem::CreateComputeShader(const RHIShaderDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIPipeLine> VkSystem::CreatePipeLine(const RHIPipeLineDesc desc)
	{
		return {};
	}

	Memory::RefPtr<RHIRenderTarget> VkSystem::CreateRenderTarget(const RHIRenderTargetDesc desc)
	{
		return {};
	}

	void VkSystem::InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
	}

	void VkSystem::InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
	}

	void VkSystem::InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	void VkSystem::InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler)
	{
	}

	void VkSystem::InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader)
	{
	}

	void VkSystem::InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline)
	{
	}

	void VkSystem::InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target)
	{
	}

	bool VkSystem::InitializeFrameBuffer(Memory::RefPtr<const RHITexture> texture, const eAttachment attach, const uint32_t slot)
	{
		return {};
	}

	bool VkSystem::InitializeAttribute(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		return {};
	}

	bool VkSystem::InitializeSlot(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		return {};
	}

	bool VkSystem::InitializeTexture1D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::InitializeTexture2D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::InitializeTexture3D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::UpdateTexture1D(const RHITextureUpdateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::UpdateTexture2D(const RHITextureUpdateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::UpdateTexture3D(const RHITextureUpdateDesc info, const uint32_t textureID)
	{
		return {};
	}

	void VkSystem::UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
	}

	void VkSystem::UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	void VkSystem::UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
	}

	void VkSystem::ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
	}

	void VkSystem::ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	bool VkSystem::ResizeTexture1D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::ResizeTexture2D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		return {};
	}

	bool VkSystem::ResizeTexture3D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		return {};
	}

	void VkSystem::SetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const std::string& slotName)
	{
	}

	void VkSystem::SetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
	{
	}

	void VkSystem::SetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
	{
	}

	void VkSystem::SetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
	{
	}

	void VkSystem::UnsetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const std::string& slotName)
	{
	}

	void VkSystem::UnsetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
	{
	}

	void VkSystem::UnsetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
	{
	}

	void VkSystem::UnsetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
	{
	}

	void VkSystem::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
	}

	void VkSystem::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
	}

	void VkSystem::SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target)
	{
	}

	void VkSystem::ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state)
	{
	}

	void VkSystem::UnsetVertexLayout()
	{
	}

	void VkSystem::UnsetPipeLine()
	{
	}

	void VkSystem::UnsetRenderTarget()
	{
	}

	void VkSystem::DispatchCompute(const RHIDispatchDesc info)
	{
	}

	void VkSystem::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
	}

	uint32_t VkSystem::GetBufferType(const eBufferType buffer) const
	{
		return {};
	}

	uint32_t VkSystem::GetDataAccess(const eDataAccess access) const
	{
		return {};
	}

	uint32_t VkSystem::GetDataType(const eDataType data) const
	{
		return {};
	}

	uint32_t VkSystem::GetMapAccess(const eMapAccess access) const
	{
		return {};
	}

	uint32_t VkSystem::GetTextureType(const eTextureType texture) const
	{
		return {};
	}

	uint32_t VkSystem::GetTextureDimension(const eTextureType texture) const
	{
		return {};
	}

	uint32_t VkSystem::GetInternalFormat(const ePixelFormat pixel) const
	{
		return {};
	}

	uint32_t VkSystem::GetBaseFormat(const ePixelFormat pixel) const
	{
		return {};
	}

	uint32_t VkSystem::GetPixelDataType(const ePixelFormat pixel) const
	{
		return {};
	}

	uint32_t VkSystem::GetFilterMode(const eFilterMode filter) const
	{
		return {};
	}

	uint32_t VkSystem::GetWrapMode(const eWrapMode wrap) const
	{
		return {};
	}

	uint32_t VkSystem::GetCullFace(const eCullFace face) const
	{
		return {};
	}

	uint32_t VkSystem::GetFrontFace(const eFrontFace face) const
	{
		return {};
	}

	uint32_t VkSystem::GetDrawMode(const eDrawMode mode) const
	{
		return {};
	}

	uint32_t VkSystem::GetPrimitiveMode(const ePrimitiveMode mode) const
	{
		return {};
	}

	uint32_t VkSystem::GetCompareFunc(const eCompareFunc func) const
	{
		return {};
	}

	uint32_t VkSystem::GetStencilOp(const eStencilOp op) const
	{
		return {};
	}

	uint32_t VkSystem::GetBlendFunc(const eBlendFunc func) const
	{
		return {};
	}

	uint32_t VkSystem::GetBlendOp(const eBlendOp op) const
	{
		return {};
	}

	uint32_t VkSystem::GetShaderType(const eShaderType type) const
	{
		return {};
	}

	uint32_t VkSystem::GetColorAttachment(const uint32_t slot) const
	{
		return {};
	}

	uint32_t VkSystem::GetDepthStencilAttachment(const eAttachment attach) const
	{
		return {};
	}
};
