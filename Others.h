#pragma once
//******* 画面範囲 *******//	
const int screenRangeX = 600;//プレイ画面の大きさX
const int screenRangeY = 730;//プレイ画面の大きさY
const int kScreenLeft = 340;//プレイ画面の座標左
const int kScreenRight = 340 + screenRangeX;//プレイ画面の座標右
const int kScreenTop = 20;//プレイ画面の座標上
const int kScreenDown = 20 + screenRangeY;//プレイ画面の座標下


//******* 背景 *******//
int gsPosX[2] = {kScreenLeft,kScreenLeft};
int gsPosY[2] = { 0,770 };

//******* ステージ *******//	
int Stage = 0;//現在のステージ
int Wave = 0;//現在のウェーブ
int isNextWave = 1;//次のウェーブに行ったか
int StageTime = 0;//現在のステージの経過時間
int TyutolialTime = 0;

//******* シーン切り替え *******//	
int SceneChange = 0;
int SelectScene = 1;

//******* スコアの合計 *******//	
int ToatalScore = 0;

//******* アニメーション *******//	
const int kEnemyAnimeSpeed = 8;//アニメーションする速さ（仮）
const int kExplosionSpeed = 10;
const int kPlayerAnimeSpeed = 7;
const int kSkillSpeed = 6;//必殺技の最初3枚の速さ
const int kSkillLastSpeed = 42;//必殺技の最後の絵の速さ
//敵が死んだときのアニメーション
const int kEnemyDeathAnimeSpeed = 5;//アニメーションする速さ（仮）


//******* ブロック *******//	
int kBlockMeterMax = 400;//ブロック量の上限
float meterUpSpeed = 0.5f;//メーターの増減のスピード

struct Block {
	Vector2 pos;//位置
	int width = 600;//幅
	int height = 600;//高さ
	int animeTimer = 0;//生きてるときのアニメーションタイマー
	int animePage = 4;//アニメーションの枚数
	int meter = 0;//ブロックの量
	int ifMeter = 0;//ブロックの増加予想
};

Block block;
Block blockDefault;

int IsPlayBGM = 0;
//音関係の変数
int EnemyDefeatSEFlag = 0;
int PlayerShotSEFlag = 0;
int DropSEFlag = 0;
int EnemyShotSEFlag = 0;
int DecisionSEFlag = 0;
int SelectSEFlag = 0;
int SmallSkillSEFlag = 0;
int BigSkillSEFlag = 0;