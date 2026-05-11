#ifndef __WTR_SHADERSET_H__
#define __WTR_SHADERSET_H__

#include <Renderer/RenderResource.h>
#include <Renderer/ShaderState.h>

#include <Container/include/HashMap.h>

namespace wtr
{
	class ShaderSet : public RenderResource
	{
	public:
		ShaderSet();
		virtual ~ShaderSet() = default;

	public :
		template<typename T>
		Memory::RefPtr<const ShaderState> GetShader() const
		{
			static_assert(Reflection::Utils::IsBase<ShaderState, T>::value || Reflection::Utils::IsSame<ShaderState, T>::value, "T must be a class derived from ShaderState");

			const Reflection::TypeInfo* typeInfo = T::GetStaticTypeInfo();
			const size_t typeHash = typeInfo->GetTypeHash();

			auto itr = m_shaders.find(typeHash);
			if (itr != m_shaders.end())
			{
				return itr->second;
			}
			else
			{
				return nullptr;
			}
		}

		template<typename T>
		bool HasShader() const
		{
			static_assert(Reflection::Utils::IsBase<ShaderState, T>::value || Reflection::Utils::IsSame<ShaderState, T>::value, "T must be a class derived from ShaderState");

			const Reflection::TypeInfo* typeInfo = T::GetStaticTypeInfo();
			const size_t typeHash = typeInfo->GetTypeHash();

			auto itr = m_shaders.find(typeHash);
			return itr != m_shaders.end();
		}

	public :
		virtual eResourceState GetResourceState() const override;

		void AddShader(Memory::RefPtr<const ShaderState> shaderState);

	private:
		using ShaderMap = wtr::HashMap<size_t, Memory::RefPtr<const ShaderState>>;

		ShaderMap m_shaders;
	};
}

#endif // __WTR_SHADERSET_H__