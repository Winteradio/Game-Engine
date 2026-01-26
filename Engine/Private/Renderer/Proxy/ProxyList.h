#ifndef __WTR_PROXYLIST_H__
#define __WTR_PROXYLIST_H__

#include <Framework/CommandList.h>
#include <Container/include/DynamicArray.h>

namespace wtr
{
	template<typename ProxyBase>
	class ProxyList : public CommandList<ProxyBase>
	{
	public :
		using Super = CommandList<ProxyBase>;

		ProxyList()
			: Super()
			, m_proxies()
		{}

		virtual ~ProxyList()
		{}

	public :
		template<typename Command, typename... Args>
		ProxyBase* Create(Args&&... args)
		{
			ProxyBase* proxy = Super::Create(std::forward<Args>(args)...);
			if (nullptr != proxy)
			{
				m_proxies.PushBack(proxy);
			}

			return proxy;
		}

		void Clear()
		{
			for (auto* proxy : m_proxies)
			{
				if (nullptr != proxy)
				{
					proxy->~ProxyBase();
				}
			}

			m_proxies.Clear();

			Super::Clear();
		}

		const wtr::DynamicArray<ProxyBase*>& GetProxies() const
		{
			return m_proxies;
		}

	protected :
		wtr::DynamicArray<ProxyBase*> m_proxies;
	};
}

#endif // __WTR_PROXYLIST_H__