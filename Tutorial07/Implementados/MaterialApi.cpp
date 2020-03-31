#include "..\Encabezados\MaterialApi.h"



void MaterialApi::Shutdown(){

#ifdef D3D11
	if (m_TexDif) {

		m_TexDif->Release();
}
#endif // D3D11
}