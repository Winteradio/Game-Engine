#ifndef __WTR_INPUTHANDLER_H__
#define __WTR_INPUTHANDLER_H__

#include <cstdint>
#include <string>

namespace wtr
{
	struct InputDesc;
};

namespace wtr
{
	class InputHandler
	{
		public :
			InputHandler() = default;
			virtual ~InputHandler() = default;

			InputHandler(const InputHandler&) = delete;
			InputHandler& operator=(const InputHandler&) = delete;

		public :
			virtual void OnInputEvent(const InputDesc& inputDesc) = 0;
			virtual bool PopInputEvent() = 0;
			virtual const InputDesc& GetInputEvent() const = 0;
			virtual bool IsEmpty() const = 0;
	};
};

#endif // __WTR_INPUTHANDLER_H__