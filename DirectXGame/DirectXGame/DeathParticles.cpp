#include "DeathParticles.h"
#include <cassert>
#include <MathUtilityForText.h>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{
	
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// ワールド変換の初期化
	for(auto& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	viewProjection_ = viewProjection;
}

void DeathParticles::Update()
{
	if (isFinished_)
	{
		return;
	}

	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration)
	{
		counter_ = kDuration;

		isFinished_ = true;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		// 回転角を計算する
		float angle = kAngleUnit * i;
		// 軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		worldTransforms_[i].translation_ += velocity;
	}

	for (auto& worldTransform : worldTransforms_)
	{
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw()
{
	if (isFinished_)
	{
		return;
	}

	for (auto& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, *viewProjection_);
	}
}