#include <Renderer/RenderCommandList.h>

namespace wtr
{
	RenderCommandList::RenderCommandList()
		: m_frame(0)
	{}

	RenderCommandList::~RenderCommandList()
	{}

	size_t RenderCommandList::GetFrame() const
	{
		return m_frame;
	}

	void RenderCommandList::SetFrame(const size_t frame)
	{
		m_frame = frame;
	}
}