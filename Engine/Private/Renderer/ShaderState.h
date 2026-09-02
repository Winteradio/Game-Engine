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

#define SHADER_STATE(className, baseClassName) \
	class className : public baseClassName \
	{ \
		GENERATE(className); \
	public: \
		using baseClassName::baseClassName; \
	};

	SHADER_STATE(VSState, ShaderState);
	SHADER_STATE(PSState, ShaderState);
	SHADER_STATE(GSState, ShaderState);
	SHADER_STATE(HSState, ShaderState);
	SHADER_STATE(DSState, ShaderState);
	SHADER_STATE(CSState, ShaderState);

	SHADER_STATE(GeometryVSState, VSState);
	SHADER_STATE(GeometryPSState, PSState);
	SHADER_STATE(GeometryGSState, GSState);
	SHADER_STATE(GeometryHSState, HSState);
	SHADER_STATE(GeometryDSState, DSState);
	SHADER_STATE(GeometryCSState, CSState);

	SHADER_STATE(LightVSState, VSState);
	SHADER_STATE(LightPSState, PSState);

	SHADER_STATE(ShadowVSState, VSState);
	SHADER_STATE(ShadowPSState, PSState);

	SHADER_STATE(ForwardVSState, VSState);
	SHADER_STATE(ForwardPSState, PSState);

	SHADER_STATE(ComputeCSState, CSState);
	SHADER_STATE(DebugVSState, VSState);
	SHADER_STATE(DebugPSState, PSState);
};

#endif // __WTR_SHADERSTATE_H__