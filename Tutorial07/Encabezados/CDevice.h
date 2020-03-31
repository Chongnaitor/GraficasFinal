#pragma once
#include "Defines.h"
/**
 * @file CDevice.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del Device.
 *
 *En este encabezado estableceremos los metodos y variables para el uso de device que se encarga de crear recursos.
 *
 *
 */

 /**
  * @brief Descriptor.
  *
  * Descripcion: Descriptor de tipo device en el cual se usa para manejo de datos mas efectivo
  */

struct DeviceDescriptor {

	UINT s_createDeviceFlags;/**<Descibe parametros usados para crear un device*/
	UINT s_numFeatureLevels;/**<Numero de elemrntos en featurelevel*/
	DRIVER_TYPE		g_driverType;/**<Tipo de dryvers como opcion*/
	FEATURE_LEVEL	g_featureLevel;/**<Describe el conjuneto de features usado por un Direct3D device*/
};

/**
 * @brief CDevice: Clase de tipo device.
 *
 * Descripcion: Se encarga de crear recursos
 */

class CDevice {

	public:
		
		DeviceDescriptor m_DescDevice;/**<Estructura de descripcion para mejor manejo de datos*/
		
		/**
		*@brief Constructor
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		CDevice() {};
		/**
	*@brief Destructor
	*@param No tiene parametros
	*@return No devuelve nada
	*/
		~CDevice() {};

		/**
		*@brief Inicializador del device 
		*@param1 @c DeviceDescriptor @c  _devDesc Miembro de tipo descriptor
		*@return No devuelve nada
		*/
		void
		Init(DeviceDescriptor _devDesc);
	/**
	*@brief Actualiza el device (actualmente no esta en uso)
	*@param No tiene parametros
	*@return No devuelve nada
	*/
		void
		Update();
	
		/**
	*@brief Establece el device en nulo(actualmente no esta en uso)
	*@param No tiene parametros
	*@return No devuelve nada
	*/
		void
		Delete();
	/**
	*@brief Obtiene el device, se usa mas que nada para punteros void
	*@param No tiene parametros
	*@return No devuelve nada
	*/

		void*
		GetDev();


#ifdef D3D11
		ID3D11Device*		g_pd3dDeviceD3D11;/**<Device de tipo DirectX*/
		D3D_DRIVER_TYPE		g_driverTypeD3D11;/**<Drivertype de tipo DirectX*/
		D3D_FEATURE_LEVEL	g_featureLevelD3D11;/**<Feature level de tipo DirectX*/
		
#endif // D3D11
};