#pragma once
#include "CDeviceContext.h"
#include "CBuffer.h"
#include <vector>
#include "Defines.h"
#include <string>
#include "MaterialApi.h"
/**
 * @file Mesh.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos para cargar Meshes.
 *
 *En este encabezado estableceremos los metodos y variables para el uso de Meshes.
 *
 *
 */


 /**
  * @brief Clase mesh.
  *
  * En esta clase se usaran los recursos necesario para encadenar meshes en una escena
  */

class MESH
{
	public:
		void
			Update();
		/**
		*@brief Constructor, establece todo las variables con puntero
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		MESH();
		/**
		*@brief Inicializa el mesh con sus respectivos buffers
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
		Init();

		/**
		*@brief Funcion que renderiza los meshes
		*@param1 @c CDeviceContext* @c _devCont
		*@param2 @c CBuffer* @c _bufferData
		*@param3 @c CDevice* @c _dev
		*@return No devuelve nada
		*/
		void
		Render(CDeviceContext *_devCont, CBuffer* _bufferData, CDevice* _dev);
		/**
		*@brief Borra todos los recursos con puntero y los vuelve nulos
		*@param No tiene parametras
		*@return No devuelve nada
		*/
	
		void
		Delete();
		/**
		*@brief Añade meshes a el vector de meshes @c m_Children
		*@param1 @c MESH* @c _newChild
		*@return No devuelve nada
		*/
		void
		AddChildren(MESH* _newChild);
		/**
		*@brief Establece la variable @c m_Parent como el padre
		*@param1 @c MESH* @c _parent
		*@return No devuelve nada
		*/
		void
		SetParent(MESH* _parent);
		/**
		*@brief Set los vertices del modelo
		*@param1 @c SimpleVertex* @c MyVertex
		*@param2 @c int @c numVertex
		*@return No devuelve nada
		*/
		void 
		SetVertex(SimpleVertex* MyVertex, int numVertex);
		/**
		*@brief Establece la lista de indices para la geometria
		*@param1 @c WORD* @c _index
		*@param2 @c int @c numVertex
		*@return No devuelve nada
		*/
		void 
		SetIndexList(WORD* _index, int numIndex);
		/**
		*@brief Obtiene la lista de Indices
		*@param1 @c SimpleVertex* @c Myvertex
		*@param2 @c int @c numVertex
		*@return @c WORD*
		*/
		WORD*&
		GetIndexList(SimpleVertex* MyVertex, int numVertex) { return m_MyIndex; };
		/**
		*@brief Getter de vertices
		*@param No tiene parametros
		*@return @c SimpleVertex
		*/
		SimpleVertex* 
		GetVertex() { return m_MyVertex; };
		/**
		*@brief Getter de numero de vertices
		*@param No tiene parametros
		*@return @c int
		*/
		int
		GetVertexNum() { return m_VertexNum; };
		/**
		*@brief Getter de numero de indices 
		*@param No tiene parametro
		*@return @c int
		*/
		int 
		GetIndexNum() { return m_IndexNum; };
		/**
		*@brief Getter de vertex buffer
		*@param No tiene parametros
		*@return @c CBuffer*
		*/
		CBuffer*&
		GetVertexBuffer() { return m_VertexBuffer; };
		
#ifdef D3D11
		/**
		*@brief Setter de vetex buffer
		*@param @c ID3D11Buffer* @c _pVertexB
		*@return No devuelve nada
		*/
		void
		SetVertexBuffer(ID3D11Buffer* _pVertexB) { m_VertexBuffer->m_BufferD3D11 = _pVertexB; };
		/**
		*@brief Setter de index buffer
		*@param @c ID3D11Buffer* @c _pIndexB)
		*@return No devuelve nada
		*/
		void
		SetIndexBuffer(ID3D11Buffer* _pIndexB) { m_Index->m_BufferD3D11 = _pIndexB; };
#endif // D3D11
		/**
		*@brief Getter de indec buffer
		*@param No tiene paramentros
		*@return @c CBuffer*
		*/
		CBuffer*&
		GetIndexBuffer() { return m_Index; };
		/**
		*@brief Asigna un Id a la escena
		*@param @c int @c ID
		*@return No devuelve nada
		*/
		void 
		SetSceneID(int ID) { m_SceneId = ID; };
		/**
		*@brief Getter de Id de escena
		*@param No tiene Parametro
		*@return @c int
		*/
		int 
		GetSceneID() { return m_SceneId; };

		std::string		m_DifuceName;/**<Nombre del difuse*/
		MaterialApi* m_Materials;/**<Material*/


	public:
		
		MESH*				m_Parent;/**<Mesh padre*/
		std::vector<MESH*>	m_Children;/**<Meshes hijos*/
		glm::vec4		m_Transform;/**<*/
		CBuffer*		m_VertexBuffer	= nullptr;/**<Buffer de vertices*/
		CBuffer*		m_Index			= nullptr;/**<Buffer de indices*/
		SimpleVertex*		m_MyVertex		= nullptr;/**<Vertices*/
		WORD*				m_MyIndex		= nullptr;/**<Indices*/
		int					m_VertexNum		= 0;/**<Numero de vertices*/
		int					m_IndexNum		= 0;/**<Numero de indices*/
		int					m_SceneId		= -1;/**<Numero de id de la escena*/
		CBChangesEveryFrame m_MeshData;/**<Datos del mesh*/

};