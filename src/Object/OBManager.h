#ifndef __OBMANAGER_H__
#define __OBMANAGER_H__

#include "DXCommon.h"
#include "OBModel.h"
#include "OBShader.h"

class OBMANAGERW
{
	public :
		OBMANAGER();
		OBMANAGER( const OBMANAGER* Other );
		~OBMANAGER();

	public :
		bool Init();
		bool CreateModel();
		bool CreateShader();

		void SetShadertoModel( OBMODEL* model, OBSHADER* shader );

		OBMODEL* GetModels();
		OBSHADER* GetShaders();

		OBMODEL& GetModel( int index );
		OBSHADER& GetShader( int index );

	private :
		OBMODEL* m_Models;
		OBSHADER* m_Shaders;
};

#endif // __OBMANAGER_H__