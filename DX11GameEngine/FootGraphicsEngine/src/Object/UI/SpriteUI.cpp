#include "GraphicsPch.h"

#include "Object/UI/SpriteUI.h"
#include "Manager/UIUtilsManager.h"

namespace GraphicsEngineSpace
{
	SpriteUI::SpriteUI()
		: texture(nullptr)
	{
	}

	SpriteUI::~SpriteUI()
	{

	}

	void SpriteUI::SetTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = texture;
	}

	std::shared_ptr<Texture> SpriteUI::GetTexture()
	{
		return texture;
	}

	void SpriteUI::Render(float tick)
	{
		if (isEnable != true)
			return;

		// 그림 그리기
		if (texture != nullptr)
		{
			auto screenPos = GetScreenPosition();
			auto screenScale = GetScreenScale();

			// Batch로 그려주기..
				// UIUtilManager에 있는 batch를 사용합니다.
			std::shared_ptr<SpriteBatch> batch = UIUtilsManager::GetInstance()->GetSpriteBatch();
			auto depthState = UIUtilsManager::GetInstance()->GetDepthState();
			auto blendState = UIUtilsManager::GetInstance()->GetBlendState();

			batch->Begin(DirectX::SpriteSortMode_Texture, blendState, nullptr, depthState);
			batch->Draw(texture->GetMapSRV().Get(), RECT{
			static_cast<long>(screenPos.x),
			static_cast<long>(screenPos.y),
			static_cast<long>(screenPos.x + (width * screenScale.x)),
			static_cast<long>(screenPos.y + (height * screenScale.y))}, nullptr,
			DirectX::Colors::White, 0.0f, {0.f, 0.f}, 
			DirectX::SpriteEffects_None, screenPos.z);
			batch->End();

			for(auto iter : children)
			{
				if(iter != nullptr)
					iter->Render(tick);
			}

		}
	}
}
