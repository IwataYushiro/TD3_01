#include "Collision.h"
#include "MyMath.h"

const float Collision::EPSILON_ = 1.0e-5f;

bool Collision::CheckSphere2Sphere(const Sphere& sphereA,
	const Sphere& sphereB, Vector3* inter) {

	float distance =
		(sphereB.center_.x - sphereA.center_.x) * (sphereB.center_.x - sphereA.center_.x)
		+ (sphereB.center_.y - sphereA.center_.y) * (sphereB.center_.y - sphereA.center_.y)
		+ (sphereB.center_.z - sphereA.center_.z) * (sphereB.center_.z - sphereA.center_.z);

	float radian =
		(sphereA.radius_ + sphereB.radius_) * (sphereA.radius_ + sphereB.radius_);

	if (distance <= radian) {
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere,
	const Plane& plane, Vector3* inter) {
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = Vector3Dot(sphere.center_, plane.normal_);

	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - plane.distance_;

	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (AbsoluteValue(dist) > sphere.radius_) return false;

	//�^����_���v�Z
	if (inter) {
		//���ʏ�̍ŋߐړ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal_ + sphere.center_;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest) {
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	Vector3 p0_p2 = triangle.p2_ - triangle.p0_;
	Vector3 p0_pt = point - triangle.p0_;

	float d1 = Vector3Dot(p0_p1, p0_pt);
	float d2 = Vector3Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f) {
		// p0���ŋߖT
		*closest = triangle.p0_;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p1_pt = point - triangle.p1_;

	float d3 = Vector3Dot(p0_p1, p1_pt);
	float d4 = Vector3Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3) {
		// p1���ŋߖT
		*closest = triangle.p1_;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		float v = d1 / (d1 - d3);
		*closest = triangle.p0_ + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.p2_;

	float d5 = Vector3Dot(p0_p1, p2_pt);
	float d6 = Vector3Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6) {
		*closest = triangle.p2_;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		float w = d2 / (d2 - d6);
		*closest = triangle.p0_ + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1_ + w * (triangle.p2_ - triangle.p1_);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0_ + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter) {
	Vector3 p;

	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center_, triangle, &p);

	//�_p�Ƌ��̒��S�̍����x�N�g��
	Vector3 v = p - sphere.center_;

	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	float vDot = Vector3Dot(v, v);

	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (vDot > sphere.radius_ * sphere.radius_) return false;

	//�^����_���v�Z
	if (inter) {
		//�O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}

	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter) {

	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = Vector3Dot(plane.normal_, ray.dir_);

	//���ʂɂ͓�����Ȃ�
	if (d1 > -EPSILON_) { return false; }

	//���_�ƌ��_�̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W�̓���
	float d2 = Vector3Dot(plane.normal_, ray.start_);

	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance_;

	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;

	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)return false;

	//��������������
	if (distance) { *distance = t; }

	//��_���v�Z
	if (inter) { *inter = ray.start_ + t * ray.dir_; }

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter) {
	//�O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	Vector3 interPlace;
	plane.normal_ = triangle.normal_;
	plane.distance_ = Vector3Dot(triangle.normal_, triangle.p0_);

	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &interPlace)) { return false; }

	//���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩�𔻒�
	Vector3 m;

	//��p0_p1�ɂ���
	Vector3 pt_p0 = triangle.p0_ - interPlace;
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	m = Vector3Cross(pt_p0, p0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (Vector3Dot(m, triangle.normal_) < -EPSILON_) { return false; }

	//��p1_p2�ɂ���
	Vector3 pt_p1 = triangle.p1_ - interPlace;
	Vector3 p1_p2 = triangle.p2_ - triangle.p1_;
	m = Vector3Cross(pt_p1, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (Vector3Dot(m, triangle.normal_) < -EPSILON_) { return false; }

	//��p2_p0�ɂ���
	Vector3 pt_p2 = triangle.p2_ - interPlace;
	Vector3 p2_p0 = triangle.p0_ - triangle.p2_;
	m = Vector3Cross(pt_p2, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (Vector3Dot(m, triangle.normal_) < -EPSILON_) { return false; }

	//�����Ȃ̂œ������Ă���
	if (inter) {
		*inter = interPlace;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter) {
	Vector3 m = ray.start_ - sphere.center_;
	float b = Vector3Dot(m, ray.dir_);
	float c = Vector3Dot(m, m) - sphere.radius_ * sphere.radius_;

	//ray�̎n�_��sphere�̊O���ɂ���(c > 0)�Aray��sphere���痣��Ă����������w���Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	//���̔��ʎ��͋����O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) { return false; }

	//���C�͋��ƌ������Ă���
	//��������ŏ��̒l���v�Z
	float t = -b - SquareRoot(discr);

	//t�����ł���ꍇ�A���C�����̓�������J�n���Ă���̂ŁAt���[���ɃN�����v
	if (t < 0)t = 0.0f;
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start_ + t * ray.dir_; }

	return true;
}