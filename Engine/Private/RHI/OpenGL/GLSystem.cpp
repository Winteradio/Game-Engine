#include <RHI/OpenGL/GLSystem.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>
#include <glad/glad.h>

#include <RHI/RHIStates.h>
#include <RHI/RHIResources.h>
#include <RHI/OpenGL/GLResources.h>

#define __GL_DEBUG__

namespace wtr
{
	GLSystem::GLSystem()
		: m_context()
		, m_colorState()
		, m_depthState()
		, m_stencilState()
		, m_blendState()
		, m_rasterizerState()
	{}

	GLSystem::~GLSystem()
	{}

	bool GLSystem::Init(void* nativeHandle)
	{
		if (!m_context.Init(nativeHandle))
		{
			LOGERROR() << "[GL] Failed to initialize the OpenGL, cause failed to initialize the WGL";
			return false;
		}

#ifdef __GL_DEBUG__
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(
			[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				LOGERROR() << "[GL] Error : " << message;
			},
			nullptr
		);

#endif

		return true;
	}

	void GLSystem::Clear(const RHIClearState& state)
	{
		GLbitfield clearMask = 0;

		if (eClearBuffer::eNone != (state.clearBuffer & eClearBuffer::eColor))
		{
			glClearColor(state.color.x, state.color.y, state.color.z, state.color.w);
			clearMask |= GL_COLOR_BUFFER_BIT;
		}

		if (eClearBuffer::eNone != (state.clearBuffer & eClearBuffer::eDepth))
		{
			glClearDepth(state.depth);
			clearMask |= GL_DEPTH_BUFFER_BIT;
		}

		if (eClearBuffer::eNone != (state.clearBuffer & eClearBuffer::eStencil))
		{
			glClearStencil(state.stencil);
			clearMask |= GL_STENCIL_BUFFER_BIT;
		}

		glClear(clearMask);
	}

	void GLSystem::Flush()
	{
		glFlush();
	}

	void GLSystem::MakeCurrent()
	{
		m_context.MakeCurrent();
	}

	void GLSystem::ReleaseCurrent()
	{
		if (m_context.GetCurrent())
		{
			m_context.ReleaseCurrent();
		}
	}

	void GLSystem::Present()
	{
		m_context.Present();
	}

	void GLSystem::Resize(const uint32_t width, const uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void GLSystem::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
		glViewport(posX, posY, width, height);
	}

	void GLSystem::SetColorState(const RHIColorState& state)
	{
		if (m_colorState.red != state.red || m_colorState.green != state.green ||
			m_colorState.blue != state.blue || m_colorState.alpha != state.alpha)
		{
			GLboolean red = state.red ? GL_TRUE : GL_FALSE;
			GLboolean green = state.green ? GL_TRUE : GL_FALSE;
			GLboolean blue = state.blue ? GL_TRUE : GL_FALSE;
			GLboolean alpha = state.alpha ? GL_TRUE : GL_FALSE;

			glColorMask(red, green, blue, alpha);

			m_colorState = state;
		}
	}

	void GLSystem::SetDepthState(const RHIDepthState& state)
	{
		if (m_depthState.enable != state.enable)
		{
			if (state.enable)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		if (m_depthState.write != state.write)
		{
			GLboolean writeMask = state.write ? GL_TRUE : GL_FALSE;
			glDepthMask(writeMask);
		}

		if (m_depthState.func != state.func)
		{
			uint32_t func = GetCompareFunc(state.func);

			glDepthFunc(func);
		}

		m_depthState = state;
	}

	void GLSystem::SetStencilState(const RHIStencilState& state)
	{
		if (m_stencilState.enable != state.enable)
		{
			if (state.enable)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}
		}

		if (m_stencilState.refMask != state.refMask ||
			m_stencilState.readMask != state.readMask || 
			m_stencilState.func != state.func)
		{
			uint32_t func = GetCompareFunc(state.func);
			glStencilFunc(func, state.refMask, state.readMask);
		}

		if (m_stencilState.writeMask != state.writeMask)
		{
			glStencilMask(state.writeMask);
		}

		if (m_stencilState.failOp != state.failOp ||
			m_stencilState.depthFailOp != state.depthFailOp ||
			m_stencilState.passOp != state.passOp)
		{
			uint32_t failOp = GetStencilOp(state.failOp);
			uint32_t depthFailOp = GetStencilOp(state.depthFailOp);
			uint32_t passOp = GetStencilOp(state.passOp);

			glStencilOp(failOp, depthFailOp, passOp);
		}

		m_stencilState = state;
	}

	void GLSystem::SetBlendState(const RHIBlendState& state)
	{
		if (m_blendState.enable != state.enable)
		{
			if (state.enable)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}

		if (m_blendState.srcColor != state.srcColor ||
			m_blendState.destColor != state.destColor ||
			m_blendState.srcAlpha != state.srcAlpha ||
			m_blendState.destAlpha != state.destAlpha)
		{
			uint32_t srcColor = GetBlendFunc(state.srcColor);
			uint32_t destColor = GetBlendFunc(state.destColor);
			uint32_t srcAlpha = GetBlendFunc(state.srcAlpha);
			uint32_t destAlpha = GetBlendFunc(state.destAlpha);

			glBlendFuncSeparate(srcColor, destColor, srcAlpha, destAlpha);
		}

		if (m_blendState.colorOp != state.colorOp || 
			m_blendState.alphaOp != state.alphaOp
		)
		{
			uint32_t colorOp = GetBlendOp(state.colorOp);
			uint32_t alphaOp = GetBlendOp(state.alphaOp);

			glBlendEquationSeparate(colorOp,  alphaOp);
		}

		if (m_blendState.writeMask != state.writeMask)
		{
			glColorMask(
				(state.writeMask & 0x1) ? GL_TRUE : GL_FALSE,
				(state.writeMask & 0x2) ? GL_TRUE : GL_FALSE,
				(state.writeMask & 0x4) ? GL_TRUE : GL_FALSE,
				(state.writeMask & 0x8) ? GL_TRUE : GL_FALSE
			);
		}

		m_blendState = state;
	}

	void GLSystem::SetRasterizerState(const RHIRasterizerState& state)
	{
		if (m_rasterizerState.cullEnable != state.cullEnable)
		{
			if (state.cullEnable)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}

		if (m_rasterizerState.cullFace != state.cullFace)
		{
			uint32_t cullFace = GetCullFace(state.cullFace);

			glCullFace(cullFace);
		}

		if (m_rasterizerState.frontFace != state.frontFace)
		{
			uint32_t frontFace = GetFrontFace(state.frontFace);

			glFrontFace(frontFace);
		}

		if (m_rasterizerState.fillMode != state.fillMode)
		{
			uint32_t fillMode = GetPrimitiveMode(state.fillMode);

			eCullFace viewFace = eCullFace::eNone;
			if (eCullFace::eBack == state.cullFace)
			{
				viewFace = eCullFace::eFront;
			}
			else if (eCullFace::eFront == state.cullFace)
			{
				viewFace = eCullFace::eBack;
			}
			else if (eCullFace::eFront_Back == state.cullFace)
			{
				viewFace = eCullFace::eNone;
			}
			else
			{
				viewFace = eCullFace::eFront_Back;
			}

			uint32_t face = GetCullFace(viewFace);
			glPolygonMode(face, fillMode);
		}

		m_rasterizerState = state;
	}

	Memory::RefPtr<RHIBuffer> GLSystem::CreateBuffer(const RHIBufferDesc desc)
	{
		Memory::RefPtr<RHIBuffer> refBuffer = Memory::MakeRef<GLBuffer>(desc);

		return refBuffer;
	}

	Memory::RefPtr<RHITexture> GLSystem::CreateTexture(const RHITextureDesc desc)
	{
		Memory::RefPtr<RHITexture> refTexture = Memory::MakeRef<GLTexture>(desc);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> GLSystem::CreateSampler(const RHISamplerDesc desc)
	{
		Memory::RefPtr<RHISampler> refSampler = Memory::MakeRef<RHISampler>(desc);

		return refSampler;
	}

	Memory::RefPtr<RHIVertexShader> GLSystem::CreateVertexShader(const RHIVertexShaderDesc desc)
	{
		Memory::RefPtr<RHIVertexShader> refVertexShader = Memory::MakeRef<GLVertexShader>(desc);

		return refVertexShader;
	}

	Memory::RefPtr<RHIGeometryShader> GLSystem::CreateGeometryShader(const RHIGeometryShaderDesc desc)
	{
		Memory::RefPtr<RHIGeometryShader> refGeometryShader = Memory::MakeRef<GLGeometryShader>(desc);

		return refGeometryShader;
	}

	Memory::RefPtr<RHIHullShader> GLSystem::CreateHullShader(const RHIHullShaderDesc desc)
	{
		Memory::RefPtr<RHIHullShader> refHullShader = Memory::MakeRef<GLHullShader>(desc);
	
		return refHullShader;
	}

	Memory::RefPtr<RHIPixelShader> GLSystem::CreatePixelShader(const RHIPixelShaderDesc desc)
	{
		Memory::RefPtr<RHIPixelShader> refPixelShader = Memory::MakeRef<GLPixelShader>(desc);

		return refPixelShader;
	}

	Memory::RefPtr<RHIComputeShader> GLSystem::CreateComputeShader(const RHIComputeShaderDesc desc)
	{
		Memory::RefPtr<RHIComputeShader> refComputeShader = Memory::MakeRef<GLComputeShader>(desc);

		return refComputeShader;
	}

	Memory::RefPtr<RHIPipeLine> GLSystem::CreatePipeLine(const RHIPipeLineDesc desc)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = Memory::MakeRef<GLPipeLine>(desc);

		return refPipeLine;
	}

	void GLSystem::InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!info.rawBuffer || info.rawBuffer->data.Empty() || !buffer)
		{
			return;
		}

		GLBuffer* glBuffer = reinterpret_cast<GLBuffer*>(buffer->GetRawBuffer());
		if (!glBuffer)
		{
			return;
		}

		const uint32_t bufferType = GetBufferType(info.bufferType);
		const uint32_t accessType = GetDataAccess(info.accessType);
		const uint32_t dataType = GetDataType(info.rawBuffer->componentType);
		const uint32_t dataSize = info.rawBuffer->data.Size();
		const uint32_t stride = info.stride;
		const uint32_t componentCount = info.rawBuffer->numComponents;

		uint32_t bufferID = GL_NONE;
		glGenBuffers(1, &bufferID);

		if (bufferID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to generate buffer";
			return;
		}

		glBindBuffer(bufferType, bufferID);
		glBufferSubData(bufferType, 0, dataSize, info.rawBuffer->data.Data());
		glBindBuffer(bufferType, GL_NONE);

		glBuffer->SetID(bufferID);
		glBuffer->SetState(eResourceState::eReady);
	}

	void GLSystem::InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	void GLSystem::InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler)
	{}

	void GLSystem::InitializeVertexShader(const RHIVertexShaderCreateDesc info, Memory::RefPtr<RHIVertexShader> shader)
	{}

	void GLSystem::InitializeGeometryShader(const RHIGeometryShaderCreateDesc info, Memory::RefPtr<RHIGeometryShader> shader)
	{}

	void GLSystem::InitializeHullShader(const RHIHullShaderCreateDesc info, Memory::RefPtr<RHIHullShader> shader)
	{}

	void GLSystem::InitializePixelShader(const RHIPixelShaderCreateDesc info, Memory::RefPtr<RHIPixelShader> shader)
	{}

	void GLSystem::InitializeComputeShader(const RHIComputeShaderCreateDesc info, Memory::RefPtr<RHIComputeShader> shader)
	{}

	void GLSystem::InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline)
	{}

	void GLSystem::UpdateBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{}

	void GLSystem::UpdateTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{}

	void GLSystem::RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer)
		{
			return;
		}

		GLBuffer* glBuffer = reinterpret_cast<GLBuffer*>(buffer->GetRawBuffer());
		if (!glBuffer)
		{
			return;
		}

		const uint32_t bufferID = glBuffer->GetID();
		glDeleteBuffers(1, &bufferID);

		glBuffer->SetID(GL_NONE);
		glBuffer->SetState(eResourceState::eNone);
	}

	void GLSystem::RemoveTexture(Memory::RefPtr<RHITexture> texture)
	{}

	void GLSystem::RemoveSampler(Memory::RefPtr<RHISampler> sampler)
	{}

	void GLSystem::RemoveShader(Memory::RefPtr<RHIShader> shader)
	{}

	void GLSystem::RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
	{}

	void GLSystem::DrawIndexPrimitive()
	{}

	const uint32_t GLSystem::GetBufferType(const eBufferType buffer) const
	{
		if (eBufferType::eVertex == buffer)
		{
			return GL_ARRAY_BUFFER;
		}
		else if (eBufferType::eIndex == buffer)
		{
			return GL_ELEMENT_ARRAY_BUFFER;
		}
		else if (eBufferType::eConst == buffer)
		{
			return GL_UNIFORM_BUFFER;
		}
		else if (eBufferType::eStorage == buffer)
		{
			return GL_SHADER_STORAGE_BUFFER;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetDataAccess(const eDataAccess access) const
	{
		if (eDataAccess::eStatic == access)
		{
			return GL_STATIC_DRAW;
		}
		else if (eDataAccess::eDynamic == access)
		{
			return GL_DYNAMIC_DRAW;
		}
		else if (eDataAccess::eStream == access)
		{
			return GL_STREAM_DRAW;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetDataType(const eDataType data) const
	{
		if (eDataType::eByte == data)
		{
			return GL_BYTE;
		}
		else if (eDataType::eUByte == data)
		{
			return GL_UNSIGNED_BYTE;
		}
		else if (eDataType::eShort == data)
		{
			return GL_SHORT;
		}
		else if (eDataType::eUShort == data)
		{
			return GL_UNSIGNED_SHORT;
		}
		else if (eDataType::eInt == data)
		{
			return GL_INT;
		}
		else if (eDataType::eUInt == data)
		{
			return GL_UNSIGNED_INT;
		}
		else if (eDataType::eFloat == data)
		{
			return GL_FLOAT;
		}
		else if (eDataType::eDouble == data)
		{
			return GL_DOUBLE;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetPixelFormat(const ePixelFormat pixel) const
	{
		if (ePixelFormat::eNone == pixel)
		{
			return GL_NONE;
		}
		else if (ePixelFormat::eR8 == pixel)
		{
			return GL_R8;
		}
		else if (ePixelFormat::eR8G8 == pixel)
		{
			return GL_RG8;
		}
		else if (ePixelFormat::eR8G8B8 == pixel)
		{
			return GL_RGB8;
		}
		else if (ePixelFormat::eR8G8B8A8 == pixel)
		{
			return GL_RGBA8;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetFilterMode(const eFilterMode filter) const
	{
		if (eFilterMode::eNearest == filter)
		{
			return GL_NEAREST;
		}
		else if (eFilterMode::eLinear == filter)
		{
			return GL_LINEAR;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetWrapMode(const eWrapMode wrap) const
	{
		if (eWrapMode::eRepeat == wrap)
		{
			return GL_REPEAT;
		}
		else if (eWrapMode::eMirroedRepeat == wrap)
		{
			return GL_MIRRORED_REPEAT;
		}
		else if (eWrapMode::eClampToEdge == wrap)
		{
			return GL_CLAMP_TO_EDGE;
		}
		else if (eWrapMode::eClampToBorder == wrap)
		{
			return GL_CLAMP_TO_BORDER;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetCullFace(const eCullFace mode) const
	{
		if (eCullFace::eFront == mode)
		{
			return GL_FRONT;
		}
		else if (eCullFace::eBack == mode)
		{
			return GL_BACK;
		}
		else if (eCullFace::eFront_Back == mode)
		{
			return GL_FRONT_AND_BACK;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetFrontFace(const eFrontFace face) const
	{
		if (eFrontFace::eCCW == face)
		{
			return GL_CCW;
		}
		else if (eFrontFace::eCW == face)
		{
			return GL_CW;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetDrawMode(const eDrawMode mode) const
	{
		if (eDrawMode::ePoints == mode)
		{
			return GL_POINTS;
		}
		else if (eDrawMode::eLines == mode)
		{
			return GL_LINES;
		}
		else if (eDrawMode::eLine_Loop == mode)
		{
			return GL_LINE_LOOP;
		}
		else if (eDrawMode::eLine_Strip == mode)
		{
			return GL_LINE_STRIP;
		}
		else if (eDrawMode::eTriangles == mode)
		{
			return GL_TRIANGLES;
		}
		else if (eDrawMode::eTriangle_Strip == mode)
		{
			return GL_TRIANGLE_STRIP;
		}
		else if (eDrawMode::eTriangle_Fan == mode)
		{
			return GL_TRIANGLE_FAN;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetPrimitiveMode(const ePrimitiveMode mode) const
	{
		if (ePrimitiveMode::ePoint == mode)
		{
			return GL_POINT;
		}
		else if (ePrimitiveMode::eLine == mode)
		{
			return GL_LINE;
		}
		else if (ePrimitiveMode::eFill == mode)
		{
			return GL_FILL;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetCompareFunc(const eCompareFunc func) const
	{
		if (eCompareFunc::eNever == func)
		{
			return GL_NEVER;
		}
		else if (eCompareFunc::eEqual == func)
		{
			return GL_EQUAL;
		}
		else if (eCompareFunc::eNotEqual == func)
		{
			return GL_NOTEQUAL;
		}
		else if (eCompareFunc::eLess == func)
		{
			return GL_LESS;
		}
		else if (eCompareFunc::eLessEqual == func)
		{
			return GL_LEQUAL;
		}
		else if (eCompareFunc::eGreater == func)
		{
			return GL_GREATER;
		}
		else if (eCompareFunc::eGreatorEqual == func)
		{
			return GL_GEQUAL;
		}
		else if (eCompareFunc::eAlways == func)
		{
			return GL_ALWAYS;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetStencilOp(const eStencilOp op) const
	{
		if (eStencilOp::eKeep == op)
		{
			return GL_KEEP;
		}
		else if (eStencilOp::eZero == op)
		{
			return GL_ZERO;
		}
		else if (eStencilOp::eReplace == op)
		{
			return GL_REPLACE;
		}
		else if (eStencilOp::eInc_Clamp == op)
		{
			return GL_INCR;
		}
		else if (eStencilOp::eDec_Clamp == op)
		{
			return GL_DECR;
		}
		else if (eStencilOp::eInvert == op)
		{
			return GL_INVERT;
		}
		else if (eStencilOp::eInc_Wrap == op)
		{
			return GL_INCR_WRAP;
		}
		else if (eStencilOp::eDec_Wrap == op)
		{
			return GL_DECR_WRAP;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetBlendFunc(const eBlendFunc func) const
	{
		if (eBlendFunc::eZero == func)
		{
			return GL_ZERO;
		}
		else if (eBlendFunc::eOne == func)
		{
			return GL_ONE;
		}
		else if (eBlendFunc::eSrc_Color == func)
		{
			return GL_SRC_COLOR;
		}
		else if (eBlendFunc::eOne_Minus_Src_Color == func)
		{
			return GL_ONE_MINUS_SRC_COLOR;
		}
		else if (eBlendFunc::eDst_Color == func)
		{
			return GL_DST_COLOR;
		}
		else if (eBlendFunc::eOne_Minus_Dst_Color == func)
		{
			return GL_ONE_MINUS_DST_COLOR;
		}
		else if (eBlendFunc::eSrc_Alpha == func)
		{
			return GL_SRC_ALPHA;
		}
		else if (eBlendFunc::eOne_Minus_Src_Alpha == func)
		{
			return GL_ONE_MINUS_SRC_ALPHA;
		}
		else if (eBlendFunc::eDst_Alpha == func)
		{
			return GL_DST_ALPHA;
		}
		else if (eBlendFunc::eOne_Minus_Dst_Alpha == func)
		{
			return GL_ONE_MINUS_DST_ALPHA;
		}
		else
		{
			return GL_NONE;
		}
	}

	const uint32_t GLSystem::GetBlendOp(const eBlendOp op) const
	{
		if (eBlendOp::eAdd == op)
		{
			return GL_FUNC_ADD;
		}
		else if (eBlendOp::eSub == op)
		{
			return GL_FUNC_SUBTRACT;
		}
		else if (eBlendOp::eReverse_Sub == op)
		{
			return GL_FUNC_REVERSE_SUBTRACT;
		}
		else if (eBlendOp::eMin == op)
		{
			return GL_MIN;
		}
		else if (eBlendOp::eMax == op)
		{
			return GL_MAX;
		}
		else
		{
			return GL_NONE;
		}
	}
}