#include "Encabezados/CCamera.h"

///
/// Camera initializer
///
CCamera::CCamera(){
	
	mDesc.dWidht = 0.0f;
	mDesc.dHeight = 0.0f;
	mDesc.dNear = 0.0f;
	mDesc.dFar = 0.0f;
	mDesc.dFoV = 0.0f;
	
	mFront = glm::vec3(0.0f, 0.0f, 0.0f);
	mUp = glm::vec3(0.0f, 0.0f, 0.0f);
	mRight = glm::vec3(0.0f, 0.0f, 0.0f);
	mDesc.dEye = glm::vec3(0.0f, 0.0f, 0.0f);
	mDesc.dAt = glm::vec3(0.0f, 0.0f, 0.0f);
}

///
/// Camera destructor
///
CCamera::~CCamera(){}

///
/// Camera Init
///
int CCamera::Init(CCameraDescriptor _cameraDesc){

	mDesc = _cameraDesc;

	SetWidht(mDesc.dWidht);
	SetHeight(mDesc.dHeight);
	SetNear(mDesc.dNear);
	SetFar(mDesc.dFar);
	SetFoV(mDesc.dFoV);
	SetEye(mDesc.dEye);
	SetAt(mDesc.dAt);
	SetUp(mDesc.dUp);

	CreateView();
	GenerateProjectionMatrix();
	UpdateVM();

	return 0;
}

///
/// Camera GetMUp
///
glm::vec3 CCamera::GetMUp(){

	return mUp;
}

///
/// Camera GetMFront
///
glm::vec3 CCamera::GetMFront(){

	return mFront;
}

///
/// Camera GetMRight
///
glm::vec3 CCamera::GetMRight(){

	return mRight;
}

void CCamera::SetView(glm::mat4x4 _view)
{
}

///
/// Camera SetWidht, GetWidht 
///
void CCamera::SetWidht(float _widht){

	mDesc.dWidht = _widht;
}
float CCamera::GetWidht(){

	return mDesc.dWidht;
}

///
/// Camera SetHeight, GetHeight
///
void CCamera::SetHeight(float _height){

	mDesc.dHeight = _height;
}
float CCamera::GetHeight(){

	return mDesc.dHeight;
}

///
/// Camera SetNear, GetNear
///
void CCamera::SetNear(float _near){

	mDesc.dNear = _near;
}
float CCamera::GetNear(){

	return mDesc.dNear;
}

///
/// Camera SetFar, GetFar
///
void CCamera::SetFar(float _far){

	mDesc.dFar = _far;
}
float CCamera::GetFar(){

	return mDesc.dFar;
}

///
/// Camera SetFoV, GetFoV
///
void CCamera::SetFoV(float _fOv){

	mDesc.dFoV = _fOv;
}
float CCamera::GetFoV(){

	return mDesc.dFoV;
}

///
/// Camera SetEye, GetEye
///
void CCamera::SetEye(glm::vec3 _vec3){

	mDesc.dEye = _vec3;
}
glm::vec3 CCamera::GetEye(){

	return mDesc.dEye;
}

///
/// Camera SetAt, GetAt
///
void CCamera::SetAt(glm::vec3 _vec3){

	mDesc.dAt = _vec3;
}
glm::vec3 CCamera::GetAt(){

	return mDesc.dAt;
}

///
/// Camera SetUp, GetUp
///
void CCamera::SetUp(glm::vec3 _vec3){

	mDesc.dUp = _vec3;
}
glm::vec3 CCamera::GetUp(){

	return mDesc.dUp;
}

///
/// Camera UpdateVM
///
void CCamera::UpdateVM(){

	//mDesc.s_Eye = {m_Projection.data_[0].w, m_Projection.data_[1].w , m_Projection.data_[2].w };
	mRight = { mView[0].x, mView[0].y, mView[0].z };
	mUp	= { mView[1].x, mView[1].y, mView[1].z };
	mFront = { mView[2].x, mView[2].y, mView[2].z };
}

///
/// Camera GenerateProjectionMatrix
///
void CCamera::GenerateProjectionMatrix(){

#ifdef D3D11
	mMProjection = XMMatrixPerspectiveFovLH(mDesc.dFoV, mDesc.dWidht / mDesc.dHeight, mDesc.dNear, mDesc.dFar);

	mProjection =
	{
		mMProjection._11, mMProjection._12, mMProjection._13, mMProjection._14,
		mMProjection._21, mMProjection._22, mMProjection._23, mMProjection._24,
		mMProjection._31, mMProjection._32, mMProjection._33, mMProjection._34,
		mMProjection._41, mMProjection._42, mMProjection._43, mMProjection._44
	};

	mProjection = glm::transpose(mProjection);
#endif
}

///
/// Camera CreateView
///
void CCamera::CreateView() {

	//Usamos left hand
	mFront = mDesc.dAt - mDesc.dEye;
	mFront=glm::normalize(mFront);

	mRight =glm::cross(mDesc.dUp, mFront);
	mRight=glm::normalize(mRight);

	mUp = glm::cross(mFront, mRight);
	//mUp.Normalize();

	mAxis = 
	{
		mRight.x,	mRight.y,	mRight.z,	0,
		mUp.x,		mUp.y,		mUp.z,		0,
		mFront.x,	mFront.y,	mFront.z,	0,
		0,			0,			0,			1
	};

	mPosition =
	{
		1,	0,	0, -mDesc.dEye.x,
		0,	1,	0, -mDesc.dEye.y,
		0,	0,	1, -mDesc.dEye.z,
		0,	0,	0,	1
	};

	mPosition *= mAxis;
	mView = mPosition; //Ya está la matriz view
}

///
/// Camera Move
///
void CCamera::Move(WPARAM _param){

	if (_param == 'w' || _param == 'W') {

		mDesc.dEye += mFront;
	}
	else if (_param == 's' || _param == 'S') {

		mDesc.dEye -= mFront;
	}
	else if (_param == 'a' || _param == 'A') {

		mDesc.dEye -= mRight;
	}
	else if (_param == 'd' || _param == 'D') {

		mDesc.dEye += mRight;
	}
	else if (_param == 'q' || _param == 'Q') {

		mDesc.dEye += mUp;
	}
	else if (_param == 'e' || _param == 'E') {

		mDesc.dEye -= mUp;
	}

	//-----
	mAxis =
	{
		mRight.x,	mRight.y,	mRight.z,	0,
		mUp.x,		mUp.y,		mUp.z,		0,
		mFront.x,	mFront.y,	mFront.z,	0,
		0,			0,			0,			1
	};

	mPosition =
	{
		1,	0,	0, -mDesc.dEye.x,
		0,	1,	0, -mDesc.dEye.y,
		0,	0,	1, -mDesc.dEye.z,
		0,	0,	0,	1
	};


	mPosition *= mAxis;
	mView = mPosition; //Ya está la matriz view
	UpdateVM();
}

///
/// Camera PitchX
///
void CCamera::PitchX(WPARAM _param){

	glm::mat4x4 rot;
	float speedrot = 0.10f;

	if (_param == VK_UP) {

		rot = 
		{
			1,	0,				0,					0,
			0,	cosf(speedrot),	-sinf(speedrot),	0,
			0,	sinf(speedrot),	cosf(speedrot),		0,
			0,	0,				0,					1
		};

		//rot *= mView;
		//mView = rot;
	}
	else if (_param == VK_DOWN) {

		rot = 
		{
			1,	0,					0,					0,
			0,	cosf(-speedrot),	-sinf(-speedrot),	0,
			0,	sinf(-speedrot),	cosf(-speedrot),	0,
			0,	0,					0,					1
		};
		//rot *= mView;
		//mView = rot;
	}
	mView *= rot;

	UpdateVM();
}  

///
/// Camera YawZ
///
void CCamera::YawZ(WPARAM _param){

	glm::mat4x4 rot;
	float speedrot = 0.10f;

	if (_param == 'z'|| _param == 'Z') {

		rot =
		{
			cosf(speedrot),		0,	sinf(speedrot),	0,
			0,					1,	0,				0,
			-sinf(speedrot),	0,	cosf(speedrot),	0,
			0,					0,	0,				1
		};
		//rot *= mView;
		//mView = rot;
	}
	else if (_param == 'c' || _param == 'C') {

		rot = 
		{
			cosf(-speedrot),		0,	sinf(-speedrot),	0,
			0,						1,	0,					0,
			-sinf(-speedrot),		0,	cosf(-speedrot),	0,
			0,						0,	0,					1
		};
		//rot *= mView;
		//mView = rot;
	}
	mView *= rot;

	UpdateVM();
}

///
/// Camera RollY
///
void CCamera::RollY(WPARAM _param){

	glm::mat4x4 rot;
	float speedrot = 0.10f;

	if (_param == VK_RIGHT) {

		rot =
		{
			cosf(speedrot),	-sinf(speedrot),	0,	0,
			sinf(speedrot),	cosf(speedrot),		0,	0,
			0,				0,					1,	0,
			0,				0,					0,	1
		};
		//rot *= mView;
		//mView = rot;
	}
	else if (_param == VK_LEFT) {

		rot = 
		{
			cosf(-speedrot),	-sinf(-speedrot),		0,		0,
			sinf(-speedrot),	cosf(-speedrot),		0,		0,
			0,				0,							1,		0,
			0,				0,							0,		1
		};
		//rot *= mView;
		//mView = rot;
	}
	mView *= rot;

	UpdateVM();
}

///
/// Camera MouseRotation
///
void CCamera::MouseRotation(){
	glm::vec2 Initial;
	glm::vec2 LastPos;

	glm::mat4 Pitch = { 1,0,0,0,
		0,1,0,0,
	0,0,1,0,
	0,0,0,1 };
	glm::mat4 Yaw = { 1,0,0,0,
		0,1,0,0,
	0,0,1,0,
	0,0,0,1 };
	POINT TempPoint;
	GetCursorPos(&TempPoint);
	//ScreenToClient(wScreen, &TempPoint);
	Initial.x = TempPoint.x;
	Initial.y = TempPoint.y;
	glm::vec2 Origen = { Initial.x,Initial.y };
	float SpeedRot = 0.04;

	if (Initial.x > OriginalMousePos.x)
	{
		Yaw = {
		 cosf(-SpeedRot),0,sinf(-SpeedRot),0,
		 0,1,0,0,
		 -sinf(-SpeedRot),0,cosf(-SpeedRot),0,
		 0,0,0,1
		};

	}
	else if (Initial.x < OriginalMousePos.x)
	{
		Yaw = {
		 cosf(SpeedRot),0,sinf(SpeedRot),0,
		 0,1,0,0,
		 -sinf(SpeedRot),0,cosf(SpeedRot),0,
		 0,0,0,1
		};

	}
	if (Initial.y > OriginalMousePos.y)
	{

		Pitch = {
	   1,0,0,0,
	   0,cosf(-SpeedRot),-sinf(-SpeedRot),0,
	   0,sinf(-SpeedRot),cosf(-SpeedRot),0,
	   0,0,0,1
		};

	}
	else if (Initial.y < OriginalMousePos.y)
	{

		Pitch = {
		1,0,0,0,
		0,cosf(SpeedRot),-sinf(SpeedRot),0,
		0,sinf(SpeedRot),cosf(SpeedRot),0,
		0,0,0,1
		};

	}
	std::cout << "Pos		   :" << Initial.x << "," << Initial.y << std::endl;
	std::cout << "Original pos:" << OriginalMousePos.x << "," << OriginalMousePos.y << std::endl;
	SetCursorPos(OriginalMousePos.x, OriginalMousePos.y);
	mView *= Pitch;
	UpdateVM();
	mView *= Yaw;
	UpdateVM();
	
}

///
/// Camera inputs
///
void CCamera::inputs(WPARAM _param){

	if (_param == VK_UP || _param == VK_DOWN){

		PitchX(_param);
	}
	if (_param == VK_RIGHT || _param == VK_LEFT) {

		RollY(_param);
	}
	if (_param == 'z' || _param == 'Z' || _param == 'c' || _param == 'C') {

		YawZ(_param);
	}
	else{

		Move(_param);
	}
}


///
/// Function overload for OpenGL
///
void CCamera::Input(int _param){

	if (_param == GLFW_KEY_UP || _param == GLFW_KEY_DOWN) {

		PitchX(_param);
	}
	if (_param == GLFW_KEY_RIGHT || _param == GLFW_KEY_LEFT) {

		RollY(_param);
	}
	if (_param == GLFW_KEY_Z || _param == GLFW_KEY_C) {

		YawZ(_param);
	}
	else {

		Move(_param);
	}
}

void CCamera::Move(int _param){

	if (_param == GLFW_KEY_W) {

		mDesc.dEye += mFront;
	}
	else if (_param == GLFW_KEY_S) {

		mDesc.dEye -= mFront;
	}
	else if (_param == GLFW_KEY_A) {

		mDesc.dEye -= mRight;
	}
	else if (_param == GLFW_KEY_D) {

		mDesc.dEye += mRight;
	}
	else if (_param == GLFW_KEY_Q) {

		mDesc.dEye += mUp;
	}
	else if (_param == GLFW_KEY_E) {

		mDesc.dEye -= mUp;
	}

	//-----
	mAxis =
	{
		mRight.x,	mRight.y,	mRight.z,	0,
		mUp.x,		mUp.y,		mUp.z,		0,
		mFront.x,	mFront.y,	mFront.z,	0,
		0,			0,			0,			1
	};

	mPosition =
	{
		1,	0,	0, -mDesc.dEye.x,
		0,	1,	0, -mDesc.dEye.y,
		0,	0,	1, -mDesc.dEye.z,
		0,	0,	0,	1
	};


	mPosition *= mAxis;
	mView = mPosition; //Ya está la matriz view
	UpdateVM();
}

void CCamera::PitchX(int _param){

	glm::mat4x4 rot;
	float speedrot = 0.10f;

	if (_param == GLFW_KEY_UP) {

		rot =
		{
			1,	0,				0,					0,
			0,	cosf(speedrot),	-sinf(speedrot),	0,
			0,	sinf(speedrot),	cosf(speedrot),		0,
			0,	0,				0,					1
		};
	}
	else if (_param == GLFW_KEY_DOWN) {

		rot =
		{
			1,	0,					0,					0,
			0,	cosf(-speedrot),	-sinf(-speedrot),	0,
			0,	sinf(-speedrot),	cosf(-speedrot),	0,
			0,	0,					0,					1
		};
	}
	mView *= rot;

	UpdateVM();
}

void CCamera::YawZ(int _param) {

	glm::mat4x4 rot;
	float speedrot = 0.10f;

	if (_param == GLFW_KEY_Z) {

		rot =
		{
			cosf(speedrot),		0,	sinf(speedrot),	0,
			0,					1,	0,				0,
			-sinf(speedrot),	0,	cosf(speedrot),	0,
			0,					0,	0,				1
		};
	}
	else if (_param == GLFW_KEY_C) {

		rot =
		{
			cosf(-speedrot),		0,	sinf(-speedrot),	0,
			0,						1,	0,					0,
			-sinf(-speedrot),		0,	cosf(-speedrot),	0,
			0,						0,	0,					1
		};
	}
	mView *= rot;

	UpdateVM();
}

void CCamera::RollY(int _param){

	glm::mat4x4 rot;
	float speedrot = 0.10f;

	if (_param == GLFW_KEY_RIGHT) {

		rot =
		{
			cosf(speedrot),	-sinf(speedrot),	0,	0,
			sinf(speedrot),	cosf(speedrot),		0,	0,
			0,				0,					1,	0,
			0,				0,					0,	1
		};
	}
	else if (_param == GLFW_KEY_LEFT) {

		rot =
		{
			cosf(-speedrot),	-sinf(-speedrot),		0,		0,
			sinf(-speedrot),	cosf(-speedrot),		0,		0,
			0,				0,							1,		0,
			0,				0,							0,		1
		};
	}
	mView *= rot;

	UpdateVM();
}
