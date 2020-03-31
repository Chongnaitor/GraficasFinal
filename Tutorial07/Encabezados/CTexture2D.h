#pragma once
#include "Defines.h"
 /**
  * @file CTexture2D.h
  * @author Juan Pablo Chong Valdés
  * @date 30 Marzo 2020
  * @brief Creacion de y usos de Texturas2D.
  *
  *En este encabezado estableceremos los metodos y variables para el uso y creacion de texturas que en este caso tienen mucha similitud con el render target.
  *
  *
  */

 /**
   * @brief Descriptor de tipo Texture2D
   *
   * Descripcion: Se crea un descriptor para mejor gestion de la informacion 
   */
struct Texture2Desc {

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

class CTexture2D 
{

	public:
        /**
        *@brief Constructor setea en null
        *@param No tiene parametros
        *@return No devuelve nada
        */
		CTexture2D() {
#ifdef D3D11
            m_TextureD3D11 = NULL;
#endif // D3D11
        };
    

        Texture2Desc m_TextDesc;/**<Objeto del tipo de estructura manejada anteriormente*/
        /**
        *@brief Inicializa el objeto local con el descriptor
        *@param1 @c Texture2Desc @c _text
        *@return No devuelve nada
        */
		void
		Init(Texture2Desc _text);
        /**
        *@brief Se usa principalmente para obtener la textura en caso de tener que usar caesteos a vid
        *@param No tiene parametros
        *@return No devuelve nada
        */
        void*
        GetDepth();

#ifdef D3D11
        ID3D11Texture2D* m_TextureD3D11;/**<Variable de tipo textura2D de DirectX*/
        D3D11_TEXTURE2D_DESC m_TextDescD3D11;/**<Variable de descripcion de texture2D de Directx principalmente para casteos*/
#endif // D3D11
};