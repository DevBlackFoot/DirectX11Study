#pragma once
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// 자주 사용하는 헤더파일들을 정리해둔 헤더 파일
#include <Windows.h>
#include <functional>
#include <map>
#include <memory>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <set>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <queue>

// 현재 프로젝트 설정에 따라 스트링을 사용해주기 위해..
#include <xstring>


// 각종 비트단위 int의 별칭을 정해준다
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

#include "SimpleMath.h"
#include "SpriteBatch.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace GameEngineSpace
{
	namespace FootMath
	{
		const float Pi = 3.1415926535f;
	}
}