#pragma once

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RenderScene;
	class RenderCommandList;
};

namespace wtr
{
	class Renderer
	{
	public :
		Renderer();
		~Renderer();

	public :
		Memory::RefPtr<RenderCommandList> GetCommandList();

	private :
		Memory::RefPtr<Render
	};
};