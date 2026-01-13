#ifndef __WTR_RENDERCOMMAND_H__
#define __WTR_RENDERCOMMAND_H__

#include <ECS/include/UUID/UUID.h>
#include <Renderer/RenderTypes.h>

namespace wtr
{
	struct RenderCommandBase
	{
		ECS::UUID pipeLine;
	};

	struct RenderCommandClear : public RenderCommandBase
	{
		eClearBuffer clear;
	};

	struct RenderCommandDraw : public RenderCommandBase
	{
		
	};

	struct RenderCommandDispatch : public RenderCommandBase
	{};

	struct RenderCommandUpdateBuffer : public RenderCommandBase
	{};

	struct RenderCommandUpdateTexture : public RenderCommandBase
	{};

	struct RenderCommandUpdateSmapler : public RenderCommandBase
	{};

	struct RenderCommandCopyBuffer : public RenderCommandBase
	{};

	struct RenderCommandCopyTexture : public RenderCommandBase
	{};

	struct RenderCommandViewport : public RenderCommandBase
	{};
}

#endif // __WTR_RENDERCOMMAND_H__