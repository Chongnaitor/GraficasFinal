#pragma once
#include "Encabezados/Defines.h"
#include "Encabezados/CBuffer.h"
#include"glm/glm/gtc/matrix_transform.hpp"
#include"glm/glm/mat4x4.hpp"
#include"glm/glm/vec3.hpp"
#include"glm/glm/glm.hpp"
#include"glm/glm/gtx/transform.hpp"
/**
 * @file CCamera.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de camaras.
 *
 *En este encabezado estableceremos los miembros y metodos para crear una camara libre.
 *
 *
 */



/**
 * @brief Descriptor para llenar una camara.
 *
 * Usamos descriptores para hacer que el manejo de datos sea mas fluido
 */
struct CCameraDescriptor {

	float dWidht; /**<El ancho de la camara*/
	float dHeight;/**<El largo*/
	float dNear;/**<Cercania que va usar la camara al visualizar*/
	float dFar;/**<Que tan lejr puede ver la camara*/
	float dFoV;/**<Apertura del lente*/

	glm::vec3 dEye;/**<Vector para la camara*/
	glm::vec3 dAt;/**<Vector para la camara*/
	glm::vec3 dUp;/**<Vector para la camara*/
}; 


/**
 * @brief Elementos necesarios para crear una camara.
 *
 * Creamos la clase, en base a esta se hara las demas camaras.
 */
class CCamera {

	protected:
		CCameraDescriptor mDesc;/**<Miembro de tipo descriptor para que se va a usar para retencion de datos*/

		
		glm::vec3 mUp;/**<Vector neceario para el funcionamiento de la camara*/
		glm::vec3 mFront;/**<Vector neceario para el funcionamiento de la camara*/
		glm::vec3 mRight;/**<Vector neceario para el funcionamiento de la camara*/
		
		glm::vec3 originalEye;/**<Vector neceario para el funcionamiento de la camara, haciendo referencia a una posicion anterior*/
		glm::vec3 OriginalUp;/**<Vector neceario para el funcionamiento de la camara, haciendo referencia a una posicion anterior*/
		glm::vec3 OriginalAt;/**<Vector neceario para el funcionamiento de la camara, haciendo referencia a una posicion anterior*/
		glm::vec3 mUpLimit;/**<Vector neceario para el funcionamiento de la camara, limintando que tan arriba se puede rotar*/

	
		glm::mat4x4 mProjection;/**<Matriz de proyeccion*/
		glm::mat4x4 mAxis;/**<Matriz de vista*/
		glm::mat4x4 mPosition;/**<Matriz de posicion basada en la posicion del mundo*/

		
#ifdef D3D11
		XMMATRIX mMProjection;
#endif // D3D11

		float angule;/**<Angulo normal de rotacion de la camara*/
		float maxAngule = 60;/**<Angulo maximo al que puede rotar la camara*/

	public:

		glm::vec2 OriginalMousePos;/**<Rastrea la posicion anterior del mouse*/

		//!
		//!Se penso en la inclusion de los buffer en la camara para el uso mas casteos void
		//!
		CBuffer		g_pCBNeverChangesCamera;/**<Buffer neceasario para funcionamento de la camara*/
		CBuffer		g_pCBChangeOnResizeCamera;/**<Buffer necesario para funcionamiento de la camara*/
		CBuffer		g_pCBChangesEveryFrameCamera;/**<Buffer necesario para funcionamiento de la camara*/

		bool mClickPressed = false;/**<Comprueba si el boton del cursor esta siendo presionado*/

		glm::mat4x4 mView;/**<Matriz de vista*/

/**
*@brief Usa el descriptor que se pasa en el parametro para llenar el descriptor dentro de la clase.
*@param1 @c __cameraDesc Usa un descriptor de tipo @c CCameraDescriptor 
*@return @c int es retornado para saber en que condicion se inicializo la camara.
*/
int Init(CCameraDescriptor _cameraDesc);

/**
*@brief Getter de Up de la camara
*@param No tiene parametros
*@return @c glm::vec3 
*/
glm::vec3 GetMUp();
/**
*@brief Getter de Front de la camara
*@param No tiene parametros
*@return @c glm::vec3
*/		
glm::vec3 GetMFront();
/**
*@brief Getter de Right de la camara
*@param No tiene parametros
*@return @c glm::vec3
*/
glm::vec3 GetMRight();	
/**
*@brief Getter de la matriz de proyeccion
*@param No tiene parametros
*@return @c glm::mat4x4
*/
glm::mat4x4 GetProjection() { return mProjection; };
/**
*@brief Setter de la matriz de vista
*@param1 @c glm::mat4x4 @c _view 
*@return No regresa nada
*/
void SetView(glm::mat4x4 _view);
/**
*@brief Getter de la matriz de vista
*@param No tiene paramtros
*@return Regresa una matriz de vista @c glm::mat4x4 @c mView
*/
glm::mat4x4 GetView(){ return mView; };
/**
*@brief Setter del width de la camera
*@param1 @c float @c _Width
*@return Regresa nada
*/
void SetWidht(float _widht);
/**
*@brief Getter del width de la camrara
*@param No tiene paramtros
*@return Regresa el width de la camara @c float @c mWidth
*/
float GetWidht();
/**
*@brief Setter del height de la camera
*@param1 @c float @c _height
*@return Regresa nada
*/
void SetHeight(float _height);
/**
*@brief Getter del height de la camera
*@param No tiene parametros
*@return Regresa @c float @c _heihgt
*/

		float GetHeight();
	
/**
*@brief Setter del near de la camera
*@param1 @c float @c _near
*@return Regresa @c float @c _near
*/

		void SetNear(float _near);
/**
*@brief Getter del near de la camera
*@param No tiene parametros
*@return Regresa @c float @c _near
*/

float GetNear();
/**
*@brief Setter del far de la camera
*@param1 @c float @c _far
*@return No regresa nada
*/
	
void SetFar(float _far);
/**
 *@brief Getter de Far de la camara
 *@param No tiene parametros
 *@return @c float @c far
 */
		float GetFar();
		/**
*@brief Setter del fov de la camera
*@param1 @c float @c _fOv_
* @return No regresa nada
*/
	
		void SetFoV(float _fOv);
		/**
 *@brief Getter de fov de la camara
 *@param No tiene parametros
 * @return @c glm::vec3 @c fov
 */
		float GetFoV();
		/**
*@brief Setter del eye de la camera
*@param1 @c glm::vec3 @c _vec3
* @return No regresa nada
*/
		void SetEye(glm::vec3 _vec3);
		/**
 *@brief Getter de Eye de la camara
 *@param No tiene parametros
 *@return @c glm::vec3 @c eye
 */
		glm::vec3 GetEye();

		/**
	*@brief Setter del LookAt de la camera
	*@param No tiene parametros
	*@return Regresa nada
	*/
		void SetAt(glm::vec3 _vec3);
		/**
 *@brief Getter de Right de la camara
 *@param No tiene parametros
 *@return @c glm::vec3 @c LookAt
 */
		glm::vec3 GetAt();
		/**
*@brief Setter del Up de la camera
*@param1 @c glm::vec3 @c _vec3
* @return No regresa nada
*/

		void SetUp(glm::vec3 _vec3);
/**
*@brief Getter de Up de la camara
*@param No tiene parametros
* @return @c glm::vec3 @c Up
*/
glm::vec3 GetUp();
/**
*@brief Actualiza la matriz de proyeccion y es virtual para que otras camaras le hagan override
*@param No tiene para metros
*@return No returnea nada
*/
		
virtual void UpdateVM();
/**
*@brief Genera la matriz de proyeccion
*@param No tiene parametros
*@return No regresa nada
*/

void GenerateProjectionMatrix();
		
/**
 *@brief Crea matriz de vista vista y es virtual para que otras camaras hagan override
 *@param No tiene parametros
 *@return No regresa nada
 */

virtual void CreateView();

/**
 *@brief Sirve para mover la camara con inputs
 *@param1 @c WPARAM @c _param parametro de tipo windows 
 * @return No regresa a nada
 */

void  Move(WPARAM _param);
/**
 *@brief Sirve para rotar la camara con inputs
 *@param1 @c WPARAM @c _param parametro de tipo windows
 * @return No regresa  nada
 */

void PitchX(WPARAM _param);
		
/**
*@brief Sirve para rotar la camara con inputs
*@param1  @c WPARAM @c _param parametro de tipo windows
* @return No regresa  nada
*/

		void YawZ(WPARAM _param);
		
/**
*@brief Sirve para rotar la camara con inputs
*@param1 @c WPARAM @c _param parametro de tipo windows
* @return No regresa  nada
*/
void RollY(WPARAM _param);
/**
*@brief Sirve para rotar la camara con el mouse
*@param1 @c WPARAM @c _param parametro de tipo windows
* @return No regresa  nada
*/
virtual void MouseRotation();
/**
*@brief Setter de la posicion anterior del mouse en click
*@param1 @c float @c _x
*@param2 @c float @c _y
* @return No regresa nada
*/
void SetOriginalMousePos(float _x, float _y) { OriginalMousePos = { _x, _y }; };
/**
*@brief Llama a todas las funciones de inputs
*@param1 @c WPARAM @c _param parametro de tipo windows
* @return No regresa  nada
*/

void inputs(WPARAM _param);
/**
*@brief Llama a todas las funciones de inputs para OpenGL
*@param1 @c int @c _param parametro de tipo windows
* @return No regresa nada
*/


void Input(int _param);
/**
 *@brief Sirve para mover la camara con inputs OpenGL
 *@param1 @c int @c _param parametro de tipo windows
 *@return No regresa  nada
 */
void Move(int _param);
/**
*@brief Sirve para rotar la camara con inputs OpenGL
*@param1 @c int @c _param parametro de tipo windows
*@return No regresa  nada
*/
void PitchX(int _param);
/**
*@brief Sirve para rotar la camara con inputs OpenGL
*@param1 @c int @c _param parametro de tipo windows
*@return No regresa  nada
*/
void YawZ(int _param);
/**
*@brief Sirve para rotar la camara con inputs OpenGL
*@param1 @c int @c _param parametro de tipo windows
* @return No regresa  nada
*/
void RollY(int _param);
/**
*@brief Sirve para rotar la camara con inputs OpenGL
*@param1 @c int @c _param parametro de tipo windows
*@return No regresa  nada
*/

/**
*@brief Constructor, estblece todo en 0
*@param No tiene parametros
*@return No regresa nada
*/
CCamera();
~CCamera();
};