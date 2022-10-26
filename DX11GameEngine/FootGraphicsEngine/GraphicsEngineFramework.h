#pragma once

/// 그래픽 엔진을 외부에서 사용하기 위한 프레임 워크..!

#include <Windows.h>

#include <DirectXMath.h>

// 렌더러
#include "inc/DX11Renderer/Renderer.h"

// 팩토리와 빌더, 각종 오브젝트
#include "inc/Factory/Factory.h"
#include "inc/Builder/BuilderManager.h"
#include "inc/Object/PosColorObj.h"
#include "inc/Object/LegacyObj.h"


// 각종 구조체
#include "inc/Helper/LightHelper.h"
#include "inc/Helper/VertexStruct.h"
#include "inc/Helper/MathHelper.h"