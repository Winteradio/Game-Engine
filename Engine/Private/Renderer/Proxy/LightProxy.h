#ifndef __WTR_LIGHTPROXY_H__
#define __WTR_LIGHTPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>
#include <Renderer/Proxy/ShaderProxy.h>
#include <Renderer/LightDesc.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
    enum class eResourceState : uint8_t;
    
    class RHIBuffer;
    class RHITexture;
    class RHIRenderTarget;
};

namespace wtr
{
    class LightProxy : public SceneProxy, public ShaderProxy
    {
        GENERATE(LightProxy);
    public :
        LightProxy(const ECS::UUID& id);
        virtual ~LightProxy() = default;

    public :
        eResourceState GetResourceState() const override;
        bool IsUploadable() const override;
        void Unload(Memory::RefPtr<RHICommandList> cmdList) override;

    public :
        void SetLightType(const eLightType lightType);
        void SetShadowType(const eShadowType shadowType);
        void SetColor(const fvec3 color);
        void SetDirection(const fvec3 direction);
        void SetIntensity(const float intensity);

        Memory::RefPtr<const RHIBuffer> GetLightBuffer() const;
		Memory::RefPtr<const RHITexture> GetShadowMap() const;
        Memory::RefPtr<const RHIRenderTarget> GetShadowTarget() const;

		const LightDesc& GetLightDesc() const;

    protected :
		LightDesc m_lightDesc;

        Memory::RefPtr<RHIBuffer> m_lightBuffer;
        Memory::RefPtr<RHITexture> m_shadowMap;
        Memory::RefPtr<RHIRenderTarget> m_shadowTarget;

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
        bool IsSyncable() const override;

        void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

		const fmat4 GetViewMatrix() const;
        const fmat4 GetProjectionMatrix() const;

    private :
        struct alignas(16) DirectionalLight
        {
            fvec3 color = fvec3(1.f);
            fvec3 direction = fvec3(0.f, -1.0, 0.f);
            fvec3 pos = fvec3(0.f);
            float intensity = 1.f;
        };

        Memory::RefPtr<ScalarData<DirectionalLight>> m_directional;
    };

    class PointLightProxy : public LightProxy
    {
        GENERATE(PointLightProxy);
    public :
		PointLightProxy(const ECS::UUID& id);
        virtual ~PointLightProxy() = default;
	
    public:
        bool IsSyncable() const override;

        void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

        const fmat4 GetViewMatrix() const;
        const fmat4 GetProjectionMatrix() const;

    public :
		void SetRange(const float range);
    
    private :
        struct alignas(16) PointLight
        {
            fvec3 color = fvec3(1.f);
            fvec3 direction = fvec3(0.f, -1.f, 0.f);
            fvec3 pos = fvec3(0.f);
            float intensity = 1.f;
            float range = 0.f;
        };

		float m_range;

        Memory::RefPtr<ScalarData<PointLight>> m_point;
	};

    class SpotLightProxy : public LightProxy
    {
        GENERATE(SpotLightProxy);
    public :
        SpotLightProxy(const ECS::UUID& id);
		virtual ~SpotLightProxy() = default;

    public :
        bool IsSyncable() const override;

        void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
        void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

        const fmat4 GetViewMatrix() const;
        const fmat4 GetProjectionMatrix() const;

    public :
        void SetRange(const float range);
		void SetInnerAngle(const float innerAngle);
		void SetOuterAngle(const float outerAngle);

    private :
        struct alignas(16) SpotLight
        {
            fvec3 color = fvec3(1.f);
            fvec3 direction = fvec3(0.f, -1.f, 0.f);
            fvec3 pos = fvec3(0.f);
            float intensity = 1.0f;
            float range = 0.f;
            float innerAngle = 30.f;
            float outerAngle = 90.f;
        };

        float m_range;
		float m_innerAngle;
		float m_outerAngle;

        Memory::RefPtr<ScalarData<SpotLight>> m_spot;
    };
};

#endif // __WTR_LIGHTPROXY_H__