#include <Platform/Win32/Win32Window.h>

#include <Framework/Input/InputHandler.h>
#include <Framework/Input/InputTypes.h>
#include <Log/include/Log.h>

namespace wtr
{
	Win32Window::Win32Window()
		: m_windowHandle(nullptr)
		, m_windowClass{0}
		, m_refInputHandler(nullptr)
		, m_name("")
		, m_width(800)
		, m_height(600)
		, m_posX(0)
		, m_posY(0)
		, m_closeCallback()
	{}

	Win32Window::~Win32Window()
	{
		if (m_windowHandle != nullptr)
		{
			Shutdown();
		}
	}

	bool Win32Window::Init(const WindowDesc& desc)
	{
		LOGINFO() << "[Window] Initializing Win32 Window : " << desc.Name;

		m_name = desc.Name;
		m_width = desc.Width;
		m_height = desc.Height;
		m_posX = desc.PosX;
		m_posY = desc.PosY;

		HINSTANCE hInstance = GetModuleHandle(0);

		m_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		m_windowClass.hInstance = hInstance;
		m_windowClass.hCursor = LoadCursor(0, IDC_ARROW);
		m_windowClass.hbrBackground = 0;
		m_windowClass.lpfnWndProc = InputCallback;
		m_windowClass.lpszClassName = m_name.c_str();

		if (!RegisterClassA(&m_windowClass))
		{
			DWORD errCode = GetLastError();
			LOGERROR() << "[Window] Failed to register window | Error : " << static_cast<UINT64>(errCode);
			return false;
		}

		RECT rect;
		rect.left = static_cast<LONG>(0);
		rect.top = static_cast<LONG>(0);
		rect.right = static_cast<LONG>(m_width);
		rect.bottom = static_cast<LONG>(m_height);

		DWORD windowStyle = WS_OVERLAPPEDWINDOW;
		AdjustWindowRect(&rect, windowStyle, false);

		m_windowHandle = CreateWindowExA(
			0,
			m_windowClass.lpszClassName,
			m_name.c_str(),
			windowStyle,
			m_posX,
			m_posY,
			rect.right - rect.left,
			rect.bottom - rect.top,
			0,
			0,
			hInstance,
			this);

		if (!m_windowHandle)
		{
			DWORD errCode = GetLastError();
			LOGERROR() << "[Window] Failed to create window instance | Error : " << static_cast<UINT64>(errCode);
			return false;
		}

		LOGINFO() << "[Window] Succssed to create window : " << m_name;

		return true;
	}

	void Win32Window::Shutdown()
	{
		LOGINFO() << "[Window] Shut down the Win32 Window";

		if (m_windowHandle)
		{
			SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, 0);

			if (IsWindow(m_windowHandle))
			{
				DestroyWindow(m_windowHandle);
				PostQuitMessage(0);
			}
			m_windowHandle = nullptr;
		}

		HINSTANCE hInstance = m_windowClass.hInstance;
		UnregisterClassA(m_windowClass.lpszClassName, m_windowClass.hInstance);

		m_refInputHandler = nullptr;
	}

	bool Win32Window::PollEvents()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};

		return true;
	};

	void Win32Window::Show()
	{
		if (m_windowHandle)
		{
			ShowWindow(m_windowHandle, SW_SHOW);

			UpdateWindow(m_windowHandle);
		}
	}

	const eWindowStatus Win32Window::GetStatus() const
	{
		if (!IsWindow(m_windowHandle))
		{
			return eWindowStatus::eClosed;
		}

		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(m_windowHandle, &placement);

		if (placement.showCmd == SW_MINIMIZE || placement.showCmd == SW_SHOWMINIMIZED)
		{
			return eWindowStatus::eMinimized;
		}

		return eWindowStatus::eActive;
	}

	void* Win32Window::GetNativeHandle() const
	{
		return static_cast<void*>(m_windowHandle);
	}

	void Win32Window::SetInputHandler(Memory::RefPtr<InputHandler> inputHandler)
	{
		m_refInputHandler = inputHandler;
	}

	Memory::RefPtr<const InputHandler> Win32Window::GetInputHandler() const
	{
		return m_refInputHandler;
	}

	void Win32Window::SetCloseCallback(const std::function<void()>& closeCallback)
	{
		m_closeCallback = closeCallback;
	}

	LRESULT CALLBACK Win32Window::InputCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Win32Window* window = nullptr;

		if (uMsg == WM_NCCREATE) 
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			window = (Win32Window*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		} 
		else 
		{
			window = (Win32Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (!window)
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		else
		{
			switch (uMsg)
			{
				case WM_CLOSE:
				case WM_DESTROY:
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_MOUSEMOVE:
				case WM_MBUTTONUP:
				case WM_MBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_LBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_MOUSEWHEEL:
				case WM_SIZE:
				case WM_SIZING:
				{
					return window->HandleMessage(uMsg, wParam, lParam);
				}

				default:
					break;
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT Win32Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (m_refInputHandler)
		{
			switch (uMsg)
			{
				case WM_CLOSE:
				case WM_DESTROY:
				{
					if (m_closeCallback)
					{
						m_closeCallback();
					}
					break;
				}

				case WM_KEYDOWN:
				{
					bool isRepeat = (lParam & (0x01 << 30)) != 0;
					eInputAction action = isRepeat ? eInputAction::eRepeat : eInputAction::ePress;
					InputDesc desc = InputDesc::Keyboard(ConvertToKeyCode(wParam), action);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_KEYUP:
				{
					InputDesc desc = InputDesc::Keyboard(ConvertToKeyCode(wParam), eInputAction::eRelease);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_MOUSEMOVE:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					const InputDesc& lastDesc = m_refInputHandler->GetInputEvent();
					InputDesc desc = InputDesc::MouseMove(x, y, 0, 0);
					desc.DeltaX = (lastDesc.Type == eInputType::eMouseMove) ? (x - lastDesc.X) : 0;
					desc.DeltaY = (lastDesc.Type == eInputType::eMouseMove) ? (y - lastDesc.Y) : 0;
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_LBUTTONDOWN:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					InputDesc desc = InputDesc::MouseButton(eKeyCode::eKey_MouseLeft, eInputAction::ePress, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_LBUTTONUP:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					InputDesc desc = InputDesc::MouseButton(eKeyCode::eKey_MouseLeft, eInputAction::eRelease, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_RBUTTONDOWN:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					InputDesc desc = InputDesc::MouseButton(eKeyCode::eKey_MouseRight, eInputAction::ePress, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_RBUTTONUP:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					InputDesc desc = InputDesc::MouseButton(eKeyCode::eKey_MouseRight, eInputAction::eRelease, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_MBUTTONDOWN:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					InputDesc desc = InputDesc::MouseButton(eKeyCode::eKey_MouseMiddle, eInputAction::ePress, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_MBUTTONUP:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					InputDesc desc = InputDesc::MouseButton(eKeyCode::eKey_MouseMiddle, eInputAction::eRelease, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_MOUSEWHEEL:
				{
					int x = LOWORD(lParam);
					int y = HIWORD(lParam);
					float scrollDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
					InputDesc desc = InputDesc::MouseScroll(scrollDelta, x, y);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_SIZE:
				{
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);
					InputDesc desc = InputDesc::WindowResize(width, height);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				case WM_SIZING:
				{
					RECT* rect = reinterpret_cast<RECT*>(lParam);
					int width = rect->right - rect->left;
					int height = rect->bottom - rect->top;
					InputDesc desc = InputDesc::WindowResize(width, height);
					m_refInputHandler->OnInputEvent(desc);
					break;
				}

				default:
					break;
			}
		}

		return DefWindowProc(m_windowHandle, uMsg, wParam, lParam);
	}

	eKeyCode Win32Window::ConvertToKeyCode(WPARAM wParam)
	{
		if (wParam >= 0x41 && wParam <= 0x5A)
		{
			return static_cast<eKeyCode>(static_cast<uint16_t>(eKeyCode::eKey_A) + (wParam - 0x41));
		}

		if (wParam >= 0x30 && wParam <= 0x39)
		{
			return static_cast<eKeyCode>(static_cast<uint16_t>(eKeyCode::eKey_0) + (wParam - 0x30));
		}

		switch (wParam)
		{
			case VK_ESCAPE:     return eKeyCode::eKey_Escape;
			case VK_TAB:        return eKeyCode::eKey_Tab;
			case VK_CAPITAL:    return eKeyCode::eKey_CapsLock;
			case VK_SPACE:      return eKeyCode::eKey_Space;
			case VK_RETURN:     return eKeyCode::eKey_Enter;
			case VK_BACK:       return eKeyCode::eKey_Backspace;

			case VK_SHIFT:      return eKeyCode::eKey_Shift;
			case VK_CONTROL:    return eKeyCode::eKey_Control;
			case VK_MENU:       return eKeyCode::eKey_Alt;

			case VK_LSHIFT:     return eKeyCode::eKey_LeftShift;
			case VK_RSHIFT:     return eKeyCode::eKey_RightShift;
			case VK_LCONTROL:   return eKeyCode::eKey_LeftControl;
			case VK_RCONTROL:   return eKeyCode::eKey_RightControl;
			case VK_LMENU:      return eKeyCode::eKey_LeftAlt;
			case VK_RMENU:      return eKeyCode::eKey_RightAlt;

			case VK_LEFT:       return eKeyCode::eKey_Left;
			case VK_UP:         return eKeyCode::eKey_Up;
			case VK_RIGHT:      return eKeyCode::eKey_Right;
			case VK_DOWN:       return eKeyCode::eKey_Down;

			default:            return eKeyCode::eNone;
		}
	}
};