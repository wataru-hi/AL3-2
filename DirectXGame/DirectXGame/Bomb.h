#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Bomb
{
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	//setter
	void setBomb(Vector3 PlayerPos) 
	{
		worldTransform_.translation_ = PlayerPos;
		bombTimer = bombTime;
	}

	Vector3 GetExPos()
	{
		return exPos;
	}

	bool isEx()
	{
		return bombCheck;
	}

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	const float bombTime = 1.0f;

	float  bombTimer = 0.0f;
	bool bombCheck = false;

	Vector3 exPos;

};

