#include "Enemy.h"
#include "MathUtilityForText.h"
#include <cassert>
#include <numbers>
#include <math.h>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, Vector3 position)
{
	assert(model);

	model_  = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update()
{
	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f / 60.0f;

	float param = std::sin(std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngelStart - kWalkMotionAngelend * (param + 1.0f)/ 2.0f;
	float xLotation = conversionRandian(radian);

	worldTransform_.rotation_.x = xLotation;

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw()
{
	 model_->Draw(worldTransform_, *viewProjection_);
}

void Enemy::OnCollision(const Player* player)
{
	(void)player;
}

AABB Enemy::getAABB()
{
	Vector3 worldPos  = GetWorldPosition(worldTransform_.matWorld_);

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}