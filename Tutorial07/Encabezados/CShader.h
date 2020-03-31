#pragma once
#include "Defines.h"
/**
 * @file CShader.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos de los shaders.
 *
 *En este encabezado estableceremos los metodos y variables para creacion de shaders. incluyendo el blob e input layout.
 *
 *
 */



/**
 * @brief Clase de Shader 
 *
 * Descripcion con esta clase manejaremos los shader y tambien crearemos un inputlayout en base a eso
 */
class CShader {

	public:
		/**
		*@brief Constructor setea el blob a nulo
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		CShader() {
#ifdef D3D11
			m_pVSBlobD3D11 = NULL;
#endif // D3D11
			 };
		/**
	   *@brief Destructor (Actualmente no esta en uso)
	   *@param No tiene parametros
	   *@return No devuelve nada
	   */
		~CShader() {};

	
		/**
		*@brief Inicializa y lee shaders
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
		Init();
	
		


#ifdef D3D11
		ID3D11VertexShader* g_pVertexShaderD3D11;/**<Variable de tipo vertex shader de DirectX */
		ID3DBlob* m_pVSBlobD3D11;/**<Variable de tipo Blob de DirectX*/
		ID3D11InputLayout* LayoutD3D11;/**<Variable de tipo input layout de DirectX*/
#endif 
		/**
		*@brief Carga de shaders para OpenGL
		*@param1 @c const @c char* @c vertex_file_path
		*@param2  @c const @c char* @c fragment_file_path
		*@return @c GLuint
		*/
		static GLuint 
		LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};