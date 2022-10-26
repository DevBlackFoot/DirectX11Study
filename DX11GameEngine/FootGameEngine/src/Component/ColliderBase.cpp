#include "GamePch.h"
#include "Component/ColliderBase.h"

#include "Object/GameObject.h"
#include "Component/Transform.h"

#include "SceneManager/SceneManager.h"
#include "SceneManager/SceneBase/SceneBase.h"

namespace GameEngineSpace
{
	ColliderBase::ColliderBase(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj, ComponentType::COLLIDER)
		, isTrigger(false)
		, color(Vector3{1.0f, 1.0f, 1.0f})
	{
		transform = GetGameObject()->GetComponent<Transform>();
	}

	ColliderBase::~ColliderBase()
	{
	}

	// TODO Awake�� �Ҹ��� �ʾƼ� �浹 ó���� ����� �� �� ���� ���� �ִ�..!
		// �ش� �̽� �����ؼ� ���߿� ���� �θ��� ������ ��ġ��..
	void ColliderBase::Awake()
	{
		// ���� ���� �ݶ��̴��� �߰����ش�.
			// �⺻������ �ݶ��̴��� ���� ������Ʈ�� ������ �߰��Ѵ�.
		SceneManager::GetInstance()->GetCurrentScene()->AddCollider(this->shared_from_this());
	}
}
