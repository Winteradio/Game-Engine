#ifndef __DXDATA_H__
#define __DXDATA_H__

enum class DXDATA
{
	D3D,
	CAMERA,
	IMGUI,
	INPUT,
	SHADER,
	MODEL,
	RENDERSTATE,
	WINDOW
};

class DXDATA
{
	public :
		DXDATA();
		DXDATA( const DXDATA* Other );
		~DXDATA();

	public :
		bool Init();
		bool Frame();
		void Release();

	public :
		void* GetData();
		map<DXDATATYPE, void*>* GetDataMapPointer();

		map<DXDATATYPE, void*> m_DXDataMap;
};

#endif // _DXDATA_H__