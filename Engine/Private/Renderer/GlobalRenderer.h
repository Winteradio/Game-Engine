#ifndef __WTR_GLOBALRENDERER_H__
#define __WTR_GLOBALRENDERER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Framework/Math/MathTypes.h>
#include <Renderer/RenderResource.h>

namespace wtr
{
	enum class eResourceSlot : uint8_t;
	enum class eGBufferSlot : uint8_t;

	struct RenderView;

	struct RHITextureDesc;
	struct RHIPipeLineCreateDesc;
	struct RHISamplerCreateDesc;

	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
	class RHIPipeLine;
	class RHISampler;
	class RHICommandList;

	class MaterialProxy;
	class LightProxy;

	template<typename T>
	class ScalarData;
};

namespace wtr
{
	namespace GlobalResource
	{
		class CameraResource : public RenderResource
		{
		private:
			struct alignas(16) CameraData
			{
				fmat4 viewMatrix = fmat4(1.f);
				fmat4 projMatrix = fmat4(1.f);

				fvec3 position = fvec3(0.f);
				fvec3 direction = fvec3(1.f);
			};

		public:
			CameraResource();
			virtual ~CameraResource() = default;

		public:
			eResourceState GetResourceState() const override;

			void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
			void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
			void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

			Memory::RefPtr<const RHIBuffer> GetBuffer() const;

		public:
			void UpdateView(const fmat4 viewMatrix);
			void UpdateProjection(const fmat4 projMatrix);
			void UpdatePosition(const fvec3 position);
			void UpdateDirection(const fvec3 direction);

		private:
			Memory::RefPtr<ScalarData<CameraData>> m_cameraData;
			Memory::RefPtr<RHIBuffer> m_buffer;
		};

		class GBufferResource : public RenderResource
		{
		public :
			GBufferResource(const RHITextureDesc& desc);
			virtual ~GBufferResource() = default;

		public :
			eResourceState GetResourceState() const override;

			void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
			void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
			void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

			Memory::RefPtr<const RHITexture> GetTexture() const;

		public :
			void Resize(const uint32_t width, const uint32_t height, const uint32_t depth = 0);

		private :
			Memory::RefPtr<RHITexture> m_texture;
			Memory::RefPtr<RHITextureDesc> m_prevDesc;
		};

		class QuadResource : public RenderResource
		{
		public :
			QuadResource();
			virtual ~QuadResource() = default;

		public :
			eResourceState GetResourceState() const override;

			void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
			void Unload(Memory::RefPtr<RHICommandList> cmdList) override;

			Memory::RefPtr<const RHIVertexLayout> GetLayout() const;

		private :
			Memory::RefPtr<RHIBuffer> m_position;
			Memory::RefPtr<RHIBuffer> m_uv;
			Memory::RefPtr<RHIBuffer> m_index;

			Memory::RefPtr<RHIVertexLayout> m_layout;
		};

		bool Init(Memory::RefPtr<RHICommandList> cmdLIst);
		void Release(Memory::RefPtr<RHICommandList> cmdList);

		void Update(Memory::RefPtr<RHICommandList> cmdList, const RenderView& renderView);

		Memory::RefPtr<const RHIBuffer> GetCamera();
		Memory::RefPtr<const RHITexture> GetGBuffer(const eGBufferSlot slot);
		Memory::RefPtr<const RHIVertexLayout> GetQuad();
		Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, const RHIPipeLineCreateDesc desc);
		Memory::RefPtr<const RHISampler> GetSampler(Memory::RefPtr<RHICommandList> cmdList, const eResourceSlot slot);
		Memory::RefPtr<const RHISampler> GetSampler(Memory::RefPtr<RHICommandList> cmdList, const RHISamplerCreateDesc desc);
		Memory::RefPtr<const MaterialProxy> GetDefaultMaterial();
		Memory::RefPtr<const LightProxy> GetDefaultLight();
	};

	namespace GlobalShaderSelector
	{
		void SetShader(Memory::RefPtr<MaterialProxy> material);
		void SetShader(Memory::RefPtr<LightProxy> light);
	}
};

#endif // __WTR_GLOBALRENDERER_H__