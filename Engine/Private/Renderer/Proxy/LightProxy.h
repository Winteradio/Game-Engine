#ifndef __WTR_LIGHTPROXY_H__
#define __WTR_LIGHTPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>
#include <Renderer/RenderResource.h>
#include <Renderer/LightDesc.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
    enum class eResourceState : uint8_t;
    
    class RHIBuffer;
    class RHITexture;
    class ShaderSet;
};

namespace wtr
{
    class LightProxy : public SceneProxy, public RenderResource
    {
        GENERATE(LightProxy);
    public :
        LightProxy(const ECS::UUID& id);
        virtual ~LightProxy() = default;

    public :
        eResourceState GetResourceState() const override;
        bool IsUploadable() const override;
		bool IsSyncable() const override;

    public :
        void SetLightType(const eLightType lightType);
        void SetShadowType(const eShadowType shadowType);
        void SetColor(const fvec3 color);
        void SetDirection(const fvec3 direction);
        void SetIntensity(const float intensity);

        Memory::RefPtr<const RHIBuffer> GetLightBuffer() const;
		Memory::RefPtr<const RHITexture> GetShadowMap() const;

        Memory::RefPtr<ShaderSet> GetShaderSet();
        Memory::RefPtr<const ShaderSet> GetShaderSet() const;

		const LightDesc& GetLightDesc() const;

    protected :
		LightDesc m_lightDesc;

        Memory::RefPtr<RHIBuffer> m_lightBuffer;
        Memory::RefPtr<RHITexture> m_shadowMap;
        Memory::RefPtr<ShaderSet> m_shaderSet;

		fvec3 m_color;
        fvec3 m_direction;
        float m_intensity;
    };

    class DirectionalLightProxy : public LightProxy
    {
        GENERATE(DirectionalLightProxy);
    public :
		DirectionalLightProxy(const ECS::UUID& id);
		virtual ~DirectionalLightProxy() = default;

    public :
        void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

		const fmat4 GetViewMatrix() const;
        const fmat4 GetProjectionMatrix() const;
    };

    class PointLightProxy : public LightProxy
    {
        GENERATE(PointLightProxy);
    public :
		PointLightProxy(const ECS::UUID& id);
        virtual ~PointLightProxy() = default;
	
    public:
        void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

        const fmat4 GetViewMatrix() const;
        const fmat4 GetProjectionMatrix() const;

    public :
		void SetRange(const float range);
    
    private :
		float m_range;
	};

    class SpotLightProxy : public LightProxy
    {
        GENERATE(SpotLightProxy);
    public :
        SpotLightProxy(const ECS::UUID& id);
		virtual ~SpotLightProxy() = default;

    public :
        void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

        const fmat4 GetViewMatrix() const;
        const fmat4 GetProjectionMatrix() const;

    public :
        void SetRange(const float range);
		void SetInnerAngle(const float innerAngle);
		void SetOuterAngle(const float outerAngle);

    private :
        float m_range;
		float m_innerAngle;
		float m_outerAngle;
    };
};

#endif // __WTR_LIGHTPROXY_H__