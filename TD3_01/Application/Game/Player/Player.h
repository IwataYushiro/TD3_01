#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Player{
public://メンバ関数
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private://メンバ変数
	Object3d* planeObj_ = nullptr;
	Model* planeModel_ = nullptr;
	Camera* camera_player = nullptr;
};