#include <Platform/Win32/Win32InputHandler.h>

#include <Log/include/Log.h>

namespace wtr
{
	Win32InputHandler::Win32InputHandler()
		: InputHandler()
		, m_inputQueue()
	{}

	Win32InputHandler::~Win32InputHandler()
	{}

	void Win32InputHandler::OnInputEvent(const InputDesc& inputDesc)
	{
		if (m_inputQueue.size() >= MAX_QUEUE_SIZE)
		{
			m_inputQueue = std::queue<InputDesc>();
		}

		m_inputQueue.push(inputDesc);
	}

	const InputDesc& Win32InputHandler::GetInputEvent() const
	{
		if (m_inputQueue.empty())
		{
			static InputDesc null;
			return null;
		}

		return m_inputQueue.front();
	}

	bool Win32InputHandler::PopInputEvent()
	{
		if (!m_inputQueue.empty())
		{
			m_inputQueue.pop();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Win32InputHandler::IsEmpty() const
	{
		return m_inputQueue.empty();
	}
}