#include <RHI/RHIUtils.h>

#include <Renderer/RenderTypes.h>
#include <RHI/RHIResources.h>
#include <Log/include/Log.h>

#include <shaderc/shaderc.h>
#include <spirv_reflect.h>

namespace wtr
{
	namespace RHIUtils
	{
		eRenderType RENDER_TYPE = eRenderType::eNone;

		shaderc_shader_kind GetShaderKind(const eShaderType type)
		{
			switch (type)
			{
			case eShaderType::eVertex:
				return shaderc_glsl_vertex_shader;

			case eShaderType::eGeometry:
				return shaderc_glsl_geometry_shader;

			case eShaderType::eHull:
				return shaderc_glsl_tess_control_shader;

			case eShaderType::eDomain:
				return shaderc_glsl_tess_evaluation_shader;

			case eShaderType::ePixel:
				return shaderc_glsl_fragment_shader;

			case eShaderType::eCompute:
				return shaderc_glsl_compute_shader;

			default:
				return shaderc_glsl_infer_from_source;
			}
		}

		shaderc_target_env GetTargetEnv()
		{
			if (RENDER_TYPE == eRenderType::eOpenGL)
			{
				return shaderc_target_env_opengl;

			}
			else if (RENDER_TYPE == eRenderType::eVulkan)
			{
				return shaderc_target_env_vulkan;
			}
			else
			{
				LOGERROR() << "[RHIUtils] Unsupported render type for shader compilation.";
				return shaderc_target_env_default;
			}
		}

		shaderc_env_version GetTargetVersion()
		{
			// TODO : Make the minor version configurable based on the API version. For now, we will use the major version only.

			if (RENDER_TYPE == eRenderType::eOpenGL)
			{
				return shaderc_env_version_opengl_4_5; // Default to OpenGL 4.5 for now
			}
			else if (RENDER_TYPE == eRenderType::eVulkan)
			{
				return shaderc_env_version_vulkan_1_4; // Default to Vulkan 1.4 for now
			}
			else
			{
				LOGERROR() << "[RHIUtils] Unsupported render type for shader compilation.";
				return shaderc_env_version_opengl_4_5; // Default to OpenGL 4.5 if the render type is not recognized
			}
		}

		eBindingType GetBindingType(const SpvReflectDescriptorType descriptorType)
		{
			switch (descriptorType)
			{
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				return eBindingType::eSampler;
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				return eBindingType::eUniformBuffer;
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				return eBindingType::eStorageBuffer;
			default:
				LOGERROR() << "[RHIUtils] Unsupported descriptor type for binding: " << static_cast<uint32_t>(descriptorType);
				return eBindingType::eNone;
			}
		}

		eDataType GetDataType(const SpvReflectFormat format)
		{
			switch (format)
			{
			case SPV_REFLECT_FORMAT_R32_SFLOAT:
			case SPV_REFLECT_FORMAT_R32G32_SFLOAT:
			case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:
			case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:
				return eDataType::eFloat;
			case SPV_REFLECT_FORMAT_R32_SINT:
			case SPV_REFLECT_FORMAT_R32G32_SINT:
			case SPV_REFLECT_FORMAT_R32G32B32_SINT:
			case SPV_REFLECT_FORMAT_R32G32B32A32_SINT:
				return eDataType::eInt;
			case SPV_REFLECT_FORMAT_R32_UINT:
			case SPV_REFLECT_FORMAT_R32G32_UINT:
			case SPV_REFLECT_FORMAT_R32G32B32_UINT:
			case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:
				return eDataType::eUInt;
			default:
				LOGERROR() << "[RHIUtils] Unsupported format for data type: " << static_cast<uint32_t>(format);
				return eDataType::eNone;
			}
		}

		RHIShaderVariable GetShaderVariable(const SpvReflectInterfaceVariable* variable)
		{
			if (variable == nullptr)
			{
				return {};
			}

			SpvReflectTypeDescription* typeDesc = variable->type_description;
			if (typeDesc == nullptr)
			{
				return {};
			}

			const bool isMatrix = (typeDesc->type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX) != 0;
			const bool isVector = (typeDesc->type_flags & SPV_REFLECT_TYPE_FLAG_VECTOR) != 0;

			RHIShaderVariable shaderVariable{};
			shaderVariable.location = variable->location;
			shaderVariable.type = GetDataType(variable->format);

			if (isMatrix)
			{
				shaderVariable.row = typeDesc->traits.numeric.matrix.row_count;
				shaderVariable.column = typeDesc->traits.numeric.matrix.column_count;

			}
			else if (isVector)
			{
				shaderVariable.row = typeDesc->traits.numeric.vector.component_count;
				shaderVariable.column = 1;
			}
			else
			{
				shaderVariable.row = 1;
				shaderVariable.column = 1;
			}

			return shaderVariable;
		}

		RHIShaderBinding GetShaderBinding(const SpvReflectDescriptorBinding* binding)
		{
			if (binding == nullptr)
			{
				return {};
			}

			RHIShaderBinding shaderBinding{};
			shaderBinding.binding = binding->binding;
			shaderBinding.set = binding->set;
			shaderBinding.type = GetBindingType(binding->descriptor_type);

			return shaderBinding;
		}

		void SetRenderType(const eRenderType renderType)
		{
			RENDER_TYPE = renderType;
		}

		void Build(const VertexKey& vertexKey, Memory::RefPtr<const RHIBuffer> buffer, RHIVertexLayoutCreateDesc& layoutDesc)
		{
			// TODO: Implement the logic to build the vertex layout description based on the provided vertex key and buffer.
		}

		DynamicArray<uint32_t> CompileShader(const RHIShaderCreateDesc& info)
		{
			if (!info.data || info.data->GetPointer() == nullptr || info.data->GetSize() == 0)
			{
				LOGERROR() << "[RHIUtils] Invalid shader data provided for compilation.";
				return {};
			}

			if (info.shaderType == eShaderType::eNone)
			{
				LOGERROR() << "[RHIUtils] Invalid shader type provided for compilation.";
				return {};
			}

			shaderc_compiler_t compiler = shaderc_compiler_initialize();
			shaderc_compile_options_t  options = shaderc_compile_options_initialize();

			const shaderc_shader_kind shaderKind = GetShaderKind(info.shaderType);
			const shaderc_target_env targetEnv = GetTargetEnv();
			const shaderc_env_version targetVersion = GetTargetVersion();

			const char* shaderSource = static_cast<const char*>(info.data->GetPointer());
			const size_t shaderSize = info.data->GetSize();

			shaderc_compile_options_set_target_env(options, targetEnv, targetVersion);
			shaderc_compilation_result_t result = shaderc_compile_into_spv(
				compiler,
				shaderSource,
				shaderSize,
				shaderKind,
				"shader.glsl",
				"main",
				options);

			DynamicArray<uint32_t> compiledSource;

			if (shaderc_result_get_compilation_status(result) == shaderc_compilation_status_success)
			{
				const size_t spirvSize = shaderc_result_get_length(result);
				const char* spirvByte = shaderc_result_get_bytes(result);

				compiledSource.Resize(spirvSize / sizeof(decltype(compiledSource)::ValueType));
				memcpy(compiledSource.Data(), spirvByte, spirvSize);

				LOGINFO() << "[RHIUtils] Shader compilation succeeded, "
					<< ", Shader Type : " << static_cast<uint32_t>(info.shaderType)
					<< ", Shader Size : " << shaderSize
					<< ", SPIR-V Size : " << spirvSize;
			}
			else
			{
				const char* errorMessage = shaderc_result_get_error_message(result);

				LOGERROR() << "[RHIUtils] Shader compilation failed: " << errorMessage;
				LOGERROR() << "[RHIUtils] Shader Type : " << static_cast<uint32_t>(info.shaderType)
					<< ", Shader Size : " << shaderSize
					<< ", Shader Source : " << shaderSource;
			}

			shaderc_result_release(result);
			shaderc_compile_options_release(options);
			shaderc_compiler_release(compiler);

			return compiledSource;
		}

		void ReflectShader(const DynamicArray<uint32_t>& shaderSource, Memory::RefPtr<RHIShader> shader)
		{
			using SourceValue = Reflection::Utils::RemoveReference_t<decltype(shaderSource)>::ValueType;

			if (shaderSource.Empty())
			{
				LOGERROR() << "[RHIUtils] Invalid shader source provided for reflection.";
				return;
			}

			if (!shader)
			{
				LOGERROR() << "[RHIUtils] Invalid shader object provided for reflection.";
				return;
			}

			SpvReflectShaderModule module{};
			SpvReflectResult result = spvReflectCreateShaderModule(
				shaderSource.Size() * sizeof(SourceValue), 
				shaderSource.Data(), 
				&module);

			if (result != SPV_REFLECT_RESULT_SUCCESS)
			{
				LOGERROR() << "[RHIUtils] Failed to create SPIR-V reflection module, error code: " << result;
				return;
			}

			// Reflect input variables
			{
				uint32_t inputCount = 0;
				spvReflectEnumerateInputVariables(&module, &inputCount, nullptr);

				DynamicArray<SpvReflectInterfaceVariable*> inputs(inputCount);
				spvReflectEnumerateInputVariables(&module, &inputCount, inputs.Data());

				for (size_t index = 0; index < inputCount; ++index)
				{
					SpvReflectInterfaceVariable* input = inputs[index];
					if (input == nullptr)
					{
						continue;
					}

					shader->AddInput(input->name, GetShaderVariable(input));
				}
			}

			// Reflect output variables
			{
				uint32_t outputCount = 0;
				spvReflectEnumerateOutputVariables(&module, &outputCount, nullptr);

				DynamicArray<SpvReflectInterfaceVariable*> outputs(outputCount);
				spvReflectEnumerateOutputVariables(&module, &outputCount, outputs.Data());

				for (size_t index = 0; index < outputCount; ++index)
				{
					SpvReflectInterfaceVariable* output = outputs[index];
					if (output == nullptr)
					{
						continue;
					}

					shader->AddOutput(output->name, GetShaderVariable(output));
				}
			}

			// Reflect descriptor bindings
			{
				uint32_t bindingCount = 0;
				spvReflectEnumerateDescriptorBindings(&module, &bindingCount, nullptr);

				DynamicArray<SpvReflectDescriptorBinding*> bindings(bindingCount);
				spvReflectEnumerateDescriptorBindings(&module, &bindingCount, bindings.Data());

				for (size_t index = 0; index < bindingCount; ++index)
				{
					SpvReflectDescriptorBinding* binding = bindings[index];
					if (binding == nullptr)
					{
						continue;
					}

					shader->AddBinding(binding->name, GetShaderBinding(binding));
				}
			}

			spvReflectDestroyShaderModule(&module);
		}
	}
}