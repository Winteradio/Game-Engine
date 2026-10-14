#ifndef __WTR_NAME_H__
#define __WTR_NAME_H__

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

namespace Log
{
	class Message;
};

namespace wtr
{
	class Name : private std::string
	{
	private :
		using super = std::string;

	public :
		static constexpr size_t npos = std::string::npos;

		using const_iterator = std::string::const_iterator;
		using std::string::size;
		using std::string::length;
		using std::string::empty;
		using std::string::c_str;
		using std::string::find;
		using std::string::substr;

	public :
		Name();
		Name(const char* str);
		Name(const std::string& str);
		Name(std::string_view str);
		Name(const Name& other) = default;
		Name(Name&& other) noexcept;
		~Name() = default;

	public :
		Name& operator=(const Name& other) = default;
		Name& operator=(Name&& other) noexcept;
		Name& operator=(const char* str);
		Name& operator=(const std::string& str);
		Name& operator=(std::string_view str);

		bool operator==(const Name& other) const;
		bool operator!=(const Name& other) const;

		bool operator<(const Name& other) const;
		bool operator>(const Name& other) const;
		bool operator<=(const Name& other) const;
		bool operator>=(const Name& other) const;

		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;
		bool operator==(const std::string& other) const;
		bool operator!=(const std::string& other) const;
		bool operator==(std::string_view other) const;
		bool operator!=(std::string_view other) const;

		char operator[](const size_t index) const;

	public :
		const_iterator begin() const;
		const_iterator end() const;

		const std::string& str() const;
		std::string_view view() const;

		uint64_t hash() const;

		void clear();
		void swap(Name& other) noexcept;
		void swap(std::string& other) noexcept;

	private :
		uint64_t m_hash = 0;
	};

	Log::Message& operator<<(Log::Message& message, const Name& name);
	Log::Message& operator<<(Log::Message&& message, const Name& name);

	inline bool operator==(const char* lhs, const Name& rhs) { return rhs == lhs; }
	inline bool operator!=(const char* lhs, const Name& rhs) { return rhs != lhs; }
	inline bool operator==(const std::string& lhs, const Name& rhs) { return rhs == lhs; }
	inline bool operator!=(const std::string& lhs, const Name& rhs) { return rhs != lhs; }
	inline bool operator==(std::string_view lhs, const Name& rhs) { return rhs == lhs; }
	inline bool operator!=(std::string_view lhs, const Name& rhs) { return rhs != lhs; }
};

namespace std
{
	template<>
	struct hash<wtr::Name>
	{
		size_t operator()(const wtr::Name& name) const noexcept
		{
			return static_cast<size_t>(name.hash());
		}
	};
}

#endif // __WTR_NAME_H__
