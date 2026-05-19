#ifndef __WTR_SHADERPROXY_H__
#define __WTR_SHADERPROXY_H__

#include <Renderer/RenderResource.h>
#include <Renderer/ShaderState.h>

#include <Container/include/HashMap.h>

namespace wtr
{
	class ShaderProxy : public RenderResource
	{
	public:
		ShaderProxy();
		virtual ~ShaderProxy() = default;

	public :
		template<typename T>
		Memory::RefPtr<const ShaderState> GetShader() const
		{
			static_assert(Reflection::Utils::IsBase<ShaderState, T>::value || Reflection::Utils::IsSame<ShaderState, T>::value, "T must be a class derived from ShaderState");

			const Reflection::TypeInfo* typeInfo = T::GetStaticTypeInfo();
			const size_t typeHash = typeInfo->GetTypeHash();

			auto itr = m_shaders.Find(typeHash);
			if (itr != m_shaders.End())
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

#endif // __WTR_SHADERPROXY_H__