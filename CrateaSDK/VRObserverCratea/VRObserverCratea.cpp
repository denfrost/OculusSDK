// VRObserverCratea.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <OVR_CAPI.h>   // Include the Oculus SDK
#include <assert.h>
#include <thread>
#include <stdio.h>

///
//This initializes LibOVR and requests information about the available HMD.
///
void ApplicationTest()
{ 
	ovrResult result = ovr_Initialize(nullptr);
	if (OVR_FAILURE(result))
		return;

	ovrSession session;
	ovrGraphicsLuid luid;
	result = ovr_Create(&session, &luid);
	if (OVR_FAILURE(result))
	{
		ovr_Shutdown();
		return;
	}

	ovrHmdDesc desc = ovr_GetHmdDesc(session);
	ovrSizei resolution = desc.Resolution;

	ovr_Destroy(session);
	ovr_Shutdown();
}


int main()
{
	std::string In;
	printf("OVR Viewer Console Start\n"
		"-------------------------------------------\n");

	// Initializes LibOVR, and the Rift
	ovrInitParams initParams = { ovrInit_Invisible | ovrInit_RequestVersion, OVR_MINOR_VERSION, NULL, 0, 0 };
	ovrResult result = ovr_Initialize(&initParams);
	assert(OVR_SUCCESS(result)); // Failed to initialize libOVR

	ApplicationTest();

	return 0;
}

