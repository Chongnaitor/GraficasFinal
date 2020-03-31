#pragma once
#include "CBuffer.h"
/**
 * @file CVertexBuffer.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del DepthStencil.
 *
 *En este encabezado estableceremos los metodos y variables para el uso del vertexbuffer.
 *
 *
 */

 /**
  * @brief Clase VertexBuffer
  *
  * Descripcion sirve para crear buffers que contngan vertices
  */
class CVertexBuffer {

	public:
     

		CBuffer m_Buffer;/**<Variable de tipo buffer*/
        /**
        *@brief Inicializa el vertex buffer con el init del buffer declarado y un subresource data
        *@param1 @c SUBRESOURCE_DATA @c _data
        *@param2 @c BufferDescriptor @c _buffer
        *@return No devuelve nada 
        */
        void
		Init(SUBRESOURCE_DATA _data, BufferDescriptor _buffer);
       

#ifdef D3D11
		D3D11_SUBRESOURCE_DATA m_SubDataD3D11;/**<Variable de subresource data de D3D11*/
#endif // D3D11
};