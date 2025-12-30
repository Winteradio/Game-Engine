#ifndef __WTR_WINDOW_H__
#define __WTR_WINDOW_H__

#include <string>
#include <cstdint>

namespace wtr
{
	class InputHandler;
};

namespace wtr
{
	enum class eWindowType : uint8_t
	{
		eNone = 0,
		eWin32 = 1,
		eEnd,
	};

	enum class eWindowStatus : uint8_t
	{
		eActive = 0,
		eMinimized = 1,
		eClosed = 2,
	};

	struct WindowDesc
	{
		eWindowType	Type;
		std::string	Name;
		float		PosX;
		float		PosY;
		float		Width;
		float		Height;

		WindowDesc()
			: Type(eWindowType::eNone)
			, Name("Default Window")
			, PosX(0.0f)
			, PosY(0.0f)
			, Width(800.0f)
			, Height(600.0f)
		{}
	};
};

namespace wtr
{

	class Window
	{
		public :
			Window() = default;
			virtual ~Window() = default;

		public :
			virtual bool Init(const WindowDesc& desc) = 0;
			virtual bool PollEvents() = 0;
			virtual void Shutdown() = 0;
			virtual void Show() = 0;

			virtual const eWindowStatus GetStatus() const = 0;
			virtual void* GetNativeHandle() const = 0;

			virtual void SetInputHandler(InputHandler* inputHandler) = 0;
			virtual const InputHandler* GetInputHandler() const = 0;
	};
};

#endif // __WTR_WINDOW_H__