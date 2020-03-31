#pragma once
#include "Defines.h"
/**
 * @file CRenderTarget.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del RenderTarget.
 *
 *En este encabezado estableceremos los metodos y variables para el uso de RenderTarget y como debe llenarse.
 *
 *
 */

 /**
  * @brief Descriptor de la Render target para el mejor manejo de datos
  *
  * Descripcion utilizamos descriptores para mejor uso de datos
  */
struct RenderTargetDesc {

    UINT Width;/**<Ancho de la textura en texeles*/
    UINT Height;/**<Alto de la textura en texeles*/
    UINT MipLevels;/**<Numero maximo de mipmaps en la textura*/
    UINT ArraySize;/**<Numero de texturas en el arreglo*/
    FORMAT Format;/**<Formato de recursos de data*/
    SAMPLE_DESC SampleDesc;/**<Estructura que especifica parametros multisampling para la textura*/
    USAGE Usage;/**<Identifica si la textura es para leer y escribir*/
    UINT BindFlags;/**<Bandera para ver como se integra a el pipeline*/
    UINT CPUAccessFlags;/**<Determina los tipos de accesos que se permite del CPU*/
    UINT MiscFlags;/**<Identifica opciones menos comunes para los recursos*/
};
/**
 * @brief Descriptor.
 *
 * Descripcion
 */
class CRenderTarget {

	public:
        /**
        *@brief Constructor (No esta en uso actualmente)
        *@param No tiene parametros
        *@return No devuelve nada
        */
        CRenderTarget() {};
        /**
        *@brief Destructor (Actualmente no esta en uso)
        *@param No tiene parametros
        *@return No devuelve nada
        */
        ~CRenderTarget() {};

        RenderTargetDesc m_RenderTDesc/**<Variable de tipo descriptor para mejor manejo de la informacion*/;

        
        GLuint m_IdRenderTarget = 0;/**<Render target de tipo OpenGL*/
        bool m_initialize = false;/**<Variable que determina cual API se esta usando*/

        /**
        *@brief Inicializador del render target con una estructura 
        *@param1 @c RenderTargetDesc @c _rtd
        *@return No devuelve nada
        */
        void
        Init(RenderTargetDesc _rtd);

        
        /**
        *@brief Crea un buffer para almacenar frames OpenGL
        *@param No tiene parametros
        *@return Mo devuelve nada
        */
        void
        InitFrameBuffer();

#ifdef D3D11
        ID3D11Texture2D* g_pDepthStencilD3D11;/**<Variables de tipo textura DirectX*/
        D3D11_TEXTURE2D_DESC m_RenderTD3D11;/**<Variable de tipo de descriptor de textura DirectX*/
#endif // D3D11
};