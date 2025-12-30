#ifndef __WTR_INPUTTYPES_H__
#define __WTR_INPUTTYPES_H__

#include <cstdint>
#include <string>

namespace wtr
{
	enum class eKeyCode : uint16_t
	{
		eNone = 0,

		// Alphabet
		eKey_A, eKey_B, eKey_C, eKey_D, eKey_E, eKey_F, eKey_G, eKey_H, eKey_I, eKey_J,
		eKey_K, eKey_L, eKey_M, eKey_N, eKey_O, eKey_P, eKey_Q, eKey_R, eKey_S, eKey_T,
		eKey_U, eKey_V, eKey_W, eKey_X, eKey_Y, eKey_Z,

		// Number
		eKey_0, eKey_1, eKey_2, eKey_3, eKey_4, eKey_5, eKey_6, eKey_7, eKey_8, eKey_9,

		// Special Keys
		eKey_Escape,
		eKey_Tab,
		eKey_CapsLock,
		eKey_Space,
		eKey_Enter,
		eKey_Backspace,

		// Modifier Keys
		eKey_Shift,         // No left/right distinction
		eKey_Control,       // No left/right distinction
		eKey_Alt,           // No left/right distinction
		eKey_LeftShift,
		eKey_LeftControl,
		eKey_LeftAlt,
		eKey_RightShift,
		eKey_RightControl,
		eKey_RightAlt,

		// Arrow Keys
		eKey_Left,
		eKey_Up,
		eKey_Right,
		eKey_Down,

		// Mouse
		eKey_MouseLeft = 0x80,
		eKey_MouseRight,
		eKey_MouseMiddle,

		eEnd,
	};

	enum class eInputAction : uint8_t
	{
		eNone = 0,
		eDown = 1 << 0,
		ePress = 1 << 1,
		eRelease = 1 << 2,
		eRepeat = 1 << 3,
		eEnd = 0xFF,
	};

	enum class eInputType : uint8_t
	{
		eNone = 0,
		eKeyboard,
		eMouseButton,
		eMouseMove,
		eMouseScroll,
		eWindowResize,
		eEnd
	};

	inline eKeyCode operator|(eKeyCode a, eKeyCode b)
	{
		return static_cast<eKeyCode>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
	}

	inline eKeyCode operator&(eKeyCode a, eKeyCode b)
	{
		return static_cast<eKeyCode>(static_cast<uint16_t>(a) & static_cast<uint16_t>(b));		
	}

	inline eInputAction operator|(eInputAction a, eInputAction b)
	{
		return static_cast<eInputAction>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eInputAction operator&(eInputAction a, eInputAction b)
	{
		return static_cast<eInputAction>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));		
	}
	
	struct InputDesc
	{
		eInputType		Type;
		eKeyCode		Key;
		eInputAction	Action;
		
		int				X;
		int				Y;
		int				DeltaX;
		int				DeltaY;
		float 			ScrollDelta;

		InputDesc();

		static InputDesc Keyboard(eKeyCode key, eInputAction action);
		static InputDesc MouseButton(eKeyCode mouse, eInputAction action, int x, int y);
		static InputDesc MouseMove(int x, int y, int deltaX, int deltaY);
		static InputDesc MouseScroll(float scrollDelta, int x, int y);
		static InputDesc WindowResize(int width, int height);

		static std::string ToString(const eKeyCode key);
		static std::string ToString(const eInputAction action);
		static std::string ToString(const eInputType type);

		static std::string ToStringKeyboard(const InputDesc& desc);
		static std::string ToStringMouseButton(const InputDesc& desc);
		static std::string ToStringMouseMove(const InputDesc& desc);
		static std::string ToStringMouseScroll(const InputDesc& desc);
		static std::string ToStringWindowResize(const InputDesc& desc);
	};
};

#endif // __WTR_INPUTTYPES_H__