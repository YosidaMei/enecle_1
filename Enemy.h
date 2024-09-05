#pragma once

#pragma region 敵
//******* 規定値 *******//	
//通常
const int kEnemySpeed = 4;//敵の速さ規定値
const int kEnemySize = 32;//敵のサイズ規定値
const int kNormalEnemyMax = 100;//通常の敵の仮の数
const int kEnemyBulletCoolTime = 400;//敵の弾のクールタイム規定値
const int kExplosionEnemyMax = 5;//爆発する敵の数
const int kEnemyDeathSize = 56;//敵が死んだアニメーションの大きさ
const int kEnemyDeathTimer = 45;//敵が死んでいる時間
const int kEnemeyDeathMax = 100;//敵が死んでいる数

//******* 爆発 *******//	
struct Explosion {
	Vector2 pos;
	int width = 128;//幅
	int height = 128;//高さ
	int timer = 70;//爆発している時間
	int coolTimer = 0;
	int isDo = 0;//爆発したか	
};
Explosion explosion[kExplosionEnemyMax];
Explosion explosionDefault;

//******* ブロックをもっていく *******//	
struct Take {
	int isHave = 0;//ブロックを持っているか
};
Take takeDefault;

//ゲージ凍らせる
int isFrozen = 0;
int frozenTimer = 0;


//******* ボス *******//	

//******* 規定値 *******//	
//ボス
const int kBossSpeed = 4;//ボスのスピードの規定値
const int kBossSize = 88;//ボスのサイズの規定値
const int kMiniBossSize = 56;
const int kBossBulletCoolTime = 300;//ボス弾のクールタイム規定値
const int kBossShieldTimer = 600;//ボスがシールドを貼っていられる時間
const int kBossShieldCoolTimer = 600;//ボスが次にシールドを貼るまでの時間
const int kBossSkillChargeTimer = 420;//チャージする時間
const int kBossSkillTimer = 200;//ボスが溜め技を打っていられる時間
const int kBossSkillCoolTimer = 600;//ボスが次に溜め技をするまでの時間

//シールド
struct BossShield {
	Vector2 pos;
	int r = 64;
	int width = 128;//幅
	int height = 128;//高さ
	int timer = kBossShieldTimer;//タイマーを貼っていられる時間
	int animeTimer = 0;//生きてるときのアニメーションタイマー
	int animePage = 7;//アニメーションの枚数
	int coolTime = kBossShieldCoolTimer;
	int isDo = 0;//シールドを貼っているか
};
BossShield bossShieldDefault;

//溜め技
struct BossSkill {
	Vector2 pos;//位置
	Vector2 p2b;//プレイヤーとボスの溜め技との距離
	int p2br;//プレイヤーとボスの溜め技との半径の合計
	int width = 400;//幅
	int height = 400;//高さ
	int timer = 0;//タイマー
	int animeTimer = 0;//生きてるときのアニメーションタイマー
	int animePage = 0;//アニメーションの枚数
	int coolTime = kBossSkillCoolTimer;
	int isReady = 0;//クールタイムが明けてチャージし始められるか
	int isShot = 0;//打っているか
};
BossSkill bossSkillDefault;

//ボス
struct BOSS{
	int isCharge;//チャージしているか
	int CoolTime = kBossBulletCoolTime;//クールタイム
	int chargeTime = kBossSkillChargeTimer;//ため攻撃までの時間
	BossShield shield;//シールド
	BossSkill skill;//スキル
};

BOSS bossDefault;

//******* 敵全体の構造体 *******//	
struct Enemy {
	Vector2 pos;//位置
	Take take;
	BOSS boss;
	int r = 0;//大きさ
	int Width = 0;//幅
	int Height = 0;//高さ
	int speed = 0;//スピード
	int life = 0;//体力
	int score = 0;//スコア
	int animeTimer = 0;//生きてるときのアニメーションタイマー
	int animePage = 0;//アニメーションの枚数
	int isAlive = 0;//生きているか
	int isShot = 0;//打っているか
	int CoolTime = kEnemyBulletCoolTime;//敵の弾のクールタイム
	int bulletType = 0;//撃つ弾の種類
	int MoveType = 0;//移動の種類
	int AliveTime = 0;//生きている時間
	int Type = 0;//敵の種類
	int MoveControl[100] = {};//移動制御用
	int MoveControlPosX = 0;//移動制御X記録用
	int MoveControlPosY = 0;//移動制御Y記録用
	int InfinityTime = 0;//無敵時間
	int BulletAngle[5] = {};
	int IsHit = 0;//弾に当たっているか
};

Enemy enemy[kNormalEnemyMax], *enemy_p[kNormalEnemyMax];
Enemy enemyDefault;


struct EnemyDeath {
	Vector2 pos;
	int deathTimer = kEnemyDeathTimer;//敵が死んで描写していられるタイマー
	int deathAnimeTimer = 0;//死んだときのアニメーションタイマー
	int deathAnimePage = 8;
	int isDeath = 0;

};
EnemyDeath enemyDeath[kEnemeyDeathMax];
EnemyDeath enemyDeathDefault;
//******* ブロック量 *******//	
int dropBlock = 10;
int bossDropBlock = 5;

#pragma endregion

#pragma region 敵の弾

//******* 規定値 *******//	
const int kEnemyBulletMax = 100;//弾の数
const int kEnemyBulletSpeed = 4;//弾のスピードの規定値
const int kEnemyBulletSize = 8;//弾のサイズの規定値


//******* 構造体 *******//	
struct EnemyBullet {
	Vector2 pos;//位置
	Vector2 p2b;//プレイヤーと敵の弾の距離
	int p2br;//プレイヤーと敵の弾の半径の合計
	int isHit = 0;//弾が当たったか
	int IsShot = 0;//弾が発射されているか
	float angle = 0;//弾の角度
	//↓弾もアニメーションするなら 
	//int animeTimer;//アニメーションタイマー
	//int animePage;//アニメーションの枚数
};


EnemyBullet enemyBullet[kEnemyBulletMax];
EnemyBullet enemyBulletDefault;
#pragma endregion

//******* 敵配置関数 *******//	
void EnemyAppearance(int PosX, int PosY, int Width, int Height, int Speed, int Life, int Score, int AnimePage, int BulletType, int MoveType, int Type) {
	for (int i = 0; i < kNormalEnemyMax; i++) {
		if (!enemy_p[i]->isAlive) {
			enemy_p[i]->pos.x = PosX;
			enemy_p[i]->pos.y = PosY;
			enemy_p[i]->Width = Width;//大きさ
			enemy_p[i]->Height = Height;//大きさ
			enemy_p[i]->speed = Speed;//スピード
			enemy_p[i]->life = Life;//体力
			enemy_p[i]->score = Score;//スコア
			enemy_p[i]->animePage = AnimePage;//アニメーションの枚数
			enemy_p[i]->isAlive = 1;//生きているか
			enemy_p[i]->bulletType = BulletType;//撃つ弾の種類
			enemy_p[i]->MoveType = MoveType;//移動の種類
			enemy_p[i]->Type = Type;//敵の種類
			break;
		}
	}
}