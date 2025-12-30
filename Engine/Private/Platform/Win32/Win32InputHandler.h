#ifndef __WTR_WIN32_INPUTHANDLER_H__
#define __WTR_WIN32_INPUTHANDLER_H__

#include <Framework/Input/InputHandler.h>
#include <Framework/Input/InputTypes.h>

#include <queue>

namespace wtr
{
	class Win32InputHandler : public InputHandler
	{
		public :
			Win32InputHandler();
			virtual ~Win32InputHandler();

		public :
			void OnInputEvent(const InputDesc& inputDesc) override;
			bool PopInputEvent() override;
			const InputDesc& GetInputEvent() const override;
			bool IsEmpty() const override;

		private :
			std::queue<InputDesc> m_InputQueue;

			static constexpr size_t MAX_QUEUE_SIZE = 256;
	};
};

#endif // __WTR_WIN32_INPUTHANDLER_H__