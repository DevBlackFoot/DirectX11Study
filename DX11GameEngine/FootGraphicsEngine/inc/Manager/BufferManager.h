#pragma once
#include <map>
#include <string>

namespace GraphicsEngineSpace
{
	class BufferBase;

	// ���̴��� ���Ǵ� Buffer���� �����ϴ� �Ŵ��� Ŭ����
		// ����� ��ǻ� cb�� ������ �ִ�.
		// �� ������ �� Obj Ŭ�������� ���ش�.
	class BufferManager
	{
	private:
		static std::shared_ptr<BufferManager> instance;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;

		// ���� ���۸� �����ص� ��
			// ������ �� ���� ��쿡�� ������ �������� �ϰ� (������ ������ �ϰ�)
			// ���ҽ� ��ü�� ������ ���ҽ� �Ŵ����� ���� �������־���.
		std::map<std::string, std::shared_ptr<BufferBase>> bufferMap;

	public:
		BufferManager()
			: device(nullptr)
			, deviceContext(nullptr)
		{}
		~BufferManager();

		// �ν��Ͻ�
		static std::shared_ptr<BufferManager> GetInstance();

		void Init(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

		// ����� ���� �����
		bool CreateConstantBuffer(std::string cbName, UINT typeSize);

		// �ʿ��� ���� �ҷ�����
		std::shared_ptr<BufferBase> GetBuffer(std::string bufferName);
	};
}