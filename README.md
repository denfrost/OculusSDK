# OculusSDK
OculusSDK for 2018
<img src="oculus.png" align="middle" width="500"/>
<a name="toc"></a>
## Table of Contents CrateaVR_Sdk

[![Build Status](https://travis-ci.org/carla-simulator/carla.svg?branch=master)](https://github.com/Pangeae/OculusSDK)

<img src="CrateaSDK_VRObserver.png" align="middle" width="3000"/>

Alpha version

<img src="CrateaPlatform.png" align="middle" width="1000"/>

Roadmap
-------

Console control VR devices CrateaVR for Native App. C++, DX
Our most immediate goals are:

- [x] OVR Oculus Library Virtual reality pluged
- [ ] App console interface for CrateaVR
- [ ] Implement for Diagnost Vr devices HMD,Inputs,Camera,Gloves,Sensors etc.
   - [ ] HMD
   - [ ] Inputs
   - [ ] Cameras
   - [ ] Gloves
   - [ ] Sensors
- [ ] Designed future plugin with Ovr for Unity3d for fast integration all Sdk DLL's.
- [ ] Support VR devices Rift,Steam,PS,Win
[![Documentation](https://readthedocs.org/projects/carla/badge/?version=latest)](https://github.com/Pangeae/OculusSDK)

1) This example initializes LibOVR and requests information about the available HMD.

`Code c++`

```cpp
	// Include the OculusVR SDK
	#include <OVR_CAPI.h>
	void Application()
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
```
2) Support SteamVR Add plugin from Steam OpenVR http://u3d.as/content/valve-corporation/steam-vr-plugin
`Code c#`
```cpp
using System;
using System.Runtime.InteropServices;
using Valve.VR;

namespace Valve.VR
{

[StructLayout(LayoutKind.Sequential)]
public struct IVRSystem
{
	[UnmanagedFunctionPointer(CallingConvention.StdCall)]
	internal delegate void _GetRecommendedRenderTargetSize(ref uint pnWidth, ref uint pnHeight);
	[MarshalAs(UnmanagedType.FunctionPtr)]
	internal _GetRecommendedRenderTargetSize GetRecommendedRenderTargetSize
}
```

**[⬆ Back to Top](#table-of-contents)**
