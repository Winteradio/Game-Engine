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
		float nearPlane;
		float farPlane;
		float fov;
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
		Memory::RefPtr<RHIBuffer> vertexBuffer;
		Memory::RefPtr<RHIBuffer> indexBuffer;
		Memory::RefPtr<RHIVertexLayout> vertexLayout;
	};

	class GlobalResource
	{
	public :
		GlobalResource();
		~GlobalResource() = default;

	public :
		bool IsReady() const;

		bool Setup(Memory::RefPtr<RHICommandList> cmdLIst);
		void Release(Memory::RefPtr<RHICommandList> cmdList);

		void UpdateCamera(const RenderView& renderView, Memory::RefPtr<RHICommandList> cmdList);

		const Memory::RefPtr<RHIBuffer> GetCameraBuffer() const;
		
		// TODO : Update Light

	private :
		BufferResource m_bufferResource;
		TextureResource m_textureResource;
		ScreenQuadResource m_screenQuadResource;

		bool m_ready;
	};
}

#endif // __WTR_GLOBALRESOURCE_H__