#include "DeathParticles.h"
#include <cassert>

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
	for (auto& worldTransform : worldTransforms_)
	{
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw()
{
	for (auto& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, *viewProjection_);
	}
}