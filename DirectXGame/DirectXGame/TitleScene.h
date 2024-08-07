#pragma once
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	void Initialize();
	void Update();
	void Drow();

	//getter
	bool isFinished() const { return finished_; }
private:
	bool finished_ = false;
};