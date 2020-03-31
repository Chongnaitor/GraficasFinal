#pragma once
#include "Defines.h"
/**
 * @file CSampleSate.h
 * @author Juan Pablo Chong Valdés
 * @date 30 Marzo 2020
 * @brief Creacion de y usos del SampleState.
 *
 *
 *
 */

 /**
  * @brief Descriptor de SampleState.
  *
  *Se usa un descriptor para el mejor uso y manejo de datos.
  */
struct SampleStateDesc {

	SAMPLER_DESC			sampDesc;/*<Miembro de tipo descripcion, por que la complejidad de esta estructura es mayor**/
	FILTER					Filter;/*<Metodo de uso cuando se samplea una textura**/
	TEXTURE_ADDRESS_MODE	AddressU;/*<Metodo usado para resolver una coordinada de textura, va de 0 a 1**/
	TEXTURE_ADDRESS_MODE	AddressV;/*<Metodo usado para resolver una coordinada de textura, va de 0 a 1**/
	TEXTURE_ADDRESS_MODE	AddressW;/*<Metodo usado para resolver una coordinada de textura, va de 0 a 1**/
	COMPARISON_FUNC			ComparisonFunc;/*<Una funcion que compara data sampleada con otra data sampleada**/

	FLOAT MinLOD;/*<Parte menor en el rango del mipmap**/
	FLOAT MaxLOD;/*<Parte mayor en el rango del mipmap**/
};

/**
 * @brief Clase SampleState.
 *
 * Es como se interpreta la informacion que estamos viendo y se esta encapsulando en una clase
 */
class CSampleState{

	public:
		/**
		   *@brief Constructor (Actualmente no esta en uso)
		   *@param No tiene parametros
		   *@return No devuelve nada
		   */
		CSampleState() {};
		/**
		   *@brief Destructor (Actualmente no esta en uso)
		   *@param No tiene parametros
		   *@return No devuelve nada
		   */
		~CSampleState() {};

		SampleStateDesc m_SampleDesc;/*<Descriptor de la estuctura previamente echa**/
		/**
		*@brief Inicializador del sample
		*@param1 @c SampleStateDesc @c _sampleDesc
		*@return No devuelve nada
		*/
		void
		Init(SampleStateDesc _sampleDesc);
		/**
		*@brief
		*@param
		*@return
		*/
		

#ifdef D3D11
		ID3D11SamplerState*			g_pSamplerLinearD3D11;/*<Puntero a sampler de DirectX**/
		D3D11_SAMPLER_DESC			sampDescD3D11;/*<Descriptro de sampler de DirectX (Uso en casteos)**/
#endif // D3D11
};