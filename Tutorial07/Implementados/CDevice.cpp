#include "..\Encabezados\CDevice.h"

///
/// function to initialize the Device variable
///
void CDevice::Init(DeviceDescriptor _devDesc){

	m_DescDevice = _devDesc;
}

void CDevice::Update(){
}

void CDevice::Delete(){
}

///
/// function to get data from Dev
///
void* CDevice::GetDev(){

#if defined(D3D11)
	return  g_pd3dDeviceD3D11;
#endif
	return nullptr;
}