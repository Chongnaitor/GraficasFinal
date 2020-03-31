#pragma once
#include "Defines.h"
/**
 * @file CDepthStencil.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del DepthStencil.
 *
 *En este encabezado estableceremos los metodos y variables para el uso del DepthStencil con la funcionalida
 * de hacer mascaras.
 *
 */



 /**
  * @brief Descriptor de DepthStencil.
  *
  * Descripcion: Es mas facil el manejo de datos con estructuras
  */

struct DepthStencilViewDesc {
	FORMAT Format;/**<Recurso de formato de datos incluyendo totalmente escritos o sin tipo de dato*/
	DSV_DIMENSION ViewDimension;/**<Tipo de recurso que especifica como se va a acceder al recurso depth stencil*/
	UINT Flags;/**<Un valor que describe si la rextura solo sera solo de lectura*/
	TEX2D_DSV Texture2D;/**<Especifica un subrecurso de textura 2D*/
};

/**
 * @brief Clase DepthStencil.
 *
 * Descripcion En esta clase hay elementos y funciones para el uso correcto del depthstencil 
 */

class CDepthStencil {

	public:
		/**
		*@brief Constructor
		*@param No recibe nada
		*@return No devuelve nada
		*/
		CDepthStencil() {};
		/**
		*@brief Destructor
		*@param No recibe nada 
		*@return No devuelve nada
		*/
		~CDepthStencil() {};

		DepthStencilViewDesc m_DepthDesc;/**<Objeto de tipo descriptor que se va a usar para la manipulacion de datos eficiente*/
#ifdef D3D11
		ID3D11DepthStencilView* g_pDepthStencilViewD3D11;/**<Variable de deptstencil DirectX para llenarse con descriptor*/
		D3D11_DEPTH_STENCIL_VIEW_DESC descDepthViewD3D11;/**<Descriptor de depthstencil de DirectX para casteo*/
#endif // D3D11
		/**
		*@brief Inicializador de el depth stencil con los datos del descriptoe
		*@param1 @c DepthStencilViewDesc @c _stencilDesc Objeto de descriptor
		*@return No devuelve nada
		*/
		void
		Init(DepthStencilViewDesc _stencilDesc);



};