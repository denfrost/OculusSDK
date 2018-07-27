/************************************************************************************
Content     :   Locomotion research application for Oculus Rift
Created     :   19th December 2017
Authors     :   Tom Heath
Copyright   :   Copyright 2017 Oculus, Inc. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************************/

#include "general.h"

namespace TESTCOUNTER2
{
	/// Variables / Demo controls
	/// =========================
	/// This demo shows a progression of the 'counter' optic flow, 
	/// where the flow only happens in a variable part of the periphery
	/// and in addition is recentred extremely often, to reduce the 
	/// visibility of the counter optic flow. 
	/// '1' recentres
	/// '9' hold 9 for lurch
	#define INWARD_SPEED 0.02f
	#define OUTWARD_SPEED 0.02f
	int framecountsForAutoSync;  // Do large number for 'off' ie 100000.  Or 100 for pretty active
	bool permanentEffect;
	int option;

	// Local
	VRLayer * variableLayer0 = 0;
	VRLayer * variableLayer1 = 0;
}
using namespace TESTCOUNTER2;

//-------------------------------
int(*TESTCOUNTER2_Init(bool argPermanentEffect, int argOption, float argDegree))(void)
{
	permanentEffect = argPermanentEffect;
	option = argOption;
	framecountsForAutoSync = option ? 4 : 10;

	ovrFovPort myFov[2][2];
	myFov[0][0] = hmdDesc.DefaultEyeFov[0];
	myFov[0][1] = hmdDesc.DefaultEyeFov[1];

	float degree = argDegree;

	myFov[1][0] = hmdDesc.DefaultEyeFov[0];
	myFov[1][0].LeftTan = degree*(hmdDesc.DefaultEyeFov[0].LeftTan + hmdDesc.DefaultEyeFov[1].LeftTan);
	myFov[1][0].RightTan = degree*(hmdDesc.DefaultEyeFov[0].RightTan + hmdDesc.DefaultEyeFov[1].RightTan);
	myFov[1][1] = hmdDesc.DefaultEyeFov[1];
	myFov[1][1].LeftTan = degree*(hmdDesc.DefaultEyeFov[0].LeftTan + hmdDesc.DefaultEyeFov[1].LeftTan);
	myFov[1][1].RightTan = degree*(hmdDesc.DefaultEyeFov[0].RightTan + hmdDesc.DefaultEyeFov[1].RightTan);

	variableLayer0 = new VRLayer(session, myFov[0][0], myFov[0][1], 1.0f, true);
	Layer0 = variableLayer0;
	variableLayer1 = new VRLayer(session, myFov[1][0], myFov[1][1], 1.0f, true);
	Layer1 = variableLayer1;

	*mainCam = Camera(&Vector3(0.0f, 2.0f, -105.0f), &Quat4());
	*counterCam = Camera(&Vector3(0.0f, 2.0f, -105.0f), &Quat4());

	return(TESTCOUNTER2_Process_And_Render);
}

//----------------------------------
int TESTCOUNTER2_Process_And_Render()
{
	// Lets see if in nausea danger
	// Just simply on any motion button press for now
	bool nauseaDanger = false;
	if ((DIRECTX.Key['A'])
		|| (DIRECTX.Key['S'])
		|| (DIRECTX.Key['D'])
		|| (DIRECTX.Key['W'])
		|| (DIRECTX.Key['Q'])
		|| (DIRECTX.Key['E'])
		|| (DIRECTX.Key['9'])
		|| (DIRECTX.Key[VK_UP])
		|| (DIRECTX.Key[VK_DOWN])
		|| (DIRECTX.Key[VK_LEFT])
		|| (DIRECTX.Key[VK_RIGHT]))
		nauseaDanger = true;

	if (permanentEffect)
		nauseaDanger = true;

	static float amountIn = 0;
	if (nauseaDanger)    amountIn += INWARD_SPEED;
	else                amountIn -= OUTWARD_SPEED;
	Maths::Limit(&amountIn, 0, 0.5f);

	bool doWeSync = false;
	if (amountIn == 0) doWeSync = true;
	if ((frameIndex % framecountsForAutoSync) == 0) doWeSync = true;

	APPCAMERA_DefaultMove(mainCam, 0.1f, 0.02f);
	APPCAMERA_DefaultMove(counterCam, -0.1f, -0.02f);

	if (DIRECTX.Key['9'])
	{
		APPCAMERA_ApplyLurchAfterCameraMove(mainCam, 0.1f, 0.1f);
		APPCAMERA_ApplyLurchAfterCameraMove(counterCam, -0.1f, -0.1f);
	}

	if (doWeSync)
	{
		counterCam->Pos = mainCam->Pos;
		counterCam->idealPos = mainCam->idealPos;
		counterCam->smoothedPos = mainCam->smoothedPos;

		counterCam->Rot = mainCam->Rot;
		counterCam->OptionalYaw = mainCam->OptionalYaw;
	}

	// Get the current pose, for standard eye offset, for general use.
	double sensorSampleTime = 0;
	ovrPosef currPose[2];

	ovrPosef HmdToEyePose[2];
	HmdToEyePose[0].Position = VRCONVERT_ovrVector3f(Vector3(-0.032f, 0, 0));
	HmdToEyePose[1].Position = VRCONVERT_ovrVector3f(Vector3(+0.032f, 0, 0));
	HmdToEyePose[0].Orientation = VRCONVERT_ovrQuatf(Quat4());
	HmdToEyePose[1].Orientation = VRCONVERT_ovrQuatf(Quat4());
	ovr_GetEyePoses(session, frameIndex, ovrTrue, HmdToEyePose, currPose, &sensorSampleTime);

	GAME_Process(mainCam->Pos, VRCONVERT_Quat4(currPose[0].Orientation)* mainCam->Rot);

	// Render Scene to Eye Buffers
	for (int eye = 0; eye < 2; ++eye)
	{
		// Layer0
		Layer0->PrepareToRender(session, eye, currPose[eye], sensorSampleTime, 0, 0, 0, 0, true);
		Matrix44 projMatrix = VRCONVERT_Matrix44(ovrMatrix4f_Projection(Layer0->ld.Fov[eye], 0.2f, 1000.0f, ovrProjection_None));
		Matrix44 viewMatrix = VRCONVERT_GetViewMatrixWithEyePose(counterCam, Layer0->ld.RenderPose[eye]);

		float greeny = option ? 1.0f : 1.0f - (amountIn);
		corridorModel->Render(&(viewMatrix * projMatrix), greeny, greeny, greeny, 1.0f, true);
		skyModel->Render(&(viewMatrix * projMatrix), greeny, greeny, greeny, 1.0f, true);

		Layer0->FinishRendering(session, eye);

		// Layer1
		Layer1->PrepareToRender(session, eye, currPose[eye], sensorSampleTime, 0, 0, 0, 0, true);
		projMatrix = VRCONVERT_Matrix44(ovrMatrix4f_Projection(Layer1->ld.Fov[eye], 0.2f, 1000.0f, ovrProjection_None));
		viewMatrix = VRCONVERT_GetViewMatrixWithEyePose(mainCam, Layer1->ld.RenderPose[eye]);

		float amountOfTrans = 1.0f;///1.0f - (amountIn); // ****************************************************************
		float bri = 0.5f;
		corridorModel->Render(&(viewMatrix * projMatrix), 1, 1, 1, amountOfTrans, true);
		skyModel->Render(&(viewMatrix * projMatrix), 1, 1, 1, amountOfTrans, true);

		GAME_Render(&(viewMatrix * projMatrix));

		// Output text
		INFOTEXT_RenderWithOwnCameraAndReset(projMatrix, Layer0->ld.RenderPose[eye]);

		Layer1->FinishRendering(session, eye);
	}

	// Submit all the layers to the compositor
	return(2);
}


