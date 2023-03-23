#ifndef __RENDERINGSTATUS_H__
#define __RENDERINGSTATUS_H__

#include "DirectXAPI.h"

class RenderingStatus
{
	private :
		RenderingStatus();
		~RenderingStatus();

	public :
		static RenderingStatus& Get();
		void Init();
		void Destroy();

		void CreateRenderTargetView();

		void CreateDepthStencilState();
		void CreateRasterizerState();
		void CreateBlendState();

		void ChangeToDepthStencilStateEnable();
		void ChangeToDepthStencilStateDisable();

		void ChangeToRasterWireFrame();
		void ChangeToRasterCWCull();
		void ChangeToRasterCCWCull();
		void ChangeToRasterNoCull();

		void ChangeToBlendDisable();
		void ChangeToBlendAlphaEnable();
		void ChangeToBlendAlphaDisable();

		ID3D11RenderTargetView*& GetRenderTargetView();
		ID3D11DepthStencilView*& GetDepthStencilView();
		ID3D11DepthStencilState*& GetDepthStencilState();
		ID3D11RasterizerState*& GetRasterizerState();
		ID3D11BlendState*& GetBlendState();

	private :
		static RenderingStatus m_RenderingStatus;

		ID3D11RenderTargetView* m_RenderTargetView;

		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11DepthStencilState* m_DepthEnable;
		ID3D11DepthStencilState* m_DepthDisable;

		ID3D11RasterizerState* m_RasterWireFrame;
		ID3D11RasterizerState* m_RasterCWCull;
		ID3D11RasterizerState* m_RasterCCWCull;
		ID3D11RasterizerState* m_RasterNoCull;

		ID3D11BlendState* m_BlendDisable;
		ID3D11BlendState* m_BlendAlphaEnable;
		ID3D11BlendState* m_BlendAlphaDisable;

		ID3D11DepthStencilState* m_CurrentDepthStencilState;
		ID3D11RasterizerState* m_CurrentRasterizerState;
		ID3D11BlendState* m_CurrentBlendState;
};
#endif // __RENDERINGSTATUS_H__