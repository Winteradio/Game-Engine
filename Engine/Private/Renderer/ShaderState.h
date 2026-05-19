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
	public :
		using ShaderState::ShaderState;

		virtual ~GeometryVSState() = default;
	};
	
	class GeometryGSState : public ShaderState
	{
		GENERATE(GeometryGSState);
	public:
		using ShaderState::ShaderState;

		virtual ~GeometryGSState() = default;
	};

	class GeometryHSState : public ShaderState
	{
		GENERATE(GeometryHSState);
	public:
		using ShaderState::ShaderState;

		virtual ~GeometryHSState() = default;
	};

	class GeometryDSState : public ShaderState
	{
		GENERATE(GeometryDSState);
	public:
		using ShaderState::ShaderState;

		virtual ~GeometryDSState() = default;
	};

	class GeometryPSState : public ShaderState
	{
		GENERATE(GeometryPSState);
	public:
		using ShaderState::ShaderState;

		virtual ~GeometryPSState() = default;
	};

	class LightVSState : public ShaderState
	{
		GENERATE(LightVSState);
	public:
		using ShaderState::ShaderState;

		virtual ~LightVSState() = default;
	};

	class LightPSState : public ShaderState
	{
		GENERATE(LightPSState);
	public:
		using ShaderState::ShaderState;

		virtual ~LightPSState() = default;
	};

	class ShadowVSState : public ShaderState
	{
		GENERATE(ShadowVSState);
	public:
		using ShaderState::ShaderState;

		virtual ~ShadowVSState() = default;
	};

	class ShadowPSState : public ShaderState
	{
		GENERATE(ShadowPSState);
	public:
		using ShaderState::ShaderState;

		virtual ~ShadowPSState() = default;
	};

	class ForwardVSState : public ShaderState
	{
		GENERATE(ForwardVSState);
	public:
		using ShaderState::ShaderState;

		virtual ~ForwardVSState() = default;
	};

	class ForwardPSState : public ShaderState
	{
		GENERATE(ForwardPSState);
	public:
		using ShaderState::ShaderState;

		virtual ~ForwardPSState() = default;
	};

	class PostProcessVSState : public ShaderState
	{
		GENERATE(PostProcessVSState);
	public:
		using ShaderState::ShaderState;

		virtual ~PostProcessVSState() = default;
	};

	class PostProcessPSState : public ShaderState
	{
		GENERATE(PostProcessPSState);
	public:
		using ShaderState::ShaderState;

		virtual ~PostProcessPSState() = default;
	};

	class ComputeCSState : public ShaderState
	{
		GENERATE(ComputeCSState);
	public:
		using ShaderState::ShaderState;

		virtual ~ComputeCSState() = default;
	};
};

#endif // __WTR_SHADERSTATE_H__