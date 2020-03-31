#pragma once
#include <string>
#include "Defines.h"
/**
 * @file MaterialApi.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de materiales para modelos.
 *
 *En este encabezado estableceremos los metodos y variables para el uso de texturas en modelos.
 *
 *
 */


 /**
  * @brief Clase MaterialApi.
  *
  * Crea materiales
  */
class MaterialApi
{
	public:
		/**
		*@brief Le da realse al Defuse
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void 
		Shutdown();

		//Variables
		std::string					m_NormalName;/**<Nombre de las normales*/
		bool						m_HasNormals;/**<Si tiene normales*/
		bool						m_HasDifuse;/**<Si tiene defues*/
		std::string					m_Diroftextures;/**<Donde se encuentran las texturas*/
		std::string					m_DifuseName;/**<Nombre del defuse*/

		GLuint m_TextureId;

#ifdef D3D11
		ID3D11ShaderResourceView* m_TexDif = NULL;/**<Textura del defuse*/
		ID3D11ShaderResourceView* m_TexNorm = NULL;/**<Textura de las normales*/
#endif // D3D11

		
};