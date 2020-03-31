#pragma once
#include "Defines.h"
#include "CDevice.h"
#include "../assimp/scene.h"
/**
 * @file CBuffer.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de buffers necesarios para el funcionamiento correcto del programa.
 *
 *En este encabezado estableceremos los metodos y variables para el uso de buffers.
 *
 *
 */

 /**
  * @brief Descriptor de la clase buffer.
  *
  * Descripcion: Creando estructuras se maneja mejor los datos
  */

struct BufferDescriptor {

	UINT ByteWidth;/**<El tamaño del buffer in bytes*/
	USAGE Usage;/**<Identifica como el buffer sera leido*/
	UINT BindFlags;/**<Identifica como el buffer se va a involucrar dentro del pipeline*/
	UINT CPUAccessFlags;/**<Especifica los tipos del CPU que pueden ser permitidos por un recurso*/
	UINT MiscFlags;/**<Identifica opciones para recursos*/
	UINT StructureByteStride;/**<El tamaño de cada elemento en la estructura del buffer en bytes*/
};

/**
 * @brief Clase Buffer
 *
 * Descripcion: Sirve para el almacenamiento de difertentes tipos de datos
 */
class CBuffer {

	public:
		
		BufferDescriptor m_DescBuff;/**<Es un objeto con la estrucutra para facilitar su manipulacion*/
	/**
	*@brief Constuctor: setea todo en nulo
	*@param Ninguno
	*@return No devuelve nada
	*/
		CBuffer() 
		{

#ifdef D3D11
			//!
			//!Se establece las variables en @c NULL
			//!
			m_BufferD3D11 = NULL;
#endif // D3D11

		};
	/**
	*@brief Destructor
	*@param Ninguno
	*@return No devuelve nada
	*/
		~CBuffer() {};
		/**
		*@brief Inicializa el buffer en base a su descriptor
		*@param1 @c BufferDescriptor @c _buff
		*@return No devuelve nada
		*/
		
		void
		Init(BufferDescriptor _buff);

		/**
		*@brief Actualiza los buffers
		*@param Ninguno 
		*@return No devuelve nada
		*/
		void
		Update();
		/**
		*@brief Borra los datos de los Buffers
		*@param Ninguno
		*@return No devuelve nada
		*/
		void
		Delete();
		/**
		*@brief No se utiliza
		*@param Ninguno
		*@return No devuelve nada
		*/
		void
		Render();

#ifdef D3D11
		ID3D11Buffer* m_BufferD3D11;/**<Buffer de DirectX11*/
		D3D11_BUFFER_DESC m_BufferDescD3D11;/**<Descripcion de del Buffer respectivo*/
			/**
			*@brief Crea el buffer de vertices
			*@param1 @c int @c _numvertex numero de vertices
			*@param2 @c const @c aiScene* @c _mode Escena de assimp
			*@param3 @c SimpleVertex* @c _vertex Vertices de la escena
			*@param4 @c ID3D11Buffer*& @c _buffer El buffer a usar en la carga de DirectX
			*@param5 @c CDevice* @c __dev Device usado para gestion de recursos
			*@return No devuelve nada
			*/
		static void 
		createVertexBuffer(int _numvertex, const aiScene* _model, SimpleVertex* _vertex, ID3D11Buffer*& _buffer, CDevice* _dev);
		/**
		*@brief Crea el buffer de indices
		*@param1 @c int @c numindices Numero de indices
		*@param2 @c const @c aiScene* @c _model El modelo del cual se van a extraer los vertices
		*@param4 @c ID3D11Buffer*& @c _buffer El buffer a usar en la carga de DirectX
		*@param5 @c CDevice* @c __dev Device usado para gestion de recursos
		*@return No devuelve nada
		*/
		static void
		createIndexBuffer(int numindices, const aiScene* _model, WORD* _index, ID3D11Buffer*& _buffer, CDevice* _dev);
#endif

		GLuint m_ID;/**<El index buffer de OpenGL*/
		GLuint m_vertexbuffer;/**<Vertex buffer de OpenGL*/
		GLuint m_elementbuffer;/**<Buffer de elemenos de OpenGL*/

			/**
			*@brief Es una funcion que contiene uno o mas objetos de vertex vuffer
			*@param Ninguno
			*@return No devuelve nada
			*/
		void 
		OpenGLVAO();

		/**
		*@brief Crea el buffer de vertices de OpenGL
		*@param1 @c int @c _numvertex Es el numero de vertices
		*@param2 @c GLuint& @c _vertex Son vertices
		*@param3 @c GLuint& @c _buffer Buffer que va a almacenar de tipo OpenGL
		*@return No devuelve nada 
		*/
		static void
		createVertexBuffer(int _numvertex, SimpleVertex* _vertex, GLuint& _buffer);
		/**
		*@brief Crea el buffer de indices de OpenGL
		*@param1 @c int @c numindices Es el numero de indices
		*@param2 @c WORD* @c _index es el Index
		*@param3 @c GLuint& @c _buffer buffer que va a almacenar la informacion de OpenGL
		*@return
		*/
		static void
		createIndexBuffer(int numindices, WORD* _index, GLuint& _buffer);
};