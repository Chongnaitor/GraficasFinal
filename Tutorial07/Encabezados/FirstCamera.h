#pragma once
#include"Encabezados/Defines.h"
#include "CCamera.h"
/**
 * @file FirstCamera.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de camara en primera persona.
 *
 *En este encabezado reescriberemos solo algunos metodos que nos permetiran el uso de una camara en primera persona.
 *
 *
 */




 /**
  * @brief Clase FirstCamera para hacer una camara en primera persona en base  la clase CCamera.
  *
  * Descripcion
  */
class FirstCamera :public CCamera{

	public:
	
		/**
		*@brief Sobrescribimos la rotacion del mouse para que este limitado
		*@param No tiene ningun parametro
		*@return No devuelve ninguno valor
		*/
		void
		MouseRotation() override;
		/**
		*@brief Sobrescribimos la funcion de actualizacion de matriz de vista 
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
		UpdateVM()override;
		/**
		*@brief Sobrescribimos como se actualiza la matriz de vista
		*@param No tiene parametros
		*@return No devuelve nada
		*/
		void
		CreateView()override;
		/**
		*@brief Sobreescribimos el movimiento de la camara
		*@param No recibe parametros
		*@return No devuelve nada
		*/
		void Movement(WPARAM wParam);
		/**
		*@brief Obtenemos la posicion de la camara
		*@param No recibe parametros
		*@return No devuelve nada
		*/
		glm::vec3 getPos();
private:
	glm::vec3 onlymRight;/**<Vector que sirve para solo ir derecha*/
	glm::vec3 onlymFRont;/**<Vector que sirve para solo ir hacia el frente*/
	glm::vec3 onlymUp;/**<Vector que sirve para solo ir hacia arriba*/
};