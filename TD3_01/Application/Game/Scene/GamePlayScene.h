#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"
#include "Light.h"

#include <memory>
#include <List>

#include "Player.h"
#include "Skydome.h"

class GamePlayScene : public BaseScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	void Initialize3d();
	void Initialize2d();

	void Update3d();
	void Update2d();

	void Draw3d();
	void Draw2d();

private:
	//基盤
	static DirectXBasis* dxBas_;
	static Input* input_;
	static DrawBasis* drawBas_;

	Camera* camera_ = nullptr;
	Light* light_ = nullptr;

	/// <summary>
	/// オブジェクト
	/// </summary>
	
	Player* player_;
	Skydome* skydome_;

	/// <summary>
	/// スプライト
	/// </summary>
	Sprite* sprite_ = new Sprite();
};