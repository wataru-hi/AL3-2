#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "AABB.h"

class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	void Initialize(Model* model, ViewProjection* viewProjection, Vector3 position);
	void Update();
	void Draw();

	void OnCollision(const Player* player);

	AABB getAABB();

private:
	static inline const float kWalkSpeed = 0.01f; //歩行の速さ
	static inline const float kWalkMotionAngelStart = 25.0f; //最初の角度[度]
	static inline const float kWalkMotionAngelend = 35.0f; //最後の角度[度]
	static inline const float kWalkMotionTime = 1.0f; //アニメーション周期時間[秒]
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};

	float walkTimer_ = 0.0f;
};