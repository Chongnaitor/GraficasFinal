#pragma once
#include "Defines.h"
/**
 * @file CViewPort.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del viewport.
 *
 *En este encabezado estableceremos los metodos y variables para el uso y dterminacion de dimensiones del viewport.
 *
 *
 */

 /**
  * @brief Descriptor del Viewport.
  *
  * Se usa un descriptor para el mejor uso y administracion de datos del viewport que es el aerea donde se pinta el render target
  */
struct ViewportDesc {

	VIEWPORT vp;/**<Descripcion del view port*/

	FLOAT Width;/**<Alto de la ventana*/
	FLOAT Height;/**<Ancho de la ventana*/
	FLOAT MinDepth;/**<Minima profundidad*/
	FLOAT MaxDepth;/**<Maxima profundidad*/
	FLOAT TopLeftX;/**<Lado izquierdo superior en x de la pantalla*/
	FLOAT TopLeftY;/**<Lado superior izquierdo en y de la pantalla*/
};
/**
 * @brief Descriptor.
 *
 * Descripcion
 */
class CViewPort {

	public:
	

		ViewportDesc m_ViewDesc;/**<Variable de tipo descriptor para mejor manejo de datos*/
		/**
		*@brief Inicializaion del viewport
		*@param1 @c ViewPortDesc @c _viewDesc
		*@return
		*/
		void 
		Init(ViewportDesc _viewDesc);
	

#ifdef D3D11
		D3D11_VIEWPORT vpD3D11;/**<Variable de tipo viewpor de DirectX*/
#endif // D3D11
};