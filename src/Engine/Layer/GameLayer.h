#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "ILayer.h"
#include "DirectXAPI.h"

class GameLayer : public ILayer
{
	public :
		GameLayer();
		GameLayer( const GameLayer& Other );
		virtual ~GameLayer();

	public :
		void Init( int PosX, int PosY, int Width, int Height );
		void Destroy();

		void CreateViewport();
		void CreateProjectionMatrix();
		void CreateOrthoMatrix();

		D3D11_VIEWPORT& GetViewport();
		XMMATRIX& GetProjectionMatrix();
		XMMATRIX& GetOrthoMatrix();
	private :
		D3D11_VIEWPORT m_Viewport;
		XMMATRIX m_ProjectionMatrix;
		XMMATRIX m_OrthoMatrix;
};

#endif // __GAMELAYER_H__