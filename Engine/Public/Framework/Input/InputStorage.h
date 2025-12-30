#ifndef __WTR_INPUTSTORAGE_H__
#define __WTR_INPUTSTORAGE_H__

#include <Framework/Input/InputTypes.h>
#include <Framework/Math/MathTypes.h>

#include <string>

namespace wtr
{
	struct InputData
	{
		// Keyboard
		// Using 128 bytes (1024 bits) to cover all key codes
		// The 4 bits has the following meaning :
		// Bit 0 : Down, Bit 1 : Press, Bit 2 : Release, Bit 3 : Repeat
		// It covers the range of eKeyCode (0 - 255)
		uint8_t keyboard[128];

		// Window
		fvec2 windowSize;

		// Mouse
		fvec2 mousePos;
		fvec2 mouseDelta;
		float mouseScroll;
		
		InputData();
		InputData(const InputData& other);

		InputData& operator=(const InputData& other);

		bool operator==(const InputData& other) const;
		bool operator!=(const InputData& other) const;

		static std::string ToString(const InputData& data);
	};

	class InputStorage
	{
		public :
			InputStorage();
			~InputStorage();

		public :
			void Prepare();
			void Update(const InputDesc& inputDesc);

			bool IsChanaged() const;
			bool IsDown(eKeyCode key) const;
			bool IsPressed(eKeyCode key) const;
			bool IsReleased(eKeyCode key) const;
			bool IsRepeated(eKeyCode key) const;

			const fvec2& GetMousePosition() const;
			const fvec2& GetMouseDelta() const;
			float GetMouseScroll() const;

			const fvec2& GetWindowSize() const;

		private :
			const size_t GetKeyIndex(eKeyCode key) const;
			const size_t GetKeyBit(eKeyCode key) const;

		private :
			InputData m_PrevData;
			InputData m_CurrData;

			static constexpr size_t KEY_BIT = 4;
	};
};

#endif // __WTR_INPUTSTORAGE_H__