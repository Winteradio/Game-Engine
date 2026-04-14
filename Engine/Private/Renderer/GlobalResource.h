#ifndef __WTR_GLOBALRESOURCE_H__
#define __WTR_GLOBALRESOURCE_H__

#include <Framework/Math/MathTypes.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	struct RenderView;

	class RHIBuffer;
	class RHITexture;
	class RHIVertexLayout;
	class RHICommandList;
};

namespace wtr
{
	struct alignas(16) CameraData
	{
		fmat4 viewMatrix;
		fmat4 projectionMatrix;
		
		fvec3 cameraPosition;
	};

	struct BufferResource
	{
		Memory::RefPtr<RHIBuffer> cameraBuffer;
	};

	struct TextureResource
	{
		Memory::RefPtr<RHITexture> position;
		Memory::RefPtr<RHITexture> normal;
		Memory::RefPtr<RHITexture> albedo;
		Memory::RefPtr<RHITexture> depth;
		Memory::RefPtr<RHITexture> stencil;
		
		// TODO : FrameBuffer
	};

	struct ScreenQuadResource
	{
		Memory::RefPtr<RHIBuffer> positionBuffer;
		Memory::RefPtr<RHIBuffer> uvBuffer;
		Memory::RefPtr<RHIBuffer> indexBuffer;
		Memory::RefPtr<RHIVertexLayout> vertexLayout;
	};

	class GlobalResource
	{
	public :
		GlobalResource();
		~GlobalResource() = default;

	public :
		bool Init(Memory::RefPtr<RHICommandList> cmdLIst);
		void Release(Memory::RefPtr<RHICommandList> cmdList);

		void UpdateCamera(const RenderView& renderView, Memory::RefPtr<RHICommandList> cmdList);

		const Memory::RefPtr<RHIBuffer> GetCameraBuffer() const;
		const Memory::RefPtr<RHIVertexLayout> GetScreenQuad() const;

		// TODO : Update Light

	public :
		bool InitBuffer(Memory::RefPtr<RHICommandList> cmdList);
		bool InitTexture(Memory::RefPtr<RHICommandList> cmdList);
		bool InitScreenQuad(Memory::RefPtr<RHICommandList> cmdList);

		void ReleaseBuffer(Memory::RefPtr<RHICommandList> cmdList);
		void ReleaseTexture(Memory::RefPtr<RHICommandList> cmdList);
		void ReleaseScreenQuad(Memory::RefPtr<RHICommandList> cmdList);

	private :
		BufferResource m_bufferResource;
		TextureResource m_textureResource;
		ScreenQuadResource m_screenQuadResource;
	};
}

#endif // __WTR_GLOBALRESOURCE_H__