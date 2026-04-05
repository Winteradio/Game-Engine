#include <Asset/AssetStream.h>

namespace wtr
{
	AssetStream::AssetStream(const std::string_view& stringView)
		: m_view(stringView)
	{
	}

	bool AssetStream::Skip(char target)
	{
		if (Empty())
		{
			return false;
		}

		if (m_view[0] == target)
		{
			m_view.remove_prefix(1);
			return true;
		}
		else
		{
			return false;
		}
	}

	void AssetStream::Skip()
	{
		const size_t first = m_view.find_first_not_of(" \t\r\n");

		if (first != std::string_view::npos)
		{
			m_view.remove_prefix(first);
		}
		else
		{
			m_view.remove_prefix(m_view.size());
			m_view = {};
		}
	}

	bool AssetStream::Empty() const
	{
		return m_view.empty();
	}
}