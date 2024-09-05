#pragma once
#include "Vector2.h"

//******* 規定値 *******//	
const int kPlayerSpeed = 5;//プレイヤーの速さ規定値
const int kPlayerSize = 32;//プレイヤーの画像サイズ規定値
const int kPlayerHitBoxSize = 12;//プレイヤーの当たり判定サイズ規定値

const int kPlayerBulletCoolTime = 10;//プレイヤー弾のクールタイム規定値
const int kPlayerBulletMax = 200;//プレイヤーの弾の最大数規定値
const int kPlayerBulletSpeed = 8;//プレイヤーの弾の速さ規定値
const int kPlayerBulletSize = 8;//プレイヤーの弾の大きさ規定値

//******* プレイヤー *******//	
struct Player {
	Vector2 pos;//位置
	int r = kPlayerHitBoxSize;//円の当たり判定の半径
	int width = 32;//幅
	int height = 54;//高さ
	int speed;//速さ
	int life = 5;//残機
	int animePage = 7;//アニメーションの枚数
	int isShot = 0;//弾を撃っているか
	int isSkill = 0;//スキルを撃っているか
	int animeTimer = 0;//アニメーションタイマー
	int skillTimer = 0;//スキルを押した長さ
	int lifeTimer = 0;//敵の弾が当たった時の復帰タイマー
	int CoolTime = kPlayerBulletCoolTime;//プレイヤーの弾のクールタイム
	int isHit = 0;//敵に当たったか

};
Player player;
Player playerDefault;


struct PlayerHit {
	Vector2 pos;
};
PlayerHit playerHit;

//******* 弾 *******//	
struct PlayerBullet {
	Vector2 pos;//位置
	Vector2 e2b;//敵とプレイヤーの弾の距離
	int e2br = 0;//敵とプレイヤーの弾の半径の合計
	int isHit = 0;//弾が当たったか
	int IsShot = 0;//弾が発射されているか

	//↓弾もアニメーションするなら 
	//int animeTimer;//アニメーションタイマー
	//int animePage;//アニメーションの枚数
};
PlayerBullet playerBullet[kPlayerBulletMax];
PlayerBullet playerBulletDefault;

//******* 必殺技 *******//	
//大必殺技
struct BigSkill {
	Vector2 pos;//位置
	int width = 600;//幅
	int height = -600;//高さ
	int animeTimer = 0;//生きてるときのアニメーションタイマー
	int animePage = 3;//アニメーションの枚数
	int timer = 0;//タイマー
	int isShot = 0;//打っているか
	int isSkill = 0;//スキルを撃っているか
};
BigSkill bigSkill;
BigSkill bigSkillDefault;

//小必殺技
struct SmallSkill {
	Vector2 pos;//位置
	int width = 300;//幅
	int height = -600;//高さ
	int animeTimer = 0;//生きてるときのアニメーションタイマー
	int animePage = 3;//アニメーションの枚数
	int timer = 0;//タイマー
	int isShot = 0;//打っているか
	int isSkill = 0;//スキルを撃っているか
};
SmallSkill smallSkill;
SmallSkill smallSkillDefault;