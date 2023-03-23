#ifndef __SHADERTYPE_H__
#define __SHADERTYPE_H__

#include "Shader.h"

class VERTEXSHADER : public SHADER
{
	public :
		VERTEXSHADER();
		VERTEXSHADER( const VERTEXSHADER& Other );
		virtual ~VERTEXSHADER();
};

class PIXELSHADER : public SHADER
{
	public :
		PIXELSHADER();
		PIXELSHADER( const PIXELSHADER& Other );
		virtual ~PIXELSHADER();
};

class HULLSHADER : public SHADER
{
	public :
		HULLSHADER();
		HULLSHADER( const HULLSHADER& Other );
		virtual ~HULLSHADER();
};

class DOMAINSHADER : public SHADER
{
	public :
		DOMAINSHADER();
		DOMAINSHADER( const DOMAINSHADER& Other );
		virtual ~DOMAINSHADER();
};

class GEOMETRYSHADER : public SHADER
{
	public :
		GEOMETRYSHADER();
		GEOMETRYSHADER( const GEOMETRYSHADER& Other );
		virtual ~GEOMETRYSHADER();	
};

class COMPUTESHADER : public SHADER
{
	public :
		COMPUTESHADER();
		COMPUTESHADER( const COMPUTESHADER& Other );
		virtual ~COMPUTESHADER();
};

#endif // __SHADERTYPE_H__