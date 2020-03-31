#pragma once
#include "Defines.h"
/**
 * @file CSwapChain.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del CSwapChain.
 *
 *En este encabezado estableceremos los metodos y variables para el uso y creacion del Swapchain, el encargado de manejo de front y back buffer.
 *
 *
 */


 /**
  * @brief Descriptor de SwapChain
  *
  * Descripcion Uso de una estructura de desripcion para mejor manejo de datos
  */

struct SwapChainDescriptor {

	SWAP_CHAIN_DESC		sd;/*<La complejidad requiere una estructura de descripcion de swapchain por razones de casteos**/

	MODE_DESC			BufferDesc;/**<Estructura que describe el modo de display del backbuffer */
	SAMPLE_DESC			SampleDesc;/**<Estructura que describe parametros multisampling*/
	UINT				BufferUsage;/**<Enumerador que describe el uso superficial y tambien las opciones de acceso del CPU al backbuffer*/
	UINT				BufferCount;/**<Describe el numero de buffers en el swapchain*/
	HWND				OutputWindow;/**<Maneja el output de la ventana y no debe ser @c NULL*/
	BOOL				Windowed;/**<Especifica como de ve la ventana del output*/
	SWAP_EFFECT			SwapEffect;/**<Describe las diferentes opciones para manejar los contenidos del buffer de presentacion*/
	UINT				Flags;/**<Describe el comportamiento del SwaoChain*/

};

/**
 * @brief Clase SwapChain
 *
 * Descripcion se encarga del uso de multiples buffer para mostrar y escribir entre frames
 */
class CSwapChain {

	public:
	
	
		SwapChainDescriptor m_ChainDesc;/**<Descriptor basado en la estructura anterior*/
		/**
		*@brief Llena el descriptor local de la clase con el que se le pase
		*@param @c SwapChainDescriptor _desc
		*@return No devuelve nada 
		*/
		void
		Init(SwapChainDescriptor _desc);
		/**
		*@brief Manda el swapchain si es diferente a null, se usa mas que nada para casteos a punteros void
		*@param @c SwaphainDescriptor @c _desc
		*@return No deuelve nada
		*/
		void*
		GetSwap();

	

#ifdef D3D11
		IDXGISwapChain* g_pSwapChainD3D11;/**<Variable de tipo Swapchain de DirectX*/
		DXGI_SWAP_CHAIN_DESC sdD3D11;/**<Variable de descripcion de swapchain de DirectX*/
#endif // D3D11
};