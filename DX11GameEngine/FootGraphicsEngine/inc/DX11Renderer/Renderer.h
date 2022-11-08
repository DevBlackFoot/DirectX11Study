#pragma once

#include "IRenderer.h"

#include <vector>

using namespace std;
using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;
	class RenderTargetDepth;
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
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		// View���� ������ �ִ� ���� Ÿ�� Ŭ����
		RenderTargetDepth* mainRenderTarget;

		// depth debug�� ���� Ÿ��.
		RenderTargetTexture* depthRenderTarget;
		RenderTargetTexture* normalRenderTarget;
		RenderTargetTexture* albedoRenderTarget;
		RenderTargetTexture* worldPosRenderTarget;

		// ��ũ������ ����ϴ� ����Ʈ�� DESC�� ���ص� screenViewport
			// CubeMap�� �׸��� ���� Viewport�� ���� ����������ϱ� ������..
		D3D11_VIEWPORT screenViewport;

		// BlendState ����
		ID3D11BlendState* blendState;

		// Sprite
		std::shared_ptr<SpriteBatch> spriteBatch;

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

		virtual void EndRender() override;
		// ĵ���� ����
		virtual std::shared_ptr<Canvas> CreateCanvas(const std::string& name, float width, float height) override;

	private:
		void BeginRender();
		void Render();
		void DebugRender();

	};
}

// ������ ��ü�� �����ϴ� �Լ��� ���� ���ش�.
extern "C" GRAPHICSENGINE_DECLSPEC GraphicsEngineSpace::IRenderer* CreateRenderer();
extern "C" GRAPHICSENGINE_DECLSPEC void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj);