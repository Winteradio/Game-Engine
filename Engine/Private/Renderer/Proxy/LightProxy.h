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
        struct Light;

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
        void SetIntensity(const float intensity);

        Memory::RefPtr<const RHIBuffer> GetLightBuffer() const;
		Memory::RefPtr<const RHITexture> GetShadowMap() const;
        Memory::RefPtr<const RHIRenderTarget> GetShadowTarget() const;

		const LightDesc& GetLightDesc() const;

    private :
        virtual Light* GetLightData() = 0;
    
    protected :
        struct Light
        {
            alignas(16) fvec3 color = fvec3(1.f);
            alignas(16) fvec3 pos = fvec3(0.f);
            float intensity = 1.f;
        };

		LightDesc m_lightDesc;

        Memory::RefPtr<RHIBuffer> m_lightBuffer;
        Memory::RefPtr<RHITexture> m_shadowMap;
        Memory::RefPtr<RHIRenderTarget> m_shadowTarget;

        static inline const fvec3 BASE_DIR = fvec3(0.f, -1.f, 0.f);
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
        Light* GetLightData() override;

    private :
        struct DirectionalLight : Light
        {
            alignas(16) fvec3 direction = BASE_DIR;
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

    private:
        Light* GetLightData() override;
    
    private :
        struct PointLight : Light
        {
            float range = 0.f;
        };

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

    private:
        Light* GetLightData() override;

    private :
        struct SpotLight : Light
        {
            alignas(16) fvec3 direction = BASE_DIR;
            float range = 0.f;
            float innerAngle = 30.f;
            float outerAngle = 90.f;
        };

        Memory::RefPtr<ScalarData<SpotLight>> m_spot;
    };
};

#endif // __WTR_LIGHTPROXY_H__