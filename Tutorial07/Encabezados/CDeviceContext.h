#pragma once
#include "Defines.h"

/**
 * @file CDeviceContext.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del DeviceContext.
 *
 *En este encabezado estableceremos los metodos y variables para el uso del DeviceContext para el seteo de recursos.
 *
 *
 */

 /**
  * @brief Descriptor de tipo DeviceContext
  *
  * Descripcion: Se crea un descriptor para mejor gestion de la informacion (No es necesaria en este caso) 
  */

struct DeviceContextDescriptor {

};
/**
 * @brief Clase Device context.
 *
 * Descripcion Sirve para la utilizacion de recursos asignados
 */

class CDeviceContext {

	public:

		
		DeviceContextDescriptor m_DescDCont/**<Miembro de tipo descriptor (No es necesario por el momento)*/;

		
		/**
		*@brief Constructor (No utilizado por el momento)
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		CDeviceContext() {};
		/**
	*@brief Destructor (No se utiliza actualmente)
	*@param No tiene parametros
	*@return No devuelve nada
	*/
		~CDeviceContext() {};

		//
#ifdef D3D11
		/**
		*@brief Inicializador del devicecontext
		*@param1 @c ID3D11DeviceContext* @c _context
		*@return No devuelve nada
		*/
		void
		Init(ID3D11DeviceContext* _context);
#endif
	
	/**
	*@brief Obtiene el device context para uso de puteros void
	*@param No tiene parametros
	*@return No devuelve nada
	*/
		void*
		GetDevCont();

#ifdef D3D11
		ID3D11DeviceContext* g_pImmediateContextD3D11/**<Variable Device context de DirectX*/;
#endif // D3D11
};