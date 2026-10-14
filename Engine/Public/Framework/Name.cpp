#include <Framework/Name.h>

#include <Log/include/Log.h>

#include <utility>

namespace wtr
{
	namespace
	{ 
		static constexpr uint64_t GetHash(std::string_view str)
		{
			if (str.empty())
			{
				return 0;
			}

			uint64_t hash = 14695981039346656037ull;
			for (const char ch : str)
			{
				hash ^= static_cast<uint8_t>(ch);
				hash *= 1099511628211ull;
			}

			return hash == 0 ? 1 : hash;
		}
	}

	Name::Name()
		: super()
		, m_hash(0)
	{}

	Name::Name(const char* str)
		: super(str != nullptr ? str : "")
		, m_hash(GetHash(view()))
	{}

	Name::Name(const std::string& str)
		: super(str)
		, m_hash(GetHash(view()))
	{}

	Name::Name(std::string_view str)
		: super(str)
		, m_hash(GetHash(view()))
	{}

	Name::Name(Name&& other) noexcept
		: super(std::move(static_cast<super&>(other)))
		, m_hash(other.m_hash)
	{
		other.super::clear();
		other.m_hash = 0;
	}

	Name& Name::operator=(Name&& other) noexcept
	{
		if (this != &other)
		{
			super::operator=(std::move(static_cast<super&>(other)));
			m_hash = other.m_hash;

			other.super::clear();
			other.m_hash = 0;
		}

		return *this;
	}

	Name& Name::operator=(const char* str)
	{
		super::assign(str != nullptr ? str : "");
		m_hash = GetHash(view());

		return *this;
	}

	Name& Name::operator=(const std::string& str)
	{
		super::assign(str);
		m_hash = GetHash(view());

		return *this;
	}

	Name& Name::operator=(std::string_view str)
	{
		super::assign(str);
		m_hash = GetHash(view());

		return *this;
	}

	bool Name::operator==(const Name& other) const
	{
		return m_hash == other.m_hash && str() == other.str();
	}

	bool Name::operator!=(const Name& other) const
	{
		return !(*this == other);
	}

	bool Name::operator<(const Name& other) const
	{
		return str() < other.str();
	}

	bool Name::operator>(const Name& other) const
	{
		return other < *this;
	}

	bool Name::operator<=(const Name& other) const
	{
		return !(other < *this);
	}

	bool Name::operator>=(const Name& other) const
	{
		return !(*this < other);
	}

	bool Name::operator==(const char* other) const
	{
		return other != nullptr ? str() == other : str().empty();
	}

	bool Name::operator!=(const char* other) const
	{
		return !(*this == other);
	}

	bool Name::operator==(const std::string& other) const
	{
		return str() == other;
	}

	bool Name::operator!=(const std::string& other) const
	{
		return !(*this == other);
	}

	bool Name::operator==(std::string_view other) const
	{
		return view() == other;
	}

	bool Name::operator!=(std::string_view other) const
	{
		return !(*this == other);
	}

	char Name::operator[](const size_t index) const
	{
		return super::operator[](index);
	}

	Name::const_iterator Name::begin() const
	{
		return super::begin();
	}

	Name::const_iterator Name::end() const
	{
		return super::end();
	}

	const std::string& Name::str() const
	{
		return static_cast<const super&>(*this);
	}

	std::string_view Name::view() const
	{
		return std::string_view(str());
	}

	uint64_t Name::hash() const
	{
		return m_hash;
	}

	void Name::clear()
	{
		super::clear();
		m_hash = 0;
	}

	void Name::swap(Name& other) noexcept
	{
		super::swap(static_cast<super&>(other));
		std::swap(m_hash, other.m_hash);
	}

	void Name::swap(std::string& other) noexcept
	{
		super::swap(other);
		m_hash = GetHash(view());
	}

	Log::Message& operator<<(Log::Message& message, const Name& name)
	{
		return message << name.str();
	}

	Log::Message& operator<<(Log::Message&& message, const Name& name)
	{
		return message << name.str();
	}
}
