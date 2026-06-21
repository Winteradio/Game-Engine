#ifndef __WTR_LIGHTPROXY_H__
#define __WTR_LIGHTPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>
#include <Renderer/Proxy/ShaderProxy.h>
#include <Renderer/LightDesc.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
    enum class eResourceState : uint8_t;
    
    struct RenderView;
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
        virtual void UpdateView(const RenderView& renderView) = 0;

        virtual float GetViewWidth() const = 0;
        virtual float GetViewHeight() const = 0;
        virtual fvec2 GetViewDepth() const = 0;

    public :
        void SetLightType(const eLightType lightType);
        void SetShadowType(const eShadowType shadowType);

        void SetColor(const fvec3 color);
        void SetIntensity(const float intensity);

        Memory::RefPtr<const RHIBuffer> GetLightBuffer() const;
		Memory::RefPtr<const RHITexture> GetShadowMap() const;
        Memory::RefPtr<const RHIRenderTarget> GetShadowTarget() const;

		const LightDesc& GetLightDesc() const;
        Memory::RefPtr<const ScalarData<fmat4>> GetLightView() const;
        Memory::RefPtr<const ScalarData<fmat4>> GetLightProj() const;

    private :
        virtual Light* GetLightData() = 0;
    
    protected :
        struct alignas(16) Light
        {
            alignas(16) fvec3 color = fvec3(1.f);
            alignas(16) fvec3 pos = fvec3(0.f);
            alignas(4) float intensity = 1.f;
        };

		LightDesc m_lightDesc;

        Memory::RefPtr<ScalarData<fmat4>> m_lightView;
        Memory::RefPtr<ScalarData<fmat4>> m_lightProj;

        Memory::RefPtr<RHIBuffer> m_lightBuffer;
        Memory::RefPtr<RHITexture> m_shadowMap;
        Memory::RefPtr<RHIRenderTarget> m_shadowTarget;
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

		void UpdateView(const RenderView& renderView) override;

        float GetViewWidth() const override;
        float GetViewHeight() const override;
        fvec2 GetViewDepth() const override;

    private :
        Light* GetLightData() override;

    private :
        struct alignas(16) DirectionalLight : Light
        {
            alignas(16) fvec3 direction = util::GetForwardDirection();
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

        void UpdateView(const RenderView& renderView) override;

        float GetViewWidth() const override;
        float GetViewHeight() const override;
        fvec2 GetViewDepth() const override;

    public :
		void SetRange(const float range);

    private:
        Light* GetLightData() override;
    
    private :
        struct alignas(16) PointLight : Light
        {
            alignas(4) float range = 30.f;
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

        void UpdateView(const RenderView& renderView) override;

		float GetViewWidth() const override;
		float GetViewHeight() const override;
		fvec2 GetViewDepth() const override;

    public :
        void SetRange(const float range);
		void SetInnerAngle(const float innerAngle);
		void SetOuterAngle(const float outerAngle);

    private:
        Light* GetLightData() override;

    private :
        struct alignas(16) SpotLight : Light
        {
            alignas(16) fvec3 direction = util::GetForwardDirection();
            alignas(4) float range = 0.f;
            alignas(4) float innerAngle = 30.f;
            alignas(4) float outerAngle = 90.f;
        };

        Memory::RefPtr<ScalarData<SpotLight>> m_spot;
    };
};

#endif // __WTR_LIGHTPROXY_H__