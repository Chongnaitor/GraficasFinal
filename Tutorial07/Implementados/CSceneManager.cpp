#include "../Encabezados/CSceneManager.h"

CSceneManager::CSceneManager(){}

CSceneManager::~CSceneManager(){}

///
/// function to be able to add the data of the mesh member
///
void CSceneManager::AddMesh(MESH* newMesh){

	newMesh->SetSceneID(m_ID);
	m_ID++;
	m_MeshInScene.push_back(newMesh);
}

///
/// function to get the mesh
///
MESH*& CSceneManager::GetMesh(int meshID){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		if (m_MeshInScene[i]->GetSceneID() == meshID)
			return m_MeshInScene[i];
	}
}

///
/// function to use the mesh on stage
///
void CSceneManager::Update(){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		m_MeshInScene[i]->Update();
	}
}

///
/// function to render the mesh
///
void CSceneManager::Render(CDeviceContext *_deviceCon, CBuffer* _buff, CDevice* _dev){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		m_MeshInScene[i]->Render(_deviceCon, _buff, _dev);
	}
}

///
/// function to remove data from a mesh on stage
///
void CSceneManager::Shutdown(){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		m_MeshInScene[i]->Delete();
	}
}

///
/// Function to initialize the vertex generator and bind the vertices
///
void CSceneManager::Init(){

	if (m_initialize) {

		return;
	}
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);
	
	m_initialize = true;
}
