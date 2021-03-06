#include "..\Encabezados\GraphicApi.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Encabezados/stb_image.h"

///
/// function for loading the DIRECTX mesh
///
bool GraphicApi::ChargeMesh(const char* _meshPath, CSceneManager* _CSceneManager, const aiScene* _model, CDeviceContext _devCont, Assimp::Importer *_importer, CDevice* _dev){

	///
	/// We import the mesh and save it in a temporary variable
	///
	_model = _importer->ReadFile
											 (_meshPath,
											 aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded |
											 aiProcess_FindInstances |
											 aiProcess_ValidateDataStructure |
											 aiProcess_OptimizeMeshes |
											 aiProcess_Debone |
											 0);

	///
	/// Condition to check if the reading was corrects
	///
	if (_model == nullptr){

		std::string toAdvice = "\nCouldn't find the object in the direcction:\n";
		toAdvice += _meshPath;
		return false;
	}

	///
	/// We create new resources
	///
	MESH* newmesh = new MESH;
	char* token = NULL;
	char* nextToken = NULL;
	std::string path = _meshPath;

	///
	/// We look for the textures in the project folder
	///
	token = strtok_s((char*)path.c_str(), "/", &nextToken);

	std::string teturesFolder = token;
	teturesFolder += "\\";
	token = (char*)teturesFolder.c_str();
	newmesh->m_Materials->m_Diroftextures = token;

	std::string dirName = newmesh->m_Materials->m_Diroftextures;

	///
	/// We send the following functions to be able to assign the data in their respective place
	///
	MeshRead(_model, newmesh, 0, _dev);
	ReadTextureMesh(_model, newmesh, 0, _dev);
	_CSceneManager->AddMesh(newmesh);

	///
	/// Finally we check and start generating the resources of the obtained mesh
	///
	if (_model->mNumMeshes > 1){

		for (unsigned int i = 1; i < _model->mNumMeshes; i += (unsigned int)1){

			MESH* childmesh = new MESH;
			childmesh->SetParent(newmesh);
			newmesh->AddChildren(childmesh);
			childmesh->m_Materials->m_Diroftextures = dirName;
			MeshRead(_model, childmesh, i, _dev);
			ReadTextureMesh(_model, childmesh, i, _dev);
			_CSceneManager->AddMesh(childmesh);
		}
	}
	return true;
}

///
/// function for loading the OPENGL mesh
///
bool GraphicApi::ChargeMesh(const char* _meshPath, const aiScene* _model, CSceneManager* _CSceneManager){

	///
	/// We import the mesh and save it in a temporary variable
	///
	_model = m_Importer->ReadFile
	(
		_meshPath,
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_FindInstances |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_Debone |
		0
	);

	///
	/// Condition to check if the reading was corrects
	///
	if (_model == nullptr) {

		std::string toAdvice = "\nCouldn't find the object in the direcction:\n";
		toAdvice += _meshPath;
		return false;
	}

	///
	/// We create new resources
	///
	MESH* newmesh = new MESH;
	char* token = NULL;
	char* nextToken = NULL;
	std::string path = _meshPath;

	///
	/// We look for the textures in the project folder
	///
	token = strtok_s((char*)path.c_str(), "/", &nextToken);

	std::string teturesFolder = token;
	teturesFolder += "\\";
	token = (char*)teturesFolder.c_str();
	newmesh->m_Materials->m_Diroftextures = token;

	std::string dirName = newmesh->m_Materials->m_Diroftextures;

	///
	/// We send the following functions to be able to assign the data in their respective place
	///
	MeshRead(_model, newmesh, 0);
	ReadTextureMesh(_model, newmesh, 0);
	_CSceneManager->AddMesh(newmesh);

	///
	/// Finally we check and start generating the resources of the obtained mesh
	///
	if (_model->mNumMeshes > 1) {

		for (unsigned int i = 1; i < _model->mNumMeshes; i += (unsigned int)1) {

			MESH* childmesh = new MESH;
			childmesh->SetParent(newmesh);
			newmesh->AddChildren(childmesh);
			childmesh->m_Materials->m_Diroftextures = dirName;
			MeshRead(_model, childmesh, i);
			ReadTextureMesh(_model, childmesh, i);
			_CSceneManager->AddMesh(childmesh);
		}
	}
	return true;
}

///
/// DIRECTX mesh reading function
///
void GraphicApi::MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex, CDevice* _dev){
	
	///
	/// We generate the necessary resources to read a mesh
	///
	std::vector <std::uint32_t> indis;
	indis.reserve(_model->mMeshes[_meshIndex]->mNumFaces * 3);
	int numVertex = _model->mMeshes[_meshIndex]->mNumVertices;
	int numVIndex = (_model->mMeshes[_meshIndex]->mNumFaces * 3);

	///
	/// We store the indices in our vector
	///
	for (std::uint32_t faceIdx = 0u; faceIdx < _model->mMeshes[_meshIndex]->mNumFaces; faceIdx++)
	{
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[0u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[1u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[2u]);
	}

	SimpleVertex* meshVertex = new SimpleVertex[numVertex];
	WORD* meshIndex = new WORD[numVIndex];

	///
	/// We assign the vertices and their positions in their respective place
	///
	for (int i = 0; i < numVertex; i++)
	{
		meshVertex[i].Pos.x = _model->mMeshes[_meshIndex]->mVertices[i].x;
		meshVertex[i].Pos.y = _model->mMeshes[_meshIndex]->mVertices[i].y;
		meshVertex[i].Pos.z = _model->mMeshes[_meshIndex]->mVertices[i].z;
		meshVertex[i].Tex.x = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].x;
		meshVertex[i].Tex.y = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].y;
	}

	_mesh->SetVertex(meshVertex, numVertex);

#ifdef D3D11
	CBuffer::createVertexBuffer(numVertex, _model, meshVertex, _mesh->GetVertexBuffer()->m_BufferD3D11, _dev);
#endif // D3D11

	for (int i = 0; i < numVIndex; i++)
	{
		meshIndex[i] = (WORD)indis[i];
	}
	_mesh->SetIndexList(meshIndex, numVIndex);

#ifdef D3D11
	CBuffer::createIndexBuffer(numVIndex, _model, meshIndex, _mesh->GetIndexBuffer()->m_BufferD3D11, _dev);
#endif // D3D11
}

///
/// function for reading the OPENGL mesh
///
void GraphicApi::MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex){

	///
	/// We generate the necessary resources to read a mesh
	///
	std::vector <std::uint32_t> indis;
	indis.reserve(_model->mMeshes[_meshIndex]->mNumFaces * 3);
	int numVertex = _model->mMeshes[_meshIndex]->mNumVertices;
	int numVIndex = (_model->mMeshes[_meshIndex]->mNumFaces * 3);
	
	///
	/// We store the indices in our vector
	///
	for (std::uint32_t faceIdx = 0u; faceIdx < _model->mMeshes[_meshIndex]->mNumFaces; faceIdx++)
	{
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[0u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[1u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[2u]);
	}

	SimpleVertex* meshVertex = new SimpleVertex[numVertex];
	WORD* meshIndex = new WORD[numVIndex];

	///
	/// We assign the vertices and their positions in their respective place
	///
	for (int i = 0; i < numVertex; i++)
	{
		meshVertex[i].Pos.x = _model->mMeshes[_meshIndex]->mVertices[i].x;
		meshVertex[i].Pos.y = _model->mMeshes[_meshIndex]->mVertices[i].y;
		meshVertex[i].Pos.z = _model->mMeshes[_meshIndex]->mVertices[i].z;
		meshVertex[i].Tex.x = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].x;
		meshVertex[i].Tex.y = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].y;
	}

	_mesh->SetVertex(meshVertex, numVertex);

	CBuffer::createVertexBuffer(numVertex, meshVertex, _mesh->GetVertexBuffer()->m_ID);

	for (int i = 0; i < numVIndex; i++)
	{
		meshIndex[i] = (WORD)indis[i];
	}

	_mesh->SetIndexList(meshIndex, numVIndex);

	CBuffer::createIndexBuffer(numVIndex, meshIndex, _mesh->GetIndexBuffer()->m_ID);
}

///
/// function to read DIRECTX mesh texture
///
void GraphicApi::ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex, CDevice* _dev) {
	
	const aiMaterial* pMaterial = _model->mMaterials[_model->mMeshes[_meshIndex]->mMaterialIndex];

	///
	/// Difuse texture
	///
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		aiString Path;
		_mesh->m_Materials->m_HasDifuse = true;

		///
		/// Within this condition we will look for the extension and format of the textures, in order to assign it to the corresponding mesh
		///
		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){

			_mesh->m_Materials->m_DifuseName = _mesh->m_Materials->m_Diroftextures;
			_mesh->m_Materials->m_DifuseName += (std::string) Path.data;

			wchar_t wtext[50];
			char* token = NULL;
			char* nextToken = NULL;
			token = strtok_s((char*)_mesh->m_Materials->m_DifuseName.c_str(), ".", &nextToken);
			std::string pngextencion = token;
			pngextencion += ".png";
			token = (char*)pngextencion.c_str();

			LPCSTR direcTextur = token;
			LPCWSTR dir = (LPCWSTR)pngextencion.c_str();

			int stringLength = MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), 0, 0);
			std::wstring wstr(stringLength, 0);	
			MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), &wstr[0], stringLength);
			dir = (LPCWSTR)wstr.c_str();

#ifdef D3D11
			D3DX11CreateShaderResourceViewFromFile(_dev->g_pd3dDeviceD3D11, dir, NULL, NULL, &_mesh->m_Materials->m_TexDif, NULL);
#endif // D3D11

		}
	}
}

///
/// function to read OPENGL mesh texture
///
void GraphicApi::ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex){

	const aiMaterial* pMaterial = _model->mMaterials[_model->mMeshes[_meshIndex]->mMaterialIndex];

	///
	/// Difuse texture
	///
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		
		aiString Path;
		_mesh->m_Materials->m_HasDifuse = true;

		///
		/// Within this condition we will look for the extension and format of the textures, in order to assign it to the corresponding mesh
		///
		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){

			_mesh->m_Materials->m_DifuseName = _mesh->m_Materials->m_Diroftextures;
			_mesh->m_Materials->m_DifuseName += (std::string) Path.data;

			wchar_t wtext[50];
			char* token = NULL;
			char* nextToken = NULL;
			token = strtok_s((char*)_mesh->m_Materials->m_DifuseName.c_str(), ".", &nextToken);
			std::string pngextencion = token;
			pngextencion += ".png";
			token = (char*)pngextencion.c_str();

			LPCSTR direcTextur = token;
			LPCWSTR dir = (LPCWSTR)pngextencion.c_str();

			int stringLength = MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), 0, 0);
			std::wstring wstr(stringLength, 0);
			MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), &wstr[0], stringLength);
			dir = (LPCWSTR)wstr.c_str();

			///
			/// We generate the textures and the bind with the functions integrated by GLFWs
			///
			glGenTextures(1, &_mesh->m_Materials->m_TextureId);
			glBindTexture(GL_TEXTURE_2D, _mesh->m_Materials->m_TextureId);

			///
			/// Set the texture wrapping/filtering options (on the currently bound texture object)
			///
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			///
			/// Load and generate the texture
			///
			int width, height, nrChannels;
			unsigned char* data = stbi_load(pngextencion.c_str(), &width, &height, &nrChannels, 0);

			if (data){

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else{

				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}