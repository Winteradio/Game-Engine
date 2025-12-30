#include <Platform/Win32/Win32InputHandler.h>

#include <Log/include/Log.h>

namespace wtr
{
	Win32InputHandler::Win32InputHandler()
		: InputHandler()
	{}

	Win32InputHandler::~Win32InputHandler()
	{}

	void Win32InputHandler::OnInputEvent(const InputDesc& inputDesc)
	{
		if (m_InputQueue.size() >= MAX_QUEUE_SIZE)
		{
			m_InputQueue = std::queue<InputDesc>();
		}

		m_InputQueue.push(inputDesc);
	}

	const InputDesc& Win32InputHandler::GetInputEvent() const
	{
		if (m_InputQueue.empty())
		{
			static InputDesc null;
			return null;
		}

		return m_InputQueue.front();
	}

	bool Win32InputHandler::PopInputEvent()
	{
		if (!m_InputQueue.empty())
		{
			m_InputQueue.pop();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Win32InputHandler::IsEmpty() const
	{
		return m_InputQueue.empty();
	}
}