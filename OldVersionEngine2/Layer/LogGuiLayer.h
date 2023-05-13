#ifndef __LOGGUILAYER_H__
#define __LOGGUILAYER_H__

#include "IGuiLayer.h"

class LogGuiLayer : public IGuiLayer
{
	public :
		LogGuiLayer();
		LogGuiLayer( const LogGuiLayer& Other );
		~LogGuiLayer();

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height );
		virtual void Destroy();
		virtual void Update();
		virtual void Render();
};

#endif // __LOGGUILAYER_H__