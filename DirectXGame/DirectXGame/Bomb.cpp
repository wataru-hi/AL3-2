#include "Bomb.h"

#include <numbers>
#include "MapChipField.h"
#include "ImGuiManager.h"

void Bomb::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{
	model_= model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
}

void Bomb::Update()
{
	

	if (bombTimer > 0.0f)
	{
		bombTimer -= 1.0f / 60.0f;
		if (bombTimer < 0.016f)
		{
			exPos = worldTransform_.translation_;
			bombCheck = true;
		}
	}
	else if (bombTimer < 0.0f)
	{
		bombTimer = 0.0f;
		bombCheck = false;
	}

	ImGui::Begin("Bomb");
	ImGui::Text("%f", bombTimer);
	ImGui::Text("%f,%f,&f", worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z);
	ImGui::End();

}

void Bomb::Draw()
{
	if(bombTimer > 0.0f) {model_->Draw(worldTransform_, *viewProjection_);}
}

