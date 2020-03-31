#pragma once
#include <vector>
#include "Mesh.h"
#include "CDeviceContext.h"
#include "CBuffer.h"
/**
 * @file CSceneManager.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del Escenas.
 *
 *En este encabezado estableceremos los metodos y variables para el uso y manejo de escenas de assimp.
 *
 *
 */

 /**
  * @brief Clase de SceneManager la cual administa todos nuestros modelos en escena.
  *
  * Descripcion Se encarga de la lectura de modelos de la escena
  */

class CSceneManager
{
	public:
		 /**
			*@brief Constructor(No esta en uso actualmente)
			* @param No tiene parametros
			* @return No devuelve nada
			*/
		CSceneManager();
		/**
	   *@brief Destructor (Actualmente no esta en uso)
	   *@param No tiene parametros
	   *@return No devuelve nada
	   */
		~CSceneManager();
		/**
		*@brief Añade meshes 
		*@param1 @c MESH* @c newMesh
		*@return No devuelve nada
		*/
		void 
		AddMesh(MESH* newMesh);
		/**
		*@brief Obtiene el mesh
		*@param1 @c int @c meshID
		*@return @c MESH*
		*/
		MESH*& 
		GetMesh(int meshID);
		/**
		*@brief Actualiza datos que se actualizan a su vez de forma recursiva
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void 
		Update();
		/**
	*@brief Renderiza la escena
	*@param1 @c CDeviceContext* @c deviceCont Device context actual
	*@param2 @c CBuffer* @c _buff Buffer donde se almaceneran los datos
	*@param3 @c CDevice* @c _dev Device actual
	*@return No devuelve nada
	*/
		void 
		Render(CDeviceContext *_deviceCon, CBuffer* _buff, CDevice* _dev);
		/**
		*@brief Se encarga de vaciar todo lo que se uso para liberacion de memoria
		*@param No tiene parametros
		*@return No devuelve nada 
		*/
		void 
		Shutdown();
		/**
		*@brief Inicializa todos los datos necesarios
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
		Init();
		CBuffer* meshData;/**<Buffer en el que se guarden los datos del mesh*/
		GLuint m_VertexArrayID;/**<Arreglo de vertices de OpenGL*/
		bool m_initialize = false;/**<Determina si se va a usar o no*/
		std::vector<MESH*> m_MeshInScene;/**<Arreglo de meshes usado en escena*/

	private:
		
	
		int m_ID = 0;/**<ID de meshes*/
};