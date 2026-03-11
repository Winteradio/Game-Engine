#include <Framework/FrameView.h>

#include <Framework/SceneView.h>
#include <Memory/include/Core.h>

namespace wtr
{
	FrameView::FrameView()
		: m_views()
		, m_viewIndex(0)
		, m_frame(0)
		, m_eState(eFrameState::eFree)
	{}

	FrameView::~FrameView()
	{}

	Memory::RefPtr<SceneView> FrameView::GetScene()
	{
		if ((m_viewIndex + 1) >= m_views.Size())
		{
			Memory::RefPtr<SceneView> newScene = Memory::MakeRef<SceneView>();
			m_views.PushBack(newScene);
		}

		m_viewIndex++;

		return m_views[m_viewIndex - 1];
	}

	void FrameView::ResetScene()
	{
		m_viewIndex = 0;

		for (auto& sceneView : m_views)
		{
			sceneView.Reset();
		}
	}

	void FrameView::ClearScene()
	{
		m_views.Clear();
		m_viewIndex = 0;
	}

	const eFrameState FrameView::GetState() const
	{
		return m_eState.load();
	}

	void FrameView::SetState(const eFrameState state)
	{
		m_eState.exchange(state);
	}

	const size_t FrameView::GetFrame() const
	{
		return m_frame;
	}

	void FrameView::SetFrame(const size_t frame)
	{
		m_frame = frame;
	}
}