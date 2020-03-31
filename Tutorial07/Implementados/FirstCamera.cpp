#include "Encabezados/FirstCamera.h"




///
/// function to update the view matrix
///
void FirstCamera::UpdateVM() {

	mRight = { mView[0].x,0,mView[0].z };
	mUp = { mView[1].x,mView[1].y,mView[1].z };
	mFront = { mView[2].x,mView[2].y,mView[2].z };
	mDesc.dAt = mDesc.dEye + mFront;
	onlymUp = { 0,mUp.y,0 };
	onlymUp = glm::normalize(onlymUp);
	onlymFRont = { mFront.x,0,mFront.z };
	onlymFRont = glm::normalize(onlymFRont);
}

///
/// function to create the view matrix
///
void FirstCamera::CreateView() {

	///
	/// We use left hand
	///
	//Creamos la vista pero usamos una variable que sea solo front para que la camara no se mueva cuando vaya en linea recta pero no hacia arriba
	mFront = mDesc.dAt - mDesc.dEye;
	mFront = glm::normalize(mFront);
	onlymFRont = { mFront.x,0,mFront.z };
	onlymFRont = glm::normalize(onlymFRont);

	mDesc.dAt = mDesc.dEye + mFront;

	mRight = glm::cross(mDesc.dUp, mFront);
	mRight = { mRight.x,0,mRight.z };
	mRight = glm::normalize(mRight);

	mUp = glm::cross(mFront, mRight);
	onlymUp = { 0,mUp.y,0 };
	onlymUp = glm::normalize(onlymUp);
	mAxis =
	{
		mRight.x,0,mRight.z, 0,
		mUp.x,mUp.y,mUp.z,0,
		mFront.x,mFront.y,mFront.z,0,
		0,0,0,1
	};
	mPosition =
	{
		1,0,0,-mDesc.dEye.x,
		0,1,0,-mDesc.dEye.y,
		0,0,1,-mDesc.dEye.z,
		0,0,0,1
	};
	mPosition *= mAxis;
	mView = mPosition;

}

void FirstCamera::Movement(WPARAM wParam)
{

	if (wParam == 'w' || wParam == 'W')
	{
		mDesc.dEye += onlymFRont;
	}
	if (wParam == 's' || wParam == 'S')
	{
		mDesc.dEye -= onlymFRont;
	}
	if (wParam == 'a' || wParam == 'A')
	{
		mDesc.dEye -= mRight;
	}
	if (wParam == 'd' || wParam == 'D')
	{
		mDesc.dEye += mRight;
	}
	//if (wParam == 'q' || wParam == 'Q')
	//{
	//	CCamera.dEye += onlymUp;
	//}
	//if (wParam == 'e' || wParam == 'E')
	//{
	//	CCamera.dEye -= onlymUp;
	//}


	glm::mat4x4 Axis{
		mRight.x,0,mRight.z, 0
				,mUp.x,mUp.y,mUp.z,0
				,mFront.x,mFront.y,mFront.z,0
				,0,0,0,1 };
	glm::mat4x4 Pos = {
		1,0,0,-mDesc.dEye.x,
		0,1,0,-mDesc.dEye.y,
		0,0,1,-mDesc.dEye.z ,
		0,0,0,1
	};
	Pos *= Axis;
	mView = Pos;
	UpdateVM();
}

glm::vec3 FirstCamera::getPos()
{
	return mDesc.dEye;
}

///
/// function for mouse movement
///
void FirstCamera::MouseRotation(){

	glm::vec2 FirstPos;
	glm::vec2 SecondPos;

	float SpeedRot = 0.0015f;
	float speedAngule = 2;

	glm::mat4x4 Yaw =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4x4 Pitch =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	POINT Temp;
	GetCursorPos(&Temp);

	FirstPos.x = Temp.x;
	SecondPos.y = Temp.y;

	if (FirstPos.x < OriginalMousePos.x) {

		mDesc.dAt -= mRight * SpeedRot;
		mDesc.dUp = mUp;

		CreateView();
	}
	if (FirstPos.x > OriginalMousePos.x) {

		mDesc.dAt += mRight * SpeedRot;
		mDesc.dUp = mUp;
		CreateView();
	}

	if (SecondPos.y < OriginalMousePos.y && angule < maxAngule) {

		angule += speedAngule;

		if (angule > maxAngule){

			angule = maxAngule;
		}
		else{

			Pitch =
			{
				1,	0,									0,										0,
				0,	cosf(speedAngule * 3.141516 / 180),	-sinf(speedAngule * 3.141516 / 180),	0,
				0,	sinf(speedAngule * 3.141516 / 180),	cosf(speedAngule * 3.141516 / 180),		0,
				0,	0,									0,										1
			};
		}

	}
	if (SecondPos.y > OriginalMousePos.y&& angule > -maxAngule) {

		angule -= speedAngule;

		if (angule < -maxAngule){

			angule = -maxAngule;
		}
		else{

			Pitch =
			{
				1,	0,										0,										0,
				0,	cosf(-speedAngule * 3.141516 / 180),	-sinf(-speedAngule * 3.141516 / 180),	0,
				0,	sinf(-speedAngule * 3.141516 / 180),	cosf(-speedAngule * 3.141516 / 180),	0,
				0,	0,										0,										1
			};
		}

	}

	SetCursorPos(OriginalMousePos.x, OriginalMousePos.y);
	mView *= Yaw;
	UpdateVM();

	mView *= Pitch;
	UpdateVM();
}