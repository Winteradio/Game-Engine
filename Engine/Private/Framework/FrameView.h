#ifndef __WTR_FRAMEVIEW_H__
#define __WTR_FRAMEVIEW_H__

#include <cstdint>
#include <atomic>

#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class SceneView;
};

namespace wtr
{
	enum class eFrameState : uint8_t
	{
		eFree = 0x00,
		eWriting = 0x01,
		eReady = 0x02,
		eReading = 0x03
	};

	class FrameView
	{
		public :
			FrameView();
			~FrameView();

		public :
			Memory::RefPtr<SceneView> GetScene();
			void ResetScene();
			void ClearScene();

			const eFrameState GetState() const;
			void SetState(const eFrameState state);

			const size_t GetFrame() const;
			void SetFrame(const size_t frame);

		private :
			wtr::DynamicArray<Memory::RefPtr<SceneView>> m_views;
			std::size_t m_viewIndex;
			std::size_t m_frame;
			std::atomic<eFrameState> m_eState;
	};
};

#endif // __WTR_FRAMEVIEW_H__