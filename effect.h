#pragma once

//******* エフェクト *******//
//敵を倒したときに出るドロップ
const int kDropEnergyMax = 50;//ドロップの最大数
const int kDropEnergySize = 8;//ドロップのサイズ
const int dropSpeed = 5;//落ちるスピード

struct DropEnergy {
	Vector2 pos;//位置
	int isHit;//ゲージに当たったか
	int isDrop = 0;//ドロップしているか
};
DropEnergy dropEnergy[kDropEnergyMax];
DropEnergy dropEnergyDefault;


//アイスドロップ
struct IceDrop {
	Vector2 pos;//位置
	int isHit;//ゲージに当たったか
	int isDrop = 0;//ドロップしているか
};
IceDrop iceDrop[kDropEnergyMax];
IceDrop iceDropDefault;

//******* パーティクル *******//
const int kParticleMax = 200;
const float gravity = 9.8f;


struct Translte {
	float x;
	float y;
};

//パーティクル物理用タイマー
float particleTimer[kParticleMax];

struct Particle {
	Translte pos;//位置
	int isDo = 0;//作動しているか
	float direction = 0;//飛ぶ向き
	int ViewTime = 60;
};
Particle particle[kParticleMax];
Particle particleDefault;

struct BubbleParticle {
	int X = 0;
	int Y = kScreenDown + 50;
	int IsDo = 0;
	int Speed = 0;
};
BubbleParticle bubble[50];
BubbleParticle bubbleDefault;
