#include "DeathParticles.h"
#include "MathUtilityForText.h"
#include <assert.h>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// ワールド変換の初期化
	for (auto& worldTransform : worldTransfroms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	viewProjection_ = viewProjection;
}

void DeathParticles::Update()
{
//	//カウンターを１フレーム分進める
//	counter_ += 1.0f / 60.0f;
//
//	if (counter_ >= kDutation)
//	{
//		counter_ = kDutation;
//		//終了扱いにする
//		idFinished_ = true;
//	}
//
//	if (idFinished_)
//	{
//		return;
//	}
//
//	for (uint32_t i = 0; i < kNumParticles; ++i)
//	{
//		//基本となる速度ベクトル
//		Vector3 velocity = {kSpeed, 0, 0};
//		//回転角を計算する
//		float angel = kAngelUnit * i;
//		//Z軸周りの回転行列
//		Matrix4x4 matrixRotation = MakeRotateZMatrix(angel);
//		//基本ベクトルを回転させて速度ベクトルを得る
//		velocity = Transform(velocity, matrixRotation);
//		//移動処理
//		worldTransfroms_[i].translation_ += velocity;
//	}
//
//	// 行列計算
//	for (auto& worldTransform : worldTransfroms_) {
//		worldTransform.UpdateMatrix();
//	}
}

void DeathParticles::Draw()
{
	if (idFinished_)
	{
		return;
	}

	for (auto& worldTransform : worldTransfroms_) {
		model_->Draw(worldTransform, *viewProjection_);
	}
}
