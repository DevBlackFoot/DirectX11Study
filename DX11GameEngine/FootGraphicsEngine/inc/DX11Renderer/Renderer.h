#pragma once

#include "IRenderer.h"

#include <vector>

using namespace std;
using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;
	class MainRenderTarget;
	class RenderTargetTexture;

	/**
	 * \brief IRenderer�� ��ӹ޾� ������ ������ Ŭ����.
	 *
	 * ���� ������ �پ� �ִ� �κ��� ����߸��� ��� ��..
	 *
	 * 2022.06.14(ȭ) Dev.BlackFoot
	 */
	class Renderer : public IRenderer
	{
		// ���� ���
		// ������ �ڵ�
		HWND hWnd;

		// Device, DC, RS ���� �� �ִ� �ھ� Ŭ����
		DX11GraphicsCore* graphicsCore;

		// View���� ������ �ִ� ���� Ÿ�� Ŭ����
		MainRenderTarget* mainRenderTarget;
		// �׽�Ʈ�� ���� Ÿ��
		RenderTargetTexture* depthRenderTarget;

		// Test�� ���� Ÿ�� ȭ�� ǥ��
		std::shared_ptr<IDXObject> depthRenderTargetObj;

		// TODO DXTK�� �ִ� ��Ʈ �ֱ�

		// Sprite
		SpriteBatch* spriteBatch;

		float deltaTime;	

		// â ������, ���� ��� ���õ� ���� ����.
			// �̰� ��������..?
		bool minimized;
		bool maximized;

		int clientWidth;
		int clientHeight;

	private:
		// ���丮�� �����
			// ȭ�鿡 �׷��� ������Ʈ ��ϵ�.
			// ���� ť? ���� ����?
		vector<std::shared_ptr<IDXObject>> renderVector;

	public:
		Renderer();
		virtual ~Renderer();

		virtual bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight) override;
		virtual void Finalize() override;
		virtual void OnResize() override;
		virtual void RenderAll() override;

		// ��Ⱦ�� ȹ��
		virtual float GetAspectRatio() const override;

		// â ������ ���� ����
		virtual bool IsVaildDevice() override;
		virtual void SetClientSize(int _width, int _height) override;
		virtual void SetWinMinMax(bool _isMinimized, bool _isMaximized) override;

		virtual void AddRenderObj(std::shared_ptr<IDXObject> obj) override;
		virtual void InitObject() override;
		virtual void ClearRenderVector() override;

	private:
		void BeginRender();
		void Render();
		void DebugRender();
		void EndRender();

	};
}

// ������ ��ü�� �����ϴ� �Լ��� ���� ���ش�.
extern "C" GRAPHICSENGINE_DECLSPEC GraphicsEngineSpace::IRenderer* CreateRenderer();
extern "C" GRAPHICSENGINE_DECLSPEC void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj);