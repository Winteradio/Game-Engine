#include <Framework/Input/InputTypes.h>

namespace wtr
{
	InputDesc::InputDesc()
		: Type(eInputType::eNone)
		, Key(eKeyCode::eNone)
		, Action(eInputAction::eNone)
		, X(0)
		, Y(0)
		, DeltaX(0)
		, DeltaY(0)
		, ScrollDelta(0.0f)
	{}

	InputDesc InputDesc::Keyboard(eKeyCode key, eInputAction action)
	{
		InputDesc desc;
		desc.Type = eInputType::eKeyboard;
		desc.Key = key;
		desc.Action = action;
		
		return desc;
	}

	InputDesc InputDesc::MouseButton(eKeyCode mouse, eInputAction action, int x, int y)
	{
		InputDesc desc;
		desc.Type = eInputType::eMouseButton;
		desc.Key = mouse;
		desc.Action = action;
		desc.X = x;
		desc.Y = y;
		
		return desc;
	}

	InputDesc InputDesc::MouseMove(int x, int y, int deltaX, int deltaY)
	{
		InputDesc desc;
		desc.Type = eInputType::eMouseMove;
		desc.X = x;
		desc.Y = y;
		desc.DeltaX = deltaX;
		desc.DeltaY = deltaY;
		
		return desc;
	}

	InputDesc InputDesc::MouseScroll(float scrollDelta, int x, int y)
	{
		InputDesc desc;
		desc.Type = eInputType::eMouseScroll;
		desc.ScrollDelta = scrollDelta;
		desc.X = x;
		desc.Y = y;
		
		return desc;
	}

	InputDesc InputDesc::WindowResize(int width, int height)
	{
		InputDesc desc;
		desc.Type = eInputType::eWindowResize;
		desc.X = width;
		desc.Y = height;
		
		return desc;
	}

	std::string InputDesc::ToString(const eKeyCode key)
	{
		if (eKeyCode::eKey_A <= key && key <= eKeyCode::eKey_Z)
		{
			return std::string(1, static_cast<char>('A' + (static_cast<uint16_t>(key) - static_cast<uint16_t>(eKeyCode::eKey_A))));
		}
		
		if (eKeyCode::eKey_0 <= key && key <= eKeyCode::eKey_9)
		{
			return std::string(1, static_cast<char>('0' + (static_cast<uint16_t>(key) - static_cast<uint16_t>(eKeyCode::eKey_0))));
		}

		switch (key)
		{
			case eKeyCode::eNone:           return "None";
			case eKeyCode::eKey_Escape:     return "Escape";
			case eKeyCode::eKey_Tab:        return "Tab";
			case eKeyCode::eKey_CapsLock:   return "CapsLock";
			case eKeyCode::eKey_Space:      return "Space";
			case eKeyCode::eKey_Enter:      return "Enter";
			case eKeyCode::eKey_Backspace:  return "Backspace";

			case eKeyCode::eKey_Shift:      return "Shift";
			case eKeyCode::eKey_Control:    return "Control";
			case eKeyCode::eKey_Alt:        return "Alt";

			case eKeyCode::eKey_LeftShift:  return "LeftShift";
			case eKeyCode::eKey_LeftControl:return "LeftControl";
			case eKeyCode::eKey_LeftAlt:    return "LeftAlt";

			case eKeyCode::eKey_RightShift: return "RightShift";
			case eKeyCode::eKey_RightControl:return "RightControl";
			case eKeyCode::eKey_RightAlt:   return "RightAlt";

			case eKeyCode::eKey_Left:       return "Left";
			case eKeyCode::eKey_Up:         return "Up";
			case eKeyCode::eKey_Right:      return "Right";
			case eKeyCode::eKey_Down:       return "Down";

			case eKeyCode::eKey_MouseLeft:   return "MouseLeft";
			case eKeyCode::eKey_MouseRight:  return "MouseRight";
			case eKeyCode::eKey_MouseMiddle: return "MouseMiddle";

			default:                        return "Unknown";
		}
	}

	std::string InputDesc::ToString(const eInputAction action)
	{
		if (eInputAction::eNone == action)
		{
			return "None";
		}
		else if (eInputAction::eDown == action)
		{
			return "Down";
		}
		else if (eInputAction::ePress == action)
		{
			return "Press";
		}
		else if (eInputAction::eRelease == action)
		{
			return "Release";
		}
		else if (eInputAction::eRepeat == action)
		{
			return "Repeat";
		}
		else
		{
			return "Unknown";
		}

		return "Unknown";
	}

	std::string InputDesc::ToString(const eInputType type)
	{
		if (eInputType::eNone == type)
		{
			return "None";
		}
		else if (eInputType::eKeyboard == type)
		{
			return "Keyboard";
		}
		else if (eInputType::eMouseButton == type)
		{
			return "MouseButton";
		}
		else if (eInputType::eMouseMove == type)
		{
			return "MouseMove";
		}
		else if (eInputType::eMouseScroll == type)
		{
			return "MouseScroll";
		}
		else
		{
			return "Unknown";
		}

		return "Unknown";
	}

	std::string InputDesc::ToStringKeyboard(const InputDesc& desc)
	{
		return "Type : " + ToString(desc.Type) +
				" | Key : " + ToString(desc.Key) +
				" | Action : " + ToString(desc.Action);
	}

	std::string InputDesc::ToStringMouseButton(const InputDesc& desc)
	{
		return "Type : " + ToString(desc.Type) +
				" | Key : " + ToString(desc.Key) +
				" | Action : " + ToString(desc.Action) +
				" | X : " + std::to_string(desc.X) +
				" | Y : " + std::to_string(desc.Y);
	}

	std::string InputDesc::ToStringMouseMove(const InputDesc& desc)
	{
		return "Type : " + ToString(desc.Type) +
				" | X : " + std::to_string(desc.X) +
				" | Y : " + std::to_string(desc.Y) +
				" | DeltaX : " + std::to_string(desc.DeltaX) +
				" | DeltaY : " + std::to_string(desc.DeltaY);
	}

	std::string InputDesc::ToStringMouseScroll(const InputDesc& desc)
	{
		return "Type : " + ToString(desc.Type) +
				" | ScrollDelta : " + std::to_string(desc.ScrollDelta) +
				" | X : " + std::to_string(desc.X) +
				" | Y : " + std::to_string(desc.Y);
	}

	std::string InputDesc::ToStringWindowResize(const InputDesc& desc)
	{
		return "Type : " + ToString(desc.Type) +
				" | Width : " + std::to_string(desc.X) +
				" | Height : " + std::to_string(desc.Y);
	}
}