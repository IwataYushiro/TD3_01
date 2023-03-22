#include "Fan.h"

#include "Input.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include <cassert>

Fan* Fan::Create(Model* model) {
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Fan* instance = new Fan();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Fan::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	ray_ = new Ray();
	ray_->start_ = Object3d::GetPosition();
	ray_->dir_ = Object3d::GetRotation();

	//�R���C�_�|�ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(
		Vector3{ 0.0f,radius,0.0f },
		radius)
	);

	collider_->SetAttribute(COLLISION_ATTR_RAY);

	isControl_ = false;

	return true;
}

void Fan::Update() {
	assert(&ray_);

	Input* input_ = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�
	// ���݂̍��W���擾
	Vector3 move = Object3d::GetPosition();

	//�ړ��X�s�[�h
	float moveSpeed = 0.4f;

	// ���݂̉�]���擾
	Vector3 rot = Object3d::GetRotation();

	if (isControl_) {
		//��]�X�s�[�h(����)
		float verticalAngle = ConvertToRadian(90.0f);

		Vector3 angleX = { 1.0f,0.0f,0.0f };
		Vector3 angleY = { 0.0f,1.0f,0.0f };
		Vector3 angleZ = { 0.0f,0.0f,1.0f };

		Quaternion rotQua = { 0,0,0 };

		//�ړ���̍��W���v�Z
		if (input_->TriggerKey(DIK_W)) {
			//rotQua = DirectionToDirection(rot, angleY * verticalAngle);
			//rotQua = DirectionToDirection(rot, angleY * (verticalAngle * 2));
			//rotVector_ = RotateVector( angleY ,rotQua );

			rotVector_ = CreateRotationVector(angleY, verticalAngle * 2);

			rot = angleY * verticalAngle * 2;

			ray_->dir_ = angleZ;
		}

		else if (input_->TriggerKey(DIK_S)) {
			//rotQua = DirectionToDirection(rot, angleY * -verticalAngle);
			//rot = RotateVector( angleY ,rotQua );

			rotVector_ = CreateRotationVector(angleY, 0);

			rot = angleY * verticalAngle * 0;

			ray_->dir_ = -angleZ; //{ -verticalAngle ,0,0 };
		}

		if (input_->TriggerKey(DIK_A)) {
			//rotQua = DirectionToDirection(rot, angleY * verticalAngle);
			//rot = RotateVector( angleY ,rotQua );

			rotVector_ = CreateRotationVector(angleY, verticalAngle);

			rot = angleY * verticalAngle;

			ray_->dir_ = -angleX; //{ 0,verticalAngle,0 };
		}

		else if (input_->TriggerKey(DIK_D)) {
			//rotQua = DirectionToDirection(rot, angleY * -verticalAngle);
			//rot = RotateVector( angleY ,rotQua );

			rotVector_ = CreateRotationVector(angleY, -verticalAngle);

			rot = angleY * -verticalAngle;

			ray_->dir_ = angleX; //{ 0,-verticalAngle,0 };
		}

		if (input_->PressKey(DIK_UP)) {
			move.z += moveSpeed;
		}

		else if (input_->PressKey(DIK_DOWN)) {
			move.z -= moveSpeed;
		}

		if (input_->PressKey(DIK_RIGHT)) {
			move.x += moveSpeed;
		}

		else if (input_->PressKey(DIK_LEFT)) {
			move.x -= moveSpeed;
		}
	}
	//rot = rotVector_;

	// �ړ��̕ύX�𔽉f
	Object3d::SetPosition(move);

	// ��]�̕ύX�𔽉f
	Object3d::SetRotation(rot);

	ray_->start_ = Object3d::GetPosition();

	camera_->Update();
	Object3d::Update();
}

void Fan::Draw() {
	Object3d::Draw();
}

void Fan::Finalize() {
	delete ray_;
}

void Fan::OnCollision(const CollisionInfo& info) {
}