#ifndef __WTR_ASSETSTREAM_H__
#define __WTR_ASSETSTREAM_H__

#include <string>
#include <sstream>
#include <charconv>

namespace wtr
{
	class AssetStream
	{
	public:
		AssetStream(const std::string_view& str);
		~AssetStream() = default;

	public:
		template<typename T>
		AssetStream& operator>>(T& out)
		{
			Skip();

			if (Empty())
			{
				return *this;
			}

			auto [ptr, ec] = std::from_chars(m_view.data(), m_view.data() + m_view.size(), out);

			if (ec == std::errc{}) {
				m_view.remove_prefix(ptr - m_view.data());
			}
			return *this;
		}

		template<>
		AssetStream& operator>><std::string>(std::string& out)
		{
			Skip();

			if (Empty())
			{
				return *this;
			}

			std::string_view view;
			*this >> view;

			if (!view.empty()) {
				out = view;
			}
			else {
				out.clear();
			}

			return *this;
		}

		template<>
		AssetStream& operator>><std::string_view>(std::string_view& out)
		{
			Skip();

			if (Empty())
			{
				return *this;
			}

			size_t delimiter = m_view.find_first_of(" \t\n\r");
			if (delimiter == std::string_view::npos) {
				out = m_view;
				m_view.remove_prefix(m_view.size());
			}
			else {
				out = m_view.substr(0, delimiter);
				m_view.remove_prefix(delimiter);
			}
			return *this;
		}

	public:
		bool Skip(char target);
		bool Empty() const;

	private:
		void Skip();

	private:
		std::string_view m_view;
	};
};

#endif // __WTR_ASSETSTREAM_H__