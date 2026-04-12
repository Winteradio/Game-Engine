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
	struct BufferDesc
	{
		GLenum enumType;
		eBindingType bindingType;
	};

	const BufferDesc BUFFER_BINDINGS[] =
	{
		{ GL_UNIFORM_BLOCK, eBindingType::eUniformBuffer },
		{ GL_SHADER_STORAGE_BLOCK, eBindingType::eStorageBuffer },
		{ GL_UNIFORM, eBindingType::eNone }
	};

	constexpr size_t MAX_GL_MESSAGE_LENGTH = 512;
	char GL_MESSAGE[MAX_GL_MESSAGE_LENGTH];

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

	Memory::RefPtr<RHIVertexLayout> GLSystem::CreateVertexLayout(const RHIVertexLayoutDesc desc)
	{
		Memory::RefPtr<RHIVertexLayout> refVertexLayout = Memory::MakeRef<GLVertexLayout>(desc);
	
		return refVertexLayout;
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

	Memory::RefPtr<RHIShader> GLSystem::CreateVertexShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refVertexShader = Memory::MakeRef<GLVertexShader>(desc);

		return refVertexShader;
	}

	Memory::RefPtr<RHIShader> GLSystem::CreateGeometryShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refGeometryShader = Memory::MakeRef<GLGeometryShader>(desc);

		return refGeometryShader;
	}

	Memory::RefPtr<RHIShader> GLSystem::CreateHullShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refHullShader = Memory::MakeRef<GLHullShader>(desc);
	
		return refHullShader;
	}

	Memory::RefPtr<RHIShader> GLSystem::CreatePixelShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refPixelShader = Memory::MakeRef<GLPixelShader>(desc);

		return refPixelShader;
	}

	Memory::RefPtr<RHIShader> GLSystem::CreateComputeShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refComputeShader = Memory::MakeRef<GLComputeShader>(desc);

		return refComputeShader;
	}

	Memory::RefPtr<RHIPipeLine> GLSystem::CreatePipeLine(const RHIPipeLineDesc desc)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = Memory::MakeRef<GLPipeLine>(desc);

		return refPipeLine;
	}

	void GLSystem::InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer || !info.data)
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
		const uint32_t dataType = GetDataType(info.componentType);
		const uint32_t dataSize = info.size;
		
		uint32_t bufferID = GL_NONE;
		glGenBuffers(1, &bufferID);

		if (bufferID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to generate buffer";
			return;
		}

		glBindBuffer(bufferType, bufferID);
		glBufferData(bufferType, dataSize, info.data, accessType);
		glBindBuffer(bufferType, GL_NONE);

		glBuffer->SetID(bufferID);
		glBuffer->SetState(eResourceState::eReady);
	}

	void GLSystem::InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (!info.vertexStreams.Empty() && !info.indexBuffer || !layout)
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
			return;
		}

		glBindVertexArray(vertexLayoutID);
		for (const auto& [vertexKey, vertexStream] : info.vertexStreams)
		{
			const uint32_t location = vertexStream.attribute.location;
			const uint64_t offset = static_cast<uint64_t>(vertexStream.attribute.offset);
			const uint32_t normalized = vertexStream.attribute.normalized ? GL_TRUE : GL_FALSE;
			const uint32_t integer = vertexStream.attribute.integer ? GL_TRUE : GL_FALSE;
			const uint32_t divisor = vertexStream.attribute.divisor;
			const uint32_t numComponents = vertexStream.attribute.numComponents;
			const uint32_t componentType = GetDataType(vertexStream.attribute.componentType);
			const uint32_t componentSize = GetDataTypeSize(vertexStream.attribute.componentType);
			const uint32_t stride = numComponents * componentSize;

			const GLBuffer* glBuffer = reinterpret_cast<const GLBuffer*>(vertexStream.buffer->GetRawBuffer());
			if (!glBuffer)
			{
				LOGERROR() << "[GL] Failed to get vertex stream buffer";
				continue;
			}

			glBindBuffer(GL_ARRAY_BUFFER, glBuffer->GetID());
			glEnableVertexAttribArray(location);

			if (integer == GL_TRUE)
			{
				glVertexAttribIPointer(location, numComponents, componentType, stride, reinterpret_cast<void*>(offset));
			}
			else
			{
				glVertexAttribPointer(location, numComponents, componentType, normalized, stride, reinterpret_cast<void*>(offset));
			}

			glVertexAttribDivisor(location, vertexStream.attribute.divisor);
		}

		const GLBuffer* glIndexBuffer = reinterpret_cast<const GLBuffer*>(info.indexBuffer->GetRawBuffer());
		if (glIndexBuffer)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndexBuffer->GetID());
		}

		glBindVertexArray(GL_NONE);
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

		glVertexLayout->SetID(vertexLayoutID);
		glVertexLayout->SetState(eResourceState::eReady);
	}

	void GLSystem::InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
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

		uint32_t textureID = GL_NONE;
		glGenTextures(1, &textureID);
		if (textureID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to generate texture";
			return;
		}

		const uint32_t textureType = GetTextureType(info.textureType);
		const uint32_t pixelFormat = GetPixelFormat(info.format);
		const uint32_t dataType = GetDataType(info.dataType);

		glBindTexture(textureType, textureID);
		if (info.compressed)
		{
			glCompressedTexImage2D(textureType, 0, pixelFormat, info.width, info.height, 0, static_cast<GLsizei>(info.dataType), info.data);
		}
		else
		{
			glTexImage2D(textureType, 0, pixelFormat, info.width, info.height, 0, pixelFormat, dataType, info.data);
		}

		if (info.generateMips)
		{
			glGenerateMipmap(textureType);
		}
		else
		{
			glTexParameteri(textureType, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(textureType, GL_TEXTURE_MAX_LEVEL, 0);
		}

		glTexture->SetID(textureID);
		glTexture->SetState(eResourceState::eReady);
	}

	void GLSystem::InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler)
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
			return;
		}

		glSampler->SetID(samplerID);
		glSampler->SetState(eResourceState::eReady);
	}

	void GLSystem::InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader)
	{
		if (!shader)
		{
			return;
		}

		GLShader* glShader = reinterpret_cast<GLShader*>(shader->GetRawBuffer());
		if (!glShader)
		{
			return;
		}

		const char* shaderSource = reinterpret_cast<const char*>(info.data);
		if (!shaderSource)
		{
			LOGERROR() << "[GL] Failed to get vertex shader source code";
			return;
		}

		const uint32_t shaderType = GetShaderType(shader->GetShaderType());
		if (shaderType == GL_NONE)
		{
			LOGERROR() << "[GL] Unrecognized shader type: " << static_cast<uint32_t>(shader->GetShaderType());
			return;
		}

		const uint32_t shaderID = glCreateShader(shaderType);
		const int32_t length = static_cast<int32_t>(info.dataSize);

		glShaderSource(shaderID, 1, &shaderSource, &length);
		glCompileShader(shaderID);

		int32_t success = GL_NONE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, MAX_GL_MESSAGE_LENGTH, nullptr, GL_MESSAGE);

			LOGERROR() << "[GL] Failed to compile shader: " << GL_MESSAGE;
			glDeleteShader(shaderID);

			return;
		}

		glShader->SetID(shaderID);
		glShader->SetState(eResourceState::eReady);
	}

	void GLSystem::InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline)
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

		GLuint programID = glCreateProgram();
		if (programID == GL_NONE)
		{
			LOGERROR() << "[GL] Failed to create shader program";
			return;
		}

		auto AttachShader = [&](const Memory::RefPtr<const RHIShader>& shader)
		{
			if (!shader)
			{
				return;
			}

			const GLShader* glShader = reinterpret_cast<const GLShader*>(shader->GetRawBuffer());
			if (!glShader || glShader->GetID() == GL_NONE)
			{
				return;
			}

			glAttachShader(programID, glShader->GetID());
		};

		AttachShader(info.vertexShader);
		AttachShader(info.geometryShader);
		AttachShader(info.hullShader);
		AttachShader(info.pixelShader);
		AttachShader(info.computeShader);

		glLinkProgram(programID);

		GLint success = GL_NONE;
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, MAX_GL_MESSAGE_LENGTH, nullptr, GL_MESSAGE);
			LOGERROR() << "[GL] Failed to link shader program: " << GL_MESSAGE;
			glDeleteProgram(programID);
			return;
		}

		glPipeLine->SetID(programID);

		if (!InitializeAttribute(pipeline) || !InitializeSlot(pipeline))
		{
			glDeleteProgram(programID);

			glPipeLine->SetID(GL_NONE);
			glPipeLine->SetState(eResourceState::eError);
		}
		else
		{
			glPipeLine->SetState(eResourceState::eReady);
		}
	}

	bool GLSystem::InitializeAttribute(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (!pipeline)
		{
			return false;
		}

		GLPipeLine* glPipeLine = reinterpret_cast<GLPipeLine*>(pipeline->GetRawBuffer());
		if (!glPipeLine || glPipeLine->GetID() == GL_NONE)
		{
			return false;
		}

		const uint32_t programID = glPipeLine->GetID();

		GLint attributeCount = GL_NONE;
		glGetProgramInterfaceiv(programID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &attributeCount);

		for (GLint index = 0; index < attributeCount; ++index)
		{
			GLsizei nameLength = GL_NONE;
			glGetProgramResourceName(programID, GL_PROGRAM_INPUT, index, MAX_GL_MESSAGE_LENGTH, &nameLength, GL_MESSAGE);

			const std::string attributeName(GL_MESSAGE, nameLength);
			if (nameLength == GL_NONE || attributeName.empty())
			{
				LOGWARN() << "[GL] Failed to get vertex attribute name for index: " << index;
				return false;
			}

			if (attributeName.substr(0, 3) == "gl_")
			{
				continue;
			}

			const VertexKey vertexKey = GetVertexKey(attributeName);
			if (vertexKey.semantic == eVertexSemantic::eNone || vertexKey.semanticIndex == 0xFF)
			{
				LOGWARN() << "[GL] Unrecognized vertex attribute: " << attributeName;
				return false;
			}

			GLint location = -1;
			const GLenum locationProp = { GL_LOCATION };

			const uint32_t expectedLocation = GetVertexLocation(vertexKey);

			glGetProgramResourceiv(programID, GL_PROGRAM_INPUT, index, 1, &locationProp, 1, nullptr, &location);

			if (static_cast<uint32_t>(location) != expectedLocation)
			{
				LOGWARN() << "[GL] Vertex attribute location mismatch: " << attributeName << ", expected location: " << expectedLocation << ", actual location: " << location;

				return false;
			}
		}

		return true;
	}

	bool GLSystem::InitializeSlot(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (!pipeline)
		{
			return false;
		}

		GLPipeLine* glPipeLine = reinterpret_cast<GLPipeLine*>(pipeline->GetRawBuffer());
		if (!glPipeLine || glPipeLine->GetID() == GL_NONE)
		{
			return false;
		}

		const uint32_t programID = glPipeLine->GetID();

		for (const auto& desc : BUFFER_BINDINGS)
		{
			GLint bindingCount = GL_NONE;
			glGetProgramInterfaceiv(programID, desc.enumType, GL_ACTIVE_RESOURCES, &bindingCount);

			for (GLint index = 0; index < bindingCount; ++index)
			{
				GLsizei nameLength = GL_NONE;
				glGetProgramResourceName(programID, desc.enumType, index, MAX_GL_MESSAGE_LENGTH, &nameLength, GL_MESSAGE);
			
				const std::string blockName(GL_MESSAGE, nameLength);
				if (nameLength == GL_NONE || blockName.empty())
				{
					continue;
				}

				RHIResourceBinding bindingSlot;
				bindingSlot.location = GL_NONE;
				bindingSlot.type = eBindingType::eNone;

				if (desc.enumType == GL_UNIFORM)
				{
					const GLenum props[] = { GL_TYPE, GL_BLOCK_INDEX, GL_LOCATION };
					GLint results[3] = { GL_NONE, GL_NONE, GL_NONE };

					glGetProgramResourceiv(programID, desc.enumType, index, 3, props, 3, nullptr, results);

					const bool bufferBlock = results[1] != -1;
					if (bufferBlock || results[0] == -1)
					{
						continue;
					}

					if (IsSampler(results[0]))
					{
						bindingSlot.location = results[2];
						bindingSlot.type = eBindingType::eSampler;
					}
					else
					{
						bindingSlot.location = results[2];
						bindingSlot.type = eBindingType::eUniform;
					}
				}
				else
				{
					GLint bindingIndex = GL_NONE;
					const GLenum props = GL_BUFFER_BINDING;
					glGetProgramResourceiv(programID, desc.enumType, index, 1, &props, 1, nullptr, &bindingIndex);

					if (bindingIndex == -1)
					{
						continue;
					}

					bindingSlot.location = static_cast<uint16_t>(bindingIndex);
					bindingSlot.type = desc.bindingType;
				}

				pipeline->AddSlot(blockName, bindingSlot);
			}
		}

		return true;
	}

	void GLSystem::UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer)
		{
			return;
		}

		if (!info.data)
		{
			LOGERROR() << "[GL] Failed to update buffer, cause the data pointer is null";
			return;
		}

		if ((info.dataOffset + info.dataSize) > buffer->GetSize())
		{
			LOGERROR() << "[GL] Buffer size is smaller than the data size, resizing the buffer";
			return;
		}

		GLBuffer* glBuffer = reinterpret_cast<GLBuffer*>(buffer->GetRawBuffer());
		if (!glBuffer)
		{
			return;
		}

		const uint32_t bufferType = GetBufferType(info.bufferType);
		const uint32_t accessType = GetMapAccess(info.mapAccess);
		const bool useMapBuffer = accessType != GL_NONE;
		glBindBuffer(bufferType, glBuffer->GetID());

		if (useMapBuffer)
		{
			void* mappedBuffer = glMapBufferRange(bufferType, info.dataOffset, info.dataSize, accessType);
			if (mappedBuffer)
			{
				std::memcpy(mappedBuffer, info.data, info.dataSize);
			}
			else
			{
				LOGERROR() << "[GL] Failed to map buffer for update";
			}

			glUnmapBuffer(bufferType);
		}
		else
		{
			glBufferSubData(bufferType, info.dataOffset, info.dataSize, info.data);
		}

		glBindBuffer(bufferType, GL_NONE);
	}

	void GLSystem::UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (!texture)
		{
			return;
		}
	}

	void GLSystem::ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer)
		{
			return;
		}

		GLBuffer* glBuffer = reinterpret_cast<GLBuffer*>(buffer->GetRawBuffer());
		if (!glBuffer || (glBuffer->GetID() == GL_NONE))
		{
			return;
		}

		const uint32_t bufferType = GetBufferType(info.bufferType);
		const uint32_t accessType = GetDataAccess(info.accessType);
		const uint32_t dataType = GetDataType(info.componentType);
		const uint32_t dataSize = info.size;

		glBindBuffer(bufferType, glBuffer->GetID());
		glBufferData(bufferType, dataSize, info.data, accessType);
		glBindBuffer(bufferType, GL_NONE);
	}

	void GLSystem::ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (!texture)
		{
			return;
		}
	}

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

	void GLSystem::RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (!layout)
		{
			return;
		}
	
		GLVertexLayout* glVertexLayout = reinterpret_cast<GLVertexLayout*>(layout->GetRawBuffer());
		if (!glVertexLayout)
		{
			return;
		}
		
		const uint32_t bufferID = glVertexLayout->GetID();
		glDeleteVertexArrays(1, &bufferID);
		
		glVertexLayout->SetID(GL_NONE);
		glVertexLayout->SetState(eResourceState::eNone);
	}

	void GLSystem::RemoveTexture(Memory::RefPtr<RHITexture> texture)
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

		const uint32_t textureID = glTexture->GetID();
		glDeleteTextures(1, &textureID);
		glTexture->SetID(GL_NONE);
		glTexture->SetState(eResourceState::eNone);
	}

	void GLSystem::RemoveSampler(Memory::RefPtr<RHISampler> sampler)
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

		const uint32_t samplerID = glSampler->GetID();
		glDeleteSamplers(1, &samplerID);
		glSampler->SetID(GL_NONE);
		glSampler->SetState(eResourceState::eNone);
	}

	void GLSystem::RemoveShader(Memory::RefPtr<RHIShader> shader)
	{
		if (!shader)
		{
			return;
		}

		GLShader* glShader = reinterpret_cast<GLShader*>(shader->GetRawBuffer());
		if (!glShader)
		{
			return;
		}

		glDeleteShader(glShader->GetID());

		glShader->SetID(GL_NONE);
		shader->SetState(eResourceState::eNone);
	}

	void GLSystem::RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
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

		glDeleteProgram(glPipeLine->GetID());
		glPipeLine->SetID(GL_NONE);
		glPipeLine->SetState(eResourceState::eNone);
	}

	void GLSystem::SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot)
	{
		if (!buffer)
		{
			return;
		}
	
		const GLBuffer* glBuffer = reinterpret_cast<const GLBuffer*>(buffer->GetRawBuffer());
		if (!glBuffer)
		{
			return;
		}

		const uint32_t bufferType = GetBufferType(buffer->GetBufferType());

		glBindBufferBase(bufferType, slot, glBuffer->GetID());
	}

	void GLSystem::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		if (!layout)
		{
			return;
		}

		const GLVertexLayout* glVertexLayout = reinterpret_cast<const GLVertexLayout*>(layout->GetRawBuffer());
		if (!glVertexLayout)
		{
			return;
		}

		glBindVertexArray(glVertexLayout->GetID());
	}

	void GLSystem::SetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot)
	{
		if (!texture)
		{
			return;
		}

		const GLTexture* glTexture = reinterpret_cast<const GLTexture*>(texture->GetRawBuffer());
		if (!glTexture)
		{
			return;
		}

		const uint32_t textureType = GetTextureType(texture->GetTextureType());
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(textureType, glTexture->GetID());
	}

	void GLSystem::SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot)
	{
		if (!sampler)
		{
			return;
		}

		const GLSampler* glSampler = reinterpret_cast<const GLSampler*>(sampler->GetRawBuffer());
		if (!glSampler)
		{
			return;
		}

		glBindSampler(slot, glSampler->GetID());
	}

	void GLSystem::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		if (!pipeline)
		{
			return;
		}

		const GLPipeLine* glPipeLine = reinterpret_cast<const GLPipeLine*>(pipeline->GetRawBuffer());
		if (!glPipeLine)
		{
			return;
		}

		glUseProgram(glPipeLine->GetID());

		SetColorState(pipeline->GetColorState());
		SetDepthState(pipeline->GetDepthState());
		SetBlendState(pipeline->GetBlendState());
		SetRasterizerState(pipeline->GetRasterizerState());
		SetStencilState(pipeline->GetStencilState());
	}

	void GLSystem::UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot)
	{
		if (!buffer)
		{
			return;
		}

		const uint32_t bufferType = GetBufferType(buffer->GetBufferType());
		glBindBufferBase(bufferType, slot, GL_NONE);
	}

	void GLSystem::UnsetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		glBindVertexArray(GL_NONE);
	}

	void GLSystem::UnsetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot)
	{
		if (!texture)
		{
			return;
		}

		const uint32_t textureType = GetTextureType(texture->GetTextureType());
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(textureType, GL_NONE);
	}

	void GLSystem::UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot)
	{
		if (!sampler)
		{
			return;
		}

		glBindSampler(slot, GL_NONE);
	}

	void GLSystem::UnsetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		glUseProgram(GL_NONE);
	}

	void GLSystem::DispatchCompute(const RHIDispatchDesc info)
	{
		glDispatchCompute(info.groupX, info.groupY, info.groupZ);
	}

	void GLSystem::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
		if (info.indexCount == 0 || info.instanceCount == 0)
		{
			return;
		}

		const uint32_t drawMode = GetDrawMode(info.drawMode);
		const uint32_t indexType = GetDataType(info.indexType);

		const bool instancing = info.instanceCount > 1;
		const bool baseVertex = info.baseVertex != 0;

		if (baseVertex)
		{
			if (instancing)
			{
				glDrawElementsInstancedBaseVertex(
					drawMode,
					info.indexCount,
					indexType,
					reinterpret_cast<void*>(static_cast<uint64_t>(info.indexOffset)),
					info.instanceCount,
					info.baseVertex
				);
			}
			else
			{
				glDrawElementsInstanced(
					drawMode,
					info.indexCount,
					indexType,
					reinterpret_cast<void*>(static_cast<uint64_t>(info.indexOffset)),
					info.instanceCount
				);
			}
		}
		else
		{
			if (instancing)
			{
				glDrawElementsInstanced(
					drawMode,
					info.indexCount,
					indexType,
					reinterpret_cast<void*>(static_cast<uint64_t>(info.indexOffset)),
					info.instanceCount
				);
			}
			else
			{
				glDrawElements(
					drawMode, 
					info.indexCount, 
					indexType, 
					reinterpret_cast<void*>(static_cast<uint64_t>(info.indexOffset))
				);
			}
		}
	}

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

	const uint32_t GLSystem::GetMapAccess(const eMapAccess access) const
	{
		GLbitfield accessFlags = GL_NONE;

		if (eMapAccess::eRead == (access & eMapAccess::eRead))
		{
			accessFlags |= GL_MAP_READ_BIT;
		}

		if (eMapAccess::eWrite == (access & eMapAccess::eWrite))
		{
			accessFlags |= GL_MAP_WRITE_BIT;
		}

		if (eMapAccess::eInvalidateRange == (access & eMapAccess::eInvalidateRange))
		{
			accessFlags |= GL_MAP_INVALIDATE_RANGE_BIT;
		}

		if (eMapAccess::eInvalidateBuffer == (access & eMapAccess::eInvalidateBuffer))
		{
			accessFlags |= GL_MAP_INVALIDATE_BUFFER_BIT;
		}

		if (eMapAccess::eFlushExplicit == (access & eMapAccess::eFlushExplicit))
		{
			accessFlags |= GL_MAP_FLUSH_EXPLICIT_BIT;
		}

		return accessFlags;
	}

	const uint32_t GLSystem::GetTextureType(const eTextureType type) const
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

	const uint32_t GLSystem::GetShaderType(const eShaderType type) const
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

	bool GLSystem::IsSampler(const int32_t type) const
	{
		return type == GL_SAMPLER_1D ||
			type == GL_SAMPLER_2D ||
			type == GL_SAMPLER_3D ||
			type == GL_SAMPLER_CUBE ||
			type == GL_SAMPLER_1D_ARRAY ||
			type == GL_SAMPLER_2D_ARRAY ||
			type == GL_SAMPLER_CUBE_MAP_ARRAY ||
			type == GL_SAMPLER_2D_MULTISAMPLE ||
			type == GL_SAMPLER_2D_MULTISAMPLE_ARRAY;
	}
}