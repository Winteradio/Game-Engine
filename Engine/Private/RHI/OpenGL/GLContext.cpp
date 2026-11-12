#include <RHI/OpenGL/GLContext.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>
#include <glad/glad.h>

#include <RHI/RHIStates.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIUtils.h>
#include <RHI/OpenGL/GLResources.h>

#include <algorithm>
#include <cmath>

#define __GL_DEBUG__

namespace wtr
{
	namespace utils
	{
		const GLenum TEXTURE_CUBEMAP_FACES[] =
		{
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		constexpr uint32_t CUBEMAP_FACE_COUNT = 6;
		constexpr uint32_t MAX_COLOR_ATTACHMENT = 8;

		constexpr size_t MAX_MESSAGE_LENGTH = 512;
		char MESSAGE[MAX_MESSAGE_LENGTH];
	};

	GLContext::GLContext()
		: m_context()
	{}

	GLContext::~GLContext()
	{
	}

	bool GLContext::Init(void* nativeHandle)
	{
		if (!m_context.Init(nativeHandle))
		{
			LOGERROR() << "[GL] Failed to initialize the OpenGL, cause failed to initialize the WGL";
			return false;
		}

		m_context.MakeCurrent();
#ifdef __GL_DEBUG__

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(
			[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				if (severity == GL_DEBUG_SEVERITY_HIGH) LOGERROR() << "[GL] Error : " << message;
				else if (severity == GL_DEBUG_SEVERITY_MEDIUM) LOGWARN() << "[GL] Warn : " << message;
				//else LOGINFO() << "[GL] Info : " << message;
			},
			nullptr
		);
#endif

		InitializeState();

		m_context.ReleaseCurrent();

		return true;
	}

	void GLContext::BeginContext()
	{
		m_context.MakeCurrent();
	}

	void GLContext::EndContext()
	{
		glFinish();
		m_context.ReleaseCurrent();
	}

	void GLContext::BeginFrame()
	{
		glFinish();
	}

	void GLContext::EndFrame()
	{
		m_context.Present();
	}

	void GLContext::Resize(const uint32_t width, const uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void GLContext::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
		glViewport(posX, posY, width, height);
	}

	Memory::RefPtr<RHIVertexLayout> GLContext::CreateVertexLayout(const RHIVertexLayoutDesc& desc)
	{
		Memory::RefPtr<RHIVertexLayout> refVertexLayout = Memory::MakeRef<GLVertexLayout>(desc);

		return refVertexLayout;
	}

	Memory::RefPtr<RHIBuffer> GLContext::CreateBuffer(const RHIBufferDesc& desc)
	{
		Memory::RefPtr<RHIBuffer> refBuffer = Memory::MakeRef<GLBuffer>(desc);

		return refBuffer;
	}

	Memory::RefPtr<RHITexture> GLContext::CreateTexture(const RHITextureDesc& desc)
	{
		Memory::RefPtr<RHITexture> refTexture = Memory::MakeRef<GLTexture>(desc);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> GLContext::CreateSampler(const RHISamplerDesc& desc)
	{
		Memory::RefPtr<RHISampler> refSampler = Memory::MakeRef<GLSampler>(desc);

		return refSampler;
	}

	Memory::RefPtr<RHIShader> GLContext::CreateVertexShader(const RHIShaderDesc& desc)
	{
		Memory::RefPtr<RHIShader> refVertexShader = Memory::MakeRef<GLVertexShader>(desc);

		return refVertexShader;
	}

	Memory::RefPtr<RHIShader> GLContext::CreateGeometryShader(const RHIShaderDesc& desc)
	{
		Memory::RefPtr<RHIShader> refGeometryShader = Memory::MakeRef<GLGeometryShader>(desc);

		return refGeometryShader;
	}

	Memory::RefPtr<RHIShader> GLContext::CreateHullShader(const RHIShaderDesc& desc)
	{
		Memory::RefPtr<RHIShader> refHullShader = Memory::MakeRef<GLHullShader>(desc);

		return refHullShader;
	}

	Memory::RefPtr<RHIShader> GLContext::CreateDomainShader(const RHIShaderDesc& desc)
	{
		Memory::RefPtr<RHIShader> refDomainShader = Memory::MakeRef<GLDomainShader>(desc);

		return refDomainShader;
	}

	Memory::RefPtr<RHIShader> GLContext::CreatePixelShader(const RHIShaderDesc& desc)
	{
		Memory::RefPtr<RHIShader> refPixelShader = Memory::MakeRef<GLPixelShader>(desc);

		return refPixelShader;
	}

	Memory::RefPtr<RHIShader> GLContext::CreateComputeShader(const RHIShaderDesc& desc)
	{
		Memory::RefPtr<RHIShader> refComputeShader = Memory::MakeRef<GLComputeShader>(desc);

		return refComputeShader;
	}

	Memory::RefPtr<RHIPipeLine> GLContext::CreatePipeLine(const RHIPipeLineDesc& desc)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = Memory::MakeRef<GLPipeLine>(desc);

		return refPipeLine;
	}

	Memory::RefPtr<RHIRenderPass> GLContext::CreateRenderPass(const RHIRenderPassDesc& desc)
	{
		Memory::RefPtr<RHIRenderPass> refRenderPass = Memory::MakeRef<GLRenderPass>(desc);

		return refRenderPass;
	}

	Memory::RefPtr<RHIBufferView> GLContext::CreateBufferView(Memory::RefPtr<RHIBuffer> buffer)
	{
		Memory::RefPtr<RHIBufferView> refBufferView = Memory::MakeRef<GLBufferView>(buffer);

		return refBufferView;
	}

	Memory::RefPtr<RHITextureView> GLContext::CreateTextureView(Memory::RefPtr<RHITexture> texture)
	{
		Memory::RefPtr<RHITextureView> refTextureView = Memory::MakeRef<GLTextureView>(texture);

		return refTextureView;
	}

	bool GLContext::ReleaseVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (!layout)
		{
			LOGWARN() << "[GL] Failed to release the vertex layout, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLVertexLayout> refLayout = Memory::Cast<GLVertexLayout>(layout);
		if (!refLayout)
		{
			LOGWARN() << "[GL] Failed to release the vertex layout, the instance is not implementation of the gl vertex layout";
			return false;
		}

		const uint32_t layoutID = refLayout->GetID();
		if (layoutID == GL_NONE)
		{
			LOGINFO() << "[GL] The vertex layout is already released";
			return true;
		}

		glDeleteVertexArrays(1, &layoutID);

		refLayout->SetState(eResourceState::eNone);
		refLayout->SetID(GL_NONE);

		return true;
	}

	bool GLContext::ReleaseBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer)
		{
			LOGWARN() << "[GL] Failed to release the buffer, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLBuffer> refBuffer = Memory::Cast<GLBuffer>(buffer);
		if (!refBuffer)
		{
			LOGWARN() << "[GL] Failed to release the buffer, the instance is not implementation of the gl buffer";
			return false;
		}

		const uint32_t bufferID = refBuffer->GetID();
		if (bufferID == GL_NONE)
		{
			LOGINFO() << "[GL] The buffer is already released";
			return true;
		}

		glDeleteBuffers(1, &bufferID);

		refBuffer->SetState(eResourceState::eNone);
		refBuffer->SetID(GL_NONE);

		return true;
	}

	bool GLContext::ReleaseTexture(Memory::RefPtr<RHITexture> texture)
	{
		if (!texture)
		{
			LOGWARN() << "[GL] Failed to release the texture, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLTexture> refTexture = Memory::Cast<GLTexture>(texture);
		if (!refTexture)
		{
			LOGWARN() << "[GL] Failed to release the texture, the instance is not implementation of the gl texture";
			return false;
		}

		const uint32_t textureID = refTexture->GetID();
		if (textureID == GL_NONE)
		{
			LOGINFO() << "[GL] The texture is already released";
			return true;
		}

		glDeleteTextures(1, &textureID);

		refTexture->SetState(eResourceState::eNone);
		refTexture->SetID(GL_NONE);

		return true;
	}

	bool GLContext::ReleaseSampler(Memory::RefPtr<RHISampler> sampler)
	{
		if (!sampler)
		{
			LOGWARN() << "[GL] Failed to release the sampler, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLSampler> refSampler = Memory::Cast<GLSampler>(sampler);
		if (!refSampler)
		{
			LOGWARN() << "[GL] Failed to release the sampler, the instance is not implementation of the gl sampler";
			return false;
		}

		const uint32_t samplerID = refSampler->GetID();
		if (samplerID == GL_NONE)
		{
			LOGINFO() << "[GL] The sampler is already released";
			return true;
		}

		glDeleteSamplers(1, &samplerID);

		refSampler->SetState(eResourceState::eNone);
		refSampler->SetID(GL_NONE);

		return true;
	}

	bool GLContext::ReleaseShader(Memory::RefPtr<RHIShader> shader)
	{
		if (!shader)
		{
			LOGWARN() << "[GL] Failed to release the shader, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLShader> refShader = Memory::Cast<GLShader>(shader);
		if (!refShader)
		{
			LOGWARN() << "[GL] Failed to release the shader, the instance is not implementation of the gl shader";
			return false;
		}

		const uint32_t shaderID = refShader->GetID();
		if (shaderID == GL_NONE)
		{
			LOGINFO() << "[GL] The shader is already released";
			return true;
		}

		glDeleteShader(shaderID);

		refShader->SetState(eResourceState::eNone);
		refShader->SetID(GL_NONE);

		return true;
	}

	bool GLContext::ReleasePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (!pipeline)
		{
			LOGWARN() << "[GL] Failed to release the pipeline, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLPipeLine> refPipeline = Memory::Cast<GLPipeLine>(pipeline);
		if (!refPipeline)
		{
			LOGWARN() << "[GL] Failed to release the pipeline, the instance is not implementation of the gl pipeline";
			return false;
		}

		const uint32_t pipelineID = refPipeline->GetID();
		if (pipelineID == GL_NONE)
		{
			LOGINFO() << "[GL] The pipeline is already released";
			return true;
		}

		glDeleteProgram(pipelineID);

		refPipeline->SetState(eResourceState::eNone);
		refPipeline->SetID(GL_NONE);

		return true;
	}

	bool GLContext::ReleaseRenderPass(Memory::RefPtr<RHIRenderPass> pass)
	{
		if (!pass)
		{
			LOGWARN() << "[GL] Failed to release the render pass, the instance is invalid";
			return false;
		}

		Memory::RefPtr<GLRenderPass> refPass = Memory::Cast<GLRenderPass>(pass);
		if (!refPass)
		{
			LOGWARN() << "[GL] Failed to release the render pass, the instance is not implementation of the gl render pass";
			return false;
		}

		const uint32_t passID = refPass->GetID();
		if (passID == GL_NONE)
		{
			LOGINFO() << "[GL] The render pass is already released";
			return true;
		}

		glDeleteFramebuffers(1, &passID);

		refPass->SetState(eResourceState::eNone);
		refPass->SetID(GL_NONE);

		return true;
	}

	void GLContext::InitializeVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc& info)
	{
		if (info.streams.Empty() || !layout)
		{
			return;
		}

		GLVertexLayout* glVertexLayout = reinterpret_cast<GLVertexLayout*>(layout->GetRawBuffer());
		if (!glVertexLayout)
		{
			return;
		}

		uint32_t vertexLayoutID = GL_NONE;
		glGenVertexArrays(1, &vertexLayoutID);

		if (vertexLayoutID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to generate vertex layout buffer";
			glVertexLayout->SetState(eResourceState::eError);
			return;
		}

		glBindVertexArray(vertexLayoutID);

		for (const auto& [vertexKey, vertexStream] : info.streams)
		{
			const auto& attribute = vertexStream.attribute;
			const auto& slot = vertexStream.slot;

			const uint32_t divisor = GetDataDivisor(slot.divisor);

			const bool normalized = attribute.normalized ? GL_TRUE : GL_FALSE;
			const bool integer = IsInteger(attribute.componentType);

			glEnableVertexAttribArray(attribute.location);

			if (integer)
			{
				glVertexAttribIFormat(attribute.location,
					attribute.componentCount,
					GetDataType(attribute.componentType),
					attribute.componentOffset);
			}
			else
			{
				glVertexAttribFormat(attribute.location,
					attribute.componentCount,
					GetDataType(attribute.componentType),
					normalized,
					attribute.componentOffset);
			}

			glVertexAttribBinding(attribute.location, slot.binding);
			glVertexBindingDivisor(slot.binding, divisor);
		}

		glBindVertexArray(GL_NONE);

		glVertexLayout->SetID(vertexLayoutID);
		glVertexLayout->SetState(eResourceState::eReady);
	}

	void GLContext::InitializeBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc& info)
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

		const uint32_t dataUsage = GetDataUsage(info.dataUsage);
		const uint32_t bufferUsage = GetBufferUsage(info.bufferUsage);
		const uint32_t bufferSize = info.size;

		uint32_t bufferID = GL_NONE;

		glCreateBuffers(1, &bufferID);
		if (bufferID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to generate buffer";
			glBuffer->SetState(eResourceState::eError);
			return;
		}

		glNamedBufferStorage(bufferID, bufferSize, nullptr, dataUsage);

		uint32_t offset = 0;
		for (const auto& region : info.regions)
		{
			const void* dataPointer = region.data->GetPointer();
			const uint32_t dataSize = region.dataSize;
			if (!dataPointer || dataSize == 0)
			{
				continue;
			}

			if (offset + dataSize > bufferSize)
			{
				LOGERROR() << "[GL] Failed to initialize the buffer, the data size is bigger than the buffer size : " << bufferSize;

				glDeleteBuffers(1, &bufferID);

				glBuffer->SetState(eResourceState::eError);
				
				return;
			}

			glNamedBufferSubData(bufferID, offset, dataSize, dataPointer);
			offset += dataSize;
		}

		RHIBufferState initState;
		initState.usage = info.bufferUsage;
		initState.state = eDataState::eWrite;
		initState.stage = eDataStage::eTransfer;

		glBuffer->SetID(bufferID);
		glBuffer->SetState(eResourceState::eReady);
		glBuffer->SetBufferState(initState);
	}

	void GLContext::InitializeTexture(Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc& info)
	{
		if (!texture)
		{
			return;
		}

		GLTexture* glTexture = reinterpret_cast<GLTexture*>(texture->GetRawBuffer());
		if (!glTexture)
		{
			return;
		}

		const uint32_t textureDimension = GetTextureDimension(info.textureType);

		uint32_t textureID = GL_NONE;
		if (textureDimension == 1)
		{
			textureID = InitializeTexture1D(info);
		}
		else if (textureDimension == 2)
		{
			textureID = InitializeTexture2D(info);
		}
		else if (textureDimension == 3)
		{
			textureID = InitializeTexture3D(info);
		}
		else
		{
			LOGERROR() << "[GL] Unrecognized texture type: " << static_cast<uint32_t>(info.textureType);
		}

		if (textureID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to initialize texture";

			glDeleteTextures(1, &textureID);

			glTexture->SetID(GL_NONE);
			glTexture->SetState(eResourceState::eError);
			return;
		}

		RHITextureState initState;
		initState.usage = info.textureUsage;
		initState.state = eDataState::eWrite;
		initState.stage = eDataStage::eTransfer;

		glTexture->SetID(textureID);
		glTexture->SetState(eResourceState::eReady);
		glTexture->SetTextureState(initState);
	}

	void GLContext::InitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc& info)
	{
		if (!sampler)
		{
			return;
		}

		GLSampler* glSampler = reinterpret_cast<GLSampler*>(sampler->GetRawBuffer());
		if (!glSampler)
		{
			return;
		}

		uint32_t samplerID = GL_NONE;

		glCreateSamplers(1, &samplerID);
		if (samplerID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to create sampler";
			glSampler->SetState(eResourceState::eError);
			return;
		}

		const uint32_t minFilter = GetSamplerFilter(info.minFilter);
		const uint32_t magFilter = GetSamplerFilter(info.magFilter);

		const uint32_t wrapS = GetSamplerWrap(info.wrapS);
		const uint32_t wrapT = GetSamplerWrap(info.wrapT);
		const uint32_t wrapR = GetSamplerWrap(info.wrapR);

		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, minFilter);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, magFilter);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, wrapS);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, wrapT);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_R, wrapR);

		if ((eSamplerWrap::eClampToBorder == info.wrapS) ||
			(eSamplerWrap::eClampToBorder == info.wrapT) ||
			(eSamplerWrap::eClampToBorder == info.wrapR))
		{
			const fvec4 borderColor = GetSamplerBorderColor(info.wrapBorderColor);

			glSamplerParameterfv(samplerID, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<const GLfloat*>(&borderColor));
		}

		glSampler->SetID(samplerID);
		glSampler->SetState(eResourceState::eReady);
	}

	void GLContext::InitializeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc& info)
	{
		if (!shader || !info.data)
		{
			return;
		}

		GLShader* glShader = reinterpret_cast<GLShader*>(shader->GetRawBuffer());
		if (!glShader)
		{
			return;
		}

		const char* shaderSource = reinterpret_cast<const char*>(info.data->GetPointer());
		if (!shaderSource)
		{
			LOGERROR() << "[GL] Failed to get shader source code, the shader type : " << static_cast<uint32_t>(info.shaderType);
			glShader->SetState(eResourceState::eError);
			return;
		}

		const uint32_t shaderType = GetShaderType(shader->GetShaderType());
		if (shaderType == GL_NONE)
		{
			LOGERROR() << "[GL] Unrecognized shader type: " << static_cast<uint32_t>(shader->GetShaderType());
			glShader->SetState(eResourceState::eError);
			return;
		}

		const uint32_t shaderID = glCreateShader(shaderType);
		const int32_t shaderLength = static_cast<int32_t>(info.data->GetSize());

		glShaderSource(shaderID, 1, &shaderSource, &shaderLength);
		glCompileShader(shaderID);

		int32_t success = GL_NONE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, utils::MAX_MESSAGE_LENGTH, nullptr, utils::MESSAGE);

			LOGERROR() << "[GL] Failed to compile shader: " << utils::MESSAGE;
			glDeleteShader(shaderID);

			glShader->SetState(eResourceState::eError);
			return;
		}

		const DynamicArray<uint32_t> convertedSource = RHIUtils::CompileShader(info);
		if (convertedSource.Empty())
		{
			LOGERROR() << "[GL] Failed to compile shader source code, the shader type : " << static_cast<uint32_t>(info.shaderType);
			glDeleteShader(shaderID);

			glShader->SetState(eResourceState::eError);
			return;
		}

		RHIUtils::ReflectShader(convertedSource, shader);

		glShader->SetID(shaderID);
		glShader->SetState(eResourceState::eReady);
	}

	void GLContext::InitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc& info)
	{
		if (!pipeline)
		{
			return;
		}

		GLPipeLine* glPipeLine = reinterpret_cast<GLPipeLine*>(pipeline->GetRawBuffer());
		if (!glPipeLine)
		{
			return;
		}

		const uint32_t programID = glCreateProgram();
		if (programID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to create shader program";
			glPipeLine->SetState(eResourceState::eError);
			return;
		}

		glPipeLine->Clear();

		bool attached = true;
		auto AttachShader = [&](const Memory::RefPtr<const RHIShader>& shader)
		{
			if (!shader || !attached)
			{
				return;
			}

			const GLShader* glShader = reinterpret_cast<const GLShader*>(shader->GetRawBuffer());
			if (!glShader || glShader->GetID() == GL_NONE || !glPipeLine->AddShader(shader))
			{
				attached = false;
				return;
			}

			glAttachShader(programID, glShader->GetID());
		};

		AttachShader(info.vertexShader);
		AttachShader(info.geometryShader);
		AttachShader(info.hullShader);
		AttachShader(info.domainShader);
		AttachShader(info.pixelShader);
		AttachShader(info.computeShader);

		if (!attached)
		{
			LOGERROR() << "[GL] Failed to attach the shader on the shader program";
			glDeleteProgram(programID);

			glPipeLine->SetState(eResourceState::eError);
			return;
		}

		Memory::RefPtr<const RHIVertexLayout> layout = info.layout;
		if (layout && !glPipeLine->AddLayout(layout))
		{
			LOGERROR() << "[GL] Failed to add the vertex layout on the pipeline";
			glDeleteProgram(programID);

			glPipeLine->SetState(eResourceState::eError);
			return;
		}

		Memory::RefPtr<const RHIRenderPass> pass = info.target;
		if (pass && !glPipeLine->AddPass(pass))
		{
			LOGERROR() << "[GL] Failed to add the render pass on the pipeline";
			glDeleteProgram(programID);

			glPipeLine->SetState(eResourceState::eError);
			return;
		}

		glLinkProgram(programID);

		GLint success = GL_NONE;
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLsizei messageLength = GL_NONE;
			glGetProgramInfoLog(programID, utils::MAX_MESSAGE_LENGTH, &messageLength, utils::MESSAGE);

			const std::string message(utils::MESSAGE, messageLength);
			LOGERROR() << "[GL] Failed to link shader program: " << message;
			glDeleteProgram(programID);

			glPipeLine->SetState(eResourceState::eError);
			return;
		}

		glPipeLine->SetID(programID);
		glPipeLine->SetState(eResourceState::eReady);
	}

	void GLContext::InitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc& info)
	{
		if (!pass)
		{
			return;
		}

		GLRenderPass* glRenderPass = reinterpret_cast<GLRenderPass*>(pass->GetRawBuffer());
		if (!glRenderPass)
		{
			return;
		}

		if (info.colors.Empty() && !info.depthStencil.texture)
		{
			glRenderPass->SetID(GL_NONE);
			glRenderPass->SetState(eResourceState::eReady);
			return;
		}

		GLuint frameBufferID = GL_NONE;
		glCreateFramebuffers(1, &frameBufferID);
		glGenFramebuffers(1, &frameBufferID);
		if (frameBufferID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to create the frame buffer";

			glRenderPass->SetState(eResourceState::eError);
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		auto Fail = [&](const char* message)
		{
			LOGERROR() << "[GL] " << message;

			glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
			glDeleteFramebuffers(1, &frameBufferID);

			glRenderPass->SetState(eResourceState::eError);
		};

		uint32_t drawBufferCount = 0;
		for (const auto& colorAttach : info.colors)
		{
			if (colorAttach.location >= utils::MAX_COLOR_ATTACHMENT)
			{
				Fail("Failed to initialize the render pass, invalid the color attachment location");
				return;
			}

			if (!InitializeFrameBuffer(colorAttach.texture, eAttachmentType::eColor, colorAttach.location))
			{
				Fail("Failed to bind the texture on the color attachment for the frame buffer");
				return;
			}

			drawBufferCount = (std::max)(drawBufferCount, colorAttach.location + 1);
		}

		if (info.depthStencil.texture)
		{
			eAttachmentType depthStencilType = info.depthStencil.type;
			if (depthStencilType == eAttachmentType::eNone)
			{
				const bool hasStencil = (info.depthStencil.texture->GetTextureFormat() == eTextureFormat::eD24_S8);
				depthStencilType = hasStencil ? (eAttachmentType::eDepth | eAttachmentType::eStencil) : eAttachmentType::eDepth;
			}

			if (!InitializeFrameBuffer(info.depthStencil.texture, depthStencilType))
			{
				Fail("Failed to bind the texture on the depth stencil attachment for the frame buffer");
				return;
			}
		}

		if (drawBufferCount == 0)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else
		{
			wtr::DynamicArray<uint32_t> drawBuffers;
			drawBuffers.Resize(drawBufferCount, GL_NONE);

			for (const auto& colorAttach : info.colors)
			{
				drawBuffers[colorAttach.location] = GetColorAttachment(colorAttach.location);
			}

			glDrawBuffers(static_cast<GLsizei>(drawBuffers.Size()), drawBuffers.Data());
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Fail("Failed to initialize the render pass, invalid the frame buffer bined by texture");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

		glRenderPass->SetID(frameBufferID);
		glRenderPass->SetState(eResourceState::eReady);
	}

	void GLContext::InitializeBufferView(Memory::RefPtr<RHIBufferView> bufferView, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!bufferView)
		{
			return;
		}

		if (!buffer || buffer->GetState() != eResourceState::eReady)
		{
			LOGERROR() << "[GL] Failed to initialize the buffer view, the buffer is not ready";
			bufferView->SetState(eResourceState::eError);
			return;
		}

		const uint64_t viewEnd = static_cast<uint64_t>(bufferView->GetOffset()) + bufferView->GetSize();
		if (viewEnd > buffer->GetSize())
		{
			LOGERROR() << "[GL] Failed to initialize the buffer view, the range is out of the buffer : " << viewEnd << " > " << buffer->GetSize();
			bufferView->SetState(eResourceState::eError);
			return;
		}

		bufferView->SetState(eResourceState::eReady);
	}

	void GLContext::InitializeTextureView(Memory::RefPtr<RHITextureView> textureView, Memory::RefPtr<RHITexture> texture)
	{
		if (!textureView)
		{
			return;
		}

		if (!texture || texture->GetState() != eResourceState::eReady)
		{
			LOGERROR() << "[GL] Failed to initialize the texture view, the texture is not ready";
			textureView->SetState(eResourceState::eError);
			return;
		}

		textureView->SetState(eResourceState::eReady);
	}

	void GLContext::UpdateBuffer(Memory::RefPtr<RHIBufferView> bufferView, const RHIBufferUpdateDesc& info)
	{
		if (!bufferView || info.regions.Empty())
		{
			return;
		}

		Memory::RefPtr<RHIBuffer> buffer = bufferView->GetBuffer();
		if (!buffer)
		{
			return;
		}

		GLBuffer* glBuffer = reinterpret_cast<GLBuffer*>(buffer->GetRawBuffer());
		if (!glBuffer || glBuffer->GetID() == GL_NONE)
		{
			return;
		}

		const uint64_t viewBegin = bufferView->GetOffset();
		const uint64_t viewEnd = (bufferView->GetSize() == 0) ? buffer->GetSize() : viewBegin + bufferView->GetSize();

		uint64_t offset = viewBegin + info.offset;
		for (const auto& region : info.regions)
		{
			const void* dataPointer = region.data->GetPointer();
			const uint32_t dataSize = region.dataSize;
			if (!dataPointer || dataSize == 0)
			{
				continue;
			}

			if (offset + dataSize > viewEnd)
			{
				LOGERROR() << "[GL] Failed to update the buffer, the data is out of the buffer view : " << (offset + dataSize) << " > " << viewEnd;
				return;
			}

			glNamedBufferSubData(glBuffer->GetID(), static_cast<GLintptr>(offset), dataSize, dataPointer);
			offset += dataSize;
		}
	}

	void GLContext::UpdateTexture(Memory::RefPtr<RHITextureView> textureView, const RHITextureUpdateDesc& info)
	{
		if (!textureView || info.regions.Empty())
		{
			return;
		}

		Memory::RefPtr<RHITexture> texture = textureView->GetTexture();
		if (!texture)
		{
			return;
		}

		GLTexture* glTexture = reinterpret_cast<GLTexture*>(texture->GetRawBuffer());
		if (!glTexture || (glTexture->GetID() == GL_NONE))
		{
			return;
		}

		RHITextureUpdateDesc update(texture->GetDesc());
		update.regions = info.regions;
		update.generateMips = info.generateMips;

		const eTextureType type = update.textureType;
		const uint32_t textureID = glTexture->GetID();

		bool updated = false;
		if (type == eTextureType::eTexture1D ||
			type == eTextureType::eTexture1DArray)
		{
			updated = UpdateTexture1D(update, textureID);
		}
		else if (type == eTextureType::eTexture2D ||
			type == eTextureType::eTexture2DArray ||
			type == eTextureType::eTextureCube ||
			type == eTextureType::eTextureCubeArray)
		{
			updated = UpdateTexture2D(update, textureID);
		}
		else if (type == eTextureType::eTexture3D)
		{
			updated = UpdateTexture3D(update, textureID);
		}
		else
		{
			LOGERROR() << "[GL] Unrecognized texture type: " << static_cast<uint32_t>(update.textureType);
		}

		if (!updated)
		{
			LOGERROR() << "[GL] Failed to update texture";
		}
	}

	void GLContext::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		if (!layout)
		{
			return;
		}

		const GLVertexLayout* glVertexLayout = reinterpret_cast<const GLVertexLayout*>(layout->GetRawBuffer());
		if (!glVertexLayout || glVertexLayout->GetID() == GL_NONE)
		{
			return;
		}

		glBindVertexArray(glVertexLayout->GetID());
	}

	void GLContext::SetVertexBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding)
	{
		if (!bufferView)
		{
			return;
		}

		Memory::RefPtr<const GLBuffer> refBuffer = Memory::Cast<const GLBuffer>(bufferView->GetBuffer());
		if (!refBuffer || refBuffer->GetID() == GL_NONE)
		{
			return;
		}

		const uint32_t bufferID = refBuffer->GetID();
		const uint32_t bufferOffset = bufferView->GetOffset();
		const uint32_t bufferStride = bufferView->GetStride();

		if (bufferStride == 0)
		{
			LOGWARN() << "[GL] The vertex buffer view has no stride, binding : " << binding;
		}

		glBindVertexBuffer(binding, bufferID, bufferOffset, bufferStride);
	}

	void GLContext::SetIndexBuffer(Memory::RefPtr<const RHIBufferView> bufferView)
	{
		if (!bufferView)
		{
			return;
		}

		Memory::RefPtr<const GLBuffer> refBuffer = Memory::Cast<const GLBuffer>(bufferView->GetBuffer());
		if (!refBuffer || refBuffer->GetID() == GL_NONE)
		{
			return;
		}

		const uint32_t bufferID = refBuffer->GetID();
		const uint32_t bufferOffset = bufferView->GetOffset();

		if (bufferOffset != 0)
		{
			LOGWARN() << "[GL] The index buffer view offset is ignored : " << bufferOffset;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	}

	void GLContext::SetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding)
	{
		const GLBuffer* glBuffer = GetGLBuffer(bufferView);
		if (!glBuffer)
		{
			return;
		}

		const eBufferUsage usage = bufferView->GetBufferState().usage;
		if ((usage & eBufferUsage::eIndirect) != eBufferUsage::eNone)
		{
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, glBuffer->GetID());
			return;
		}

		const uint32_t bufferType = GetBufferUsage(usage);
		if (bufferType != GL_UNIFORM_BUFFER && bufferType != GL_SHADER_STORAGE_BUFFER)
		{
			LOGWARN() << "[GL] Failed to set the buffer, unrecognized buffer usage : " << static_cast<uint32_t>(usage);
			return;
		}

		if (bufferView->GetSize() == 0)
		{
			glBindBufferBase(bufferType, binding, glBuffer->GetID());
		}
		else
		{
			// Bind a part of the buffer (ring buffer slice), the offset must be a multiple of GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
			glBindBufferRange(bufferType, binding, glBuffer->GetID(), bufferView->GetOffset(), bufferView->GetSize());
		}
	}

	void GLContext::SetTexture(Memory::RefPtr<const RHITextureView> textureView, const uint32_t binding)
	{
		const GLTexture* glTexture = GetGLTexture(textureView);
		if (!glTexture)
		{
			return;
		}

		glBindTextureUnit(binding, glTexture->GetID());
	}

	void GLContext::SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t binding)
	{
		if (!sampler)
		{
			return;
		}

		const GLSampler* glSampler = reinterpret_cast<const GLSampler*>(sampler->GetRawBuffer());
		if (!glSampler || glSampler->GetID() == GL_NONE)
		{
			return;
		}

		glBindSampler(binding, glSampler->GetID());
	}

	void GLContext::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		if (!pipeline)
		{
			return;
		}

		const GLPipeLine* glPipeLine = reinterpret_cast<const GLPipeLine*>(pipeline->GetRawBuffer());
		if (!glPipeLine || glPipeLine->GetID() == GL_NONE)
		{
			return;
		}

		glUseProgram(glPipeLine->GetID());

		SetColorState(pipeline->GetColorState());
		SetDepthState(pipeline->GetDepthState());
		SetStencilState(pipeline->GetStencilState());
		SetBlendState(pipeline->GetBlendState());
		SetRasterizerState(pipeline->GetRasterizerState());
	}

	void GLContext::SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass)
	{
		if (!pass)
		{
			return;
		}

		const GLRenderPass* glRenderPass = reinterpret_cast<const GLRenderPass*>(pass->GetRawBuffer());
		if (!glRenderPass || glRenderPass->GetState() != eResourceState::eReady)
		{
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, glRenderPass->GetID());

		// TODO : RHIAttachment has no clear value, use the default for now
		const RHIClearState clearState = {};
		const RHIRenderPassDesc& desc = pass->GetDesc();

		// glClearBuffer* respects the write masks, open them before clearing (SetPipeLine sets them again)
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilMask(0xFF);

		for (const auto& color : desc.colors)
		{
			if (color.loadOp != eAttachmentLoadOp::eClear || !color.texture)
			{
				continue;
			}

			if (IsUIntFormat(color.texture->GetTextureFormat()))
			{
				const GLuint colors[] = {
					static_cast<GLuint>(clearState.color[0]),
					static_cast<GLuint>(clearState.color[1]),
					static_cast<GLuint>(clearState.color[2]),
					static_cast<GLuint>(clearState.color[3]),
				};

				glClearBufferuiv(GL_COLOR, color.location, colors);
			}
			else
			{
				const GLfloat colors[] = {
					static_cast<GLfloat>(clearState.color[0]),
					static_cast<GLfloat>(clearState.color[1]),
					static_cast<GLfloat>(clearState.color[2]),
					static_cast<GLfloat>(clearState.color[3]),
				};

				glClearBufferfv(GL_COLOR, color.location, colors);
			}
		}

		const auto& depthStencil = desc.depthStencil;
		if (depthStencil.loadOp != eAttachmentLoadOp::eClear || !depthStencil.texture)
		{
			return;
		}

		const eTextureFormat format = depthStencil.texture->GetTextureFormat();
		if (format == eTextureFormat::eD32)
		{
			glClearBufferfv(GL_DEPTH, 0, &clearState.depth);
		}
		else if (format == eTextureFormat::eD24_S8)
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, clearState.depth, static_cast<GLint>(clearState.stencil));
		}
		else
		{
			// TODO - Stencil Only
		}
	}

	void GLContext::UnsetVertexLayout()
	{
		glBindVertexArray(GL_NONE);
	}

	void GLContext::UnsetVertexBuffer(const uint32_t binding)
	{
		glBindVertexBuffer(binding, GL_NONE, 0, 0);
	}

	void GLContext::UnsetIndexBuffer()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	}

	void GLContext::UnsetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding)
	{
		if (!bufferView)
		{
			return;
		}

		const eBufferUsage usage = bufferView->GetBufferState().usage;
		if ((usage & eBufferUsage::eIndirect) != eBufferUsage::eNone)
		{
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, GL_NONE);
			return;
		}

		const uint32_t bufferType = GetBufferUsage(usage);
		if (bufferType != GL_UNIFORM_BUFFER && bufferType != GL_SHADER_STORAGE_BUFFER)
		{
			return;
		}

		glBindBufferBase(bufferType, binding, GL_NONE);
	}

	void GLContext::UnsetTexture(Memory::RefPtr<const RHITextureView> textureView, const uint32_t binding)
	{
		if (!textureView)
		{
			return;
		}

		glBindTextureUnit(binding, GL_NONE);
	}

	void GLContext::UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t binding)
	{
		if (!sampler)
		{
			return;
		}

		glBindSampler(binding, GL_NONE);
	}

	void GLContext::UnsetPipeLine()
	{
		glUseProgram(GL_NONE);
	}

	void GLContext::UnsetRenderPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void GLContext::Transition(const RHITransitionDesc& info)
	{
		// GL has no layout, glMemoryBarrier is only needed after the shader writes (SSBO / image store)
		GLbitfield barrier = GL_NONE;

		for (const auto& transition : info.buffers)
		{
			const bool written = (transition.before.state & eDataState::eWrite) != eDataState::eNone;
			const bool storage = (transition.before.usage & eBufferUsage::eStorage) != eBufferUsage::eNone;
			if (!written || !storage)
			{
				continue;
			}

			barrier |= GetBufferBarrier(transition.after.usage);
		}

		for (const auto& transition : info.textures)
		{
			const eTextureUsage implicitUsage = eTextureUsage::eTransfer | eTextureUsage::eColorAttach | eTextureUsage::eDepthStencilAttach;

			const bool written = (transition.before.state & eDataState::eWrite) != eDataState::eNone;
			const bool shaderWritten = (transition.before.usage & implicitUsage) == eTextureUsage::eNone;
			if (!written || !shaderWritten)
			{
				continue;
			}

			barrier |= GetTextureBarrier(transition.after.usage);
		}

		if (barrier != GL_NONE)
		{
			glMemoryBarrier(barrier);
		}
	}

	void GLContext::DispatchCompute(const RHIDispatchDesc& info)
	{
		glDispatchCompute(info.groupX, info.groupY, info.groupZ);
	}

	void GLContext::DrawDirect(const RHIDrawDirectDesc& info)
	{
		if (info.indexCount == 0 || info.instanceCount == 0)
		{
			return;
		}

		const uint32_t drawMode = GetDrawMode(info.drawMode);

		if (info.indexType == eDataType::eNone)
		{
			glDrawArraysInstanced(drawMode, info.baseVertex, info.indexCount, info.instanceCount);
			return;
		}

		const uint32_t indexType = GetDataType(info.indexType);
		const void* indexOffset = reinterpret_cast<const void*>(static_cast<uintptr_t>(info.indexOffset));

		glDrawElementsInstancedBaseVertex(
			drawMode,
			info.indexCount,
			indexType,
			indexOffset,
			info.instanceCount,
			info.baseVertex
		);
	}

	void GLContext::DrawIndirect(const RHIDrawIndirectDesc& info)
	{
		if (info.indexCount == 0 || info.instanceCount == 0)
		{
			return;
		}

		// TODO : The desc has no argument buffer, draw with the arguments from the CPU for now
		const uint32_t drawMode = GetDrawMode(info.drawMode);

		if (info.indexType == eDataType::eNone)
		{
			glDrawArraysInstancedBaseInstance(drawMode, info.baseVertex, info.indexCount, info.instanceCount, info.baseInstance);
			return;
		}

		const uint32_t indexType = GetDataType(info.indexType);
		const void* indexOffset = reinterpret_cast<const void*>(static_cast<uintptr_t>(info.indexOffset));

		glDrawElementsInstancedBaseVertexBaseInstance(
			drawMode,
			info.indexCount,
			indexType,
			indexOffset,
			info.instanceCount,
			info.baseVertex,
			info.baseInstance
		);
	}

	uint32_t GLContext::GetBindingType(const eBindingType type) const
	{
		if (eBindingType::eUniform == type)
		{
			return GL_UNIFORM_BUFFER;
		}
		else if (eBindingType::eStorage == type)
		{
			return GL_SHADER_STORAGE_BUFFER;
		}
		else if (eBindingType::eSampler == type)
		{
			return GL_SAMPLER;
		}
		else if (eBindingType::eTexture == type)
		{
			return GL_TEXTURE;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetDataUsage(const eDataUsage usage) const
	{
		if (eDataUsage::eStatic == usage)
		{
			return GL_STATIC_DRAW;
		}
		else if (eDataUsage::eDynamic == usage)
		{
			return GL_DYNAMIC_DRAW;
		}
		else if (eDataUsage::eStream == usage)
		{
			return GL_STREAM_DRAW;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetDataState(const eDataState state) const
	{
		const bool read = (state & eDataState::eRead) != eDataState::eNone;
		const bool write = (state & eDataState::eWrite) != eDataState::eNone;

		if (read && write)
		{
			return GL_READ_WRITE;
		}
		else if (read)
		{
			return GL_READ_ONLY;
		}
		else if (write)
		{
			return GL_WRITE_ONLY;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetDataStage(const eDataStage stage) const
	{
		if (eDataStage::eVertex == stage)
		{
			return GL_VERTEX_SHADER;
		}
		else if (eDataStage::eHull == stage)
		{
			return GL_TESS_CONTROL_SHADER;
		}
		else if (eDataStage::eDomain == stage)
		{
			return GL_TESS_EVALUATION_SHADER;
		}
		else if (eDataStage::eGeometry == stage)
		{
			return GL_GEOMETRY_SHADER;
		}
		else if (eDataStage::ePixel == stage)
		{
			return GL_FRAGMENT_SHADER;
		}
		else if (eDataStage::eCompute == stage)
		{
			return GL_COMPUTE_SHADER;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetDataType(const eDataType type) const
	{
		if (eDataType::eByte == type)
		{
			return GL_BYTE;
		}
		else if (eDataType::eUByte == type)
		{
			return GL_UNSIGNED_BYTE;
		}
		else if (eDataType::eShort == type)
		{
			return GL_SHORT;
		}
		else if (eDataType::eUShort == type)
		{
			return GL_UNSIGNED_SHORT;
		}
		else if (eDataType::eInt == type)
		{
			return GL_INT;
		}
		else if (eDataType::eUInt == type)
		{
			return GL_UNSIGNED_INT;
		}
		else if (eDataType::eFloat == type)
		{
			return GL_FLOAT;
		}
		else if (eDataType::eDouble == type)
		{
			return GL_DOUBLE;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetDataDivisor(const eDataDivisor divisor) const
	{
		if (eDataDivisor::ePerVertex == divisor)
		{
			return 0;
		}
		else if (eDataDivisor::ePerInstance == divisor)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	uint32_t GLContext::GetBufferUsage(const eBufferUsage usage) const
	{
		if ((usage & eBufferUsage::eStorage) != eBufferUsage::eNone)
		{
			return GL_SHADER_STORAGE_BUFFER;
		}
		else if ((usage & eBufferUsage::eUniform) != eBufferUsage::eNone)
		{
			return GL_UNIFORM_BUFFER;
		}
		else if ((usage & eBufferUsage::eIndirect) != eBufferUsage::eNone)
		{
			return GL_DRAW_INDIRECT_BUFFER;
		}
		else if ((usage & eBufferUsage::eIndex) != eBufferUsage::eNone)
		{
			return GL_ELEMENT_ARRAY_BUFFER;
		}
		else if ((usage & eBufferUsage::eVertex) != eBufferUsage::eNone)
		{
			return GL_ARRAY_BUFFER;
		}
		else if ((usage & eBufferUsage::eTransfer) != eBufferUsage::eNone)
		{
			return GL_COPY_WRITE_BUFFER;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetTextureFormat(const eTextureFormat format) const
	{
		if (eTextureFormat::eR8_UNorm == format)
		{
			return GL_R8;
		}
		else if (eTextureFormat::eR8G8_UNorm == format)
		{
			return GL_RG8;
		}
		else if (eTextureFormat::eR8G8B8_UNorm == format)
		{
			return GL_RGB8;
		}
		else if (eTextureFormat::eR8G8B8A8_UNorm == format)
		{
			return GL_RGBA8;
		}
		else if (eTextureFormat::eR16_UNorm == format)
		{
			return GL_R16;
		}
		else if (eTextureFormat::eR16G16_UNorm == format)
		{
			return GL_RG16;
		}
		else if (eTextureFormat::eR16G16B16_UNorm == format)
		{
			return GL_RGB16;
		}
		else if (eTextureFormat::eR16G16B16A16_UNorm == format)
		{
			return GL_RGBA16;
		}
		else if (eTextureFormat::eR8G8B8A8_sRGB == format)
		{
			return GL_SRGB8_ALPHA8;
		}
		else if (eTextureFormat::eR8_UInt == format)
		{
			return GL_R8UI;
		}
		else if (eTextureFormat::eR8G8_UInt == format)
		{
			return GL_RG8UI;
		}
		else if (eTextureFormat::eR8G8B8_UInt == format)
		{
			return GL_RGB8UI;
		}
		else if (eTextureFormat::eR8G8B8A8_UInt == format)
		{
			return GL_RGBA8UI;
		}
		else if (eTextureFormat::eR16_UInt == format)
		{
			return GL_R16UI;
		}
		else if (eTextureFormat::eR16G16_UInt == format)
		{
			return GL_RG16UI;
		}
		else if (eTextureFormat::eR16G16B16_UInt == format)
		{
			return GL_RGB16UI;
		}
		else if (eTextureFormat::eR16G16B16A16_UInt == format)
		{
			return GL_RGBA16UI;
		}
		else if (eTextureFormat::eR32_UInt == format)
		{
			return GL_R32UI;
		}
		else if (eTextureFormat::eR32G32_UInt == format)
		{
			return GL_RG32UI;
		}
		else if (eTextureFormat::eR32G32B32_UInt == format)
		{
			return GL_RGB32UI;
		}
		else if (eTextureFormat::eR32G32B32A32_UInt == format)
		{
			return GL_RGBA32UI;
		}
		else if (eTextureFormat::eR16_Float == format)
		{
			return GL_R16F;
		}
		else if (eTextureFormat::eR16G16_Float == format)
		{
			return GL_RG16F;
		}
		else if (eTextureFormat::eR16G16B16_Float == format)
		{
			return GL_RGB16F;
		}
		else if (eTextureFormat::eR16G16B16A16_Float == format)
		{
			return GL_RGBA16F;
		}
		else if (eTextureFormat::eR32_Float == format)
		{
			return GL_R32F;
		}
		else if (eTextureFormat::eR32G32_Float == format)
		{
			return GL_RG32F;
		}
		else if (eTextureFormat::eR32G32B32_Float == format)
		{
			return GL_RGB32F;
		}
		else if (eTextureFormat::eR32G32B32A32_Float == format)
		{
			return GL_RGBA32F;
		}
		else if (eTextureFormat::eD24_S8 == format)
		{
			return GL_DEPTH24_STENCIL8;
		}
		else if (eTextureFormat::eD32 == format)
		{
			return GL_DEPTH_COMPONENT32F;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetTextureSwizzle(const eTextureSwizzle swizzle) const
	{
		if (eTextureSwizzle::eRed == swizzle)
		{
			return GL_RED;
		}
		else if (eTextureSwizzle::eGreen == swizzle)
		{
			return GL_GREEN;
		}
		else if (eTextureSwizzle::eBlue == swizzle)
		{
			return GL_BLUE;
		}
		else if (eTextureSwizzle::eAlpha == swizzle)
		{
			return GL_ALPHA;
		}
		else if (eTextureSwizzle::eZero == swizzle)
		{
			return GL_ZERO;
		}
		else if (eTextureSwizzle::eOne == swizzle)
		{
			return GL_ONE;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetTextureType(const eTextureType type) const
	{
		if (eTextureType::eTexture1D == type)
		{
			return GL_TEXTURE_1D;
		}
		else if (eTextureType::eTexture2D == type)
		{
			return GL_TEXTURE_2D;
		}
		else if (eTextureType::eTexture3D == type)
		{
			return GL_TEXTURE_3D;
		}
		else if (eTextureType::eTextureCube == type)
		{
			return GL_TEXTURE_CUBE_MAP;
		}
		else if (eTextureType::eTexture1DArray == type)
		{
			return GL_TEXTURE_1D_ARRAY;
		}
		else if (eTextureType::eTexture2DArray == type)
		{
			return GL_TEXTURE_2D_ARRAY;
		}
		else if (eTextureType::eTextureCubeArray == type)
		{
			return GL_TEXTURE_CUBE_MAP_ARRAY;
		}
		else if (eTextureType::eTextureMultisample == type)
		{
			return GL_TEXTURE_2D_MULTISAMPLE;
		}
		else if (eTextureType::eTextureMultisampleArray == type)
		{
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetTextureDataType(const eTextureFormat format) const
	{
		if (eTextureFormat::eR8_UNorm == format ||
			eTextureFormat::eR16_UNorm == format ||
			eTextureFormat::eR16_Float == format ||
			eTextureFormat::eR32_Float == format)
		{
			return GL_RED;
		}
		else if (eTextureFormat::eR8G8_UNorm == format ||
			eTextureFormat::eR16G16_UNorm == format ||
			eTextureFormat::eR16G16_Float == format ||
			eTextureFormat::eR32G32_Float == format)
		{
			return GL_RG;
		}
		else if (eTextureFormat::eR8G8B8_UNorm == format ||
			eTextureFormat::eR16G16B16_UNorm == format ||
			eTextureFormat::eR16G16B16_Float == format ||
			eTextureFormat::eR32G32B32_Float == format)
		{
			return GL_RGB;
		}
		else if (eTextureFormat::eR8G8B8A8_UNorm == format ||
			eTextureFormat::eR16G16B16A16_UNorm == format ||
			eTextureFormat::eR8G8B8A8_sRGB == format ||
			eTextureFormat::eR16G16B16A16_Float == format ||
			eTextureFormat::eR32G32B32A32_Float == format)
		{
			return GL_RGBA;
		}
		else if (eTextureFormat::eR8_UInt == format ||
			eTextureFormat::eR16_UInt == format ||
			eTextureFormat::eR32_UInt == format)
		{
			return GL_RED_INTEGER;
		}
		else if (eTextureFormat::eR8G8_UInt == format ||
			eTextureFormat::eR16G16_UInt == format ||
			eTextureFormat::eR32G32_UInt == format)
		{
			return GL_RG_INTEGER;
		}
		else if (eTextureFormat::eR8G8B8_UInt == format ||
			eTextureFormat::eR16G16B16_UInt == format ||
			eTextureFormat::eR32G32B32_UInt == format)
		{
			return GL_RGB_INTEGER;
		}
		else if (eTextureFormat::eR8G8B8A8_UInt == format ||
			eTextureFormat::eR16G16B16A16_UInt == format ||
			eTextureFormat::eR32G32B32A32_UInt == format)
		{
			return GL_RGBA_INTEGER;
		}
		else if (eTextureFormat::eD24_S8 == format)
		{
			return GL_DEPTH_STENCIL;
		}
		else if (eTextureFormat::eD32 == format)
		{
			return GL_DEPTH_COMPONENT;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetTextureDataFormat(const eTextureFormat format) const
	{
		if (eTextureFormat::eR8_UNorm == format ||
			eTextureFormat::eR8G8_UNorm == format ||
			eTextureFormat::eR8G8B8_UNorm == format ||
			eTextureFormat::eR8G8B8A8_UNorm == format ||
			eTextureFormat::eR8G8B8A8_sRGB == format ||
			eTextureFormat::eR8_UInt == format ||
			eTextureFormat::eR8G8_UInt == format ||
			eTextureFormat::eR8G8B8_UInt == format ||
			eTextureFormat::eR8G8B8A8_UInt == format)
		{
			return GL_UNSIGNED_BYTE;
		}
		else if (eTextureFormat::eR16_Float == format ||
			eTextureFormat::eR16G16_Float == format ||
			eTextureFormat::eR16G16B16_Float == format ||
			eTextureFormat::eR16G16B16A16_Float == format)
		{
			return GL_HALF_FLOAT;
		}
		else if (eTextureFormat::eR16_UNorm == format ||
			eTextureFormat::eR16G16_UNorm == format ||
			eTextureFormat::eR16G16B16_UNorm == format ||
			eTextureFormat::eR16G16B16A16_UNorm == format ||
			eTextureFormat::eR16_UInt == format ||
			eTextureFormat::eR16G16_UInt == format ||
			eTextureFormat::eR16G16B16_UInt == format ||
			eTextureFormat::eR16G16B16A16_UInt == format)
		{
			return GL_UNSIGNED_SHORT;
		}
		else if (eTextureFormat::eR32_Float == format ||
			eTextureFormat::eR32G32_Float == format ||
			eTextureFormat::eR32G32B32_Float == format ||
			eTextureFormat::eR32G32B32A32_Float == format ||
			eTextureFormat::eD32 == format)
		{
			return GL_FLOAT;
		}
		else if (eTextureFormat::eR32_UInt == format ||
			eTextureFormat::eR32G32_UInt == format ||
			eTextureFormat::eR32G32B32_UInt == format ||
			eTextureFormat::eR32G32B32A32_UInt == format)
		{
			return GL_UNSIGNED_INT;
		}
		else if (eTextureFormat::eD24_S8 == format)
		{
			return GL_UNSIGNED_INT_24_8;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetTextureDimension(const eTextureType type) const
	{
		if (eTextureType::eTexture1D == type || 
			eTextureType::eTexture1DArray == type)
		{
			return 1;
		}
		else if (eTextureType::eTexture2D == type || 
			eTextureType::eTexture2DArray == type || 
			eTextureType::eTextureCube == type || 
			eTextureType::eTextureCubeArray == type || 
			eTextureType::eTextureMultisample == type || 
			eTextureType::eTextureMultisampleArray == type)
		{
			return 2;
		}
		else if (eTextureType::eTexture3D == type)
		{
			return 3;
		}
		else
		{
			return 0;
		}
	}

	uint32_t GLContext::GetSamplerFilter(const eSamplerFilter filter) const
	{
		if (eSamplerFilter::eNearest == filter)
		{
			return GL_NEAREST;
		}
		else if (eSamplerFilter::eLinear == filter)
		{
			return GL_LINEAR;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetSamplerWrap(const eSamplerWrap wrap) const
	{
		if (eSamplerWrap::eRepeat == wrap)
		{
			return GL_REPEAT;
		}
		else if (eSamplerWrap::eMirroredRepeat == wrap)
		{
			return GL_MIRRORED_REPEAT;
		}
		else if (eSamplerWrap::eClampToEdge == wrap)
		{
			return GL_CLAMP_TO_EDGE;
		}
		else if (eSamplerWrap::eClampToBorder == wrap)
		{
			return GL_CLAMP_TO_BORDER;
		}
		else
		{
			return GL_NONE;
		}
	}

	fvec4 GLContext::GetSamplerBorderColor(const eSamplerBorderColor borderColor) const
	{
		if (eSamplerBorderColor::eTransparentBlack == borderColor)
		{
			return fvec4(0.f, 0.f, 0.f, 0.f);
		}
		else if (eSamplerBorderColor::eOpaqueBlack == borderColor)
		{
			return fvec4(0.f, 0.f, 0.f, 1.f);
		}
		else if (eSamplerBorderColor::eOpaqueWhite == borderColor)
		{
			return fvec4(1.f, 1.f, 1.f, 1.f);
		}
		else
		{
			return fvec4(0.f, 0.f, 0.f, 0.f);
		}
	}

	uint32_t GLContext::GetCullFace(const eCullFace face) const
	{
		if (eCullFace::eFront == face)
		{
			return GL_FRONT;
		}
		else if (eCullFace::eBack == face)
		{
			return GL_BACK;
		}
		else if (eCullFace::eFront_Back == face)
		{
			return GL_FRONT_AND_BACK;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetFrontFace(const eFrontFace face) const
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

	uint32_t GLContext::GetDrawMode(const eDrawMode mode) const
	{
		if (eDrawMode::ePointList == mode)
		{
			return GL_POINTS;
		}
		else if (eDrawMode::eLineList == mode)
		{
			return GL_LINES;
		}
		else if (eDrawMode::eLineStrip == mode)
		{
			return GL_LINE_STRIP;
		}
		else if (eDrawMode::eTriangleList == mode)
		{
			return GL_TRIANGLES;
		}
		else if (eDrawMode::eTriangleStrip == mode)
		{
			return GL_TRIANGLE_STRIP;
		}
		else if (eDrawMode::eTriangleFan == mode)
		{
			return GL_TRIANGLE_FAN;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetPrimitiveMode(const ePrimitiveMode mode) const
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

	uint32_t GLContext::GetCompareFunc(const eCompareFunc func) const
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
		else if (eCompareFunc::eGreaterEqual == func)
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

	uint32_t GLContext::GetStencilOp(const eStencilOp op) const
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

	uint32_t GLContext::GetBlendFunc(const eBlendFunc func) const
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
		else if (eBlendFunc::eConst_Color == func)
		{
			return GL_CONSTANT_COLOR;
		}
		else if (eBlendFunc::eOne_Minus_Const_Color == func)
		{
			return GL_ONE_MINUS_CONSTANT_COLOR;
		}
		else if (eBlendFunc::eConst_Alpha == func)
		{
			return GL_CONSTANT_ALPHA;
		}
		else if (eBlendFunc::eOne_Minus_Const_Alpha == func)
		{
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		}
		else if (eBlendFunc::eSrc_Alpha_Saturate == func)
		{
			return GL_SRC_ALPHA_SATURATE;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetBlendOp(const eBlendOp op) const
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

	uint32_t GLContext::GetShaderType(const eShaderType type) const
	{
		if (eShaderType::eVertex == type)
		{
			return GL_VERTEX_SHADER;
		}
		else if (eShaderType::eGeometry == type)
		{
			return GL_GEOMETRY_SHADER;
		}
		else if (eShaderType::eHull == type)
		{
			return GL_TESS_CONTROL_SHADER;
		}
		else if (eShaderType::eDomain == type)
		{
			return GL_TESS_EVALUATION_SHADER;
		}
		else if (eShaderType::ePixel == type)
		{
			return GL_FRAGMENT_SHADER;
		}
		else if (eShaderType::eCompute == type)
		{
			return GL_COMPUTE_SHADER;
		}
		else
		{
			return GL_NONE;
		}
	}

	uint32_t GLContext::GetColorAttachment(const uint32_t slot) const
	{
		uint32_t colorAttach = GL_COLOR_ATTACHMENT0;
		colorAttach += slot;

		return colorAttach;
	}

	uint32_t GLContext::GetDepthStencilAttachment(const eAttachmentType attach) const
	{
		const bool depth = (attach & eAttachmentType::eDepth) != eAttachmentType::eNone;
		const bool stencil = (attach & eAttachmentType::eStencil) != eAttachmentType::eNone;

		if (depth && stencil)
		{
			return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		else if (depth)
		{
			return GL_DEPTH_ATTACHMENT;
		}
		else if (stencil)
		{
			return GL_STENCIL_ATTACHMENT;
		}
		else
		{
			return GL_NONE;
		}
	}

	eCompareFunc GLContext::GetCompareFunc(const uint32_t func) const
	{
		if (GL_NEVER == func)
		{
			return eCompareFunc::eNever;
		}
		else if (GL_EQUAL == func)
		{
			return eCompareFunc::eEqual;
		}
		else if (GL_NOTEQUAL == func)
		{
			return eCompareFunc::eNotEqual;
		}
		else if (GL_LESS == func)
		{
			return eCompareFunc::eLess;
		}
		else if (GL_LEQUAL == func)
		{
			return eCompareFunc::eLessEqual;
		}
		else if (GL_GREATER == func)
		{
			return eCompareFunc::eGreater;
		}
		else if (GL_GEQUAL == func)
		{
			return eCompareFunc::eGreaterEqual;
		}
		else if (GL_ALWAYS == func)
		{
			return eCompareFunc::eAlways;
		}
		else
		{
			return eCompareFunc::eNone;
		}
	}

	eStencilOp GLContext::GetStencilOp(const uint32_t op) const
	{
		if (GL_KEEP == op)
		{
			return eStencilOp::eKeep;
		}
		else if (GL_ZERO == op)
		{
			return eStencilOp::eZero;
		}
		else if (GL_REPLACE == op)
		{
			return eStencilOp::eReplace;
		}
		else if (GL_INCR == op)
		{
			return eStencilOp::eInc_Clamp;
		}
		else if (GL_DECR == op)
		{
			return eStencilOp::eDec_Clamp;
		}
		else if (GL_INVERT == op)
		{
			return eStencilOp::eInvert;
		}
		else if (GL_INCR_WRAP == op)
		{
			return eStencilOp::eInc_Wrap;
		}
		else if (GL_DECR_WRAP == op)
		{
			return eStencilOp::eDec_Wrap;
		}
		else
		{
			return eStencilOp::eNone;
		}
	}

	eBlendFunc GLContext::GetBlendFunc(const uint32_t func) const
	{
		if (GL_ZERO == func)
		{
			return eBlendFunc::eZero;
		}
		else if (GL_ONE == func)
		{
			return eBlendFunc::eOne;
		}
		else if (GL_SRC_COLOR == func)
		{
			return eBlendFunc::eSrc_Color;
		}
		else if (GL_ONE_MINUS_SRC_COLOR == func)
		{
			return eBlendFunc::eOne_Minus_Src_Color;
		}
		else if (GL_DST_COLOR == func)
		{
			return eBlendFunc::eDst_Color;
		}
		else if (GL_ONE_MINUS_DST_COLOR == func)
		{
			return eBlendFunc::eOne_Minus_Dst_Color;
		}
		else if (GL_SRC_ALPHA == func)
		{
			return eBlendFunc::eSrc_Alpha;
		}
		else if (GL_ONE_MINUS_SRC_ALPHA == func)
		{
			return eBlendFunc::eOne_Minus_Src_Alpha;
		}
		else if (GL_DST_ALPHA == func)
		{
			return eBlendFunc::eDst_Alpha;
		}
		else if (GL_ONE_MINUS_DST_ALPHA == func)
		{
			return eBlendFunc::eOne_Minus_Dst_Alpha;
		}
		else if (GL_CONSTANT_COLOR == func)
		{
			return eBlendFunc::eConst_Color;
		}
		else if (GL_ONE_MINUS_CONSTANT_COLOR == func)
		{
			return eBlendFunc::eOne_Minus_Const_Color;
		}
		else if (GL_CONSTANT_ALPHA == func)
		{
			return eBlendFunc::eConst_Alpha;
		}
		else if (GL_ONE_MINUS_CONSTANT_ALPHA == func)
		{
			return eBlendFunc::eOne_Minus_Const_Alpha;
		}
		else if (GL_SRC_ALPHA_SATURATE == func)
		{
			return eBlendFunc::eSrc_Alpha_Saturate;
		}
		else
		{
			return eBlendFunc::eNone;
		}
	}

	eBlendOp GLContext::GetBlendOp(const uint32_t op) const
	{
		if (GL_FUNC_ADD == op)
		{
			return eBlendOp::eAdd;
		}
		else if (GL_FUNC_SUBTRACT == op)
		{
			return eBlendOp::eSub;
		}
		else if (GL_FUNC_REVERSE_SUBTRACT == op)
		{
			return eBlendOp::eReverse_Sub;
		}
		else if (GL_MIN == op)
		{
			return eBlendOp::eMin;
		}
		else if (GL_MAX == op)
		{
			return eBlendOp::eMax;
		}
		else
		{
			return eBlendOp::eNone;
		}
	}

	eCullFace GLContext::GetCullFace(const uint32_t face) const
	{
		if (GL_FRONT == face)
		{
			return eCullFace::eFront;
		}
		else if (GL_BACK == face)
		{
			return eCullFace::eBack;
		}
		else if (GL_FRONT_AND_BACK == face)
		{
			return eCullFace::eFront_Back;
		}
		else
		{
			return eCullFace::eNone;
		}
	}

	eFrontFace GLContext::GetFrontFace(const uint32_t face) const
	{
		if (GL_CCW == face)
		{
			return eFrontFace::eCCW;
		}
		else if (GL_CW == face)
		{
			return eFrontFace::eCW;
		}
		else
		{
			return eFrontFace::eNone;
		}
	}

	ePrimitiveMode GLContext::GetPrimitiveMode(const uint32_t mode) const
	{
		if (GL_POINT == mode)
		{
			return ePrimitiveMode::ePoint;
		}
		else if (GL_LINE == mode)
		{
			return ePrimitiveMode::eLine;
		}
		else if (GL_FILL == mode)
		{
			return ePrimitiveMode::eFill;
		}
		else
		{
			return ePrimitiveMode::eNone;
		}
	}

	uint32_t GLContext::GetBufferBarrier(const eBufferUsage usage) const
	{
		uint32_t barrier = 0;

		if ((usage & eBufferUsage::eTransfer) != eBufferUsage::eNone)
		{
			barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
		}

		if ((usage & eBufferUsage::eVertex) != eBufferUsage::eNone)
		{
			barrier |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
		}

		if ((usage & eBufferUsage::eIndex) != eBufferUsage::eNone)
		{
			barrier |= GL_ELEMENT_ARRAY_BARRIER_BIT;
		}

		if ((usage & eBufferUsage::eUniform) != eBufferUsage::eNone)
		{
			barrier |= GL_UNIFORM_BARRIER_BIT;
		}

		if ((usage & eBufferUsage::eStorage) != eBufferUsage::eNone)
		{
			barrier |= GL_SHADER_STORAGE_BARRIER_BIT;
		}

		// TODO : Divide the barrier flag between indirect draw and dispatch buffers
		if ((usage & eBufferUsage::eIndirect) != eBufferUsage::eNone)
		{
			barrier |= GL_DRAW_INDIRECT_BUFFER | GL_DISPATCH_INDIRECT_BUFFER;
		}

		return barrier;
	}

	uint32_t GLContext::GetTextureBarrier(const eTextureUsage usage) const
	{
		uint32_t barrier = 0;

		// TODO : 

		return barrier;
	}

	bool GLContext::IsInteger(const eDataType type) const
	{
		return (type == eDataType::eInt || type == eDataType::eUInt);
	}

	bool GLContext::IsInteger(const eTextureFormat format) const
	{
		return (format == eTextureFormat::eR8_UInt || 
			format == eTextureFormat::eR8G8_UInt || 
			format == eTextureFormat::eR8G8B8_UInt || 
			format == eTextureFormat::eR8G8B8A8_UInt ||
			format == eTextureFormat::eR16_UInt || 
			format == eTextureFormat::eR16G16_UInt || 
			format == eTextureFormat::eR16G16B16_UInt || 
			format == eTextureFormat::eR16G16B16A16_UInt ||
			format == eTextureFormat::eR32_UInt || 
			format == eTextureFormat::eR32G32_UInt || 
			format == eTextureFormat::eR32G32B32_UInt || 
			format == eTextureFormat::eR32G32B32A32_UInt);
	}

	void GLContext::InitializeState()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_FRAMEBUFFER_SRGB);

		SetColorState(RHIColorState());
		SetDepthState(RHIDepthState());
		SetStencilState(RHIStencilState());
		SetBlendState(RHIBlendState());
		SetRasterizerState(RHIRasterizerState());
	}

	bool GLContext::InitializeFrameBuffer(Memory::RefPtr<const RHITexture> texture, const eAttachmentType attach, const uint32_t slot)
	{
		if (!texture)
		{
			return false;
		}

		const GLTexture* glTexture = reinterpret_cast<const GLTexture*>(texture->GetRawBuffer());
		if (glTexture == nullptr || glTexture->GetID() == GL_NONE)
		{
			return false;
		}

		const bool isColor = (attach == eAttachmentType::eColor);
		const uint32_t glAttach = isColor ? GetColorAttachment(slot) : GetDepthStencilAttachment(attach);
		if (glAttach == GL_NONE)
		{
			return false;
		}

		const uint32_t level = GL_ZERO; // Default for the frame buffer;

		// Array / cube textures are attached with every layer (layered attachment)
		glFramebufferTexture(GL_FRAMEBUFFER, glAttach, glTexture->GetID(), level);

		return true;
	}

	uint32_t GLContext::InitializeTexture1D(const RHITextureCreateDesc& info)
	{
		uint32_t textureID = GL_NONE;
		const uint32_t textureType = GetTextureType(info.textureType);

		glCreateTextures(textureType, 1, &textureID);
		if (textureID == GL_NONE)
		{
			return GL_NONE;
		}

		if (!ResizeTexture1D(info, textureID))
		{
			glDeleteTextures(1, &textureID);
			LOGERROR() << "[GL] Failed to resize the texture";
			return GL_NONE;
		}

		if (!info.regions.Empty() && !UpdateTexture1D(RHITextureUpdateDesc(info), textureID))
		{
			return GL_NONE;
		}

		return textureID;
	}

	uint32_t GLContext::InitializeTexture2D(const RHITextureCreateDesc& info)
	{
		if (!ResizeTexture2D(info, textureID))
		{
			return false;
		}

		if (!info.regions.Empty() && !UpdateTexture2D(RHITextureUpdateDesc(info), textureID))
		{
			return false;
		}

		return true;
	}

	uint32_t GLContext::InitializeTexture3D(const RHITextureCreateDesc& info)
	{
		if (!ResizeTexture3D(info, textureID))
		{
			return false;
		}

		if (!info.regions.Empty() && !UpdateTexture3D(RHITextureUpdateDesc(info), textureID))
		{
			return false;
		}

		return true;
	}

	bool GLContext::UpdateTexture1D(const RHITextureUpdateDesc info, const uint32_t textureID)
	{
		if (textureID == GL_NONE)
		{
			return false;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t internalFormat = GetTextureFormat(info.textureFormat);
		const uint32_t baseFormat = GetTextureDataFormat(info.textureFormat);
		const uint32_t pixelDataType = GetTextureDataType(info.textureFormat);

		glBindTexture(textureType, textureID);

		for (const auto& region : info.regions)
		{
			const void* dataPointer = region.data->GetPointer();
			const uint32_t dataSize = region.dataSize;
			if (!dataPointer || dataSize == 0 || region.level >= info.level)
			{
				continue;
			}

			const uint32_t width = 

			if (info.compressed)
			{
				glCompressedTexSubImage1D(textureType, region.level, 0, width, internalFormat, dataSize, dataPointer);
			}
			else
			{
				glTexSubImage1D(textureType, region.level, 0, width, baseFormat, pixelDataType, dataPointer);
			}
		}

		if (info.generateMips)
		{
			glGenerateMipmap(textureType);
		}

		glBindTexture(textureType, GL_NONE);

		return true;
	}

	bool GLContext::UpdateTexture2D(const RHITextureUpdateDesc info, const uint32_t textureID)
	{
		if (textureID == GL_NONE)
		{
			return false;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t internalFormat = GetTextureFormat(info.textureFormat);
		const uint32_t baseFormat = GetTextureDataFormat(info.textureFormat);
		const uint32_t pixelDataType = GetTextureDataType(info.textureFormat);

		const bool isArray = (textureType == GL_TEXTURE_1D_ARRAY);
		const bool isCubeMap = (textureType == GL_TEXTURE_CUBE_MAP);
		const bool isMulti = (textureType == GL_TEXTURE_2D_MULTISAMPLE);

		if (isMulti)
		{
			LOGERROR() << "[GL] Failed to update the texture, the multisample texture can not be uploaded";
			return false;
		}

		glBindTexture(textureType, textureID);

		for (const auto& region : info.regions)
		{
			const void* dataPointer = GetRegionPointer(region);
			const uint32_t dataSize = GetRegionSize(region);
			if (!dataPointer || dataSize == 0 || region.level >= info.level)
			{
				continue;
			}

			if ((isCubeMap && region.face >= CUBEMAP_FACE_COUNT) || (isArray && region.layer >= info.layer))
			{
				continue;
			}

			const uint32_t target = isCubeMap ? TEXTURE_CUBEMAP_FACES[region.face] : textureType;
			const uint32_t width = GetMipSize(info.width, region.level);
			const uint32_t height = isArray ? 1 : GetMipSize(info.height, region.level);
			const uint32_t yOffset = isArray ? region.layer : 0;

			if (info.compressed)
			{
				glCompressedTexSubImage2D(target, region.level, 0, yOffset, width, height, internalFormat, dataSize, dataPointer);
			}
			else
			{
				glTexSubImage2D(target, region.level, 0, yOffset, width, height, baseFormat, pixelDataType, dataPointer);
			}
		}

		if (info.generateMips)
		{
			glGenerateMipmap(textureType);
		}

		glBindTexture(textureType, GL_NONE);

		return true;
	}

	bool GLContext::UpdateTexture3D(const RHITextureUpdateDesc info, const uint32_t textureID)
	{
		if (textureID == GL_NONE)
		{
			return false;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t internalFormat = GetTextureFormat(info.textureFormat);
		const uint32_t baseFormat = GetTextureDataFormat(info.textureFormat);
		const uint32_t pixelDataType = GetTextureDataType(info.textureFormat);

		const bool isArray = (textureType == GL_TEXTURE_2D_ARRAY);
		const bool isCubeMap = (textureType == GL_TEXTURE_CUBE_MAP_ARRAY);
		const bool isMulti = (textureType == GL_TEXTURE_2D_MULTISAMPLE_ARRAY);

		if (isMulti)
		{
			LOGERROR() << "[GL] Failed to update the texture, the multisample texture can not be uploaded";
			return false;
		}

		glBindTexture(textureType, textureID);

		for (const auto& region : info.regions)
		{
			const void* dataPointer = GetRegionPointer(region);
			const uint32_t dataSize = GetRegionSize(region);
			if (!dataPointer || dataSize == 0 || region.level >= info.level)
			{
				continue;
			}

			if ((isArray || isCubeMap) && region.layer >= info.layer)
			{
				continue;
			}

			if (isCubeMap && region.face >= CUBEMAP_FACE_COUNT)
			{
				continue;
			}

			const uint32_t width = GetMipSize(info.width, region.level);
			const uint32_t height = GetMipSize(info.height, region.level);

			// Array : one layer per region, cube array : layer * 6 + face
			uint32_t zOffset = 0;
			uint32_t depth = GetMipSize(info.depth, region.level);
			if (isArray)
			{
				zOffset = region.layer;
				depth = 1;
			}
			else if (isCubeMap)
			{
				zOffset = region.layer * CUBEMAP_FACE_COUNT + region.face;
				depth = 1;
			}

			if (info.compressed)
			{
				glCompressedTexSubImage3D(textureType, region.level, 0, 0, zOffset, width, height, depth, internalFormat, dataSize, dataPointer);
			}
			else
			{
				glTexSubImage3D(textureType, region.level, 0, 0, zOffset, width, height, depth, baseFormat, pixelDataType, dataPointer);
			}
		}

		if (info.generateMips)
		{
			glGenerateMipmap(textureType);
		}

		glBindTexture(textureType, GL_NONE);

		return true;
	}

	bool GLContext::ResizeTexture1D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		if (textureID == GL_NONE)
		{
			return false;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t internalFormat = GetTextureFormat(info.textureFormat);
		const uint32_t baseFormat = GetTextureDataFormat(info.textureFormat);
		const uint32_t pixelDataType = GetTextureDataType(info.textureFormat);

		glBindTexture(textureType, textureID);

		for (uint32_t mipLevel = 0; mipLevel < info.level; mipLevel++)
		{
			const uint32_t width = GetMipSize(info.width, mipLevel);
			glTexImage1D(textureType, mipLevel, internalFormat, width, 0, baseFormat, pixelDataType, nullptr);
		}

		glBindTexture(textureType, GL_NONE);

		return true;
	}

	bool GLContext::ResizeTexture2D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		if (textureID == GL_NONE)
		{
			return false;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t internalFormat = GetTextureFormat(info.textureFormat);
		const uint32_t baseFormat = GetTextureDataFormat(info.textureFormat);
		const uint32_t pixelDataType = GetTextureDataType(info.textureFormat);

		const bool isArray = (textureType == GL_TEXTURE_1D_ARRAY);
		const bool isCubeMap = (textureType == GL_TEXTURE_CUBE_MAP);
		const bool isMulti = (textureType == GL_TEXTURE_2D_MULTISAMPLE);

		glBindTexture(textureType, textureID);

		if (isMulti)
		{
			glTexImage2DMultisample(textureType, info.sample, internalFormat, info.width, info.height, GL_TRUE);
		}
		else
		{
			for (uint32_t mipLevel = 0; mipLevel < info.level; mipLevel++)
			{
				const uint32_t width = GetMipSize(info.width, mipLevel);
				const uint32_t height = isArray ? info.layer : GetMipSize(info.height, mipLevel);

				if (isCubeMap)
				{
					for (uint32_t faceIndex = 0; faceIndex < CUBEMAP_FACE_COUNT; ++faceIndex)
					{
						glTexImage2D(TEXTURE_CUBEMAP_FACES[faceIndex], mipLevel, internalFormat, width, height, 0, baseFormat, pixelDataType, nullptr);
					}
				}
				else
				{
					glTexImage2D(textureType, mipLevel, internalFormat, width, height, 0, baseFormat, pixelDataType, nullptr);
				}
			}
		}

		glBindTexture(textureType, GL_NONE);

		return true;
	}

	bool GLContext::ResizeTexture3D(const RHITextureCreateDesc info, const uint32_t textureID)
	{
		if (textureID == GL_NONE)
		{
			return false;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t internalFormat = GetTextureFormat(info.textureFormat);
		const uint32_t baseFormat = GetTextureDataFormat(info.textureFormat);
		const uint32_t pixelDataType = GetTextureDataType(info.textureFormat);

		const bool isArray = (textureType == GL_TEXTURE_2D_ARRAY);
		const bool isCubeMap = (textureType == GL_TEXTURE_CUBE_MAP_ARRAY);
		const bool isMulti = (textureType == GL_TEXTURE_2D_MULTISAMPLE_ARRAY);

		glBindTexture(textureType, textureID);

		if (isMulti)
		{
			glTexImage3DMultisample(textureType, info.sample, internalFormat, info.width, info.height, info.layer, GL_TRUE);
		}
		else
		{
			for (uint32_t mipLevel = 0; mipLevel < info.level; mipLevel++)
			{
				const uint32_t width = GetMipSize(info.width, mipLevel);
				const uint32_t height = GetMipSize(info.height, mipLevel);

				uint32_t depth = GetMipSize(info.depth, mipLevel);
				if (isArray)
				{
					depth = info.layer;
				}
				else if (isCubeMap)
				{
					depth = info.layer * CUBEMAP_FACE_COUNT;
				}

				glTexImage3D(textureType, mipLevel, internalFormat, width, height, depth, 0, baseFormat, pixelDataType, nullptr);
			}
		}

		glBindTexture(textureType, GL_NONE);

		return true;
	}

	void GLContext::SetColorState(const RHIColorState& state)
	{
		GLboolean red = state.red ? GL_TRUE : GL_FALSE;
		GLboolean green = state.green ? GL_TRUE : GL_FALSE;
		GLboolean blue = state.blue ? GL_TRUE : GL_FALSE;
		GLboolean alpha = state.alpha ? GL_TRUE : GL_FALSE;

		glColorMask(red, green, blue, alpha);
	}

	void GLContext::SetDepthState(const RHIDepthState& state)
	{
		if (state.enable)
		{
			GLboolean writeMask = state.write ? GL_TRUE : GL_FALSE;
			uint32_t func = GetCompareFunc(state.func);

			glEnable(GL_DEPTH_TEST);

			glDepthMask(writeMask);
			glDepthFunc(func);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLContext::SetStencilState(const RHIStencilState& state)
	{
		if (state.enable)
		{
			uint32_t func = GetCompareFunc(state.func);
			uint32_t failOp = GetStencilOp(state.failOp);
			uint32_t depthFailOp = GetStencilOp(state.depthFailOp);
			uint32_t passOp = GetStencilOp(state.passOp);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(func, state.refMask, state.readMask);
			glStencilMask(state.writeMask);
			glStencilOp(failOp, depthFailOp, passOp);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	void GLContext::SetBlendState(const RHIBlendState& state)
	{
		if (state.enable)
		{
			uint32_t srcColor = GetBlendFunc(state.srcColor);
			uint32_t destColor = GetBlendFunc(state.destColor);
			uint32_t srcAlpha = GetBlendFunc(state.srcAlpha);
			uint32_t destAlpha = GetBlendFunc(state.destAlpha);

			uint32_t colorOp = GetBlendOp(state.colorOp);
			uint32_t alphaOp = GetBlendOp(state.alphaOp);

			glEnable(GL_BLEND);

			glBlendFuncSeparate(srcColor, destColor, srcAlpha, destAlpha);
			glBlendEquationSeparate(colorOp, alphaOp);
			glColorMask(
				(state.writeMask & 0x1) ? GL_TRUE : GL_FALSE,
				(state.writeMask & 0x2) ? GL_TRUE : GL_FALSE,
				(state.writeMask & 0x4) ? GL_TRUE : GL_FALSE,
				(state.writeMask & 0x8) ? GL_TRUE : GL_FALSE
			);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLContext::SetRasterizerState(const RHIRasterizerState& state)
	{
		const uint32_t frontFace = GetFrontFace(state.frontFace);
		const uint32_t fillMode = GetPrimitiveMode(state.fillMode);

		glFrontFace(frontFace);
		glPolygonMode(GL_FRONT_AND_BACK, fillMode);

		if (state.cullEnable)
		{
			const uint32_t cullFace = GetCullFace(state.cullFace);

			glEnable(GL_CULL_FACE);
			glCullFace(cullFace);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}
