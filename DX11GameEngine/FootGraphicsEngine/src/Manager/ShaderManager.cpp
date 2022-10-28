#include "GraphicsPch.h"
#include "Manager/ShaderManager.h"
#include "Object/Shader/VertexShader.h"
#include "Object/Shader/PixelShader.h"
#include "Resources/InputLayout.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ShaderManager> ShaderManager::instance = nullptr;

	ShaderManager::~ShaderManager()
	{
		if (shaderBlob != nullptr)
			shaderBlob->Release();
	}

	std::shared_ptr<ShaderManager> ShaderManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ShaderManager>();

		return instance;
	}

	void ShaderManager::Init(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext)
	{
		device = _device;
		deviceContext = _deviceContext;
	}

	bool ShaderManager::CompileAllShader()
	{
		// 일단은 하드코딩으로
			// 이후에 밖으로 빼낼 수 있으면.. 새로 함수를 하나 파는 것도 나쁘지 않을듯.

		// shaderBlob은 쉐이더 클래스의 객체를 만들 때 마다 바꿔주어야한다.
			// 그걸 여기서 줄줄줄 만들어주는 것으로...
			// 쉐이더를 만들때 템플릿 함수를 부르는 식으로 하기도 했지만.. shaderManager의 맵에 넣어주기 위해서 여기서 해보는 것으로
		// 버텍스셰이더 생성
		CreateVertexShader("Shader/PosColor/PosColorVS.hlsl", "main", "vs_5_0", "PosColorVS", InputLayoutDesc::PosColor, 2);
		CreateVertexShader("Shader/TextureRect/TextureRectVS.hlsl", "main", "vs_5_0", "TextureRectVS", InputLayoutDesc::TextureRect, 2);
		CreateVertexShader("Shader/LegacyModel/LegacyModelVS.hlsl", "main", "vs_5_0", "LegacyModelVS", InputLayoutDesc::LegacyModel, 4);

		// 픽셀셰이더 생성
		CreatePixelShader("Shader/PosColor/PosColorPS.hlsl", "main", "ps_5_0", "PosColorPS");
		CreatePixelShader("Shader/TextureRect/TextureRectPS.hlsl", "main", "ps_5_0", "TextureRectPS");
		CreatePixelShader("Shader/LegacyModel/LegacyModelPS.hlsl", "main", "ps_5_0", "LegacyModelPS");

		return true;
	}

	std::shared_ptr<ShaderBase> ShaderManager::GetShader(const std::string& shaderName)
	{
		if (shaderMap.find(shaderName) == shaderMap.end())
			return nullptr;

		return shaderMap.at(shaderName);
	}

	void ShaderManager::Finalize()
	{
		// 맵을 순회 하면서 모든 코드 삭제
		for(auto shader : shaderMap)
		{
			shader.second.reset();
		}

		shaderMap.clear();

		SafeReset(instance)
	}

	/**
	 * \brief hlsl하나를 컴파일하는 함수
	 * \param path 쉐이더 파일의 path
	 * \param entryName shader 코드 내부의 entryPointer name
	 * \param shaderModel 사용할 셰이더 프로그램의 종류와 대상 버전.
	 * \param blobOut 쉐이더를 컴파일해서 내보낼 blob의 포인터
	 * \return 성공 여부
	 */
	bool ShaderManager::CompileShaderFromFile(const TCHAR* path, const char* entryName, const char* shaderModel,
		ID3DBlob** blobOut)
	{
		if (path == nullptr || entryName == nullptr || shaderModel == nullptr || blobOut == nullptr)
			return false;

		*blobOut = nullptr;

		UINT flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(
			path,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryName,
			shaderModel,
			flags,
			0,
			blobOut,
			&errorBlob);

		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
				errorBlob->Release();
			}

			return false;
		}

		if (errorBlob != nullptr)
			errorBlob->Release();

		return true;
	}

	bool ShaderManager::CreateVertexShader(const std::string& path, const std::string& entryName,
		const std::string& shaderModel, const std::string& shaderName, D3D11_INPUT_ELEMENT_DESC* desc,
		UINT elementsSize)
	{
		std::shared_ptr<ShaderBase> tmpShader;

		// 혹시 모를 블롭 리셋
		if (shaderBlob != nullptr)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}

		// 위의 인자를 사용합니다.
		std::wstring wpath;

		wpath.assign(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob) != true)
			return false;

		// 추가로 해당 버텍스 쉐이더를 사용하는 인풋 레이아웃을 만들어준다.
		tmpShader.reset(new VertexShader(device, deviceContext, shaderBlob, desc, elementsSize));

		if (tmpShader == nullptr)
		{
			assert(0);		// 예외 처리
		}

		// 만들어준 버텍스 쉐이더를 맵에 넣어준다.
		shaderMap.insert(std::pair{shaderName, tmpShader });

		// 여기까지 오면 성공
		return true;
	}

	bool ShaderManager::CreatePixelShader(const std::string& path, const std::string& entryName,
		const std::string& shaderModel, const std::string& shaderName)
	{
		// 기본적으로 위의 것과 비슷하나 pixelshader를 생성합니다.
		std::shared_ptr<ShaderBase> tmpShader;

		// 혹시 모를 블롭 리셋
		if (shaderBlob != nullptr)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}

		// 위의 인자를 사용합니다.
		std::wstring wpath;

		wpath.assign(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob) != true)
			return false;

		// 추가로 해당 버텍스 쉐이더를 사용하는 인풋 레이아웃을 만들어준다.
		tmpShader.reset(new PixelShader(device, deviceContext, shaderBlob));

		if (tmpShader == nullptr)
		{
			assert(0);		// 예외 처리
		}

		// 만들어준 픽셀 쉐이더를 맵에 넣어준다.
		shaderMap.insert(std::pair{ shaderName, tmpShader });

		// 여기까지 오면 성공
		return true;
	}
}
