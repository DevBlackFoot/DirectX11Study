#pragma once
#include "Object.h"
#include "Component/Transform.h"

namespace GameEngineSpace
{

	class ComponentBase;
	class Transform;
	class ColliderBase;
	class ITriggerable;

	/**
	 * \brief �⺻������ Scene�� Ŭ���̾�Ʈ �������� ���� �߰��� �� �ִ� ���� ������Ʈ ����
	 *
	 * ���������� �θ�, �ڽİ� ���� ���� ���� �� ����� ����� ������Ʈ���� ������ �ִ�.
	 *
	 * �⺻������ Ʈ�������� �⺻������ ������ �ִ�.
	 *
	 * 2022.07.22(��) Dev.BlackFoot
	 */
	 // ���� ������Ʈ�� �ٸ� Ŭ������ ��� ���� �ʽ��ϴ�. (�θ� Ŭ������ ���� �ʽ��ϴ�.) => final Ű���� ���
	 // ���� Shared_ptr�� �������� �����ϱ� ���� enable_shared_from_this�� ����մϴ�.
	class GameObject final : public Object, public std::enable_shared_from_this<GameObject>
	{
		// ������Ʈ�� �̸�
		tstring name;
		// �±�
		tstring tag;
		// ���̾�
		uint32 layer;
		// �θ� ������
		std::weak_ptr<GameObject> parent;
		// �ڽ� ������
			// �ڽ��� �������� �� �� �ֽ��ϴ�.
		std::vector<std::shared_ptr<GameObject>> children;
		// �ߺ����� �ʴ� ������Ʈ
		std::set<std::shared_ptr<ComponentBase>> components;
		// Ʈ�������� ������ �ֽ��ϴ�.
		std::shared_ptr<Transform> transform;
		// Ʈ���� ������ ������Ʈ���� ������ �ִ� ����.
			// �ϳ��� ������Ʈ�� �پ��� �ݶ��̴����� ���� �� �ֽ��ϴ�.
		std::vector<std::shared_ptr<ITriggerable>> triggerables;

	public:
		GameObject();
		virtual ~GameObject();

		virtual void Release() override;

		// ���� ���̵� �� �Լ���
		void Awake() override;
		void Start() override;
		void PreUpdate(float tick) override;
		void Update(float tick) override;
		void LateUpdate(float tick) override;

		// ���� ���� ����
		const tstring& GetName() { return name; }
		void SetName(const tstring& _name) { name = _name; }

		const tstring& GetTag() { return tag; }
		void SetTag(const tstring& _tag) { tag = _tag; }

		const uint32& GetLayer() { return layer; }
		void SetLayer(const uint32& _layer) { layer = _layer; }

		std::shared_ptr<GameObject> GetParent() { return parent.lock(); }

		std::shared_ptr<GameObject> GetChild(tstring _name);
		std::vector<std::shared_ptr<GameObject>>& GetChildren() { return children; }
		void SetChild(std::shared_ptr<GameObject> _child);

		void SetGameObjectEnable(bool _isEnable);

		void EraseParent();

	private:
		void SetParent(std::shared_ptr<GameObject> _parent);

	public:
		// Ʈ���� �̺�Ʈ ����..!
		void OnTriggerEnter(std::shared_ptr<ColliderBase> other);
		void OnTriggerStay(std::shared_ptr<ColliderBase> other);
		void OnTriggerExit(std::shared_ptr<ColliderBase> other);

	public:
		// ������Ʈ Get �� Add
			// Ư�� TComponent�� ��ȯ�ϴ� GetComponent
		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent()
		{
			// ������Ʈ�� �ݵ�� ��ӹ޾ƾ� �մϴ�. ������Ʈ ���üũ
			static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent�� ComponentBase�� ��ӹ޾ƾ� �մϴ�.");

			// Ʈ���� ���̸� Ʈ������ ��ȯ => Ʈ�������� ���� ������Ʈ Set�� �� ���� �ʱ� ����
				/// --- ���γ�Ʈ ---
				// �̶� cosntexper �� ��� ǥ������ ��������. �ش� ���ǽ��� ������Ÿ��(?)�� �˻��ؼ�
				// ���ø��� ���� �� �Ǽ��� ���� �ʵ����Ѵ�.
				///	---------------
			if constexpr (std::is_same<TComponent, Transform>::value == true)
				return transform;

			// �װ� �ƴҰ�� Set�� ��ȸ�ϸ鼭 ã�´�.
			for (auto& component : components)
			{
				// �ڽ� �����ͷ� ���̳��� ĳ������ ���ְ�.. �ش� ������Ʈ�� �����Ѵٸ� ��ȯ���ش�.

				/// --- ���� ��Ʈ ---
				// Components�� Shared_ptr<ComponentBase> (== ComponentBase*)�� set�̰�
				// ���ø��� ������ ������ TComponent�� ComponentBase�� ����� Ư�� ������Ʈ�̴�.
				// ���� ���� ĳ������ dynamic_cast�� ���ؼ� ���ְ�, �ش� ĳ������ ����� nullptr�� �ƴҰ��
				// TComponent�� Components�� �����Ѵٴ� ���� Ȯ���� �� �ְ� �ȴ�.
				// Shared_ptr���� �ٷ� dynamic_cast�� �� �� ���� ������, ���̳��� ĳ��Ʈ�� ������ ������ �ϴ�
				// dynamic_pointer_cast�� ����Ѵ�.
				/// -----------------
				std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(component);

				if (dcComponent != nullptr)
					return dcComponent;
			}

			// ��� Set�� �� ã�Ƶ� ������Ʈ�� �������� ������ nullptr�� �����Ѵ�.
			return nullptr;
		}

		// ������Ʈ�� �޾Ƽ� Components�� �߰����ִ� �Լ�.
			// ���ø� ������ Ȱ���ؼ�..
			// AddComponent<TComponent>() �� ���� �������� Ȱ���Ѵ�.
		template <typename TComponent>
		std::shared_ptr<TComponent> AddComponent()
		{
			// ���� ���������� ������Ʈ ���� Ȯ�����ش�.
			static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent�� ComponentBase�� ��ӹ޾ƾ� �մϴ�.");

			if constexpr (std::is_same<TComponent, Transform>::value == true)
			{
				// Ʈ���� ���̸� �׳� m_TransForm�� �־��ش�.
				transform = std::make_shared<Transform>(this->shared_from_this());

				components.emplace(transform);

				return transform;
			}

			std::shared_ptr<TComponent> tmpComponent = std::make_shared<TComponent>(this->shared_from_this());

			components.emplace(tmpComponent);

			// Ʈ���� ������ ������Ʈ�� �ش� ������Ʈ�� ���� ĳ�����ش�.
				// ���Ŀ� �̺�Ʈ�� �����ֱ� ���� 
			if constexpr (std::is_base_of<ITriggerable, TComponent>::value == true)
				triggerables.push_back(static_cast<std::shared_ptr<ITriggerable>>(tmpComponent));

			return tmpComponent;

		}
	};

}
