#pragma once

/// 그래픽 엔진을 외부에서 사용하기 위한 프레임 워크..!

#include <Windows.h>

#include <DirectXMath.h>

// 렌더러
#include "inc/DX11Renderer/Renderer.h"

// 팩토리와 빌더, 각종 오브젝트
#include "inc/Factory/Factory.h"
#include "inc/Builder/BuilderManager.h"
#include "inc/Object/LineObj.h"
#include "inc/Object/LegacyStaticObj.h"
#include "inc/Object/SkyBox.h"
#include "inc/Object/IDXObject.h"

// 각종 구조체
#include "inc/Helper/LightHelper.h"
#include "inc/Helper/VertexStruct.h"
#include "inc/Helper/MathHelper.h"

// 그래픽 렌더링 데이터
#include "inc/Utils/GraphicsRenderingData.h"

// 각종 UI
#include "Object/UI/UIBase.h"
#include "Object/UI/Canvas.h"
#include "Object/UI/SpriteUI.h"

#include "inc/Manager/ResourceManager.h"