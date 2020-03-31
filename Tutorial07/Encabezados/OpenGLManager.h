#pragma once
#include "CRenderTarget.h"
#include "CRenderTargetView.h"
#include "GraphicApi.h"
#include <Windows.h>
#include "Encabezados/CBuffer.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "FirstCamera.h"
/**
 * @file OpenGLManager.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos de OpenGL Pog.
 *
 *En este encabezado estableceremos los metodos y variables para la API de OpenGL.
 *
 *
 */


 /**
  * @brief Clase OpenGL.
  *
  * Uso del Api y administracion de recursos
  */

class OpenGLManager {

	public:
	
		CSceneManager			m_CSceneManager;/**<Scenemanager para opengl*/
		GraphicApi				m_GraphicApi;/**<Graphics api para opengl*/
		CRenderTarget			m_RenderTarget;/**<Render target en el cual se pinta*/
		CRenderTargetView	m_RenderTargetView;/**<Profundidad*/
		CBuffer				m_Buffers;/**<Bufferes */
		CCamera                  m_FreeCamera;/**<Camra libre*/
		FirstCamera             m_FPSCamera;/**< Camara en primera persona*/

	
		GLuint		m_frameBufferName;/**<Nombre del buffer*/
		GLuint		m_programShaderID;/**<Shader id con la que se va iniciar el programa*/
		GLFWwindow* m_window;/**<Ventana*/
		GLuint		m_TextureBillboard;/**<Textura del billboard*/
		GLuint		m_vertexBufferBillBoard;/**< vertex del billboard*/
		GLuint		m_IndexBufferBillBoard;/**<Buffer de indices del billboard*/
		GLuint		renderedTexture;/**<Textura que renderizara*/
	
		/**
		*@brief Se inicia el dispositivo
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void 
		InitDevice();
		/**
		*@brief Crea el fram buffer
		*@param No tiene parametros
		*@return @c HRESULT
		*/
		HRESULT 
		FrameBuffer();
		/**
		*@brief Toma y recibe inputs
		*@param1 @c GLFWwindow* @c window
		*@param2 @c int @c key 
		*@param3 @c int @c scancode
		*@param4 @c int @c action
		*@param5 @c int @c mods
		*@return No devuelve nada
		*/
		void 
		processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
		/**
		*@brief Usa GLFW para crear ventana 
		*@param No tiene parametros
		*@return @c int
		*/
		int 
		WindowGLFW();
		/**
		*@brief Loopea el programa para actualizar frames
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void 
		GameLoop();
		/**
		*@brief Inicializacion de camaras
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void 
		InitCameras();
		/**
		*@brief Crea el billboard
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
			BillBoard();
		/**
		*@brief Actualiza el billboard 
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
			UpdateBillBoard();
		/**
		*@brief Render para la primera camara
		*@param No tiene parametros
		*@return No devuelve nada 
		*/
		void
			render1();
		/**
		*@brief Render para la segunda camara
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
			render2();
};