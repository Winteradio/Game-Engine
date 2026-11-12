#include <RHI/RHIUtils.h>

#include <Renderer/RenderType.h>
#include <Renderer/RenderUtils.h>
#include <RHI/RHIContext.h>
#include <RHI/RHIResources.h>
#include <Log/include/Log.h>

#include <spirv_reflect.h>

namespace wtr
{
	namespace RHIUtils
	{
		eRenderType RENDER_TYPE = eRenderType::eNone;

		bool CheckShaderVariable(Memory::RefPtr<RHIShader> shader)
		{
			if (!shader)
			{
				return false;
			}

			const auto& inputs = shader->GetInputs();
			for (const auto& [name, attribute] : inputs)
			{
				const VertexKey vertexKey = GetVertexKey(name);
				if (vertexKey.semantic == eVertexSemantic::eNone || vertexKey.semanticIndex == 0xFF)
				{
					LOGWARN() << "[RHI] Unrecognized vertex attribute: " << name;
					continue;
				}

				const int32_t expectedLocation = GetVertexLocation(vertexKey);
				const int32_t actualLocation = attribute.location;

				if (actualLocation != expectedLocation)
				{
					LOGWARN() << "[RHI] Vertex attribute location mismatch: " << name
						<< ", expected location : " << expectedLocation
						<< ", actual location : " << actualLocation;

					return false;
				}
			}

			return true;
		}

		const wchar_t* GetHLSLProfile(const eShaderType type)
		{
			switch (type)
			{
			case eShaderType::eVertex:
				return L"vs_6_0";

			case eShaderType::eGeometry:
				return L"gs_6_0";

			case eShaderType::eHull:
				return L"hs_6_0";

			case eShaderType::eDomain:
				return L"ds_6_0";

			case eShaderType::ePixel:
				return L"ps_6_0";

			case eShaderType::eCompute:
				return L"cs_6_0";

			default:
				return nullptr;
			}
		}

		eDataStage GetDataStage(const eShaderType type)
		{
			switch (type)
			{
			case eShaderType::eVertex:
				return eDataStage::eVertex;

			case eShaderType::eGeometry:
				return eDataStage::eGeometry;

			case eShaderType::eHull:
				return eDataStage::eHull;

			case eShaderType::eDomain:
				return eDataStage::eDomain;

			case eShaderType::ePixel:
				return eDataStage::ePixel;

			case eShaderType::eCompute:
				return eDataStage::eCompute;

			default:
				return eDataStage::eNone;
			}
		}

		eDataState GetDataState(const SpvReflectDescriptorBinding* binding)
		{
			if (binding->resource_type == SPV_REFLECT_RESOURCE_FLAG_SRV ||
				(binding->decoration_flags & SPV_REFLECT_DECORATION_NON_WRITABLE) != 0)
			{
				return eDataState::eRead;
			}

			if ((binding->decoration_flags & SPV_REFLECT_DECORATION_NON_READABLE) != 0)
			{
				return eDataState::eWrite;
			}

			return eDataState::eRead | eDataState::eWrite;
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

		// DXC는 타입 이름에 type. 접두사를 붙이고(type.ConstantBuffer.xxx, type.StructuredBuffer.xxx) 변수 이름이 곧 버퍼 이름
		std::string GetBlockName(const SpvReflectDescriptorBinding* binding)
		{
			return binding->name != nullptr ? binding->name : "";
		}

		// DXC는 스테이지 입출력을 in.var.<SEMANTIC> / out.var.<SEMANTIC>으로 이름 붙임
		// 시맨틱만 남겨서 VertexKey(POSITION, TEXCOORD0 ...)로 바로 쓰게 함
		std::string GetInterfaceName(const SpvReflectInterfaceVariable* variable)
		{
			std::string name = variable->name != nullptr ? variable->name : "";

			static const std::string PREFIXES[] = { "in.var.", "out.var." };
			for (const std::string& prefix : PREFIXES)
			{
				if (name.compare(0, prefix.size(), prefix) == 0)
				{
					return name.substr(prefix.size());
				}
			}

			return name;
		}

		RHIShaderAttrib GetShaderAttrib(const SpvReflectInterfaceVariable* variable)
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

			RHIShaderAttrib shaderAttrib{};
			shaderAttrib.location = variable->location;
			shaderAttrib.componentType = GetDataType(variable->format);

			if (isMatrix)
			{
				shaderAttrib.componentCount = typeDesc->traits.numeric.matrix.row_count * typeDesc->traits.numeric.matrix.column_count;
			}
			else if (isVector)
			{
				shaderAttrib.componentCount = typeDesc->traits.numeric.vector.component_count;
			}
			else
			{
				shaderAttrib.componentCount = 1;
			}

			return shaderAttrib;
		}

		// Vk push constant : 멤버에 binding / location이 없어서 블록 안 offset을 binding 자리에 둔다
		RHIShaderConst GetShaderConst(const SpvReflectBlockVariable* member, const eDataStage stage)
		{
			RHIShaderConst shaderConst{};
			shaderConst.binding = member->absolute_offset;
			shaderConst.stage = stage;

			return shaderConst;
		}

		RHIShaderBuffer GetShaderBuffer(const SpvReflectDescriptorBinding* binding, const eBufferUsage usage, const eDataStage stage)
		{
			RHIShaderBuffer shaderBuffer{};
			shaderBuffer.binding = binding->binding;
			shaderBuffer.usage = usage;
			shaderBuffer.stage = stage;
			shaderBuffer.state = ((usage & eBufferUsage::eStorage) != eBufferUsage::eNone) ? GetDataState(binding) : eDataState::eRead;

			return shaderBuffer;
		}

		RHIShaderSampler GetShaderSampler(const SpvReflectDescriptorBinding* binding, const eDataStage stage)
		{
			RHIShaderSampler shaderSampler{};
			shaderSampler.binding = binding->binding;
			shaderSampler.usage = eTextureUsage::eSampler;
			shaderSampler.stage = stage;
			shaderSampler.state = eDataState::eRead;

			return shaderSampler;
		}

		RHIShaderAttach GetShaderAttach(const SpvReflectDescriptorBinding* binding, const eDataStage stage)
		{
			RHIShaderAttach shaderAttach{};
			shaderAttach.location = binding->binding;
			shaderAttach.stage = stage;
			shaderAttach.usage = (binding->resource_type == SPV_REFLECT_RESOURCE_FLAG_STORAGE_IMAGE) ? eTextureUsage::eTransfer : eTextureUsage::eColorAttach;
			shaderAttach.state = ((binding->resource_type == SPV_REFLECT_RESOURCE_FLAG_STORAGE_IMAGE) && (binding->accessed_by_shader)) ? GetDataState(binding) : eDataState::eRead;

			return shaderAttach;
		}

		void SetRenderType(const eRenderType renderType)
		{
			RENDER_TYPE = renderType;
		}

		void Build(const VertexKey& vertexKey, Memory::RefPtr<const RHIBuffer> buffer, RHIVertexLayoutCreateDesc& layoutDesc)
		{
			// TODO: Implement the logic to build the vertex layout description based on the provided vertex key and buffer.
		}

		// 시맨틱(POSITION, TEXCOORD0 ...) → VertexKey → GetVertexLocation. 엔진이 모르는 시맨틱이면 -1
		int32_t GetSemanticLocation(const SpvReflectInterfaceVariable* variable)
		{
			const VertexKey key = RenderUtils::GetVertexKey(GetInterfaceName(variable));
			if (key.semantic == eVertexSemantic::eNone)
			{
				return -1;
			}

			return RenderUtils::GetVertexLocation(key);
		}

		// DXC는 시맨틱과 상관없이 선언 순서대로 location을 붙이므로, 컴파일 직후 엔진 고정값으로 고쳐 쓴다
		// - 정점 입력 : 메시 하나의 VertexLayout을 어느 셰이더와도 같이 쓰기 위해
		// - varying : 선언 순서가 다르거나 PS가 VS 출력의 일부만 읽어도 앞뒤 단계 번호가 같도록
		// - 픽셀 출력 : SV_TargetN이 곧 어태치먼트 slot이라 손대지 않음
		bool FixInterfaceLocations(DynamicArray<uint32_t>& spirv, const eShaderType shaderType)
		{
			using SourceValue = Reflection::Utils::RemoveReference_t<decltype(spirv)>::ValueType;

			SpvReflectShaderModule module{};
			if (spvReflectCreateShaderModule(spirv.Size() * sizeof(SourceValue), spirv.Data(), &module) != SPV_REFLECT_RESULT_SUCCESS)
			{
				LOGERROR() << "[RHIUtils] Failed to create SPIR-V reflection module for fixing the locations";
				return false;
			}

			bool valid = true;

			auto FixVariables = [&](const bool isInput)
			{
				uint32_t count = 0;
				if (isInput)
				{
					spvReflectEnumerateInputVariables(&module, &count, nullptr);
				}
				else
				{
					spvReflectEnumerateOutputVariables(&module, &count, nullptr);
				}

				DynamicArray<SpvReflectInterfaceVariable*> variables(count);
				if (isInput)
				{
					spvReflectEnumerateInputVariables(&module, &count, variables.Data());
				}
				else
				{
					spvReflectEnumerateOutputVariables(&module, &count, variables.Data());
				}

				HashMap<int32_t, std::string> usedLocations;
				for (size_t index = 0; index < count; ++index)
				{
					SpvReflectInterfaceVariable* variable = variables[index];
					if (variable == nullptr || variable->name == nullptr)
					{
						continue;
					}

					// SV_Position / SV_InstanceID 같은 내장 변수는 location이 없음
					if ((variable->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) != 0)
					{
						continue;
					}

					const std::string semantic = GetInterfaceName(variable);
					const int32_t location = GetSemanticLocation(variable);
					if (location < 0)
					{
						LOGERROR() << "[RHIUtils] The semantic is not the vertex key (POSITION, NORMAL, TEXCOORD0 ...) : " << semantic;
						valid = false;
						continue;
					}

					auto used = usedLocations.Find(location);
					if (used != usedLocations.End())
					{
						LOGERROR() << "[RHIUtils] The semantics share the same location " << location << " : " << used->second << ", " << semantic;
						valid = false;
						continue;
					}

					usedLocations[location] = semantic;

					const SpvReflectResult result = isInput
						? spvReflectChangeInputVariableLocation(&module, variable, static_cast<uint32_t>(location))
						: spvReflectChangeOutputVariableLocation(&module, variable, static_cast<uint32_t>(location));

					if (result != SPV_REFLECT_RESULT_SUCCESS)
					{
						LOGERROR() << "[RHIUtils] Failed to change the location of the semantic : " << semantic;
						valid = false;
					}
				}
			};

			FixVariables(true);

			if (shaderType != eShaderType::ePixel)
			{
				FixVariables(false);
			}

			if (valid)
			{
				const uint32_t codeSize = spvReflectGetCodeSize(&module);
				const uint32_t* code = spvReflectGetCode(&module);

				spirv.Resize(codeSize / sizeof(SourceValue));
				memcpy(spirv.Data(), code, codeSize);
			}

			spvReflectDestroyShaderModule(&module);

			return valid;
		}

		// DXC는 register가 없으면 파일마다 따로 선언 순서대로 binding을 붙이므로 VS / PS 사이에서 겹친다
		// 컴파일 직후 리소스 이름 → GetResourceBinding으로 set 0의 엔진 고정 번호로 고쳐 쓴다
		// (결합형 샘플러 짝은 Macro.hlsli의 TEXTURE2D가 임시 번호로 묶어두고, 여기서 텍스처 이름 기준으로 옮김)
		bool FixDescriptorBindings(DynamicArray<uint32_t>& spirv)
		{
			using SourceValue = Reflection::Utils::RemoveReference_t<decltype(spirv)>::ValueType;

			SpvReflectShaderModule module{};
			if (spvReflectCreateShaderModule(spirv.Size() * sizeof(SourceValue), spirv.Data(), &module) != SPV_REFLECT_RESULT_SUCCESS)
			{
				LOGERROR() << "[RHIUtils] Failed to create SPIR-V reflection module for fixing the bindings";
				return false;
			}

			uint32_t count = 0;
			spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);

			DynamicArray<SpvReflectDescriptorBinding*> bindings(count);
			spvReflectEnumerateDescriptorBindings(&module, &count, bindings.Data());

			bool valid = true;
			HashMap<uint32_t, std::string> usedBindings;

			for (size_t index = 0; index < count; ++index)
			{
				SpvReflectDescriptorBinding* binding = bindings[index];
				if (binding == nullptr)
				{
					continue;
				}

				const std::string name = GetBlockName(binding);
				const uint32_t engineBinding = RenderUtils::GetResourceBinding(name);
				if (engineBinding == UINT32_MAX)
				{
					LOGERROR() << "[RHIUtils] The resource has no engine binding (RenderUtils::GetResourceBinding) : " << name;
					valid = false;
					continue;
				}

				auto used = usedBindings.Find(engineBinding);
				if (used != usedBindings.End())
				{
					LOGERROR() << "[RHIUtils] The resources share the same binding " << engineBinding << " : " << used->second << ", " << name;
					valid = false;
					continue;
				}

				usedBindings[engineBinding] = name;

				if (spvReflectChangeDescriptorBindingNumbers(&module, binding, engineBinding, 0) != SPV_REFLECT_RESULT_SUCCESS)
				{
					LOGERROR() << "[RHIUtils] Failed to change the binding of the resource : " << name;
					valid = false;
				}
			}

			if (valid)
			{
				const uint32_t codeSize = spvReflectGetCodeSize(&module);
				const uint32_t* code = spvReflectGetCode(&module);

				spirv.Resize(codeSize / sizeof(SourceValue));
				memcpy(spirv.Data(), code, codeSize);
			}

			spvReflectDestroyShaderModule(&module);

			return valid;
		}

		DynamicArray<uint32_t> CompileShader(const RHIShaderCreateDesc& info)
		{
			const RefData& bulk = info.blob.bulk;
			if (!bulk || bulk->Empty() || info.blob.offset >= bulk->GetSize())
			{
				LOGERROR() << "[RHIUtils] Invalid shader data provided for shader compilation.";
				return {};
			}

			const wchar_t* profile = GetHLSLProfile(info.shaderType);
			if (profile == nullptr)
			{
				LOGERROR() << "[RHIUtils] Invalid shader type provided for shader compilation.";
				return {};
			}

			IDxcCompiler3* compiler = nullptr;
			if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler3), reinterpret_cast<void**>(&compiler))) || compiler == nullptr)
			{
				LOGERROR() << "[RHIUtils] Failed to create the DXC compiler instance.";
				return {};
			}

			// size가 0이면 offset부터 bulk 끝까지
			const uint32_t remainSize = bulk->GetSize() - info.blob.offset;
			const char* shaderSource = static_cast<const char*>(bulk->GetPointer()) + info.blob.offset;
			const size_t shaderSize = (info.blob.size == 0 || info.blob.size > remainSize) ? remainSize : info.blob.size;

			DxcBuffer source{};
			source.Ptr = shaderSource;
			source.Size = shaderSize;
			source.Encoding = DXC_CP_UTF8;

			// -spirv : DXIL 대신 SPIR-V 출력, vulkan1.0 : 현재 목표 버전
			// -fvk-use-gl-layout : cbuffer는 std140, StructuredBuffer는 std430 → C++ 쪽 구조체 배치와 맞춤
			// include handler를 넘기지 않으므로 HLSL 안의 #include는 아직 지원하지 않음
			const wchar_t* arguments[] =
			{
				L"-E", L"main",
				L"-T", profile,
				L"-spirv",
				L"-fspv-target-env=vulkan1.0",
				L"-fvk-use-gl-layout",
			};
			constexpr UINT32 argumentCount = static_cast<UINT32>(sizeof(arguments) / sizeof(arguments[0]));

			IDxcResult* result = nullptr;
			HRESULT status = compiler->Compile(&source, arguments, argumentCount, nullptr, __uuidof(IDxcResult), reinterpret_cast<void**>(&result));
			if (SUCCEEDED(status) && result != nullptr)
			{
				result->GetStatus(&status);
			}

			DynamicArray<uint32_t> compiledSource;

			if (SUCCEEDED(status) && result != nullptr)
			{
				IDxcBlob* object = nullptr;
				result->GetOutput(DXC_OUT_OBJECT, __uuidof(IDxcBlob), reinterpret_cast<void**>(&object), nullptr);

				if (object != nullptr && object->GetBufferSize() > 0)
				{
					const size_t spirvSize = object->GetBufferSize();

					compiledSource.Resize(spirvSize / sizeof(decltype(compiledSource)::ValueType));
					memcpy(compiledSource.Data(), object->GetBufferPointer(), spirvSize);

					LOGINFO() << "[RHIUtils] Shader compilation succeeded"
						<< ", Shader Type : " << static_cast<uint32_t>(info.shaderType)
						<< ", Shader Size : " << shaderSize
						<< ", SPIR-V Size : " << spirvSize;
				}

				if (object != nullptr)
				{
					object->Release();
				}
			}
			else
			{
				IDxcBlobUtf8* errors = nullptr;
				if (result != nullptr)
				{
					result->GetOutput(DXC_OUT_ERRORS, __uuidof(IDxcBlobUtf8), reinterpret_cast<void**>(&errors), nullptr);
				}

				const bool hasError = errors != nullptr && errors->GetStringLength() > 0;

				LOGERROR() << "[RHIUtils] Shader compilation failed: " << (hasError ? errors->GetStringPointer() : "unknown error");
				LOGERROR() << "[RHIUtils] Shader Type : " << static_cast<uint32_t>(info.shaderType)
					<< ", Shader Size : " << shaderSize
					<< ", Shader Source : " << std::string(shaderSource, shaderSize);

				if (errors != nullptr)
				{
					errors->Release();
				}
			}

			if (result != nullptr)
			{
				result->Release();
			}

			compiler->Release();

			if (!compiledSource.Empty() && !FixInterfaceLocations(compiledSource, info.shaderType))
			{
				LOGERROR() << "[RHIUtils] Failed to fix the interface locations, Shader Type : " << static_cast<uint32_t>(info.shaderType);
				compiledSource.Clear();
			}

			if (!compiledSource.Empty() && !FixDescriptorBindings(compiledSource))
			{
				LOGERROR() << "[RHIUtils] Failed to fix the descriptor bindings, Shader Type : " << static_cast<uint32_t>(info.shaderType);
				compiledSource.Clear();
			}

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

			const eDataStage stage = GetDataStage(shader->GetShaderType());

			// Reflect input variables
			{
				uint32_t inputCount = 0;
				spvReflectEnumerateInputVariables(&module, &inputCount, nullptr);

				DynamicArray<SpvReflectInterfaceVariable*> inputs(inputCount);
				spvReflectEnumerateInputVariables(&module, &inputCount, inputs.Data());

				for (size_t index = 0; index < inputCount; ++index)
				{
					SpvReflectInterfaceVariable* input = inputs[index];
					if (input == nullptr || input->name == nullptr)
					{
						continue;
					}

					// gl_VertexID 같은 내장 변수는 정점 입력이 아님
					if ((input->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) != 0)
					{
						continue;
					}

					shader->AddInput(GetInterfaceName(input), GetShaderAttrib(input));
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
					if (output == nullptr || output->name == nullptr)
					{
						continue;
					}

					// gl_Position / gl_FragDepth 같은 내장 변수는 어태치먼트 출력이 아님
					if ((output->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) != 0)
					{
						continue;
					}

					shader->AddOutput(GetInterfaceName(output), GetShaderAttrib(output));
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

					switch (binding->descriptor_type)
					{
					case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
					case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
						shader->AddBuffer(GetBlockName(binding), GetShaderBuffer(binding, eBufferUsage::eUniform, stage));
						break;

					case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
					case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
						shader->AddBuffer(GetBlockName(binding), GetShaderBuffer(binding, eBufferUsage::eStorage, stage));
						break;

					case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
					case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
					case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
						shader->AddSampler(binding->name != nullptr ? binding->name : "", GetShaderSampler(binding, stage));
						break;

					default:
						// STORAGE_IMAGE는 레이아웃(GENERAL)이 달라 eSampler로 묶을 수 없음. eTextureUsage에 자리가 생기면 추가
						LOGWARN() << "[RHIUtils] Unsupported descriptor type for binding : " << (binding->name != nullptr ? binding->name : "")
							<< ", type : " << static_cast<uint32_t>(binding->descriptor_type);
						break;
					}
				}
			}

			// Reflect push constants (Vk)
			{
				uint32_t blockCount = 0;
				spvReflectEnumeratePushConstantBlocks(&module, &blockCount, nullptr);

				DynamicArray<SpvReflectBlockVariable*> blocks(blockCount);
				spvReflectEnumeratePushConstantBlocks(&module, &blockCount, blocks.Data());

				for (size_t index = 0; index < blockCount; ++index)
				{
					SpvReflectBlockVariable* block = blocks[index];
					if (block == nullptr)
					{
						continue;
					}

					for (uint32_t memberIndex = 0; memberIndex < block->member_count; ++memberIndex)
					{
						const SpvReflectBlockVariable& member = block->members[memberIndex];
						if (member.name == nullptr)
						{
							continue;
						}

						shader->AddConst(member.name, GetShaderConst(&member, stage));
					}
				}
			}

			spvReflectDestroyShaderModule(&module);
		}
	}
}
