#pragma once
#include "Object/ComponentBase.h"

#include "PCH/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class GameObject;

	enum class Space
	{
		WORLD,
		LOCAL
	};

	/**
	 * \brief 모든 게임 오브젝트가 반드시 가지고 있는 트랜스폼 컴포넌트 입니다.
	 *
	 * 오브젝트의 위치, 각도, 스케일 정보를 가지고 있습니다.
	 *
	 * 2022.07.22(금) Dev.BlackFoot
	 */
	class Transform : public ComponentBase
	{
		// 각종 위치 관련 변수들
		// Transform Matrix
			// Local TM == 부모의 원점에서 부모 공간에서의 자기 위치로 이동시키는 TM / LocalTM
			// World TM == 씬의 원점에서 World 위치로 가는 TM
		Matrix localTM;
		Matrix worldTM;

		// 기본적으로 지훈이형의 Vector는 w팩터 까지 들어있다..
		Vector3 localPosition;
		Vector3 localRotation;
		Vector3 localScale;

		Vector3 worldPosition;
		Vector3 worldRotation;
		Vector3 worldScale;

		// 중점기준으로, 앞, 위 오른쪽 벡터
		Vector3 look;
		Vector3 up;
		Vector3 right;

		// TM 업데이트를 위한 각종 변수 및 함수.
		// 현재 자신의 TM을 변경 해줘야하는가.
		bool needTMUpdate;

	public:
		Transform(std::weak_ptr<GameObject> _gameObject);

		virtual ~Transform();

		virtual void Release() override;

		virtual void Update(float tick) override;

		// 변경할 위치 정보와, 기반이 되는 공간을 지정해줍니다.
		void SetPosition(const Vector3& _newPos, Space relativeTo = Space::WORLD);
		void SetRotation(const Vector3& _eulerAngle, Space relativeTo = Space::WORLD);
		void AddRotation(const Vector3& _eulerAngle, Space relativeTo = Space::WORLD);
		// Scale의 경우 월드를 고려해주지 않습니다..
		void SetScale(const Vector3& _newLocalScale);

		// 각각의 Private 변수 게터.
		const Vector3& GetLocalPosition() { return localPosition; }
		const Vector3& GetLocalRotation() { return localRotation; }
		const Vector3& GetLocalScale() { return localScale; }

		const Vector3& GetWorldPosition() { return worldPosition; }
		const Vector3& GetWorldRotation() { return worldRotation; }
		const Vector3& GetWorldScale() { return worldScale; }


		void LookAt(const Vector3& targetPos);

		const Vector3& GetLook() { return look; }
		void SetLook(const Vector3& _look);
		const Vector3& GetUp() { return up; }
		void SetUp(const Vector3& _up);
		const Vector3& GetRight() { return right; }
		void SetRight(const Vector3& _right);

		const Matrix& GetLocalTM();
		const Matrix& GetWorldTM();

		void SetLocalTM(const Matrix& _localTM);
		void SetWorldTM(const Matrix& _worldTM);

	private:
		// 로컬 TM의 변경
		void UpdateLocalTM();
		// WorldTM의 변경
		void UpdateWorldTM();
		// needTM UPdate 변수 True로 변경
		void CheckingNeedTMUpdate();

		// Decompose
		void DecomposeMatrix(Vector3& scale, Vector3& rotation, Vector3& position, Matrix matrix);
	};
}