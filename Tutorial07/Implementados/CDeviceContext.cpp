#include "..\Encabezados\CDeviceContext.h"

///
/// function to initialize the Device Context variable
///
#ifdef D3D11
void CDeviceContext::Init(ID3D11DeviceContext* _context){

	g_pImmediateContextD3D11 = _context;
}
#endif



///
/// function to get data from Dev Cont
///
void* CDeviceContext::GetDevCont()
{
#if defined(D3D11)
	return  g_pImmediateContextD3D11;
#endif
	return nullptr;
}
