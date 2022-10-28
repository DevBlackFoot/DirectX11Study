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
		// �ϴ��� �ϵ��ڵ�����
			// ���Ŀ� ������ ���� �� ������.. ���� �Լ��� �ϳ� �Ĵ� �͵� ������ ������.

		// shaderBlob�� ���̴� Ŭ������ ��ü�� ���� �� ���� �ٲ��־���Ѵ�.
			// �װ� ���⼭ ������ ������ִ� ������...
			// ���̴��� ���鶧 ���ø� �Լ��� �θ��� ������ �ϱ⵵ ������.. shaderManager�� �ʿ� �־��ֱ� ���ؼ� ���⼭ �غ��� ������
		// ���ؽ����̴� ����
		CreateVertexShader("Shader/PosColor/PosColorVS.hlsl", "main", "vs_5_0", "PosColorVS", InputLayoutDesc::PosColor, 2);
		CreateVertexShader("Shader/TextureRect/TextureRectVS.hlsl", "main", "vs_5_0", "TextureRectVS", InputLayoutDesc::TextureRect, 2);
		CreateVertexShader("Shader/LegacyModel/LegacyModelVS.hlsl", "main", "vs_5_0", "LegacyModelVS", InputLayoutDesc::LegacyModel, 4);

		// �ȼ����̴� ����
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
		// ���� ��ȸ �ϸ鼭 ��� �ڵ� ����
		for(auto shader : shaderMap)
		{
			shader.second.reset();
		}

		shaderMap.clear();

		SafeReset(instance)
	}

	/**
	 * \brief hlsl�ϳ��� �������ϴ� �Լ�
	 * \param path ���̴� ������ path
	 * \param entryName shader �ڵ� ������ entryPointer name
	 * \param shaderModel ����� ���̴� ���α׷��� ������ ��� ����.
	 * \param blobOut ���̴��� �������ؼ� ������ blob�� ������
	 * \return ���� ����
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

		// Ȥ�� �� ��� ����
		if (shaderBlob != nullptr)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}

		// ���� ���ڸ� ����մϴ�.
		std::wstring wpath;

		wpath.assign(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob) != true)
			return false;

		// �߰��� �ش� ���ؽ� ���̴��� ����ϴ� ��ǲ ���̾ƿ��� ������ش�.
		tmpShader.reset(new VertexShader(device, deviceContext, shaderBlob, desc, elementsSize));

		if (tmpShader == nullptr)
		{
			assert(0);		// ���� ó��
		}

		// ������� ���ؽ� ���̴��� �ʿ� �־��ش�.
		shaderMap.insert(std::pair{shaderName, tmpShader });

		// ������� ���� ����
		return true;
	}

	bool ShaderManager::CreatePixelShader(const std::string& path, const std::string& entryName,
		const std::string& shaderModel, const std::string& shaderName)
	{
		// �⺻������ ���� �Ͱ� ����ϳ� pixelshader�� �����մϴ�.
		std::shared_ptr<ShaderBase> tmpShader;

		// Ȥ�� �� ��� ����
		if (shaderBlob != nullptr)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}

		// ���� ���ڸ� ����մϴ�.
		std::wstring wpath;

		wpath.assign(path.begin(), path.end());

		if (CompileShaderFromFile(wpath.c_str(), entryName.c_str(), shaderModel.c_str(), &shaderBlob) != true)
			return false;

		// �߰��� �ش� ���ؽ� ���̴��� ����ϴ� ��ǲ ���̾ƿ��� ������ش�.
		tmpShader.reset(new PixelShader(device, deviceContext, shaderBlob));

		if (tmpShader == nullptr)
		{
			assert(0);		// ���� ó��
		}

		// ������� �ȼ� ���̴��� �ʿ� �־��ش�.
		shaderMap.insert(std::pair{ shaderName, tmpShader });

		// ������� ���� ����
		return true;
	}
}
