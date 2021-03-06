#include <../Encabezados/Mesh.h>
#include<sstream> 
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"


///
/// initializer
///
MESH::MESH(){

	m_VertexBuffer	= new CBuffer;
	m_Index			= new CBuffer;
	m_Materials		= new MaterialApi;
	m_MeshData.mWorld = {
		1,0,0,0.010,
		0,1,0,0.010,
		0,0,1,0.010,
		0,0,0,1
	};
	//m_MeshData.vMeshColor = { 1,0,0,1 };
}



///
/// Creation of new buffers
///
void MESH::Init() {

	m_VertexBuffer	= new CBuffer;
	m_Index			= new CBuffer;
}

void MESH::Update(){
}

///
/// function to start rendering the Meshs
///
void MESH::Render(CDeviceContext *_devCont, CBuffer* _bufferData, CDevice* _dev){

	m_MeshData.mWorld = {
		1,0,0,0.0010,
		0,1,0,0.0010,
		0,0,1,0.0010,
		0,0,0,1
	};
	//m_MeshData.vMeshColor = { 1,0,0,1 };

#ifdef D3D11
	_devCont->g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &m_Materials->m_TexDif);
	_devCont->g_pImmediateContextD3D11->VSSetShaderResources(0, 1, &m_Materials->m_TexDif);
#endif // D3D11

	//_devCont->g_pImmediateContextD3D11->UpdateSubresource	(_bufferData->m_BufferD3D11, 0, NULL, &m_MeshData, 0, 0);
	//
	//_devCont->g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &_bufferData->m_BufferD3D11);
	//_devCont->g_pImmediateContextD3D11->PSSetConstantBuffers(2, 1, &_bufferData->m_BufferD3D11);

	std::string title;
	std::string title2;

	std::stringstream ss;

	ss << "mesh ";
	ss << m_SceneId;
	ss >> title;
	ss >> title2;

	title += " ";
	title += title2;
	
	//ImGui::Begin(title.c_str(), 0);
	//ImGui::ColorEdit4("color", m_MeshData.vMeshColor.Vector);
	//ImGui::End();

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

#ifdef D3D11

	_devCont->g_pImmediateContextD3D11->IASetVertexBuffers
	(	0,

		///
		/// number of buffers we are using
		///
		1,

		///
		/// pointer to the buffers list
		///
		&m_VertexBuffer->m_BufferD3D11,

		///
		/// a uint indicating the size of a single vertex
		///
		&stride,

		///
		/// a uint that indicates the number of the byte in the vertex from which you want to start painting
		///
		&offset
	);

	_devCont->g_pImmediateContextD3D11->IASetIndexBuffer
	(
		m_Index->m_BufferD3D11,
		DXGI_FORMAT_R16_UINT,
		0
	);

	///
	/// Topology type
	///

	///
	/// This second function tells Direct3D what type of primitive is used.
	///

	///
	/// _devCont.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///

	///
	/// Draws the vertex buffer in the back buffer
	///
	_devCont->g_pImmediateContextD3D11->DrawIndexed(m_IndexNum, 0, 0);
	if (m_Children.size() > 0){}
#endif // D3D11
}

///
/// function to be able to delete data
///
void MESH::Delete(){

	m_Materials->Shutdown();

	if (m_VertexBuffer != nullptr){

		m_VertexBuffer->Delete();
		delete m_VertexBuffer;
		m_VertexBuffer = nullptr;
	}
	if (m_Index != nullptr){

		m_Index->Delete();
		delete m_Index;
		m_Index = nullptr;
	}
	if (m_MyVertex != nullptr){

		delete m_MyVertex;
		m_MyVertex = nullptr;
	}
}

///
/// function to set child data
///
void MESH::AddChildren(MESH* _newChild){

	m_Children.push_back(_newChild);
}

///
/// function to set parent data
///
void MESH::SetParent(MESH* _parent){

	m_Parent = _parent;
}

///
/// function to set vertices
///
void MESH::SetVertex(SimpleVertex* MyVertex, int numVertex){

	m_VertexNum = numVertex;
	m_MyVertex	= MyVertex;
}

///
/// function to set the indexes
///
void MESH::SetIndexList(WORD* _index, int numIndex) {

	m_MyIndex	= _index;
	m_IndexNum	= numIndex;
}