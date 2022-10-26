#pragma once
#include <map>

#include "Builder/IBuilder.h"
#include "GraphicsCore/RasterizerState.h"

/// Dll ��ũ��
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief �ܺο��� �������� ����ϱ� ���� ������ ������ �̱��� �Ŵ���
	 *
	 * ������(�׷��Ƚ� ����) ���ο��� ������ ����ϴ� ���� �ƴ�, ���� ���� Ȥ�� Ŭ���̾�Ʈ ��ġ���� ������ ����ؾ��ϱ� ������
	 * ���� �Լ����� ����� �� �ֵ��� �ܺη� �̾Ƴ��� Ŭ�����̴�.
	 * �ش� Ŭ�������� ����̽�, DC, RS ���� �ʱ�ȭ �ϰ�, ����� �� �̾��ش�.
	 *
	 * 2022.07.12(ȭ) Dev.BlackFoot
	 */
	class BuilderManger
	{
	private:
		// �ܺο��� �޾��� �ν��Ͻ�
		static BuilderManger* m_Instance;

		// ������ ������ ��
		std::map<std::string, IBuilder*> m_BuilderMap;

		// ���ӿ��� or Ŭ���̾�Ʈ���� ������ �߰��Ѵٰ� ���� ��.. Device�� DC���� ������ �� ���� ������ �װ� ���⼭ ������ �ִ´�.
		// �װ� �ƴ϶�� ������ �߰� �� ������ �� Ŭ������ �������� ������ �߰����ִ� ����� �ִ�.(�ϵ� �ڵ�)
			// ����(22.07.12) Effect Ŭ������ InputLayout Ŭ������ �׷��� �Ǿ��ִ�.
		// �ϴ��� ���⼭ DC���� ������ �ְ�, �ʿ� ������ �߰��� �� �־��ִ� ������� �Ѵ�.
		ID3D11Device* m_D3DDevice = nullptr;
		ID3D11DeviceContext* m_D3DDeviceContext = nullptr;
	public:
		// �׷��Ƚ� ���� �ȿ��� ����� �Լ�
		// ���� ����ϴ� ������ �ѹ��� Init
		void InitBuilderAll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
		// ���ο� ������ ���� �߰��� ��..
			// T ��� ������ ��� ���� ������ �߰����ش�.
		template <typename T>
		void AddBuilder(std::string _builderName);

		// �׷��Ƚ� ���� �ܺο��� ��� ������ �Լ���.
		GRAPHICSENGINE_DECLSPEC static BuilderManger* GetInstance()
		{
			if (m_Instance == nullptr)
			{
				m_Instance = new BuilderManger;
			}

			return m_Instance;
		}
		GRAPHICSENGINE_DECLSPEC void DeleteBuildManager();

		// Ư�� ������ �̾Ƴ���
		GRAPHICSENGINE_DECLSPEC IBuilder* GetBuilder(std::string _builderName);
	};

	template <typename T>
	void BuilderManger::AddBuilder(std::string _builderName)
	{
		// ����ó��
		assert(m_D3DDevice);
		assert(m_D3DDeviceContext);
		assert(RasterizerState::GetSolidRS());
		assert(RasterizerState::GetWireFrameRS());

		// �������� �ñ״��ĸ� ��ġ��Ų��.
		IBuilder* _tmpBuilder = new T();

		// ���� �ʱ�ȭ
			// ��� ������ Init�� ���� �Ǿ��־���Ѵ�.
		_tmpBuilder->InitBuilder(m_D3DDevice, m_D3DDeviceContext);

		// �ش� ������ �߰���Ų��.
		m_BuilderMap.insert({ _builderName, _tmpBuilder });
	}
}
