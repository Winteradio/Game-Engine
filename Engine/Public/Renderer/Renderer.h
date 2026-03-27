#pragma once

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RenderScene;
	class RenderCommandList;
	class RHICommandList;
};

namespace wtr
{
	class Renderer
	{
	public :
		Renderer();
		~Renderer();

	public :
		void Execute(Memory::RefPtr<RHICommandList> cmdList);

		Memory::RefPtr<RenderCommandList> GetCommandList();

	private :
		Memory::RefPtr<RenderScene> m_refScene;
		Memory::RefPtr<RenderCommandList> m_refCommandList;
	};
};