#include <Asset/StbiImage.h>

#include <Log/include/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace wtr
{
	StbiImage::StbiImage()
		: m_pixels(nullptr)
		, m_size(0)
		, m_width(0)
		, m_height(0)
		, m_channels(0)
		, m_16bit(false)
	{}

	StbiImage::~StbiImage()
	{
		if (m_pixels)
		{
			stbi_image_free(m_pixels);
			m_pixels = nullptr;
		}
	}

	bool StbiImage::LoadFromBuffer(const uint8_t* buffer, uint32_t size)
	{
		if (!buffer || size == 0)
		{
			LOGINFO() << "[STDIMAGE] Failed to load the image from the buffer, the buffer is invalid";
			return false;
		}

		m_pixels = stbi_load_from_memory(buffer, static_cast<int>(size), 
			reinterpret_cast<int*>(&m_width), reinterpret_cast<int*>(&m_height), 
			reinterpret_cast<int*>(&m_channels), 0);

		if (m_pixels)
		{
			m_size = m_width * m_height * m_channels;
			m_16bit = stbi_is_16_bit_from_memory(buffer, static_cast<int>(size)) != 0;
		
			LOGINFO() << "[STDIMAGE] Succeed to load the image from the buffer, width : " << m_width
				<< ", height : " << m_height << ", channels : " << m_channels;
			return true;
		}
		else
		{
			LOGINFO() << "[STDIMAGE] Failed to load the image from the buffer, stbi_load_from_memory returned null";
			return false;
		}
	}

	const void* StbiImage::GetPixels() const
	{
		return m_pixels;
	}

	const uint32_t StbiImage::GetSize() const
	{
		return m_size;
	}

	const int32_t StbiImage::GetWidth() const
	{
		return m_width;
	}

	const int32_t StbiImage::GetHeight() const
	{
		return m_height;
	}

	const int32_t StbiImage::GetChannels() const
	{
		return m_channels;
	}

	const bool StbiImage::Is16Bit() const
	{
		return m_16bit;
	}
}