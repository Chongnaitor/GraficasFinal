#pragma once
#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"
#include "../assimp/matrix4x4.h"
#include "../assimp/cimport.h"
#include "CDeviceContext.h"
#include <string>
#include "Mesh.h"
#include "CSceneManager.h"

 /**
  * @file GraphicApi.h
  * @author Juan Pablo Chong Valdés
  * @date 30 Marzo 2020
  * @brief Creacion de y usos de nuestra API de modelos.
  *
  *En este encabezado estableceremos los metodos y variables para el uso de modelos y sus respectivos atributos
  *
  *
  */

 /**
  * @brief Clase GraphicApi
  *
  * Descripcion Usa otras clases y asimp para la carga de modelos y sus respectivos atribustos ya sea DirectX u OpenGL
  */
class GraphicApi {

	public:
		
		//DIRECTX Methods
				/**
				*@brief Carga el modelo para DirectX
				*@param1 @c const @c char* @c _meshPath 
				*@param2 @c CSceneManager* @c _CSeneManager
				*@param3 @c const @c aiScene* @c _model
				*@param4 @c CDeviceContext @c _devCont
				*@param5 @c Assim::Importer* @c _importer
				*@param6 @c CDevice* @c _dev
				*@return @c bool
				*/
		bool
		ChargeMesh(const char* _meshPath, CSceneManager* _CSceneManager, const aiScene* _model, CDeviceContext _devCont, Assimp::Importer* _importer, CDevice* _dev);

		//OPENGL Methods
				/**
				*@brief Carga de modelo para OpenGL
				*@param1 @c const @c char* @c _meshPath
				*param2 @c const @c aiScene* @c _model
				*param3 @c CSceneManger @c _SceneManager
				*@return bool
				*/
		bool
		ChargeMesh(const char* _meshPath, const aiScene* _model, CSceneManager* _CSceneManager);


		const aiScene* m_Model = new const aiScene();/**<Creamos una escena que contiene modelos*/
		
		Assimp::Importer* m_Importer=new Assimp::Importer();/**<Creamos un importador de modelos*/

	private:

		//DIRECTX Methods
				/**
				*@brief Lee la informacion extraida de los meshes para su interpretacion DirectX
				*@param1 @c const @c aiScene* @c _model
				*@param2 @c MESH* @c _mesh
				*@param3 @c int @c _meshIndex 
				*param4 @c CDevice* @c _dev
				*@return No devuelve nada
				*/
		void
		MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex, CDevice* _dev);
		/**
		*@brief Lee la textura del modelo
		*@param1 @c const @c aiScene* @c _model
		*@param2 @c MESH* @c _mesh
		*@param3 @c int @c _meshIndex 
		*param4 @c CDevice* @c _dev
		*@return No devuelve nada
		*/
		void
		ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex, CDevice* _dev);

		//OPENGL Methods
				/**
				*@brief Carga de modelos opengl
				*@param1 @c const @c aiScene* @c _model
				*@param2 @c MESH* @c _mesh
				*@param3 @c int @c _meshIndex 
				*@return  No devuelve nada
				*/
		void
		MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex);
		/**
		*@brief Lee textura para Opengl
		*@param1 @c const @c aiScene* @c _model
		*@param2 @c MESH* @c _mesh
		*@param3 @c int @c _meshIndex 
		*@return No devuelve nada
		*/
		void
		ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex);
};