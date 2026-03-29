#ifndef __WTR_WIN32_WINDOW_H__
#define __WTR_WIN32_WINDOW_H__

#include <Framework/Window.h>
#include <windows.h>

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

namespace wtr
{
	enum class eKeyCode : uint16_t;
};

namespace wtr
{
	class Win32Window : public Window
	{
		public :
			Win32Window();
			virtual ~Win32Window();

		public :
			bool Init(const WindowDesc& desc) override;
			bool PollEvents() override;
			void Shutdown() override;
			void Show() override;

			const eWindowStatus GetStatus() const override;
			void* GetNativeHandle() const override;

			void SetInputHandler(Memory::RefPtr<InputHandler> inputHandler) override;
			Memory::RefPtr<const InputHandler> GetInputHandler() const override;

			void SetCloseCallback(const std::function<void()>& callback) override;

		private :
			static LRESULT CALLBACK InputCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
			eKeyCode ConvertToKeyCode(WPARAM wParam);

		private :
			HWND		m_windowHandle;
			WNDCLASSA	m_windowClass;

			Memory::RefPtr<InputHandler> m_refInputHandler;

			std::string m_name;
			int m_width;
			int m_height;
			int m_posX;
			int m_posY;

			std::function<void()> m_closeCallback;
	};
};

#endif // __WTR_WIN32_WINDOW_H__