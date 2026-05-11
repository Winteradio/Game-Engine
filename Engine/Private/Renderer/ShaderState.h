#ifndef __WTR_SHADERSTATE_H__
#define __WTR_SHADERSTATE_H__

#include <Reflection/include/Type/TypeMacro.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Renderer/RenderResource.h>

namespace wtr
{
	class RHIShader;
	class ShaderAsset;
};

namespace wtr
{
	class ShaderState : public RenderState
	{
		GENERATE(ShaderState);
	public:
		ShaderState(Memory::RefPtr<const ShaderAsset> shaderAsset);
		virtual ~ShaderState() = default;

	public:
		eResourceState GetResourceState() const override;
		Memory::RefPtr<const RHIShader> GetShader() const;

	private:
		Memory::RefPtr<const ShaderAsset> m_shaderAsset;
	};

	class GeometryVSState : public ShaderState
	{
		GENERATE(GeometryVSState);
	};

	class GeometryPSState : public ShaderState
	{
		GENERATE(GeometryPSState);
	};

	class LightVSState : public ShaderState
	{
		GENERATE(LightVSState);
	};

	class LightPSState : public ShaderState
	{
		GENERATE(LightPSState);
	};

	class ShadowVSState : public ShaderState
	{
		GENERATE(ShadowVSState);
	};

	class ShadowPSState : public ShaderState
	{
		GENERATE(ShadowPSState);
	};

	class ComputeCSState : public ShaderState
	{
		GENERATE(ComputeCSState);
	};
};

#endif // __WTR_SHADERSTATE_H__