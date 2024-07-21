#pragma once


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "targetver.h"
#include "framework.h"
#include "Resource.h"

#include "Types.h"
#include "Values.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>


// WIN
#include <assert.h>
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>


// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <DirectXMath.h>

#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTex.inl"

#ifdef _DEBUG
#pragma comment(lib, DirectXTex/DirectXTex_d.lib)
#else
#pragma comment(lib, DirectXTex/DirectXTex.lib)
#endif