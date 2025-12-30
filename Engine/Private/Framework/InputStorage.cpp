#include <Framework/Input/InputStorage.h>

#include <Log/include/Log.h>

namespace wtr
{
	InputData::InputData()
		: windowSize(800.0f, 600.0f)
		, mousePos(0.0f, 0.0f)
		, mouseDelta(0.0f, 0.0f)
		, mouseScroll(0.0f)
	{
		memset(keyboard, 0, sizeof(keyboard));
	}

	InputData::InputData(const InputData& other)
		: windowSize(other.windowSize)
		, mousePos(other.mousePos)
		, mouseDelta(other.mouseDelta)
		, mouseScroll(other.mouseScroll)
	{
		memcpy(keyboard, other.keyboard, sizeof(keyboard));
	}

	InputData& InputData::operator=(const InputData& other)
	{
		if (this != &other)
		{
			memcpy(keyboard, other.keyboard, sizeof(keyboard));
			windowSize = other.windowSize;
			mousePos = other.mousePos;
			mouseDelta = other.mouseDelta;
			mouseScroll = other.mouseScroll;
		}
		return *this;
	}

	bool InputData::operator==(const InputData& other) const
	{
		if (windowSize != other.windowSize)
		{
			return false;
		}
		else if (mousePos != other.mousePos)
		{
			return false;
		}
		else if (mouseDelta != other.mouseDelta)
		{
			return false;
		}
		else if (mouseScroll != other.mouseScroll)
		{
			return false;
		}
		else
		{
			for (size_t index = 0; index < sizeof(keyboard); ++index)
			{
				if (keyboard[index] != other.keyboard[index])
				{
					return false;
				}
			}

			return true;
		}
	}

	bool InputData::operator!=(const InputData& other) const
	{
		if (*this == other)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	std::string InputData::ToString(const InputData& data)
	{
		std::string result = "";

		result += "Window Size: (" + std::to_string(data.windowSize.x) + ", " + std::to_string(data.windowSize.y) + "), ";
		result += "Position: (" + std::to_string(data.mousePos.x) + ", " + std::to_string(data.mousePos.y) + "), ";
		result += "Delta: (" + std::to_string(data.mouseDelta.x) + ", " + std::to_string(data.mouseDelta.y) + "), ";
		result += "Scroll: " + std::to_string(data.mouseScroll);

		for (size_t index = 1; index < sizeof(data.keyboard) * 2; ++index)
		{
			uint8_t keyData = (index % 2 == 0) ? data.keyboard[index / 2] & 0x0F : (data.keyboard[index / 2] >> 4) & 0x0F;
			eKeyCode key = static_cast<eKeyCode>(index);
			eInputAction keyAction = static_cast<eInputAction>(keyData);

			if (eInputAction::eNone != keyAction)
			{
				result += " Key " + InputDesc::ToString(key) + " : ";

				for (uint8_t action = static_cast<uint8_t>(eInputAction::eDown); action <= static_cast<uint8_t>(eInputAction::eRepeat); action <<= 1)
				{
					if ((static_cast<uint8_t>(keyAction) & action) == action)
					{
						result += InputDesc::ToString(static_cast<eInputAction>(action)) + " ";
					}
				}
			}
		}

		return result;
	}

	InputStorage::InputStorage()
		: m_PrevData()
		, m_CurrData()
	{}

	InputStorage::~InputStorage()
	{}

	void InputStorage::Prepare()
	{
		//if (IsChanaged())
		//{
		//	LOGINFO() << "[Input] " << InputData::ToString(m_CurrData);
		//}

		m_PrevData = m_CurrData;
		m_CurrData.mouseDelta = fvec2(0.f);
		m_CurrData.mouseScroll = 0.f;
		
		for (size_t index = 1; index < sizeof(m_CurrData.keyboard) * 2; ++index)
		{
			size_t keyIndex = index / 2;
			uint8_t& keyData = m_CurrData.keyboard[keyIndex];
			keyData &= (index % 2 == 0) ? 0x10 : 0x01;
		}
	}

	void InputStorage::Update(const InputDesc& inputDesc)
	{
		if ((eInputType::eKeyboard == inputDesc.Type) ||
			(eInputType::eMouseButton == inputDesc.Type))
		{
			const size_t keyIndex = GetKeyIndex(inputDesc.Key);
			const size_t keyBit = GetKeyBit(inputDesc.Key);

			uint8_t& keyData = m_CurrData.keyboard[keyIndex];
			keyData |= (static_cast<uint8_t>(inputDesc.Action) << keyBit);
			keyData |= (eInputAction::eRelease != inputDesc.Action) ? (0x01 << keyBit) : (0x00 << keyBit);
		}
		else if (eInputType::eMouseMove == inputDesc.Type)
		{
			m_CurrData.mousePos = fvec2(static_cast<float>(inputDesc.X), static_cast<float>(inputDesc.Y));
			m_CurrData.mouseDelta = m_CurrData.mousePos - m_PrevData.mousePos;
		}
		else if (eInputType::eMouseScroll == inputDesc.Type)
		{
			m_CurrData.mouseScroll = inputDesc.ScrollDelta;
		}
		else if (eInputType::eWindowResize == inputDesc.Type)
		{
			m_CurrData.windowSize = fvec2(static_cast<float>(inputDesc.X), static_cast<float>(inputDesc.Y));
		}
		else
		{}
	}

	bool InputStorage::IsChanaged() const
	{
		return m_PrevData != m_CurrData;
	}

	bool InputStorage::IsDown(eKeyCode key) const
	{
		const size_t keyIndex = GetKeyIndex(key);
		const size_t keyBit = GetKeyBit(key);

		const eInputAction keyData = static_cast<eInputAction>(m_CurrData.keyboard[keyIndex] << keyBit);

		return (keyData & eInputAction::eDown) != eInputAction::eNone;
	}

	bool InputStorage::IsPressed(eKeyCode key) const
	{
		const size_t keyIndex = GetKeyIndex(key);
		const size_t keyBit = GetKeyBit(key);

		const eInputAction keyData = static_cast<eInputAction>(m_CurrData.keyboard[keyIndex] << keyBit);

		return (keyData & eInputAction::ePress) != eInputAction::eNone;
	}

	bool InputStorage::IsReleased(eKeyCode key) const
	{
		const size_t keyIndex = GetKeyIndex(key);
		const size_t keyBit = GetKeyBit(key);

		const eInputAction keyData = static_cast<eInputAction>(m_CurrData.keyboard[keyIndex] << keyBit);

		return (keyData & eInputAction::eRelease) != eInputAction::eNone;
	}

	bool InputStorage::IsRepeated(eKeyCode key) const
	{
		const size_t keyIndex = GetKeyIndex(key);
		const size_t keyBit = GetKeyBit(key);

		const eInputAction keyData = static_cast<eInputAction>(m_CurrData.keyboard[keyIndex] << keyBit);

		return (keyData & eInputAction::eRepeat) != eInputAction::eNone;
	}

	const fvec2& InputStorage::GetMousePosition() const
	{
		return m_CurrData.mousePos;
	}

	const fvec2& InputStorage::GetMouseDelta() const
	{
		return m_CurrData.mouseDelta;
	}

	float InputStorage::GetMouseScroll() const
	{
		return m_CurrData.mouseScroll;
	}

	const fvec2& InputStorage::GetWindowSize() const
	{
		return m_CurrData.windowSize;
	}

	const size_t InputStorage::GetKeyIndex(eKeyCode key) const
	{
		return static_cast<size_t>(key) / 2;
	}

	const size_t InputStorage::GetKeyBit(eKeyCode key) const
	{
		return (static_cast<size_t>(key) % 2) * KEY_BIT;
	}
}