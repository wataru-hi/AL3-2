#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Bomb.h"

#include "AABB.h"

class MapChipField;
class Enemy;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	// setter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void OnCollision(const Enemy* enemy);

	// getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	const Vector3& GetPosition() const { return worldTransform_.translation_; }
	AABB getAABB();
	bool IsDead() const { return isDead_; }
	bool IsEx() const { return bombCheck; }
	Vector3 GetExPos() { return exPos; }

private:
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kAttenuationWall = 0.2f;
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kLimitRunSpeed = 0.5f;
	static inline const float kTimeTurn = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;
	static inline const float kGroundSearchHeight = 0.06f;
	static inline const float BombCooltime = 3.0f;

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
	bool onGround_ = true;
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	MapChipField* mapChipField_ = nullptr;
	bool isDead_ = false;
	float bombCoolTimer_ = 0.0f;
	Bomb* bomb_ = nullptr;
	Vector3 exPos = {};
	bool bombCheck = false;

	void InputMove();
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void UpdateOnGround(const CollisionMapInfo& info);
	void AnimateTurn();
	void BombPut();

	Vector3 CornerPosition(const Vector3& center, Corner corner);
};
