#pragma once

#include "Vector3.h"
#include <cstdint>
#include <string>
#include <vector>

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	Switch, //スイッチ
	Door,	//ドア
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	void MapChipdelete(const Vector3 deletePosition);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetMapChipTypeByPosition(const Vector3& position);
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};
