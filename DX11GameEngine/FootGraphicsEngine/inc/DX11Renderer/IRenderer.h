#pragma once
/// Dll ��ũ��
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;
	class Canvas;

	/**
	* \brief Renderer�� ��ɸ� �����ص� �������̽� Ŭ����
	*
	* 2022.06.14(ȭ) Dev.BlackFoot
	*/
	class GRAPHICSENGINE_DECLSPEC IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() {}

		// �ݵ�� ����������ϴ� ���
		// �ʱ�ȭ => Device, DC, SwapChain ����
		// �ʱ�ȭ �� �� �޾Ƽ� Render���� �Ѱ��ش�.
		virtual bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight) abstract;


		// ������ ��ü�� �ʱ�ȭ�� ���� ��, ������Ʈ ���� �ʱ�ȭ ���ִ� �Լ� �߰�
		virtual void InitObject() abstract;

		// ���� ����
		virtual void Finalize() abstract;
		// ȭ�� â�� �ٲ���� �� ���ִ� onResize(���� Ÿ�� ����)
		virtual void OnResize() abstract;

		// �׸��� �Լ�
			// Begin, Render, DebugRender ������ �� �ִ� ���� �Լ�
		virtual void RenderAll() abstract;
		// �׸��� �����Լ�
			// present�� ���� ������ ��ġ�� �Լ�
		virtual void EndRender() abstract;

		// â ������� ���õ� �Լ� ��� => �ܺο��� ��� �����ؾ��ϱ� ������ �������̽��� �߰�
		virtual bool IsVaildDevice() abstract;
		virtual void SetClientSize(int _width, int _height) {}
		virtual void SetWinMinMax(bool _isMinimized, bool _isMaximized) {}

		// ȭ��� ������
		virtual float GetAspectRatio() const abstract;

		// ���� ������Ʈ �߰�.
		virtual void AddRenderObj(std::shared_ptr<IDXObject> obj) abstract;
		
		// ���� �ٲ� �� ���� obj�� �����ִ� �Լ�
		virtual void ClearRenderVector() abstract;

		// UI ���� �����Լ�
		virtual std::shared_ptr<Canvas> CreateCanvas(const std::string& name, float width, float height) abstract;
	};

}