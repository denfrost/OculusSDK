// VRObserverCratea.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Source.h"

#define BUFLEN 256

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

	printf("Choose your VR Interface firmware\n"
		"0.Cratea\n"
		"1.Oculus\n"
		"2.Steam\n"
		"3.PS\n"
		"4.Windows\n");

	char buffer[BUFLEN];   /* the string is stored through pointer to this buffer */
	printf("Please Enter Command:");
	fflush(stdout);
	fgets(buffer, BUFLEN, stdin); /* buffer is sent as a pointer to fgets */
	//printf("the string was: %s", buffer);

	if (buffer[0] == char('1'))
		CheckCommand(buffer);
	else return 1;
	return 0;
}

