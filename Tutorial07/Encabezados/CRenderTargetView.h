#pragma once
#include "Defines.h"
/**
 * @file CRenderTargetView.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del RenderTargetView.
 *
 *En este encabezado estableceremos los metodos y variables para el RenderTargetView el cual determina profundidad.
 *
 *
 */

 /**
  * @brief Descriptor del render target view que se utiliza para simplificar el manejo de de datos
  *
  * Descripcion El uso de descriptores es mejor para manejar el uso de datos
  */

struct RenderTargetViewDesc {

	FORMAT Format;/**<Formato de recursos de data*/
	RTV_DIMENSION ViewDimension;/**<El tipo de recurso que especifica como se va a acceder al render target*/
    BUFFER_RTV Buffer;/**<Especifica cual buffer de elementos puede ser accedidido*/
    TEX1D_RTV Texture1D;/**<Especifica el subrecurso en una Textura1D que puede ser accedida*/
    TEX1D_ARRAY_RTV Texture1DArray;/**<Especifica los subrecursos 1D de una arreglo de texturas que pueden ser accedidas*/
    TEX2D_RTV Texture2D;/**<Especifica el subrecurso en una textura 2D que puede ser accedida*/
    TEX2D_ARRAY_RTV Texture2DArray;/**<Especifica los subrecursos 2D de una arreglo de texturas que pueden ser accedidas*/
    TEX2DMS_RTV Texture2DMS;/**<Especifica un solo subrecurso ya que una textura multisampleada 2D solo puede tener un subrecurso*/
    TEX2DMS_ARRAY_RTV Texture2DMSArray;/**<Especifica los subrecursos en un arreglo de  texturas multisampleadas 2D que pueden ser accedidas*/
    TEX3D_RTV Texture3D;/**<Especifica el subrecurso en una textura 3D que puede ser accedida*/
};
/**
 * @brief Clase de RenderTargetView
 *
 * Esta clase se encargara de utiliar arreglo de depth stencil y render target
 */

class CRenderTargetView {

	public:
            /**
            *@brief Constructor(No esta en uso actualmente)
            * @param No tiene parametros
            * @return No devuelve nada
            */
		CRenderTargetView() {};
        /**
        *@brief Destructor (Actualmente no esta en uso)
        *@param No tiene parametros
        *@return No devuelve nada
        */
		~CRenderTargetView() {};

        RenderTargetViewDesc m_renderTVD;/**<Descriptor del render target*/

        //Miembros OpenGL
        GLuint m_IdRenderTargetView = 0;/**<Render targetview de tipo OpenGL*/
         /**
        *@brief Es un inicializador del render taget view
        *@param1 @c RenderTargetViewDesc @c _rtvd
        *@return No devuelve nada
        */
        void
        Init(RenderTargetViewDesc _rtvd);

#ifdef D3D11
        ID3D11RenderTargetView* g_pRenderTargetViewD3D11;/**<Render target view de DirectX*/
        D3D11_RENDER_TARGET_VIEW_DESC m_renderTVD3D11;/**<Descriptor de render target view DirectX*/
#endif // D3D11
};