#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <conio.h>
#include "Player.h"
#include "Enemy.h"
#include "Others.h"
#include "effect.h"
#include "Fanction.h"

const char kWindowTitle[] = "1108_エネクル";

//ゲームシーン
enum GameScenes {
	TITLE,//0
	TUTORIAL,//1
	PLAY,//2
	GAMEOVER,//3
	GAMECLEAR,//4
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 770);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//乱数の初期化
	srand(time(nullptr));

#pragma region 変数の宣言

#pragma region プレイヤー
	//******* プレイヤー *******//
	player.pos.x = 640;//位置の初期化X
	player.pos.y = 400;//位置の初期化Y

	playerDefault.pos.x = 640;
	playerDefault.pos.y = 400;

	playerHit.pos.x = -6000;
	playerHit.pos.y = -6000;

	int Player = Novice::LoadTexture("./Resources/player_gam.png");
	int PlayerHp = Novice::LoadTexture("./Resources/player_HP.png");
	int PlayerCharge = Novice::LoadTexture("./Resources/player_gam_charge.png");

	//******* プレイヤーの弾 *******//
	for (int i = 0; i < kPlayerBulletMax; i++) {
		//位置
		playerBullet[i].pos.x = 40;
		playerBullet[i].pos.y = 40;
		//プレイヤーの弾と敵の距離
		playerBullet[i].e2b.x = 0;
		playerBullet[i].e2b.y = 0;
	}
	int PlayerBullet = Novice::LoadTexture("./Resources/player_bullet_one.png");

	//初期値の設定
	playerBulletDefault.pos.x = 40;
	playerBulletDefault.pos.y = 40;
	playerBulletDefault.e2b.x = 0;
	playerBulletDefault.e2b.y = 0;

	//******* 必殺技 *******//
	//大必殺技
	bigSkill.pos.x = -1000;//位置
	bigSkill.pos.y = -1000;
	int BigSkill = Novice::LoadTexture("./Resources/player_bigSkill.png");

	//小必殺技
	smallSkill.pos.x = -1000;//位置
	smallSkill.pos.y = -1000;
	int SmallSkill = Novice::LoadTexture("./Resources/player_smallSkill.png");

#pragma endregion

#pragma region 敵
	//******* 敵 *******//	
		//位置の初期化
	for (int i = 0; i < kNormalEnemyMax; i++) {
		enemy[i].pos.x = -100;
		enemy[i].pos.y = -100;
		//******* ボスのシールド *******//	
		enemy[i].boss.shield.pos.x = -500;
		enemy[i].boss.shield.pos.y = -500;

		//******* ボスの溜め技 *******//	
		enemy[i].boss.skill.pos.x = -1000;//位置
		enemy[i].boss.skill.pos.y = -1000;

	}
	//死んだときの位置
	for (int i = 0; i < kEnemeyDeathMax; i++) {
		enemyDeath[i].pos.x = -200;
		enemyDeath[i].pos.y = -200;
	}
	//画像の読み込み
	int NormalEnemy = Novice::LoadTexture("./Resources/enemy_nomal.png");
	int TakeEnemy = Novice::LoadTexture("./Resources/enemy_take.png");
	int TakeHaveEnemy = Novice::LoadTexture("./Resources/enemy_take-have.png");
	int IceEnemy = Novice::LoadTexture("./Resources/enemy_ice.png");
	int ExplosionEnemy = Novice::LoadTexture("./Resources/enemy_explosion.png");
	int BossHp = Novice::LoadTexture("./Resources/boss_HP.png");
	int MiniBoss = Novice::LoadTexture("./Resources/enemy_mBoss.png");
	int Boss = Novice::LoadTexture("./Resources/enemy_boss.png");
	int BossShield = Novice::LoadTexture("./Resources/enemy_shield.png");
	int DeathEnemy = Novice::LoadTexture("./Resources/effect_explosion.png");

	enemyDefault.pos.x = -100;
	enemyDefault.pos.y = -100;

	//爆発する敵
	for (int i = 0; i < kExplosionEnemyMax; i++) {
		explosion[i].pos.x = -1000;
		explosion[i].pos.y = -1000;
	}

	//ポインタの初期化
	for (int i = 0; i < kNormalEnemyMax; i++) {
		enemy_p[i] = &enemy[i];
	}

	//******* 敵の弾 *******//
	//位置の初期化
	for (int i = 0; i < kEnemyBulletMax; i++) {
		enemyBullet[i].pos.x = 0;
		enemyBullet[i].pos.y = 0;
		enemyBullet[i].p2b.x = 0;
		enemyBullet[i].p2b.y = 0;

	}
	int EnemyBullet = Novice::LoadTexture("./Resources/enemy_bullet.png");
#pragma endregion

#pragma region その他

	//******* 音楽 *******//
	int TitleBgm = Novice::LoadAudio("./Resources/sounds/Title.mp3");//タイトル
	int PlayBgm = Novice::LoadAudio("./Resources/sounds/Play.mp3");//プレイ
	int GameOverBgm = Novice::LoadAudio("./Resources/sounds/Game_Over.mp3");//ゲームオーバー
	int GameClearBgm = Novice::LoadAudio("./Resources/sounds/Game_Clear.mp3");//ゲームクリア
	int TitleBgm_B = -1;
	int PlayBgm_B = -1;
	int GameOverBgm_B = -1;
	int GameClearBgm_B = -1;
	int EnemyDefeatSE = Novice::LoadAudio("./Resources/sounds/18.mp3");//敵撃破時
	int PlayerShotSE = Novice::LoadAudio("./Resources/sounds/piko.mp3");//プレイヤーのショット
	int DropSE = Novice::LoadAudio("./Resources/sounds/01.mp3");//ドロップ
	int EnemyShotSE = Novice::LoadAudio("./Resources/sounds/atack04.wav");//敵のショット
	int DecisionSE = Novice::LoadAudio("./Resources/sounds/24.mp3");//決定
	int SelectSE = Novice::LoadAudio("./Resources/sounds/05.mp3");//選択
	int SmallSkillSE = Novice::LoadAudio("./Resources/sounds/bimu.mp3");//選択
	int BigSkillSE = Novice::LoadAudio("./Resources/sounds/bimuhou.mp3");//選択

	//全体の枠
	int Frame = Novice::LoadTexture("./Resources/frame_game.png");
	int Keys = Novice::LoadTexture("./Resources/keys.png");
	int Space = Novice::LoadTexture("./Resources/key_space.png");
	int ShotUi = Novice::LoadTexture("./Resources/shot_ui.png");

	//******* ブロック *******//
	//位置
	block.pos.x = 340;
	block.pos.y = 630;
	//幅
	block.width = 600;
	//高さ
	block.height = 600;
	//ブロック増加予測
	block.ifMeter = screenRangeY - block.pos.y;
	//ブロックの量
	block.meter = 0;

	int Gauge = Novice::LoadTexture("./Resources/player_gage.png");//通常ゲージ
	int CollectGauge = Novice::LoadTexture("./Resources/player_gage_big.png");//大必殺技がたまったゲージ
	int MaxGauge = Novice::LoadTexture("./Resources/player_gage_max.png");//ゲージがマックス
	int IceGauge = Novice::LoadTexture("./Resources/player_gage_ice.png");//ゲージが凍る

	//******* ドロップエネルギー *******//
	//位置
	for (int i = 0; i < kDropEnergyMax; i++) {
		//位置
		dropEnergy[i].pos.x = -100;
		dropEnergy[i].pos.y = -100;
		//ゲージに当たったか
		dropEnergy[i].isHit = 0;
		//ドロップしているか
		dropEnergy[i].isDrop = 0;

	}

	int Drop = Novice::LoadTexture("./Resources/enemy_drop.png");
	int IceDrop = Novice::LoadTexture("./Resources/enemy_drop_ice.png");

	//******* アイスドロップ *******//
	//位置
	for (int i = 0; i < kDropEnergyMax; i++) {
		//位置
		iceDrop[i].pos.x = -200;
		iceDrop[i].pos.y = -200;
	}

	//パーティクル物理用のタイマー
	for (int i = 0; i < kParticleMax; i++) {
		particleTimer[i] = 0.0f;
	}
	int Particle = Novice::LoadTexture("./Resources/particle.png");
	int Bable = Novice::LoadTexture("./Resources/bubble.png");

	/*particleDefault.pos.x = -100;
	particleDefault.pos.y = -100;*/
	//******* アイスドロップ *******//
	// 背景
	int TitleName = Novice::LoadTexture("./Resources/title.png");
	int TitleUi = Novice::LoadTexture("./Resources/title_UI.png");
	int PlayBack = Novice::LoadTexture("./Resources/back.png");
	int GameOver = Novice::LoadTexture("./Resources/back-gameOver.png");
	int GameOverUi = Novice::LoadTexture("./Resources/gameOver_UI.png");
	int GameOverUis = Novice::LoadTexture("./Resources/gameOver_UIs.png");
	int GameClear = Novice::LoadTexture("./Resources/back-gameClear.png");
	int GameClearUi = Novice::LoadTexture("./Resources/gameClear.png");
	int GameClearUis = Novice::LoadTexture("./Resources/gameClear_UI1.png");
	int WaveUi = Novice::LoadTexture("./Resources/wave_UI.png");
	//スコア
	int Number = Novice::LoadTexture("./Resources/score_numbers.png");
	int ScoreUi = Novice::LoadTexture("./Resources/score_UI.png");

	//チュートリアルの画面
	int TutorialUi = Novice::LoadTexture("./Resources/tutorial_UI1.png");
	int TutorialEtc1 = Novice::LoadTexture("./Resources/tutorial_UI2.png");
	int TutorialEtc2 = Novice::LoadTexture("./Resources/tutorial_UI3.png");
	int TutorialEtc3 = Novice::LoadTexture("./Resources/tutorial_UI4.png");
	int TutorialEtc4 = Novice::LoadTexture("./Resources/tutorial_UI5.png");

	//******* ゲームシーン *******//
	//最初のゲームシーン
	GameScenes gameScene = TITLE;
#pragma endregion

#pragma endregion

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//音関係の変数初期化
		EnemyDefeatSEFlag = 0;
		PlayerShotSEFlag = 0;
		DropSEFlag = 0;
		EnemyShotSEFlag = 0;
		DecisionSEFlag = 0;
		SelectSEFlag = 0;
		SmallSkillSEFlag = 0;
		BigSkillSEFlag = 0;

		switch (gameScene) {
		case TITLE:

			//画像のスクロール
			gsPosY[0] -= 1;
			gsPosY[1] -= 1;
			if (gsPosY[0] <= -770) {

				gsPosY[0] = 770;

			}

			if (gsPosY[1] <= -770) {

				gsPosY[1] = 770;

			}

			if (!IsPlayBGM) {
				if (TitleBgm_B == -1 || Novice::IsPlayingAudio(TitleBgm_B) == 0) {
					TitleBgm_B = Novice::PlayAudio(TitleBgm, 1, 0.2);
				}
				IsPlayBGM = 1;
			}

			//セレクトシーンを1から2の間で選択する
			if (SelectScene > 1) {
				if (keys[DIK_W] != 0 && preKeys[DIK_W] == 0 ||
					keys[DIK_UP] != 0 && preKeys[DIK_UP] == 0) {
					SelectScene -= 1;
					SelectSEFlag = 1;
				}
			}
			if (SelectScene < 2) {
				if (keys[DIK_S] != 0 && preKeys[DIK_S] == 0 ||
					keys[DIK_DOWN] != 0 && preKeys[DIK_DOWN] == 0) {
					SelectScene += 1;
					SelectSEFlag = 1;
				}
			}
			//スペースキーを押したらゲームシーンを切り替える
			if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) {
				if (SelectScene == 1) {
					Novice::StopAudio(TitleBgm_B);
					Novice::StopAudio(PlayBgm_B);
					Novice::StopAudio(GameOverBgm_B);
					Novice::StopAudio(GameClearBgm_B);
					IsPlayBGM = 0;
					gameScene = TUTORIAL;
					//背景
					SelectScene = 0;
					SceneChange = 1;
					gsPosX[0] = { kScreenLeft };
					gsPosX[1] = { kScreenLeft };
					gsPosY[0] = { 0 };
					gsPosY[1] = { 770 };
				}
				else if (SelectScene == 2) {
					Novice::StopAudio(TitleBgm_B);
					Novice::StopAudio(PlayBgm_B);
					Novice::StopAudio(GameOverBgm_B);
					Novice::StopAudio(GameClearBgm_B);
					IsPlayBGM = 0;
					gameScene = PLAY;
					player.pos.x = 640;//位置の初期化X
					player.pos.y = 400;//位置の初期化Y
					SelectScene = 0;
					SceneChange = 1;
					gsPosX[0] = { kScreenLeft };
					gsPosX[1] = { kScreenLeft };
					gsPosY[0] = { 0 };
					gsPosY[1] = { 770 };
				}
				DecisionSEFlag = 1;
			}
			break;

		case TUTORIAL:
			//スペースキーを押したらゲームシーンを切り替える
		//チュートリアル用の初期化
			if (!IsPlayBGM) {
				if (PlayBgm_B == -1 || Novice::IsPlayingAudio(PlayBgm_B) == 0) {
					PlayBgm_B = Novice::PlayAudio(PlayBgm, 1, 0.2);
				}

				IsPlayBGM = 1;
			}

			player.pos.x = 740;//位置の初期化X
			player.pos.y = 400;//位置の初期化Y

			if (!Novice::CheckHitKey(DIK_SPACE)) {
				SceneChange = 0;

			}

			//画像のスクロール
			gsPosY[0] -= 1;
			gsPosY[1] -= 1;
			if (gsPosY[0] <= -770) {

				gsPosY[0] = 770;

			}

			if (gsPosY[1] <= -770) {

				gsPosY[1] = 770;

			}
			//アニメーション関連
			//タイマー系の加算
			//プレイヤー
			player.animeTimer++;
			if (smallSkill.isShot) {
				smallSkill.animeTimer++;
			}
			if (bigSkill.isShot) {
				bigSkill.animeTimer++;
			}
			//敵
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].isAlive) {
					//敵が生きているなら生きている時間を加算する
					enemy[i].AliveTime++;
					enemy[i].CoolTime++;
					enemy[i].animeTimer++;

					if (enemy[i].InfinityTime > 0) {
						enemy[i].InfinityTime--;
					}
				}
			}
			for (int i = 0; i < kEnemeyDeathMax; i++) {
				enemyDeath[i].deathAnimeTimer++;
			}

			/*---- アニメーションの初期化 ----*/
			//プレイヤー
			if (player.animeTimer > (player.animePage * kPlayerAnimeSpeed)) {
				player.animeTimer = 0;
			}
			StageTime++;
			//ゲージ
			if (block.animeTimer > (block.animePage * kPlayerAnimeSpeed)) {
				block.animeTimer = 0;
			}
			//敵
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].animeTimer > (enemy[i].animePage * kEnemyAnimeSpeed)) {
					enemy[i].animeTimer = 0;
				}
			}
			for (int i = 0; i < kEnemeyDeathMax; i++) {
				//敵が死ぬとき
				if (enemyDeath[i].isDeath && enemyDeath[i].deathTimer > 0) {
					enemyDeath[i].deathTimer--;

					if (enemy[i].Type == 1) {
						if (enemyDeath[i].deathAnimeTimer > (enemyDeath[i].deathAnimePage * kExplosionSpeed)) {
							enemyDeath[i].deathAnimeTimer = 0;
						}
					}

					else if (enemyDeath[i].deathAnimeTimer > (enemyDeath[i].deathAnimePage * kEnemyDeathAnimeSpeed)) {
						enemyDeath[i].deathAnimeTimer = 0;
					}
				}
				if (enemyDeath[i].deathTimer <= 0) {

					enemyDeath[i].isDeath = 0;
					enemyDeath[i].deathTimer = kEnemyDeathTimer;
				}
			}
			if (smallSkill.isSkill) {
				TyutolialTime++;
			}
			//******* ゲージ関連 *******//	

			if (!isFrozen) {
				if (block.ifMeter != block.meter) {
					if (block.meter > block.ifMeter) {
						block.meter--;
					}
					else if (block.meter < block.ifMeter) {
						block.meter++;
					}
				}
			}

			//ドロップの落ちる処理
			for (int j = 0; j < kDropEnergyMax; j++) {
				if (dropEnergy[j].isDrop) {
					dropEnergy[j].pos.y += dropSpeed;

					if (dropEnergy[j].pos.y >= kScreenDown - block.meter) {
						DropSEFlag = 1;
						//パーティクルの出現処理
						for (int k = 0; k < 4; k++) {
							for (int i = 0; i < kParticleMax; i++) {
								if (!particle[i].isDo) {
									particle[i].pos.x = dropEnergy[j].pos.x;
									particle[i].pos.y = dropEnergy[j].pos.y;
									particle[i].direction = DEGtoRAD(180 + (k * 45));
									particle[i].isDo = 1;
									break;
								}
							}
						}
						for (int i = 0; i < kNormalEnemyMax; i++) {
							//ブロックが貯まっていく値(仮)
							//必殺技で倒したときはドロップの数が減る
							if (StageTime >= 2750 && Wave == 0) {
								dropBlock = 40;
							}
							else if (StageTime >= 2800 && Wave == 1) {
								dropBlock = 30;
							}
							else if (smallSkill.isShot) {
								dropBlock *= 0.2;
							}
							else if (bigSkill.isShot) {
								dropBlock *= 0.6;
							}
							else if (enemy[i].Type == 3 && enemy[i].take.isHave) {
								dropBlock += 20;
							}
							else {
								dropBlock = 10;
							}
						}

						block.ifMeter += dropBlock;
						dropEnergy[j].isDrop = 0;
					}
				}
			}


			/*---- パーティクルの移動 ----*/
			for (int i = 0; i < kParticleMax; i++) {
				if (particle[i].isDo) {
					particleTimer[i]++;
					//等速直線運動
					particle[i].pos.x += 5 * cosf(particle[i].direction);
					//鉛直投げ上げ
					particle[i].pos.y += 5 * sinf(particle[i].direction) + (int)(gravity * ((float)particleTimer[i] / 70.0f));

					particle[i].ViewTime--;
				}
			}

			//パーティクルの初期化
			for (int i = 0; i < kParticleMax; i++) {
				if (particle[i].ViewTime <= 0) {
					particle[i] = particleDefault;
					particleTimer[i] = 0.0f;
				}
			}

			//敵の配置
			if (StageTime == 10) {
				EnemyAppearance(620, 220, 32, 32, 16, 5, 5, 7, 0, 0, 0);
			}

			if (TyutolialTime == 120) {
				block.ifMeter = 250;
				EnemyAppearance(620, 220, 32, 32, 16, 5, 5, 7, 0, 0, 0);
			}
#pragma region プレイヤーが必殺技を撃つ処理
			//スペース押してる間だけ必殺技ゲージがたまる
			if ((keys[DIK_SPACE]) && !SceneChange) {
				//player.isShot = 0;
				player.skillTimer++;

			}
			//スペースを離した瞬間の処理
			else if (player.skillTimer) {//もしゲージが少しでも溜まってたら

				if (player.skillTimer >= 60) {
					player.skillTimer = 0;
					//ブロックが60以上なら大必殺技を打てる
					//200が必要ブロック数
					if (block.ifMeter >= 200) {
						//大必殺技
						//bigSkill.isShot = 1;
						bigSkill.timer = 60;
						block.ifMeter -= 200;

					}
				}
				else {
					//スペースを話したらスキルタイマーを0にする
					player.skillTimer = 0;
					if (!smallSkill.isSkill) {
						//小必殺技
						//ブロックが60以上なら大必殺技を打てる
						//40が必要ブロック数
						if (block.ifMeter >= 40) {
							player.skillTimer = 0;
							block.ifMeter -= 40;
							//smallSkill.isShot = 1;
							smallSkill.timer = 60;
						}
					}
				}
			}

			if (!smallSkill.isSkill) {

				//smallSkill.timerが1以上なら小必殺技が出てる状態
				if (smallSkill.timer > 0) {
					smallSkill.timer--;
					smallSkill.isShot = 1;
					//スキルの位置をプレイヤーの位置に変更
					smallSkill.pos.x = player.pos.x - smallSkill.width / 2;
					smallSkill.pos.y = player.pos.y + 25;
					//必殺技を打っている間は弾を発射しない
					player.CoolTime = kPlayerBulletCoolTime;
					//弾を初期化
					for (int i = 0; i < kPlayerBulletMax; i++) {
						playerBullet[i] = playerBulletDefault;
					}
					//ここは0になった瞬間だけ実行される
					if (smallSkill.timer == 0) {
						smallSkill.isShot = 0;
						smallSkill.animeTimer = 0;
						smallSkill.isSkill = 1;
						enemy[0].life = 0;

					}
				}
				//描写
			}
			else if (smallSkill.isSkill && !bigSkill.isSkill) {

				//bigSkill.timerが1以上なら大必殺技が出てる状態
				if (bigSkill.timer > 0) {
					bigSkill.timer--;
					bigSkill.isShot = 1;
					//スキルの位置をプレイヤーの位置に変更
					bigSkill.pos.x = player.pos.x - bigSkill.width / 2;
					bigSkill.pos.y = player.pos.y + 25;
					//必殺技を打っている間は弾を発射しない
					player.CoolTime = kPlayerBulletCoolTime;
					//弾を初期化
					for (int i = 0; i < kPlayerBulletMax; i++) {
						playerBullet[i] = playerBulletDefault;
					}

					//ここは0になった瞬間だけ実行される
					if (bigSkill.timer == 0) {
						bigSkill.isShot = 0;
						bigSkill.animeTimer = 0;
						bigSkill.isSkill = 1;
						enemy[0].life = 0;

					}
				}
			}

#pragma endregion
			//敵が死ぬ処理
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].life <= 0 && enemy[i].isAlive) {

					//爆発する敵の処理
					for (int j = 0; j < kExplosionEnemyMax; j++) {
						//敵のTypeが1でクールタイムが0以下なら
						if (enemy[i].Type == 1) {
							if (!explosion[j].isDo && explosion[j].coolTimer <= 0) {
								//位置の初期化
								explosion[j].pos.x = enemy[i].pos.x - 32;
								explosion[j].pos.y = enemy[i].pos.y - 32;
								//爆発する
								explosion[j].isDo = 1;
								explosion[j].coolTimer = 300;
								break;
							}
						}
					}

					//敵が死んだアニメーション
					for (int j = 0; j < kEnemeyDeathMax; j++) {
						if (!enemyDeath[j].isDeath) {
							enemyDeath[j].pos.x = enemy[i].pos.x;
							enemyDeath[j].pos.y = enemy[i].pos.y;
							enemyDeath[j].isDeath = 1;
							break;
						}
					}

					//ドロップの処理
					for (int k = 0; k < kDropEnergyMax; k++) {

						if (enemy[i].Type == 7) {//氷敵ドロップ
							if (!iceDrop[k].isDrop) {
								iceDrop[k].pos.x = enemy[i].pos.x;
								iceDrop[k].pos.y = enemy[i].pos.y;
								iceDrop[k].isDrop = 1;
								break;
							}
						}
						else {//通常ドロップ
							if (!dropEnergy[k].isDrop) {
								dropEnergy[k].pos.x = enemy[i].pos.x;
								dropEnergy[k].pos.y = enemy[i].pos.y;
								dropEnergy[k].isDrop = 1;
								break;
							}
						}
					}
					enemy[i] = enemyDefault;
					EnemyDefeatSEFlag = 1;
				}


			}

			if (smallSkill.isSkill && bigSkill.isSkill) {
				player.skillTimer = 0;
				if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) {

					//初期化
					//プレイヤー
					player.pos.x = 640;//位置の初期化X
					player.pos.y = 400;//位置の初期化Y
					//スキル			
					bigSkill.pos.x = -1000;//位置
					bigSkill.pos.y = -1000;

					bigSkill = bigSkillDefault;
					smallSkill = smallSkillDefault;

					smallSkill.pos.x = -1000;//位置
					smallSkill.pos.y = -1000;

					//位置
					block.pos.x = 340;
					block.pos.y = 630;
					//幅
					block.width = 600;
					//高さ
					block.height = 600;
					//ブロック増加予測
					block.ifMeter = screenRangeY - block.pos.y;
					//ブロックの量
					block.meter = 0;
					//背景
					gsPosX[0] = { kScreenLeft };
					gsPosX[1] = { kScreenLeft };
					gsPosY[0] = { 0 };
					gsPosY[1] = { 770 };
					player.animeTimer = 0;
					block.animeTimer = 0;
					gameScene = PLAY;
					SceneChange = 1;
					smallSkill.isSkill = 0;
					bigSkill.isSkill = 0;
					StageTime = 0;

					Novice::StopAudio(TitleBgm_B);
					Novice::StopAudio(PlayBgm_B);
					Novice::StopAudio(GameOverBgm_B);
					Novice::StopAudio(GameClearBgm_B);
					IsPlayBGM = 0;

				}

			}

			break;
		case PLAY:

			if (!IsPlayBGM) {
				if (PlayBgm_B == -1 || Novice::IsPlayingAudio(PlayBgm_B) == 0) {
					PlayBgm_B = Novice::PlayAudio(PlayBgm, 1, 0.2);
				}
				IsPlayBGM = 1;
			}

			if (!Novice::CheckHitKey(DIK_SPACE)) {
				SceneChange = 0;

			}

			//初期化
			for (int i = 0; i < kNormalEnemyMax; i++) {
				enemy[i].IsHit = 0;
			}

			//******* プレイヤー *******//	

#pragma region プレイヤーの移動処理
	//8方向に移動する
			if (!(IsPressUp() && IsPressRight() && IsPressLeft()) && !(IsPressUp() && IsPressDown() && IsPressLeft()) && !(IsPressUp() && IsPressDown() &&
				Novice::CheckHitKey(DIK_RIGHT)) && !(IsPressLeft() && IsPressDown() && Novice::CheckHitKey(DIK_RIGHT))) {
				if (IsPressUp() && IsPressLeft()) {
					player.pos.x += cosf(DEGtoRAD(-135)) * kPlayerSpeed;
					player.pos.y += sinf(DEGtoRAD(-135)) * kPlayerSpeed;
				}
				else if (IsPressUp() && IsPressRight()) {
					player.pos.x += cosf(DEGtoRAD(-45)) * kPlayerSpeed;
					player.pos.y += sinf(DEGtoRAD(-45)) * kPlayerSpeed;
				}
				else if (IsPressDown() && IsPressLeft()) {
					player.pos.x += cosf(DEGtoRAD(135)) * kPlayerSpeed;
					player.pos.y += sinf(DEGtoRAD(135)) * kPlayerSpeed;
				}
				else if (IsPressDown() && IsPressRight()) {
					player.pos.x += cosf(DEGtoRAD(45)) * kPlayerSpeed;
					player.pos.y += sinf(DEGtoRAD(45)) * kPlayerSpeed;
				}
				else {
					if (IsPressUp()) {
						player.pos.y += -kPlayerSpeed;
					}
					if (IsPressLeft()) {
						player.pos.x += -kPlayerSpeed;
					}
					if (IsPressDown()) {
						player.pos.y += kPlayerSpeed;
					}
					if (IsPressRight()) {
						player.pos.x += kPlayerSpeed;
					}
				}
			}
			//画面端
			if (player.pos.x < kScreenLeft) {
				player.pos.x = kScreenLeft;
			}
			if (player.pos.x > kScreenRight - player.width) {
				player.pos.x = kScreenRight - player.width;
			}
			if (player.pos.y < kScreenTop) {
				player.pos.y = kScreenTop;
			}
			if (player.pos.y > kScreenDown - block.meter - player.height + 6) {
				player.pos.y = kScreenDown - block.meter - player.height + 6;
			}
#pragma endregion

#pragma region プレイヤーの弾の処理
			//弾発射
			if (player.CoolTime <= 0) {
				//クールタイムが0になったら弾発射
				for (int i = 0; i < kPlayerBulletMax; i++) {
					//発射されてない状態の弾を探す
					if (!playerBullet[i].IsShot) {
						playerBullet[i].pos.x = player.pos.x + 10;
						playerBullet[i].pos.y = player.pos.y;
						playerBullet[i].IsShot = 1;
						break;
					}
				}
				PlayerShotSEFlag = 1;
				//クールタイムをリセット
				player.CoolTime = kPlayerBulletCoolTime;
			}
			else {
				//クールタイムを1減らす
				player.CoolTime--;
			}
			//弾進む
			for (int i = 0; i < kPlayerBulletMax; i++) {
				if (playerBullet[i].IsShot) {//撃たれている状態なら進む
					playerBullet[i].pos.y -= kPlayerBulletSpeed;
				}
			}
			//弾画面外に行ったら初期化
			for (int i = 0; i < kPlayerBulletMax; i++) {
				if (playerBullet[i].pos.y <= -50) {
					playerBullet[i] = playerBulletDefault;
				}
			}
#pragma endregion

#pragma region プレイヤーの弾が敵に当たったときの処理
			for (int i = 0; i < kPlayerBulletMax; i++) {
				for (int j = 0; j < kNormalEnemyMax; j++) {
					if (enemy[j].isAlive) {
						//矩形の判定
						if (playerBullet[i].pos.x < enemy[j].pos.x + enemy[j].Width &&
							enemy[j].pos.x < playerBullet[i].pos.x + kPlayerBulletSize) {
							if (playerBullet[i].pos.y < enemy[j].pos.y + enemy[j].Height &&
								enemy[j].pos.y < playerBullet[i].pos.y + kPlayerBulletSize) {

								//敵のライフを減らす
								if (!enemy[j].InfinityTime) {
									if (Wave == 0) {
										enemy[j].life -= 10;
									}
									else if (Wave == 1) {
										enemy[j].life -= 10;
									}
									else if (Wave == 2) {
										enemy[j].life -= 10;
									}
								}
								playerBullet[i].isHit = 1;
								enemy[j].IsHit = 1;

								////ドロップの処理
								//for (int k = 0; k < kDropEnergyMax; k++) {
								//	if (!dropEnergy[k].isDrop) {
								//		dropEnergy[k].pos.x = enemy[j].pos.x;
								//		dropEnergy[k].pos.y = enemy[j].pos.y;
								//		dropEnergy[k].isDrop = 1;
								//		break;
								//	}

								//}
							}
						}
					}
				}
			}

			//弾があたったら初期化
			for (int i = 0; i < kPlayerBulletMax; i++) {
				if (playerBullet[i].isHit) {
					playerBullet[i] = playerBulletDefault;
				}
			}
#pragma endregion

#pragma region プレイヤーが必殺技を撃つ処理
			//スペース押してる間だけ必殺技ゲージがたまる
			if ((keys[DIK_SPACE]) && !SceneChange) {
				//player.isShot = 0;
				player.skillTimer++;

			}
			//スペースを離した瞬間の処理
			else if (player.skillTimer) {//もしゲージが少しでも溜まってたら
				if (!isFrozen) {
					if (player.skillTimer >= 60) {
						player.skillTimer = 0;
						//ブロックが60以上なら大必殺技を打てる
						//200が必要ブロック数
						if (block.ifMeter >= 200) {
							//大必殺技
							//bigSkill.isShot = 1;
							bigSkill.timer = 60;
							block.ifMeter -= 200;
							BigSkillSEFlag = 1;
						}
					}
					else {
						//小必殺技
						//スペースを話したらスキルタイマーを0にする
						player.skillTimer = 0;
						//ブロックが60以上なら大必殺技を打てる
						//40が必要ブロック数
						if (block.ifMeter >= 60) {
							player.skillTimer = 0;
							block.ifMeter -= 60;
							//smallSkill.isShot = 1;
							smallSkill.timer = 60;
							SmallSkillSEFlag = 1;
						}
					}
				}
			}

			//bigSkill.timerが1以上なら大必殺技が出てる状態
			if (bigSkill.timer > 0) {
				bigSkill.timer--;
				bigSkill.isShot = 1;
				//スキルの位置をプレイヤーの位置に変更
				bigSkill.pos.x = player.pos.x - bigSkill.width / 2;
				bigSkill.pos.y = player.pos.y + 25;
				//必殺技を打っている間は弾を発射しない
				player.CoolTime = kPlayerBulletCoolTime;
				//弾を初期化
				for (int i = 0; i < kPlayerBulletMax; i++) {
					playerBullet[i] = playerBulletDefault;
				}

				//ここは0になった瞬間だけ実行される
				if (bigSkill.timer == 0) {
					bigSkill.isShot = 0;
					bigSkill.animeTimer = 0;
				}
			}
			//smallSkill.timerが1以上なら小必殺技が出てる状態
			if (smallSkill.timer > 0) {
				smallSkill.timer--;
				smallSkill.isShot = 1;
				//スキルの位置をプレイヤーの位置に変更
				smallSkill.pos.x = player.pos.x - smallSkill.width / 2;
				smallSkill.pos.y = player.pos.y + 25;
				//必殺技を打っている間は弾を発射しない
				player.CoolTime = kPlayerBulletCoolTime;
				//弾を初期化
				for (int i = 0; i < kPlayerBulletMax; i++) {
					playerBullet[i] = playerBulletDefault;
				}
				//ここは0になった瞬間だけ実行される
				if (smallSkill.timer == 0) {
					smallSkill.isShot = 0;
					smallSkill.animeTimer = 0;
				}
			}
#pragma endregion

#pragma region 必殺技と敵の当たり判定
			for (int i = 0; i < kNormalEnemyMax; i++) {

				//大必殺技撃ってるときは弾を消す
				if (bigSkill.timer > 0) {
					for (int j = 0; j < kEnemyBulletMax; j++) {
						enemyBullet[j] = enemyBulletDefault;
					}
				}

				for (int i = 0; i < kNormalEnemyMax; i++) {
					//大必殺技
					if (bigSkill.timer > 0 && !enemy[i].boss.shield.isDo) {
						if (bigSkill.pos.x < enemy[i].pos.x + kEnemySize &&
							enemy[i].pos.x < bigSkill.pos.x + bigSkill.width) {
							if (bigSkill.pos.y > enemy[i].pos.y + kEnemySize &&
								enemy[i].pos.y < bigSkill.pos.y - bigSkill.height) {

								//当たっているならHPを減らす
								//block.ifMeter += dropBlock;
								if (!enemy[i].InfinityTime) {
									enemy[i].life--;
									enemy[i].IsHit = 1;
								}
								//ボスのチャージを止める
								enemy[i].boss.CoolTime = kBossSkillCoolTimer;

								//敵の弾を消す

							}
						}
					}

					//小必殺技
					if (smallSkill.timer > 0) {
						if (smallSkill.pos.x < enemy[i].pos.x + kEnemySize &&
							enemy[i].pos.x < smallSkill.pos.x + smallSkill.width) {
							if (smallSkill.pos.y > enemy[i].pos.y + kEnemySize &&
								enemy[i].pos.y < smallSkill.pos.y - smallSkill.height) {

								//当たっているならHPを減らす
								//block.ifMeter += dropBlock;
								if (!enemy[i].InfinityTime && !enemy[i].boss.shield.isDo) {
									enemy[i].life--;
									enemy[i].IsHit = 1;
								}

							}
						}
					}

					for (int B = 0; B < kEnemyBulletMax; B++) {
						//当たってるとこだけ弾を消す
						if (smallSkill.timer > 0) {
							if (smallSkill.pos.x < enemyBullet[B].pos.x &&
								enemyBullet[B].pos.x < smallSkill.pos.x + smallSkill.width) {
								if (smallSkill.pos.y > enemyBullet[B].pos.y &&
									enemyBullet[B].pos.y < smallSkill.pos.y - smallSkill.height) {
									enemyBullet[B] = enemyBulletDefault;
								}
							}
						}
					}

				}

#pragma endregion


				//******* 敵 *******//	

#pragma region 敵の種類
				for (int i = 0; i < kNormalEnemyMax; i++) {
					switch (enemy[i].Type) {
					case 0:
						//通常の敵
						break;

					case 1://爆発する敵
#pragma region 爆発（敵と爆発の当たり判定）
						for (int j = 0; j < kExplosionEnemyMax; j++) {
							//爆発していて、爆発している秒数が0以上なら
							if (explosion[j].isDo && explosion[j].timer > 0) {

								//爆発と敵の当たり判定
								if (enemy[i].pos.x < explosion[j].pos.x + explosion[j].width &&
									explosion[j].pos.x < enemy[i].pos.x + kEnemySize) {
									if (enemy[i].pos.y < explosion[j].pos.y + explosion[j].height &&
										explosion[j].pos.y < enemy[i].pos.y + kEnemySize) {

										enemy[i].life--;//体力が-1減る

									}
								}
							}
						}

						break;
#pragma endregion

					case 2:
						//居座る敵
						break;

					case 3:
						//ブロックを持っていく敵
						if (enemy[i].pos.y >= block.pos.y) {
							if (enemy[i].take.isHave == 0) {
								enemy[i].take.isHave = 1;
								//ブロックが減る
								block.meter -= 20;
								//enemy[i].pos.y = 0;
							}
						}
						break;

					case 4://バリアをはる中ボス
#pragma region シールド
						if (enemy[i].boss.shield.coolTime <= 0) {
							enemy[i].boss.shield.isDo = 1;
							enemy[i].boss.shield.coolTime = kBossShieldCoolTimer;
							enemy[i].boss.shield.timer = kBossShieldTimer;
						}
						//ボスのシールドのクールタイムが1以上なら減らしていく
						if (enemy[i].boss.shield.coolTime > 0 && !enemy[i].boss.shield.isDo) {
							enemy[i].boss.shield.coolTime--;
						}

						//大必殺技が当たっていないとき
						if (enemy[i].boss.shield.isDo) {
							//ボスの位置と同期
							enemy[i].boss.shield.pos.x = enemy[i].pos.x - enemy[i].boss.shield.width / 3;
							enemy[i].boss.shield.pos.y = enemy[i].pos.y - enemy[i].boss.shield.height / 3;
							//シールドを貼れる時間を減らしていく
							enemy[i].boss.shield.timer--;
						}

						//シールドを貼れる時間が0になったらシールド解除
						if (enemy[i].boss.shield.timer <= 0) {
							enemy[i].boss.shield.isDo = 0;
						}

						/****** プレイヤーの弾がシールドに当たったときの処理 ******/
						for (int i = 0; i < kPlayerBulletMax; i++) {
							for (int j = 0; j < kNormalEnemyMax; j++) {
								if (enemy[j].boss.shield.isDo && enemy[j].isAlive) {
									//矩形の判定
									if (playerBullet[i].pos.x < enemy[j].boss.shield.pos.x + enemy[j].boss.shield.width &&
										enemy[j].boss.shield.pos.x < playerBullet[i].pos.x + kPlayerBulletSize) {
										if (playerBullet[i].pos.y < enemy[j].boss.shield.pos.y + enemy[j].boss.shield.height &&
											enemy[j].boss.shield.pos.y < playerBullet[i].pos.y + kPlayerBulletSize) {
											//プレイヤーの弾を無効化する
											playerBullet[i] = playerBulletDefault;
										}
									}
								}
							}
						}

						/****** 必殺技とシールドの当たり判定 ******/
						//大必殺技
						//矩形にする		
						if (enemy[i].boss.shield.isDo && bigSkill.timer > 0) {
							if (bigSkill.pos.x < enemy[i].boss.shield.pos.x + enemy[i].boss.shield.width &&
								enemy[i].boss.shield.pos.x < bigSkill.pos.x + bigSkill.width) {
								if (bigSkill.pos.y > enemy[i].boss.shield.pos.y + enemy[i].boss.shield.height &&
									enemy[i].boss.shield.pos.y < bigSkill.pos.y - bigSkill.height) {

									//ボスのシールドに当たった
									enemy[i].boss.shield.isDo = 0;
									enemy[i].boss.shield.timer = 0;
									enemy[i].boss.shield.coolTime = kBossShieldCoolTimer;
									enemy[i].InfinityTime = 60;

								}
							}
						}

						/****** シールドとプレイヤーの当たり判定 ******/
						//シールドの中にプレイヤーが入らないようにする処理
						for (int i = 0; i < kNormalEnemyMax; i++) {
							if (enemy[i].boss.shield.isDo) {
								if (player.pos.x < enemy[i].boss.shield.pos.x + enemy[i].boss.shield.width &&
									enemy[i].boss.shield.pos.x + player.width < player.pos.x + player.width) {
									if (player.pos.y < enemy[i].boss.shield.pos.y + enemy[i].boss.shield.height &&
										enemy[i].boss.shield.pos.y + player.height < player.pos.y + player.height) {

										if (IsPressUp()) {
											player.pos.y -= -kPlayerSpeed;
										}
										if (IsPressLeft()) {
											player.pos.x -= -kPlayerSpeed;
										}
										if (IsPressDown()) {
											player.pos.y -= kPlayerSpeed;
										}
										if (IsPressRight()) {
											player.pos.x -= kPlayerSpeed;
										}

									}
								}
							}
						}
						break;
#pragma endregion

					case 5://溜め技をしてくる中ボス
#pragma region 溜め攻撃
						/******* ため攻撃を打つ処理 ******/

						//ボスのシールドのクールタイムが1以上なら減らしていく
						if (enemy[i].boss.skill.coolTime <= 0) {
							enemy[i].boss.skill.isReady = 1;
							enemy[i].boss.skill.coolTime = kBossSkillCoolTimer;
							enemy[i].boss.skill.timer = kBossSkillTimer;
							enemy[i].boss.chargeTime = kBossSkillChargeTimer;
						}
						if (enemy[i].boss.skill.coolTime > 0 && !enemy[i].boss.skill.isReady) {
							enemy[i].boss.skill.coolTime--;
						}
						//0より小さくなったらチャージをやめる

						if (enemy[i].boss.chargeTime >= 0 && enemy[i].boss.skill.isReady) {
							enemy[i].boss.chargeTime--;
							enemy[i].boss.isCharge = 1;
							//ボスのチャージタイムが0になるまでに大必殺技が打たれなかったらボスは溜め技を打つ
							if (enemy[i].boss.isCharge) {

								//ボスはチャージをやめてため技を打つ
								enemy[i].boss.skill.isShot = 1;
								enemy[i].bulletType = 5;
								//スキルの位置をの位置にボスの位置に変更
								enemy[i].boss.skill.pos.x = enemy[i].pos.x - enemy[i].boss.skill.width / 3;
								enemy[i].boss.skill.pos.y = enemy[i].pos.y;

							}

						}

						//溜め技が出ている状態
						if (enemy[i].boss.skill.timer >= 0) {
							enemy[i].boss.skill.timer--;

							//ここは0になった瞬間だけ実行される
							if (enemy[i].boss.skill.timer == 0) {
								enemy[i].boss.skill.isShot = 0;
								enemy[i].boss.skill.isReady = 0;
								enemy[i].bulletType = 2;
							}
						}

						/******* ボスの溜め技とプレイヤーの当たり判定 *******/
						if (player.lifeTimer <= 0 && enemy[i].boss.skill.isShot && enemy[i].boss.chargeTime <= 0) {


							if (enemy[i].boss.skill.pos.x < player.pos.x + kPlayerSize &&
								player.pos.x < enemy[i].boss.skill.pos.x + enemy[i].boss.skill.width) {
								if (enemy[i].boss.skill.pos.y < player.pos.y + kPlayerSize &&
									player.pos.y < enemy[i].boss.skill.pos.y + enemy[i].boss.skill.height) {


								}
							}
						}

						break;
#pragma endregion

					case  6://ボス

#pragma region シールド
						if (enemy[i].boss.shield.coolTime <= 0) {
							enemy[i].boss.shield.isDo = 1;
							enemy[i].boss.shield.coolTime = kBossShieldCoolTimer;
							enemy[i].boss.shield.timer = kBossShieldTimer;
						}
						//ボスのシールドのクールタイムが1以上なら減らしていく
						if (enemy[i].boss.shield.coolTime > 0 && !enemy[i].boss.shield.isDo) {
							enemy[i].boss.shield.coolTime--;
						}

						//大必殺技が当たっていないとき
						if (enemy[i].boss.shield.isDo) {
							//ボスの位置と同期
							enemy[i].boss.shield.pos.x = enemy[i].pos.x - enemy[i].boss.shield.width / 5;
							enemy[i].boss.shield.pos.y = enemy[i].pos.y - enemy[i].boss.shield.height / 5;
							//シールドを貼れる時間を減らしていく
							enemy[i].boss.shield.timer--;
						}

						//シールドを貼れる時間が0になったらシールド解除
						if (enemy[i].boss.shield.timer <= 0) {
							enemy[i].boss.shield.isDo = 0;
						}

						/****** プレイヤーの弾がシールドに当たったときの処理 ******/
						for (int i = 0; i < kPlayerBulletMax; i++) {
							for (int j = 0; j < kNormalEnemyMax; j++) {
								if (enemy[j].boss.shield.isDo && enemy[j].isAlive) {
									//矩形の判定
									if (playerBullet[i].pos.x < enemy[j].boss.shield.pos.x + enemy[j].boss.shield.width &&
										enemy[j].boss.shield.pos.x < playerBullet[i].pos.x + kPlayerBulletSize) {
										if (playerBullet[i].pos.y < enemy[j].boss.shield.pos.y + enemy[j].boss.shield.height &&
											enemy[j].boss.shield.pos.y < playerBullet[i].pos.y + kPlayerBulletSize) {
											//プレイヤーの弾を無効化する
											playerBullet[i] = playerBulletDefault;
										}
									}
								}
							}
						}

						/****** 必殺技とシールドの当たり判定 ******/
						//大必殺技
						//矩形にする		
						if (enemy[i].boss.shield.isDo && bigSkill.timer > 0) {
							if (bigSkill.pos.x < enemy[i].boss.shield.pos.x + enemy[i].boss.shield.width &&
								enemy[i].boss.shield.pos.x < bigSkill.pos.x + bigSkill.width) {
								if (bigSkill.pos.y > enemy[i].boss.shield.pos.y + enemy[i].boss.shield.height &&
									enemy[i].boss.shield.pos.y < bigSkill.pos.y - bigSkill.height) {

									//ボスのシールドに当たった
									enemy[i].boss.shield.isDo = 0;
									enemy[i].boss.shield.timer = 0;
									enemy[i].boss.shield.coolTime = kBossShieldCoolTimer;
									enemy[i].InfinityTime = 60;

								}
							}
						}

						/****** シールドとプレイヤーの当たり判定 ******/
						//シールドの中にプレイヤーが入らないようにする処理
						for (int i = 0; i < kNormalEnemyMax; i++) {
							if (enemy[i].boss.shield.isDo) {
								if (player.pos.x < enemy[i].boss.shield.pos.x + enemy[i].boss.shield.width &&
									enemy[i].boss.shield.pos.x + player.width < player.pos.x + player.width) {
									if (player.pos.y < enemy[i].boss.shield.pos.y + enemy[i].boss.shield.height &&
										enemy[i].boss.shield.pos.y + player.height < player.pos.y + player.height) {

										if (IsPressUp()) {
											player.pos.y -= -kPlayerSpeed;
										}
										if (IsPressLeft()) {
											player.pos.x -= -kPlayerSpeed;
										}
										if (IsPressDown()) {
											player.pos.y -= kPlayerSpeed;
										}
										if (IsPressRight()) {
											player.pos.x -= kPlayerSpeed;
										}

									}
								}
							}
						}


						//ボスのシールドのクールタイムが1以上なら減らしていく
						if (enemy[i].boss.skill.coolTime <= 0) {
							enemy[i].boss.skill.isReady = 1;
							enemy[i].boss.skill.coolTime = kBossSkillCoolTimer;
							enemy[i].boss.skill.timer = kBossSkillTimer;
							enemy[i].boss.chargeTime = kBossSkillChargeTimer;
						}
						if (enemy[i].boss.skill.coolTime > 0 && !enemy[i].boss.skill.isReady) {
							enemy[i].boss.skill.coolTime--;
						}
						//0より小さくなったらチャージをやめる

						if (enemy[i].boss.chargeTime >= 0 && enemy[i].boss.skill.isReady) {
							enemy[i].boss.chargeTime--;
							enemy[i].boss.isCharge = 1;
							//ボスのチャージタイムが0になるまでに大必殺技が打たれなかったらボスは溜め技を打つ
							if (enemy[i].boss.isCharge) {

								//ボスはチャージをやめてため技を打つ
								enemy[i].boss.skill.isShot = 1;
								enemy[i].bulletType = 5;
								//スキルの位置をの位置にボスの位置に変更
								enemy[i].boss.skill.pos.x = enemy[i].pos.x - enemy[i].boss.skill.width / 3;
								enemy[i].boss.skill.pos.y = enemy[i].pos.y;

							}

						}


						//溜め技が出ている状態
						if (enemy[i].boss.skill.timer >= 0) {
							enemy[i].boss.skill.timer--;

							//ここは0になった瞬間だけ実行される
							if (enemy[i].boss.skill.timer == 0) {
								enemy[i].boss.skill.isShot = 0;
								enemy[i].boss.skill.isReady = 0;
								enemy[i].bulletType = 6;
							}
						}


						/******* ボスの溜め技とプレイヤーの当たり判定 *******/

						if (enemy[i].boss.skill.pos.x < player.pos.x + kPlayerSize &&
							player.pos.x < enemy[i].boss.skill.pos.x + enemy[i].boss.skill.width) {
							if (enemy[i].boss.skill.pos.y < player.pos.y + kPlayerSize &&
								player.pos.y < enemy[i].boss.skill.pos.y + enemy[i].boss.skill.height) {


							}
						}

						break;

					}
#pragma endregion
				}

#pragma endregion

#pragma region 爆発する敵（case 1:）

				for (int i = 0; i < kNormalEnemyMax; i++) {
					/******* 爆発する処理(敵がブロックに当たった時) *******/
					if (enemy[i].Type == 1) {
						if (enemy[i].pos.y >= kScreenDown - block.meter) {
							//ライフを0にして敵が死んだ時の処理行き
							enemy[i].life = 0;
						}
					}
				}

				//爆発する敵		case の中だと上手くいかなかったのでここに書いた
				for (int i = 0; i < kNormalEnemyMax; i++) {
					for (int j = 0; j < kExplosionEnemyMax; j++) {
						/******* 爆発する処理(敵が死んだとき) *******/

						//爆発していて、爆発している秒数が0以上なら
						if (explosion[j].isDo && explosion[j].timer > 0) {
							//爆発の秒数を減らす
							explosion[j].timer--;
							//爆発と自分の当たり判定
							if (player.lifeTimer <= 0) {
								if (playerHit.pos.x < explosion[j].pos.x + explosion[j].width &&
									explosion[j].pos.x < playerHit.pos.x + player.r * 2) {
									if (player.pos.y < explosion[j].pos.y + explosion[j].height &&
										explosion[j].pos.y < playerHit.pos.y + player.r * 2) {

										player.isHit = 1;
										player.life--;//体力が-1減る

									}
								}
							}
						}
						//爆発している秒数の初期化
						else {
							explosion[j].timer = 60;
						}

						/******* 爆発が終わってからの処理 *******/
						//爆発が終わる
						if (explosion[j].timer <= 0) {

							explosion[j] = explosionDefault;

						}
						//次に爆発が可能なまでの時間を減らしていく
						if (explosion[j].coolTimer > 0) {
							explosion[j].coolTimer--;
						}

					}
					break;
				}

#pragma endregion


#pragma region 敵の移動処理
				//通常敵の移動処理
				for (int i = 0; i < kNormalEnemyMax; i++) {
					if (enemy[i].isAlive) {
						//ここに移動の動き書く
						switch (enemy[i].MoveType) {
						case 0://動かない
							break;
						case 1://とりあえず下に動いてく			
							enemy[i].pos.y += 2;
							break;
						case 2://画面の上のほうに居座る
							if (enemy[i].pos.y < kScreenTop + 160) {
								enemy[i].pos.y += 2;
							}
							break;
						case 3://ブロックとってく
							//ブロックに当たったらブロックを持っていく

							if (enemy[i].take.isHave) {
								//上に動いていく
								enemy[i].pos.y -= 1;
							}
							else {
								//下に動く
								enemy[i].pos.y += 2;
							}
							break;
						case 4://ガーっと左に
							enemy[i].pos.x -= 2;
							break;
						case 5://ガーっと右に
							enemy[i].pos.x += 2;
							break;
						case 6://上のほうにいてそのうち帰ってく
							if (enemy[i].AliveTime >= 500) {
								enemy[i].pos.y -= 5;
							}
							else if (enemy[i].pos.y <= kScreenTop + 200) {
								enemy[i].pos.y += 5;
							}
							break;
						case 7://インベーダー
							switch (enemy[i].MoveControl[7]) {
							case 0:
								enemy[i].pos.x -= 3;
								if (enemy[i].pos.x <= kScreenLeft + 50) {
									enemy[i].MoveControl[7] = 1;
									enemy[i].MoveControlPosY = enemy[i].pos.y;
								}
								break;
							case 1:
								enemy[i].pos.y += 3;
								if (enemy[i].pos.y >= enemy[i].MoveControlPosY + 100) {
									enemy[i].MoveControl[7] = 2;
								}
								break;
							case 2:
								enemy[i].pos.x += 3;
								if (enemy[i].pos.x >= kScreenRight - 100) {
									enemy[i].MoveControl[7] = 3;
									enemy[i].MoveControlPosY = enemy[i].pos.y;
								}
								break;
							case 3:
								enemy[i].pos.y += 3;
								if (enemy[i].pos.y >= enemy[i].MoveControlPosY + 100) {
									enemy[i].MoveControl[7] = 0;
								}
								break;
							}
							break;
						case 8://左右移動
							if (enemy[i].MoveControl[8] == 0) {
								enemy[i].pos.x -= 3;
								if (enemy[i].pos.x <= kScreenLeft + 50) {
									enemy[i].MoveControl[8] = 1;
								}
							}
							else if (enemy[i].MoveControl[8] == 1) {
								enemy[i].pos.x += 3;
								if (enemy[i].pos.x >= kScreenRight - 50) {
									enemy[i].MoveControl[8] = 0;
								}
							}
							break;
						case 9://左右移動
							if (enemy[i].MoveControl[9] == 0) {
								enemy[i].pos.x += 3;
								if (enemy[i].pos.x >= kScreenRight - 50) {
									enemy[i].MoveControl[9] = 1;
								}
							}
							else if (enemy[i].MoveControl[9] == 1) {
								enemy[i].pos.x -= 3;
								if (enemy[i].pos.x <= kScreenLeft + 50) {
									enemy[i].MoveControl[9] = 0;
								}
							}
							break;
						case 10://上のほうにいてそのうち帰ってく
							if (enemy[i].AliveTime >= 500) {
								enemy[i].pos.y -= 5;
							}
							else if (enemy[i].pos.y <= kScreenTop + 300) {
								enemy[i].pos.y += 5;
							}
							break;
						}
					}
				}



				//敵が画面外に行ったら初期化
				for (int i = 0; i < kNormalEnemyMax; i++) {
					if ((enemy[i].pos.x <= kScreenLeft - 50 || enemy[i].pos.x >= kScreenRight + 50 || enemy[i].pos.y <= kScreenTop - 50 || enemy[i].pos.y >= kScreenDown + 50) && enemy[i].isAlive) {
						enemy[i] = enemyDefault;
					}
				}
#pragma endregion

#pragma region 敵の弾の出現処理
				for (int i = 0; i < kNormalEnemyMax; i++) {
					if (enemy[i].isAlive && !(enemy[i].pos.y >= kScreenDown - block.meter)) {
						switch (enemy[i].bulletType) {
						case 0://弾を撃たない
							break;
						case 1://真っすぐ弾出る
							if (enemy[i].CoolTime % 60 == 0) {
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (!enemyBullet[j].IsShot) {
										enemyBullet[j].pos.x = enemy[i].pos.x;
										enemyBullet[j].pos.y = enemy[i].pos.y;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].angle = (DEGtoRAD(90));
										break;
									}
								}
								EnemyShotSEFlag = 1;
							}
							break;
						case 2://3秒ごとに3way弾
							if (enemy[i].CoolTime % 180 == 0) {
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (enemyBullet[j].IsShot == 0) {
										enemyBullet[j].pos.x = enemy[i].pos.x + 28 + (cosf(DEGtoRAD(90)) * 30);
										enemyBullet[j].pos.y = enemy[i].pos.y + 28 + (sinf(DEGtoRAD(90)) * 30);
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].angle = (DEGtoRAD(90));
										break;
									}
								}
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (enemyBullet[j].IsShot == 0) {
										enemyBullet[j].pos.x = enemy[i].pos.x + 28 + (cosf(DEGtoRAD(45)) * 30);
										enemyBullet[j].pos.y = enemy[i].pos.y + 28 + (sinf(DEGtoRAD(45)) * 30);
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].angle = (DEGtoRAD(45));
										break;
									}
								}
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (enemyBullet[j].IsShot == 0) {
										enemyBullet[j].pos.x = enemy[i].pos.x + 28 + (cosf(DEGtoRAD(135)) * 30);
										enemyBullet[j].pos.y = enemy[i].pos.y + 28 + (sinf(DEGtoRAD(135)) * 30);
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].angle = (DEGtoRAD(135));
										break;
									}
								}
								EnemyShotSEFlag = 1;
							}
							break;
						case 3://1秒ごとに自機狙い弾
							if (enemy[i].CoolTime % 180 == 0) {
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (enemyBullet[j].IsShot == 0) {
										enemyBullet[j].pos.x = enemy[i].pos.x;
										enemyBullet[j].pos.y = enemy[i].pos.y;
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].angle = atan2(player.pos.y - enemy[i].pos.y, player.pos.x - enemy[i].pos.x);
										break;
									}
								}
								EnemyShotSEFlag = 1;
							}
							break;
						case 4://斜め右弾出る
							if (enemy[i].CoolTime % 120 == 0) {
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (!enemyBullet[j].IsShot) {
										enemyBullet[j].pos.x = enemy[i].pos.x;
										enemyBullet[j].pos.y = enemy[i].pos.y;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].angle = (DEGtoRAD(65));
										break;
									}
								}
								EnemyShotSEFlag = 1;
							}
							break;
						case 5://ぐるぐる
							if (enemy[i].CoolTime % 5 == 0) {
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (!enemyBullet[j].IsShot) {
										enemyBullet[j].pos.x = enemy[i].pos.x + 28;
										enemyBullet[j].pos.y = enemy[i].pos.y + 28;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].angle = DEGtoRAD(enemy[i].BulletAngle[0]);
										enemy[i].BulletAngle[0] += 23;
										break;
									}
								}
								for (int j = 0; j < kEnemyBulletMax; j++) {
									if (!enemyBullet[j].IsShot) {
										enemyBullet[j].pos.x = enemy[i].pos.x + 28;
										enemyBullet[j].pos.y = enemy[i].pos.y + 28;
										enemyBullet[j].isHit = 0;
										enemyBullet[j].IsShot = 1;
										enemyBullet[j].angle = DEGtoRAD(enemy[i].BulletAngle[1]);
										enemy[i].BulletAngle[1] -= 23;
										break;
									}
								}
								EnemyShotSEFlag = 1;
							}
							break;
						case 6://斜め右弾出る
							if (enemy[i].CoolTime % 120 == 0) {
								float theta = rand();
								for (int c = 0; c < 15; c++) {
									for (int j = 0; j < kEnemyBulletMax; j++) {
										if (enemyBullet[j].IsShot == 0) {
											enemyBullet[j].pos.x = 44 + enemy[i].pos.x + (cosf(DEGtoRAD(theta)) * 30);
											enemyBullet[j].pos.y = 44 + enemy[i].pos.y + (sinf(DEGtoRAD(theta)) * 30);
											enemyBullet[j].IsShot = 1;
											enemyBullet[j].angle = DEGtoRAD(theta);
											break;
										}
									}
									theta += 24;
								}
								EnemyShotSEFlag = 1;
							}
							break;
						}
					}
				}

#pragma endregion

#pragma region 敵の弾の移動処理
				//弾進む
				for (int i = 0; i < kEnemyBulletMax; i++) {
					if (enemyBullet[i].IsShot) {
						enemyBullet[i].pos.x += cosf(enemyBullet[i].angle) * kEnemyBulletSpeed;
						enemyBullet[i].pos.y += sinf(enemyBullet[i].angle) * kEnemyBulletSpeed;
					}
				}
				//弾画面外に行ったら初期化
				for (int i = 0; i < kEnemyBulletMax; i++) {
					if ((enemyBullet[i].pos.x <= kScreenLeft - 50 || enemyBullet[i].pos.x >= kScreenRight + 50 || enemyBullet[i].pos.y <= kScreenTop - 50 || enemyBullet[i].pos.y >= kScreenDown + 50) && enemyBullet[i].IsShot) {
						enemyBullet[i] = enemyBulletDefault;
					}
				}
#pragma endregion

#pragma region 敵の弾がプレイヤーに当たったときの処理
				for (int i = 0; i < kEnemyBulletMax; i++) {
					if (player.lifeTimer <= 0) {//復帰タイマーが0なら当たり判定が復活
						//半径の計算
						enemyBullet[i].p2br = kEnemyBulletSize + player.r;
						//敵の弾とプレイヤーの距離

						enemyBullet[i].p2b.x = enemyBullet[i].pos.x - playerHit.pos.x;
						enemyBullet[i].p2b.y = enemyBullet[i].pos.y - playerHit.pos.y;

						if (enemyBullet[i].p2b.x * enemyBullet[i].p2b.x +
							enemyBullet[i].p2b.y * enemyBullet[i].p2b.y
							<= enemyBullet[i].p2br * enemyBullet[i].p2br) {
							player.isHit = 1;
							player.life--;//体力が∸1減る
							enemyBullet[i].isHit = 1;
						}
					}
					//弾があたったら初期化
					if (enemyBullet[i].isHit) {
						enemyBullet[i] = enemyBulletDefault;
					}
				}
				if (player.isHit) {
					player.lifeTimer++;//復帰タイマー
				}
				if (player.lifeTimer >= 180) {//復帰タイマーが120になったらリセット
					player.lifeTimer = 0;
					player.isHit = 0;
				}


#pragma endregion

#pragma region 敵が死ぬ処理
				for (int i = 0; i < kNormalEnemyMax; i++) {
					if (enemy[i].life <= 0 && enemy[i].isAlive) {

						//必殺技で死んだときスコアの加算
						if (smallSkill.isShot) {
							enemy[i].score *= 1.4f;
						}
						if (bigSkill.isShot) {
							enemy[i].score *= 2.0f;
						}


						//スコアの合計
						ToatalScore += enemy[i].score;

						//爆発する敵の処理
						for (int j = 0; j < kExplosionEnemyMax; j++) {
							//敵のTypeが1でクールタイムが0以下なら
							if (enemy[i].Type == 1) {
								if (!explosion[j].isDo && explosion[j].coolTimer <= 0) {
									//位置の初期化
									explosion[j].pos.x = enemy[i].pos.x - 32;
									explosion[j].pos.y = enemy[i].pos.y - 32;
									//爆発する
									explosion[j].isDo = 1;
									explosion[j].coolTimer = 300;
									break;
								}
							}
						}

						//敵が死んだアニメーション
						for (int j = 0; j < kEnemeyDeathMax; j++) {
							if (!enemyDeath[j].isDeath) {
								enemyDeath[j].pos.x = enemy[i].pos.x;
								enemyDeath[j].pos.y = enemy[i].pos.y;
								enemyDeath[j].isDeath = 1;
								break;
							}
						}

						//ドロップの処理
						for (int k = 0; k < kDropEnergyMax; k++) {

							if (enemy[i].Type == 7) {//氷敵ドロップ
								if (!iceDrop[k].isDrop) {
									iceDrop[k].pos.x = enemy[i].pos.x;
									iceDrop[k].pos.y = enemy[i].pos.y;
									iceDrop[k].isDrop = 1;
									break;
								}
							}
							else {//通常ドロップ
								if (!dropEnergy[k].isDrop) {
									dropEnergy[k].pos.x = enemy[i].pos.x;
									dropEnergy[k].pos.y = enemy[i].pos.y;
									dropEnergy[k].isDrop = 1;
									break;
								}
							}
						}
						enemy[i] = enemyDefault;
						EnemyDefeatSEFlag = 1;
					}


				}

#pragma endregion

				//******* その他 *******//	

#pragma region ステージの設定
			//******* ステージの設定 *******//	
				switch (Wave) {
					//wave1
				case 0:

					if (StageTime == 120) {
						isNextWave = 0;
					}

					for (int i = 0; i < 5; i++) {
						if (StageTime == (140 + i * 30)) {
							EnemyAppearance(kScreenRight + 50, kScreenTop, 32, 32, 16, 10, 5, 8, 3, 4, 0);
						}
					}

					for (int i = 0; i < 5; i++) {
						if (StageTime == (140 + i * 30)) {
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 100, 32, 32, 16, 10, 5, 8, 3, 5, 0);
						}
					}


					for (int i = 0; i < 5; i++) {
						if (StageTime == (400 + i * 60)) {
							EnemyAppearance(kScreenLeft + 100 + (100 * i), kScreenTop - 50, 32, 32, 16, 10, 5, 8, 2, 6, 0);
						}
					}

					for (int i = 0; i < 10; i++) {
						if (StageTime == (800 + i * 30)) {
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 100, 32, 32, 16, 10, 5, 8, 3, 9, 0);
						}
					}

					for (int i = 0; i < 10; i++) {
						if (StageTime == (1000 + i * 30)) {
							EnemyAppearance(kScreenRight - 50, kScreenTop + 100, 32, 32, 16, 10, 5, 8, 3, 8, 0);
						}
					}

					if (StageTime == 1200) {
						EnemyAppearance(kScreenLeft + 100, kScreenTop - 50, 32, 32, 16, 10, 5, 7, 3, 3, 3);
						EnemyAppearance(kScreenRight - 100, kScreenTop - 50, 32, 32, 16, 10, 5, 7, 3, 3, 3);
					}
					for (int i = 0; i < 10; i++) {
						if (StageTime == (1400 + i * 30)) {
							EnemyAppearance(kScreenRight - 50, kScreenTop + 300, 32, 32, 16, 10, 10, 8, 3, 8, 0);
						}

					}

					/*if (StageTime == 1600) {
						EnemyAppearance(kScreenRight - 50, kScreenTop + 300, 32, 32, 16, 10, 10, 8, 3, 8, 0);
					}*/

					if (StageTime == 1800) {
						EnemyAppearance(kScreenLeft + 300 - 28, kScreenTop - 50, 56, 56, 16, 600, 500, 11, 2, 2, 4);
					}

					if (StageTime >= 2000 && StageTime % 450 == 0) {
						for (int i = 0; i < 5; i++) {
							if (!(i == 2)) {
								EnemyAppearance(kScreenLeft + 100 - 16 + (100 * i), kScreenTop - 50, 32, 32, 16, 5, 5, 8, 3, 6, 0);
							}
						}
					}

					for (int i = 0; i < kNormalEnemyMax; i++) {
						if (StageTime >= 2010 && enemy[i].Type == 4) {
							if (enemy[i].AliveTime % 400 == 0) {
								if (enemy[i].MoveType == 2) {
									enemy[i].MoveType = 8;
								}
								else if (enemy[i].MoveType == 8) {
									enemy[i].MoveType = 2;
								}
							}
						}
					}

					//Waveの切り替
					if (StageTime >= 2050) {
						for (int i = 0; i < kNormalEnemyMax; i++) {
							//ステージタイムが2600以上かつボスの0番目が死んだら
							if (enemy[i].Type == 4) {
								break;
							}
							if (i == kNormalEnemyMax - 1) {
								Wave = 1;
								StageTime = 0;
								player.life += 1;
								isNextWave = 1;
							}
						}
					}

					break;
					//wave2
				case 1:

					if (StageTime == 120) {
						isNextWave = 0;
					}

					if (StageTime == 180) {
						for (int i = 0; i < 5; i++) {
							EnemyAppearance(kScreenLeft + 200 + (i * 100), kScreenTop, 32, 32, 16, 45, 5, 8, 3, 4, 7);
						}
					}

					if (StageTime == 180) {
						for (int i = 0; i < 5; i++) {
							EnemyAppearance(kScreenLeft + 200 + (i * 100), kScreenTop + 100, 32, 32, 16, 45, 5, 8, 3, 8, 0);
						}
					}

					//正方形の陣形
					for (int i = 0; i < 5; i++) {
						if (StageTime == 400 + i * 30) {
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 50, 32, 32, 16, 45, 5, 8, 0, 9, 0);
						}
					}
					for (int i = 0; i < 5; i++) {
						if (StageTime == 400 + i * 30) {
							EnemyAppearance(kScreenRight + 50, kScreenTop + 100, 32, 32, 16, 45, 5, 8, 0, 8, 0);
						}
					}
					for (int i = 0; i < 5; i++) {
						if (StageTime == 400 + i * 30) {
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 150, 32, 32, 16, 45, 5, 8, 0, 9, 0);
						}
					}
					for (int i = 0; i < 5; i++) {
						if (StageTime == 400 + i * 30) {
							EnemyAppearance(kScreenRight + 50, kScreenTop + 200, 32, 32, 16, 45, 5, 8, 0, 8, 0);
						}
					}
					//インベーダー
					for (int i = 0; i < 23; i++) {
						if (StageTime == 900 + i * 15) {
							EnemyAppearance(kScreenLeft + 200, kScreenTop, 32, 32, 16, 45, 5, 8, 4, 7, 0);
						}
					}


					//三角
					if (StageTime == 1350) {
						EnemyAppearance(kScreenLeft + 300, kScreenTop, 32, 32, 16, 45, 5, 8, 3, 1, 0);
					}
					if (StageTime == 1410) {
						for (int i = 0; i < 3; i++) {
							EnemyAppearance(kScreenLeft + 220 + (i * 80), kScreenTop, 32, 32, 16, 45, 5, 8, 3, 1, 0);
						}
					}
					if (StageTime == 1470) {
						for (int i = 0; i < 5; i++) {
							EnemyAppearance(kScreenLeft + 140 + (i * 80), kScreenTop, 32, 32, 16, 45, 5, 8, 3, 1, 0);
						}
					}
					if (StageTime == 1530) {
						for (int i = 0; i < 7; i++) {
							EnemyAppearance(kScreenLeft + 60 + (i * 80), kScreenTop, 32, 32, 16, 45, 5, 8, 3, 1, 0);
						}
					}

					if (StageTime == 1800) {
						EnemyAppearance(kScreenLeft + 300 - 28, kScreenTop - 50, 56, 56, 16, 600, 300, 11, 2, 2, 5);
					}

					if (StageTime >= 2000 && StageTime % 300 == 0) {
						for (int i = 0; i < 5; i++) {
							if (!(i == 2)) {
								EnemyAppearance(kScreenLeft + 100 - 16 + (100 * i), kScreenTop - 50, 32, 32, 16, 5, 5, 8, 3, 6, 0);
							}
						}
					}

					for (int i = 0; i < kNormalEnemyMax; i++) {
						if (StageTime >= 2010 && enemy[i].Type == 5) {
							if (enemy[i].AliveTime % 200 == 0) {
								if (enemy[i].MoveType == 2) {
									enemy[i].MoveType = 8;
								}
								else if (enemy[i].MoveType == 8) {
									enemy[i].MoveType = 2;
								}
							}
						}
					}


					if (StageTime >= 2050) {
						for (int i = 0; i < kNormalEnemyMax; i++) {
							//ステージタイムが2800以上かつボスの0番目が死んだら
							if (enemy[i].Type == 5) {
								break;
							}
							if (i == kNormalEnemyMax - 1) {
								Wave = 2;
								StageTime = 0;
								player.life += 2;
								isNextWave = 1;
							}
						}
					}
					break;
					//wave3		
				case 2:

					if (StageTime == 120) {
						isNextWave = 0;
					}
					for (int i = 0; i < 3; i++) {
						if (StageTime == (200 + i * 30)) {
							EnemyAppearance(kScreenRight + 50, kScreenTop, 32, 32, 16, 40, 5, 8, 3, 4, 0);
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 100, 32, 32, 16, 40, 5, 8, 3, 5, 0);
						}
					}

					//左右から2列
					for (int i = 0; i < 5; i++) {
						if (StageTime == (300 + i * 30)) {
							EnemyAppearance(kScreenRight + 50, kScreenTop + 50, 32, 32, 16, 40, 5, 8, 2, 4, 0);
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 50, 32, 32, 16, 40, 5, 8, 2, 5, 0);
						}
					}

					for (int i = 0; i < 7; i++) {
						if (StageTime == (600 + i * 30)) {
							EnemyAppearance(kScreenLeft + 100, kScreenTop - 50, 32, 32, 16, 40, 5, 8, 2, 3, 0);
							EnemyAppearance(kScreenRight - 100, kScreenTop - 50, 32, 32, 16, 40, 5, 8, 2, 3, 0);
						}
					}


					if (StageTime == 1100) {
						EnemyAppearance(kScreenLeft + 100, kScreenTop - 50, 32, 32, 16, 10, 5, 7, 3, 3, 3);
						EnemyAppearance(kScreenRight - 100, kScreenTop - 50, 32, 32, 16, 10, 5, 7, 3, 3, 3);
					}
					for (int i = 0; i < 5; i++) {
						if (StageTime == (1200 + i * 30)) {
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 50, 32, 32, 16, 40, 5, 8, 2, 5, 0);
							EnemyAppearance(kScreenLeft - 50, kScreenTop + 150, 32, 32, 16, 40, 5, 8, 2, 5, 0);
						}
					}

					if (StageTime == 1500) {
						EnemyAppearance(kScreenLeft + 200, kScreenTop, 32, 32, 16, 40, 5, 8, 3, 1, 0);
						EnemyAppearance(kScreenRight - 200, kScreenTop, 32, 32, 16, 40, 5, 8, 3, 1, 0);
					}

					if (StageTime == 1550) {
						EnemyAppearance(kScreenLeft + 250, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenRight - 250, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenLeft + 150, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenRight - 150, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
					}

					if (StageTime == 1600) {
						EnemyAppearance(kScreenLeft + 200, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenRight - 200, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenLeft + 300, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenRight - 300, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenLeft + 100, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
						EnemyAppearance(kScreenRight - 100, kScreenTop, 32, 32, 16, 40, 5, 8, 2, 1, 0);
					}

					
					if (StageTime == 1900) {
						for (int i = 0; i < 5; i++) {
							EnemyAppearance(kScreenLeft + 200 + (i * 100), kScreenTop, 32, 32, 16, 45, 5, 8, 3, 4, 7);
						}
					}

					

					if (StageTime == 2100) {
						EnemyAppearance(kScreenLeft + 300 - 44, kScreenTop, 88, 88, 16, 800, 5, 8, 6, 2, 6);
					}

					if (StageTime >= 2110 && StageTime % 450 == 0) {
						for (int i = 0; i < 5; i++) {
							if (!(i == 2)) {
								EnemyAppearance(kScreenLeft + 100 - 16 + (100 * i), kScreenTop - 50, 32, 32, 16, 5, 5, 8, 3, 6, 0);
							}
						}
					}

					for (int i = 0; i < kNormalEnemyMax; i++) {
						if (StageTime >= 2110 && enemy[i].Type == 6) {
							if (enemy[i].AliveTime % 400 == 0) {
								if (enemy[i].MoveType == 2) {
									enemy[i].MoveType = 8;
								}
								else if (enemy[i].MoveType == 8) {
									enemy[i].MoveType = 2;
								}
							}
						}
					}


					if (StageTime >= 2270) {
						for (int i = 0; i < kNormalEnemyMax; i++) {
							//ステージタイムが4000以上かつボスの0番目が死んだら
							if (enemy[i].Type == 6) {
								break;
							}
							if (i == kNormalEnemyMax - 1) {
								Novice::StopAudio(TitleBgm_B);
								Novice::StopAudio(PlayBgm_B);
								Novice::StopAudio(GameOverBgm_B);
								Novice::StopAudio(GameClearBgm_B);
								IsPlayBGM = 0;
								gameScene = GAMECLEAR;
							}
						}
					}
					break;
					break;
				}
				break;
			}
#pragma endregion


#pragma region その他の挙動
			//******* その他の挙動 *******//		


			//泡が上まで行ったらリセット
			for (int i = 0; i < 50; i++) {
				if (bubble[i].IsDo) {
					bubble[i].Y -= bubble[i].Speed;
					bubble[i].X += (rand() % 3) - 1;
					if (bubble[i].Y < kScreenDown + 16 - block.meter) {
						bubble[i] = bubbleDefault;
					}
				}
			}

			//泡の配置
			if (StageTime % 10 == 0) {
				for (int i = 0; i < 50; i++) {
					if (!bubble[i].IsDo) {
						bubble[i].X = kScreenLeft + rand() % 600;
						bubble[i].Y = kScreenDown + 50;
						bubble[i].Speed = rand() % 3 + 1;
						bubble[i].IsDo = 1;
						break;
					}
				}
			}

			//******* 毎フレームする処理 *******//	
			//タイマー系の加算
			//プレイヤー
			player.animeTimer++;
			if (smallSkill.isShot) {
				smallSkill.animeTimer++;
			}
			if (bigSkill.isShot) {
				bigSkill.animeTimer++;
			}
			//敵
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].isAlive) {
					//敵が生きているなら生きている時間を加算する
					enemy[i].AliveTime++;
					enemy[i].CoolTime++;
					enemy[i].animeTimer++;
					enemy[i].boss.shield.animeTimer++;
					enemy[i].boss.skill.animeTimer++;
					if (enemy[i].InfinityTime > 0) {
						enemy[i].InfinityTime--;
					}
				}
			}
			for (int i = 0; i < kEnemeyDeathMax; i++) {
				enemyDeath[i].deathAnimeTimer++;
			}
			//その他
			StageTime++;//現在のステージのタイマー

			//ゲージ
			block.animeTimer++;

			//画像のスクロール
			gsPosY[0] -= 1;
			gsPosY[1] -= 1;
			if (gsPosY[0] <= -770) {

				gsPosY[0] = 770;

			}

			if (gsPosY[1] <= -770) {

				gsPosY[1] = 770;

			}
			//プレイヤーとプレイヤーの当たり判定の同期 	
			playerHit.pos.x = player.pos.x + 17;
			playerHit.pos.y = player.pos.y + 23;

			/*---- アニメーションの初期化 ----*/
			//プレイヤー
			if (player.animeTimer > (player.animePage * kPlayerAnimeSpeed)) {
				player.animeTimer = 0;
			}
			//ゲージ
			if (block.animeTimer > (block.animePage * kPlayerAnimeSpeed)) {
				block.animeTimer = 0;
			}

			//敵
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].animeTimer > (enemy[i].animePage * kEnemyAnimeSpeed)) {
					enemy[i].animeTimer = 0;
				}
				//シールド
				if (enemy[i].boss.shield.animeTimer > (enemy[i].boss.shield.animePage * kEnemyAnimeSpeed)) {
					enemy[i].boss.shield.animeTimer = 0;
				}
				//ため攻撃
				if (enemy[i].boss.skill.animeTimer > (enemy[i].boss.skill.animePage * kEnemyAnimeSpeed)) {
					enemy[i].boss.skill.animeTimer = 0;
				}
			}
			for (int i = 0; i < kEnemeyDeathMax; i++) {
				//敵が死ぬとき
				if (enemyDeath[i].isDeath && enemyDeath[i].deathTimer > 0) {
					enemyDeath[i].deathTimer--;

					if (enemy[i].Type == 1) {
						if (enemyDeath[i].deathAnimeTimer > (enemyDeath[i].deathAnimePage * kExplosionSpeed)) {
							enemyDeath[i].deathAnimeTimer = 0;
						}
					}

					else if (enemyDeath[i].deathAnimeTimer > (enemyDeath[i].deathAnimePage * kEnemyDeathAnimeSpeed)) {
						enemyDeath[i].deathAnimeTimer = 0;
					}
				}
				if (enemyDeath[i].deathTimer <= 0) {

					enemyDeath[i].isDeath = 0;
					enemyDeath[i].deathTimer = kEnemyDeathTimer;
				}
			}

			//******* ゲージ関連 *******//	
			//予測線に向けてゲージを変える
			if (!isFrozen) {
				if (block.ifMeter != block.meter) {
					if (block.meter > block.ifMeter) {
						block.meter--;
					}
					else if (block.meter < block.ifMeter) {
						block.meter++;
					}
				}
			}

			//ドロップの落ちる処理
			for (int j = 0; j < kDropEnergyMax; j++) {
				if (dropEnergy[j].isDrop) {
					dropEnergy[j].pos.y += dropSpeed;

					if (dropEnergy[j].pos.y >= kScreenDown - block.meter) {
						DropSEFlag = 1;
						//パーティクルの出現処理
						for (int k = 0; k < 4; k++) {
							for (int i = 0; i < kParticleMax; i++) {
								if (!particle[i].isDo) {
									particle[i].pos.x = dropEnergy[j].pos.x;
									particle[i].pos.y = dropEnergy[j].pos.y;
									particle[i].direction = DEGtoRAD(180 + (k * 45));
									particle[i].isDo = 1;
									break;
								}
							}
						}
						//ブロックが貯まっていく値(仮)
						//必殺技で倒したときはドロップの数が減る
						for (int i = 0; i < kNormalEnemyMax; i++) {
							//ブロックが貯まっていく値(仮)
							//必殺技で倒したときはドロップの数が減る
							if (StageTime >= 2750 && Wave == 0) {
								dropBlock = 40;
							}
							else if (StageTime >= 2800 && Wave == 1) {
								dropBlock = 40;
							}
							else if (StageTime >= 3900 && Wave == 2) {
								dropBlock = 40;
							}
							else if (smallSkill.isShot) {
								dropBlock *= 0.3;
							}
							else if (bigSkill.isShot) {
								dropBlock *= 0.6;
							}
							else if (enemy[i].Type == 3 && enemy[i].take.isHave) {
								dropBlock += 20;
							}
							else {
								dropBlock = 10;

							}
						}
						block.ifMeter += dropBlock;
						dropEnergy[j].isDrop = 0;
					}
				}
			}

			//アイスドロップの落ちる処理
			for (int i = 0; i < kDropEnergyMax; i++) {
				if (iceDrop[i].isDrop) {
					iceDrop[i].pos.y += dropSpeed;
					if (iceDrop[i].pos.y >= kScreenDown - block.meter) {
						DropSEFlag = 1;
						isFrozen = 1;
						iceDrop[i].isDrop = 0;
					}
				}
			}
			if (isFrozen) {
				frozenTimer++;
				if (frozenTimer >= 180) {
					isFrozen = 0;
					frozenTimer = 0;
				}
			}

			//ブロックの上限
			if (block.ifMeter >= kBlockMeterMax) {

				block.ifMeter = kBlockMeterMax;

			}
			if (block.meter >= kBlockMeterMax) {

				block.meter = kBlockMeterMax;

			}

			/*---- パーティクルの移動 ----*/
			for (int i = 0; i < kParticleMax; i++) {
				if (particle[i].isDo) {
					particleTimer[i]++;
					//等速直線運動
					particle[i].pos.x += 5 * cosf(particle[i].direction);
					//鉛直投げ上げ
					particle[i].pos.y += 5 * sinf(particle[i].direction) + (int)(gravity * ((float)particleTimer[i] / 70.0f));

					particle[i].ViewTime--;
				}
			}

			//パーティクルの初期化
			for (int i = 0; i < kParticleMax; i++) {
				if (particle[i].ViewTime <= 0) {
					particle[i] = particleDefault;
					particleTimer[i] = 0.0f;
				}
			}

			//******* シーンの切り替え *******//	

			//プレイヤーのライフが0になった瞬間ゲームオーバー
			if (player.life <= 0) {
				Novice::StopAudio(TitleBgm_B);
				Novice::StopAudio(PlayBgm_B);
				Novice::StopAudio(GameOverBgm_B);
				Novice::StopAudio(GameClearBgm_B);
				IsPlayBGM = 0;
				gameScene = GAMEOVER;
				gsPosY[0] = { 0 };
				gsPosY[1] = { 730 };
				SelectScene = 0;

			}

			//ボスのHPが０より小さくなったらゲームクリア
			/*if (enemy[i].boss.enemy.life <= 0) {

				boss.enemy.isAlive = 0;
				gameScene = GAMECLEAR;
				gsPosY[0] = { 0 };
				gsPosY[1] = { 730 };
				SelectScene = 0;
			}*/

#pragma endregion

			//
			if (block.ifMeter <= 50) {
				Novice::DrawBox(kScreenLeft, kScreenTop, screenRangeX, screenRangeY + 100, 0, 0x00000C66, kFillModeSolid);
			}

			break;

		case GAMEOVER:
			if (!IsPlayBGM) {
				if (GameOverBgm_B == -1 || Novice::IsPlayingAudio(GameOverBgm_B) == 0) {
					GameOverBgm_B = Novice::PlayAudio(GameOverBgm, 1, 0.2);
				}
				IsPlayBGM = 1;
			}
			//セレクトシーンを0から2の間で選択
			if (SelectScene > 0) {
				if (keys[DIK_W] != 0 && preKeys[DIK_W] == 0 ||
					keys[DIK_UP] != 0 && preKeys[DIK_UP] == 0) {
					SelectScene -= 2;
				}
			}
			if (SelectScene < 2) {

				if (keys[DIK_S] != 0 && preKeys[DIK_S] == 0 ||
					keys[DIK_DOWN] != 0 && preKeys[DIK_DOWN] == 0) {
					SelectScene += 2;
				}
			}

			//スペースキーを押したらゲームシーンを切り替える
			if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) {

				if (SelectScene == 0) {
					Novice::StopAudio(TitleBgm_B);
					Novice::StopAudio(PlayBgm_B);
					Novice::StopAudio(GameOverBgm_B);
					Novice::StopAudio(GameClearBgm_B);
					IsPlayBGM = 0;
					gameScene = TITLE;
					SelectScene = 1;
				}
				else if (SelectScene == 2) {
					Novice::StopAudio(TitleBgm_B);
					Novice::StopAudio(PlayBgm_B);
					Novice::StopAudio(GameOverBgm_B);
					Novice::StopAudio(GameClearBgm_B);
					IsPlayBGM = 0;
					gameScene = PLAY;
					SelectScene = 0;
					SceneChange = 1;
				}
#pragma region リトライ（初期化）
				//******* プレイヤー *******//	
				playerDefault.pos.x = 700;
				playerDefault.pos.y = 500;
				playerHit.pos.x = -6000;
				playerHit.pos.y = -6000;
				player = playerDefault;
				player.pos.x = 640;//位置の初期化X
				player.pos.y = 400;//位置の初期化Y
				//弾
				for (int i = 0; i < kPlayerBulletMax; i++) {
					playerBullet[i].pos.x = -7000;
					playerBullet[i].pos.y = -7000;
					playerBullet[i] = playerBulletDefault;
					break;
				}
				//スキル			
				bigSkill.pos.x = -1000;//位置
				bigSkill.pos.y = -1000;
				bigSkill = bigSkillDefault;

				smallSkill.pos.x = -1000;//位置
				smallSkill.pos.y = -1000;
				smallSkill = smallSkillDefault;
				//******* 敵 *******//	
				//凍らせる
				isFrozen = 0;
				frozenTimer = 0;
				for (int i = 0; i < kExplosionEnemyMax; i++) {
					explosion[i] = explosionDefault;
				}
				for (int i = 0; i < kEnemyBulletMax; i++) {
					enemy[i].take = takeDefault;
					enemy[i] = enemyDefault;
					enemyBullet[i] = enemyBulletDefault;
					//ボス
					enemy[i].boss = bossDefault;
					enemy[i].boss.shield = bossShieldDefault;
					enemy[i].boss.skill = bossSkillDefault;
				}
				//敵の爆発
				for (int i = 0; i < kEnemeyDeathMax; i++) {
					enemyDeath[i].pos.x = -200;
					enemyDeath[i].pos.y = -200;
					enemyDeath[i] = enemyDeathDefault;
				}
				//******* エフェクト *******//	
				for (int i = 0; i < kDropEnergyMax; i++) {
					//ドロップ
					dropEnergy[i] = dropEnergyDefault;
					iceDrop[i] = iceDropDefault;
				}
				for (int i = 0; i < kParticleMax; i++) {
					//パーティクル
					particleTimer[i] = 0.0f;
					particle[i] = particleDefault;
				}
				//******* その他 *******//	
				ToatalScore = 0;//スコアの合計
				Stage = 0;//ステージ
				Wave = 0;//ウェーブ
				StageTime = 0;//ステージの経過時間
				isNextWave = 1;

				//シーンの切り替え
				SceneChange = 1;

				//位置
				block.pos.x = 340;
				block.pos.y = 630;
				//幅
				block.width = 600;
				//高さ
				block.height = 600;
				//ブロック増加予測
				block.ifMeter = screenRangeY - block.pos.y;
				//ブロックの量
				block.meter = 0;
				block.pos.y = kScreenDown - block.meter;
				//背景
				gsPosX[0] = { kScreenLeft };
				gsPosX[1] = { kScreenLeft };
				gsPosY[0] = { 0 };
				gsPosY[1] = { 770 };
				TyutolialTime = 0;


#pragma endregion


			}
			break;

		case GAMECLEAR:
			if (!IsPlayBGM) {
				if (GameClearBgm_B == -1 || Novice::IsPlayingAudio(GameClearBgm_B) == 0) {
					GameClearBgm_B = Novice::PlayAudio(GameClearBgm, 1, 0.2);
				}
				IsPlayBGM = 1;
			}
			//スペースキーを押したらゲームシーンを切り替える
			if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) {
				Novice::StopAudio(TitleBgm_B);
				Novice::StopAudio(PlayBgm_B);
				Novice::StopAudio(GameOverBgm_B);
				Novice::StopAudio(GameClearBgm_B);
				IsPlayBGM = 0;
				gameScene = TITLE;
				SelectScene = 1;
#pragma region リトライ（初期化）
				//******* プレイヤー *******//	
				playerDefault.pos.x = 640;
				playerDefault.pos.y = 500;
				playerHit.pos.x = -6000;
				playerHit.pos.y = -6000;
				player = playerDefault;
				player.pos.x = 640;//位置の初期化X
				player.pos.y = 400;//位置の初期化Y
				//弾
				for (int i = 0; i < kPlayerBulletMax; i++) {
					playerBullet[i].pos.x = -7000;
					playerBullet[i].pos.y = -7000;
					playerBullet[i] = playerBulletDefault;
					break;
				}
				//スキル			
				bigSkill.pos.x = -1000;//位置
				bigSkill.pos.y = -1000;
				bigSkill = bigSkillDefault;

				smallSkill.pos.x = -1000;//位置
				smallSkill.pos.y = -1000;
				smallSkill = smallSkillDefault;
				//******* 敵 *******//	
				//凍らせる
				isFrozen = 0;
				frozenTimer = 0;
				for (int i = 0; i < kExplosionEnemyMax; i++) {
					explosion[i] = explosionDefault;
				}
				for (int i = 0; i < kEnemyBulletMax; i++) {
					enemy[i].take = takeDefault;
					enemy[i] = enemyDefault;
					enemyBullet[i] = enemyBulletDefault;
					//ボス
					enemy[i].boss = bossDefault;
					enemy[i].boss.shield = bossShieldDefault;
					enemy[i].boss.skill = bossSkillDefault;
				}
				//敵の爆発
				for (int i = 0; i < kEnemeyDeathMax; i++) {
					enemyDeath[i].pos.x = -200;
					enemyDeath[i].pos.y = -200;
					enemyDeath[i] = enemyDeathDefault;
				}
				//******* エフェクト *******//	
				for (int i = 0; i < kDropEnergyMax; i++) {
					//ドロップ
					dropEnergy[i] = dropEnergyDefault;
					iceDrop[i] = iceDropDefault;
				}
				for (int i = 0; i < kParticleMax; i++) {
					//パーティクル
					particleTimer[i] = 0.0f;
					particle[i] = particleDefault;
				}
				//******* その他 *******//	
				ToatalScore = 0;//スコアの合計
				Stage = 0;//ステージ
				Wave = 0;//ウェーブ
				StageTime = 0;//ステージの経過時間
				isNextWave = 1;

				//シーンの切り替え
				SceneChange = 1;

				//位置
				block.pos.x = 340;
				block.pos.y = 630;
				//幅
				block.width = 600;
				//高さ
				block.height = 600;
				//ブロック増加予測
				block.ifMeter = screenRangeY - block.pos.y;
				//ブロックの量
				block.meter = 0;
				block.pos.y = kScreenDown - block.meter;
				//背景
				gsPosX[0] = { kScreenLeft };
				gsPosX[1] = { kScreenLeft };
				gsPosY[0] = { 0 };
				gsPosY[1] = { 770 };
				TyutolialTime = 0;

#pragma endregion

			}
			break;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (gameScene) {
		case TITLE:
			//背景
			Novice::DrawSprite(
				gsPosX[0],
				gsPosY[0],
				PlayBack,
				1,
				1,
				0,
				0xFFFFFFFF);
			Novice::DrawSprite(
				gsPosX[1],
				gsPosY[1],
				PlayBack,
				1,
				1,
				0,
				0xFFFFFFFF);
			//タイトルの名前
			Novice::DrawSprite(
				430,
				0,
				TitleName,
				1,
				1,
				0.0f,
				0xFFFFFFFF);

			//セレクトボックス
			//チュートリアル
			if (SelectScene == 1) {
				Novice::DrawSpriteRect(
					500,
					400,
					520,
					0,
					260,
					70,
					TitleUi,
					0.25,
					1,
					0,
					0xFFFFFFFF);

			}
			//選択されていない
			else {
				Novice::DrawSpriteRect(
					500,
					400,
					0,
					0,
					260,
					70,
					TitleUi,
					0.25,
					1,
					0,
					0xFFFFFFFF);
			}
			//プレイ画面
			if (SelectScene == 2) {
				Novice::DrawSpriteRect(
					500,
					600,
					780,
					0,
					260,
					70,
					TitleUi,
					0.25,
					1,
					0,
					0xFFFFFFFF);
			}
			//選択されていない
			else {
				Novice::DrawSpriteRect(
					500,
					600,
					260,
					0,
					260,
					70,
					TitleUi,
					0.25,
					1,
					0,
					0xFFFFFFFF);
			}
			break;

		case TUTORIAL:

#pragma region チュートリアルのその他
			//プレイ画面
			Novice::DrawSprite(
				gsPosX[0],
				gsPosY[0],
				PlayBack,
				1,
				1,
				0,
				0xFFFFFFFF);
			Novice::DrawSprite(
				gsPosX[1],
				gsPosY[1],
				PlayBack,
				1,
				1,
				0,
				0xFFFFFFFF);

			//ブロックメーター
			if (!isFrozen) {

				//マックス
				if (block.ifMeter == kBlockMeterMax) {
					Novice::DrawSpriteRect(
						block.pos.x,
						kScreenDown - block.meter,
						(block.animeTimer / kPlayerAnimeSpeed) * block.width,
						0,
						block.width,
						block.height,
						MaxGauge,
						0.2,
						1,
						0,
						0xFFFFFFFF);
				}
				//大必殺技
				else if (block.ifMeter >= 200) {
					Novice::DrawSpriteRect(
						block.pos.x,
						kScreenDown - block.meter,
						(block.animeTimer / kPlayerAnimeSpeed) * block.width,
						0,
						block.width,
						block.height,
						CollectGauge,
						0.2,
						1,
						0,
						0xFFFFFFFF);
				}
				else {//普通
					Novice::DrawSpriteRect(
						block.pos.x,
						kScreenDown - block.meter,
						(block.animeTimer / kPlayerAnimeSpeed) * block.width,
						0,
						block.width,
						block.height,
						Gauge,
						0.2,
						1,
						0,
						0xFFFFFFFF);
				}
			}
			//凍ったゲージ
			else {
				Novice::DrawSpriteRect(
					block.pos.x,
					kScreenDown - block.meter,
					0,
					0,
					block.width,
					block.height,
					IceGauge,
					0.2,
					1,
					0,
					0xFFFFFFFF);
			}


			//ブロック予測線
			Novice::DrawLine(
				kScreenLeft,
				kScreenDown - block.ifMeter + 6,
				kScreenRight,
				kScreenDown - block.ifMeter + 6,
				WHITE
			);

			//ドロップの演出
			for (int i = 0; i < kDropEnergyMax; i++) {

				if (dropEnergy[i].isDrop) {
					Novice::DrawSprite(
						dropEnergy[i].pos.x,
						dropEnergy[i].pos.y,
						Drop,
						1,
						1,
						0,
						0xFFFFFF50);
				}

			}

			//アイスドロップの演出
			for (int i = 0; i < kDropEnergyMax; i++) {

				if (iceDrop[i].isDrop) {
					Novice::DrawSprite(
						iceDrop[i].pos.x,
						iceDrop[i].pos.y,
						IceDrop,
						1,
						1,
						0,
						0xFFFFFF50);

				}
				else {}
			}

			//泡のパーティクル
			/*for (int i = 0; i < 50; i++) {
				Novice::DrawSprite(bubble[i].X, bubble[i].Y, Bable, 1, 1, 0, 0xFFFFFF26);
			}*/

			//パーティクル
			for (int i = 0; i < kParticleMax; i++) {
				if (particle[i].isDo) {
					Novice::DrawSprite(
						particle[i].pos.x,
						particle[i].pos.y,
						Particle,
						1,
						1,
						0,
						0xFFFFFF50);

				}
			}

			//UI
			//チュートリアル
			Novice::DrawSprite(
				kScreenLeft + 170,
				kScreenTop + 20,
				TutorialUi,
				1,
				1,
				0,
				0xFFFFFFFF);

			//小必殺説明
			if (!smallSkill.isSkill) {
				Novice::DrawSprite(
					kScreenLeft + 120,
					kScreenTop + 100,
					TutorialEtc1,
					1,
					1,
					0,
					0xFFFFFFFF);
			}
			else if (smallSkill.isSkill && bigSkill.isSkill == 0) {
				Novice::DrawSprite(
					kScreenLeft + 120,
					kScreenTop + 100,
					TutorialEtc2,
					1,
					1,
					0,
					0xFFFFFFFF);
				Novice::DrawSprite(
					kScreenLeft + 20,
					kScreenTop + 250,
					TutorialEtc3,
					1.3,
					1.3,
					0,
					0xFFFFFFFF);
			}
			else if (smallSkill.isSkill && bigSkill.isSkill) {
				Novice::DrawSprite(
					kScreenLeft + 120,
					kScreenTop + 300,
					TutorialEtc4,
					1,
					1,
					0,
					0xFFFFFFFF);
			}


#pragma endregion

#pragma region チュートリアルのプレイヤー

			if (player.skillTimer <= 0) {
				//普通のプレイヤー
				Novice::DrawSpriteRect(
					player.pos.x - 100,
					player.pos.y,
					(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
					0,
					player.width,
					player.height,
					Player,
					0.125,
					1,
					0,
					0xFFFFFFFF);
			}
			else if (player.skillTimer > 60 && block.ifMeter >= 200) {
				Novice::DrawSpriteRect(
					player.pos.x - 100,
					player.pos.y,
					(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
					0,
					player.width,
					player.height,
					Player,
					0.125,
					1,
					0,
					0xFFDB5AFF);
			}
			else if (player.skillTimer > 0) {

				Novice::DrawSpriteRect(
					player.pos.x - 100,
					player.pos.y,
					(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
					0,
					player.width,
					player.height,
					PlayerCharge,
					0.125,
					1,
					0,
					0xFFFFFFFF);
			}
			else {

				Novice::DrawSpriteRect(
					player.pos.x,
					player.pos.y,
					(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
					0,
					player.width,
					player.height,
					PlayerCharge,
					0.125,
					1,
					0,
					0xFFFFFFFF);
			}

			//大必殺技
			if (bigSkill.timer > 0) {
				if (bigSkill.animeTimer <= 18) {
					Novice::DrawSpriteRect(
						bigSkill.pos.x - 88,
						bigSkill.pos.y - 730,
						(bigSkill.animeTimer / kSkillSpeed) * bigSkill.width,
						0,
						600,
						600,
						BigSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
				else if (bigSkill.animeTimer >= 18) {
					Novice::DrawSpriteRect(
						bigSkill.pos.x - 88,
						bigSkill.pos.y - 730,
						1800,
						0,
						600,
						600,
						BigSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
			}
			else {}

			//小必殺技
			if (smallSkill.timer > 0) {
				if (smallSkill.animeTimer <= 18) {
					Novice::DrawSpriteRect(
						smallSkill.pos.x - 88,
						smallSkill.pos.y - 730,
						(smallSkill.animeTimer / kSkillSpeed) * smallSkill.width,
						0,
						300,
						600,
						SmallSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
				else if (smallSkill.animeTimer >= 18) {

					Novice::DrawSpriteRect(
						smallSkill.pos.x - 88,
						smallSkill.pos.y - 730,
						900,
						0,
						300,
						600,
						SmallSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
			}
#pragma endregion

			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].isAlive) {
					switch (enemy[i].Type) {
					case 0://通常の敵	
						if (enemy[i].IsHit) {
							Novice::DrawSpriteRect(enemy[i].pos.x,
								enemy[i].pos.y,
								(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
								0,
								kEnemySize,
								kEnemySize,
								NormalEnemy,
								0.111,
								1,
								0,
								0x9E75A0FF);
						}
						else {
							Novice::DrawSpriteRect(enemy[i].pos.x,
								enemy[i].pos.y,
								(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
								0,
								kEnemySize,
								kEnemySize,
								NormalEnemy,
								0.111,
								1,
								0,
								0xFFFFFFFF);
						}
						break;
					}
				}
			}
			//敵の爆発
			for (int i = 0; i < kEnemeyDeathMax; i++) {
				if (enemyDeath[i].isDeath && enemyDeath[i].deathTimer > 0) {

					Novice::DrawSpriteRect(
						enemyDeath[i].pos.x,
						enemyDeath[i].pos.y,
						(enemyDeath[i].deathAnimeTimer / kEnemyDeathAnimeSpeed) * kEnemyDeathSize,
						0,
						kEnemyDeathSize,
						kEnemyDeathSize,
						DeathEnemy,
						0.111,
						1,
						0,
						0xFFFFFFFF);
				}
			}
			break;


		case PLAY:

#pragma region その他
			//******* その他 *******//	
			//プレイ画面
			Novice::DrawSprite(
				gsPosX[0],
				gsPosY[0],
				PlayBack,
				1,
				1,
				0,
				0xFFFFFFFF);
			Novice::DrawSprite(
				gsPosX[1],
				gsPosY[1],
				PlayBack,
				1,
				1,
				0,
				0xFFFFFFFF);

			//ブロックメーター
			if (!isFrozen) {

				//マックス
				if (block.ifMeter == kBlockMeterMax) {
					Novice::DrawSpriteRect(
						block.pos.x,
						kScreenDown - block.meter,
						(block.animeTimer / kPlayerAnimeSpeed) * block.width,
						0,
						block.width,
						block.height,
						MaxGauge,
						0.2,
						1,
						0,
						0xFFFFFFFF);
				}
				//大必殺技
				else if (block.ifMeter >= 200) {
					Novice::DrawSpriteRect(
						block.pos.x,
						kScreenDown - block.meter,
						(block.animeTimer / kPlayerAnimeSpeed) * block.width,
						0,
						block.width,
						block.height,
						CollectGauge,
						0.2,
						1,
						0,
						0xFFFFFFFF);
				}
				else {//普通
					Novice::DrawSpriteRect(
						block.pos.x,
						kScreenDown - block.meter,
						(block.animeTimer / kPlayerAnimeSpeed) * block.width,
						0,
						block.width,
						block.height,
						Gauge,
						0.2,
						1,
						0,
						0xFFFFFFFF);
				}
			}
			//凍ったゲージ
			else {
				Novice::DrawSpriteRect(
					block.pos.x,
					kScreenDown - block.meter,
					0,
					0,
					block.width,
					block.height,
					IceGauge,
					0.2,
					1,
					0,
					0xFFFFFFFF);
			}


			//ブロック予測線
			Novice::DrawLine(
				kScreenLeft,
				kScreenDown - block.ifMeter + 6,
				kScreenRight,
				kScreenDown - block.ifMeter + 6,
				WHITE
			);

			//ドロップの演出
			for (int i = 0; i < kDropEnergyMax; i++) {

				if (dropEnergy[i].isDrop) {
					Novice::DrawSprite(
						dropEnergy[i].pos.x,
						dropEnergy[i].pos.y,
						Drop,
						1,
						1,
						0,
						0xFFFFFF50);
				}

			}

			//アイスドロップの演出
			for (int i = 0; i < kDropEnergyMax; i++) {

				if (iceDrop[i].isDrop) {
					Novice::DrawSprite(
						iceDrop[i].pos.x,
						iceDrop[i].pos.y,
						IceDrop,
						1,
						1,
						0,
						0xFFFFFF50);

				}
				else {}
			}

			//泡のパーティクル
			for (int i = 0; i < 50; i++) {
				Novice::DrawSprite(bubble[i].X, bubble[i].Y, Bable, 1, 1, 0, 0xFFFFFF26);
			}

			//パーティクル
			for (int i = 0; i < kParticleMax; i++) {
				if (particle[i].isDo) {
					Novice::DrawSprite(
						particle[i].pos.x,
						particle[i].pos.y,
						Particle,
						1,
						1,
						0,
						0xFFFFFF50);

				}
			}



			//HPバーの枠
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if ((enemy[i].Type == 4) && enemy[i].isAlive) {
					Novice::DrawSprite(362, 47, BossHp, 1, 1, 0, 0xFFFFFFFF);
				}
				if ((enemy[i].Type == 5) && enemy[i].isAlive) {
					Novice::DrawSprite(362, 47, BossHp, 1, 1, 0, 0xFFFFFFFF);
				}
				if ((enemy[i].Type == 6) && enemy[i].isAlive) {
					Novice::DrawSprite(362, 47, BossHp, 1, 1, 0, 0xFFFFFFFF);
				}
			}

			//ボスのHPバー
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if ((enemy[i].Type == 4) && enemy[i].isAlive) {
					Novice::DrawBox(365, 53, 480 * enemy[i].life / 525, 17, 0.0f, 0xF090E9FF, kFillModeSolid);
				}
				if ((enemy[i].Type == 5) && enemy[i].isAlive) {
					Novice::DrawBox(365, 53, 480 * enemy[i].life / 525, 17, 0.0f, 0xF090E9FF, kFillModeSolid);
				}
				if ((enemy[i].Type == 6) && enemy[i].isAlive) {
					Novice::DrawBox(365, 53, 480 * enemy[i].life / 700, 17, 0.0f, 0xF090E9FF, kFillModeSolid);
				}
			}





#pragma endregion

#pragma region プレイヤー
			//プレイヤー
			if (player.life > 0) {
				if (player.lifeTimer % 10 >= 1 && player.lifeTimer % 10 <= 5) {
					if (player.lifeTimer > 0) {
						//当たったプレイヤー
						Novice::DrawSpriteRect(player.pos.x,
							player.pos.y,
							(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
							0, player.width,
							player.height,
							Player,
							0.125,
							1,
							0,
							0x9E75A0FF);

					}
				}

				else {
					if (player.skillTimer <= 0) {
						//普通のプレイヤー
						Novice::DrawSpriteRect(
							player.pos.x,
							player.pos.y,
							(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
							0,
							player.width,
							player.height,
							Player,
							0.125,
							1,
							0,
							0xFFFFFFFF);
					}
					else if (player.skillTimer > 60 && block.ifMeter >= 200) {
						Novice::DrawSpriteRect(
							player.pos.x,
							player.pos.y,
							(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
							0,
							player.width,
							player.height,
							Player,
							0.125,
							1,
							0,
							0xFFDB5AFF);
					}
					else if (player.skillTimer > 0) {


						Novice::DrawSpriteRect(
							player.pos.x,
							player.pos.y,
							(player.animeTimer / kPlayerAnimeSpeed) * kPlayerSize,
							0,
							player.width,
							player.height,
							PlayerCharge,
							0.125,
							1,
							0,
							0xFFFFFFFF);
					}


				}
			}


			//プレイヤーの当たり判定
			Novice::DrawEllipse(
				playerHit.pos.x,
				playerHit.pos.y,
				player.r,
				player.r,
				0.0f,
				RED,
				kFillModeWireFrame);


			//プレイヤーの弾
			for (int i = 0; i < kPlayerBulletMax; i++) {
				if (playerBullet[i].IsShot) {

					Novice::DrawSprite(
						playerBullet[i].pos.x,
						playerBullet[i].pos.y,
						PlayerBullet,
						1,
						1,
						0,
						0xFFFFFFFF);

				}
			}

			//大必殺技
			if (bigSkill.timer > 0) {
				if (bigSkill.animeTimer <= 18) {
					Novice::DrawSpriteRect(
						bigSkill.pos.x + 12,
						bigSkill.pos.y - 730,
						(bigSkill.animeTimer / kSkillSpeed) * bigSkill.width,
						0,
						600,
						600,
						BigSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
				else if (bigSkill.animeTimer >= 18) {
					Novice::DrawSpriteRect(
						bigSkill.pos.x + 12,
						bigSkill.pos.y - 730,
						1800,
						0,
						600,
						600,
						BigSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
			}
			else {}

			//小必殺技
			if (smallSkill.timer > 0) {
				if (smallSkill.animeTimer <= 18) {
					Novice::DrawSpriteRect(
						smallSkill.pos.x + 12,
						smallSkill.pos.y - 730,
						(smallSkill.animeTimer / kSkillSpeed) * smallSkill.width,
						0,
						300,
						600,
						SmallSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
				else if (smallSkill.animeTimer >= 18) {

					Novice::DrawSpriteRect(
						smallSkill.pos.x + 12,
						smallSkill.pos.y - 730,
						900,
						0,
						300,
						600,
						SmallSkill,
						0.25,
						1.2,
						0,
						0xFFFFFFFF);
				}
			}


#pragma endregion

#pragma region 敵
			//敵
			//爆発する敵
			for (int j = 0; j < kNormalEnemyMax; j++) {
				for (int i = 0; i < kExplosionEnemyMax; i++) {
					if (explosion[i].isDo) {

						Novice::DrawSpriteRect(
							explosion[i].pos.x,
							explosion[i].pos.y,
							(enemyDeath[i].deathAnimeTimer / kExplosionSpeed) * kEnemyDeathSize,
							0,
							kEnemyDeathSize,
							kEnemyDeathSize,
							DeathEnemy,
							0.222,
							2,
							0,
							0xFFFFFFFF);
					}
				}
				break;
			}
			//敵の爆発
			for (int i = 0; i < kEnemeyDeathMax; i++) {
				if (enemyDeath[i].isDeath && enemyDeath[i].deathTimer > 0) {

					Novice::DrawSpriteRect(
						enemyDeath[i].pos.x,
						enemyDeath[i].pos.y,
						(enemyDeath[i].deathAnimeTimer / kEnemyDeathAnimeSpeed) * kEnemyDeathSize,
						0,
						kEnemyDeathSize,
						kEnemyDeathSize,
						DeathEnemy,
						0.111,
						1,
						0,
						0xFFFFFFFF);
				}
			}
			for (int i = 0; i < kNormalEnemyMax; i++) {
				if (enemy[i].isAlive) {
					switch (enemy[i].Type) {
					case 0://通常の敵	
						if (enemy[i].IsHit) {
							Novice::DrawSpriteRect(enemy[i].pos.x,
								enemy[i].pos.y,
								(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
								0,
								kEnemySize,
								kEnemySize,
								NormalEnemy,
								0.111,
								1,
								0,
								0x9E75A0FF);
						}
						else {
							Novice::DrawSpriteRect(enemy[i].pos.x,
								enemy[i].pos.y,
								(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
								0,
								kEnemySize,
								kEnemySize,
								NormalEnemy,
								0.111,
								1,
								0,
								0xFFFFFFFF);
						}
						break;
					case 1://爆発する敵
						Novice::DrawSpriteRect(enemy[i].pos.x,
							enemy[i].pos.y,
							(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
							0,
							kEnemySize,
							kEnemySize,
							ExplosionEnemy,
							0.111,
							1,
							0,
							0xFFFFFFFF);

						break;
					case 2:
						Novice::DrawBox(
							enemy[i].pos.x,
							enemy[i].pos.y,
							kEnemySize,
							kEnemySize,
							0,
							RED,
							kFillModeSolid);
						break;


					case 3://ブロックを持っていく
						if (enemy[i].take.isHave) {
							if (enemy[i].IsHit) {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
									0,
									kEnemySize,
									kEnemySize,
									TakeHaveEnemy,
									0.111,
									1,
									0,
									0x9E75A0FF);
							}
							else {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
									0,
									kEnemySize,
									kEnemySize,
									TakeHaveEnemy,
									0.111,
									1,
									0,
									0xFFFFFFFF);
							}
						}

						else {
							if (enemy[i].IsHit) {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
									0,
									kEnemySize,
									kEnemySize,
									TakeEnemy,
									0.111,
									1,
									0,
									0x9E75A0FF);
							}
							else {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
									0,
									kEnemySize,
									kEnemySize,
									TakeEnemy,
									0.111,
									1,
									0,
									0xFFFFFFFF);
							}
						}
						break;

					case 4://シールドをはる中ボス
						if (enemy[i].isAlive) {
							if (enemy[i].IsHit) {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kMiniBossSize,
									0,
									kMiniBossSize,
									kMiniBossSize,
									MiniBoss,
									0.083,
									1,
									0,
									0x9E75A0FF);
							}
							else {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kMiniBossSize,
									0,
									kMiniBossSize,
									kMiniBossSize,
									MiniBoss,
									0.083,
									1,
									0,
									0xFFFFFFFF);
							}

							//ボスのシールド
							if (enemy[i].boss.shield.isDo) {
								Novice::DrawSpriteRect(
									enemy[i].boss.shield.pos.x,
									enemy[i].boss.shield.pos.y,
									(enemy[i].boss.shield.animeTimer / kEnemyAnimeSpeed) * enemy[i].boss.shield.width,
									0,
									enemy[i].boss.shield.width,
									enemy[i].boss.shield.height,
									BossShield,
									0.125,
									1,
									0,
									0xFFFFFFFF);
							}
							//打て！
							if (enemy[i].boss.shield.isDo) {
								Novice::DrawSprite(kScreenLeft + 200, 70, ShotUi, 1, 1, 0, 0xFFFFFFFF);
							}
						}
						break;

					case 5://溜め技を打つ中ボス
						if (enemy[i].isAlive) {
							if (enemy[i].IsHit) {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kMiniBossSize,
									0,
									kMiniBossSize,
									kMiniBossSize,
									MiniBoss,
									0.083,
									1,
									0,
									0x9E75A0FF);
							}
							else {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kMiniBossSize,
									0,
									kMiniBossSize,
									kMiniBossSize,
									MiniBoss,
									0.083,
									1,
									0,
									0xFFFFFFFF);
							}


							//溜め技
							if (enemy[i].boss.skill.isShot) {
								if (enemy[i].boss.chargeTime <= 0) {
									Novice::DrawBox(
										enemy[i].boss.skill.pos.x,
										enemy[i].boss.skill.pos.y,
										enemy[i].boss.skill.width,
										enemy[i].boss.skill.height,
										0.0f,
										BLACK,
										kFillModeWireFrame
									);
								}
							}
						}
						break;

					case 6://ボス
						if (enemy[i].isAlive) {
							if (enemy[i].IsHit) {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kBossSize,
									0,
									kBossSize,
									kBossSize,
									Boss,
									0.111,
									1,
									0,
									0x9E75A0FF);
							}
							else {
								Novice::DrawSpriteRect(enemy[i].pos.x,
									enemy[i].pos.y,
									(enemy[i].animeTimer / kEnemyAnimeSpeed) * kBossSize,
									0,
									kBossSize,
									kBossSize,
									Boss,
									0.111,
									1,
									0,
									0xFFFFFFFF);
							}


							//ボスのシールド
							if (enemy[i].boss.shield.isDo) {

								Novice::DrawSpriteRect(
									enemy[i].boss.shield.pos.x,
									enemy[i].boss.shield.pos.y,
									(enemy[i].boss.shield.animeTimer / kEnemyAnimeSpeed) * enemy[i].boss.shield.width,
									0,
									enemy[i].boss.shield.width,
									enemy[i].boss.shield.height,
									BossShield,
									0.125,
									1,
									0,
									0xFFFFFFFF);

							}
							//溜め技
							if (enemy[i].boss.skill.isShot) {
								if (enemy[i].boss.chargeTime <= 0) {
									Novice::DrawBox(
										enemy[i].boss.skill.pos.x,
										enemy[i].boss.skill.pos.y,
										enemy[i].boss.skill.width,
										enemy[i].boss.skill.height,
										0.0f,
										BLACK,
										kFillModeWireFrame
									);
								}
							}
						}
						break;
					case 7://氷の敵


						Novice::DrawSpriteRect(enemy[i].pos.x,
							enemy[i].pos.y,
							(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
							0,
							kEnemySize,
							kEnemySize,
							NormalEnemy,
							0.111,
							1,
							0,
							0x9E75A0FF);


						Novice::DrawSpriteRect(enemy[i].pos.x,
							enemy[i].pos.y,
							(enemy[i].animeTimer / kEnemyAnimeSpeed) * kEnemySize,
							0,
							kEnemySize,
							kEnemySize,
							IceEnemy,
							0.111,
							1,
							0,
							0xFFFFFFFF);

					}

				}
			}
			//敵の弾
			for (int i = 0; i < kEnemyBulletMax; i++) {
				if (enemyBullet[i].IsShot) {

					Novice::DrawSprite(
						enemyBullet[i].pos.x,
						enemyBullet[i].pos.y,
						EnemyBullet,
						1,
						1,
						0,
						0xFFFFFFFF);
				}
			}

			//プレイヤーHp
			for (int i = 0; i < player.life; i++) {
				Novice::DrawSprite(680 + (i * 50), 685, PlayerHp, 1, 1, 0, 0xFFFFFFFF);
			}


#pragma endregion

			//ウェーブ
			switch (Wave) {
			case 0:
				if (isNextWave) {
					Novice::DrawSpriteRect(
						kScreenLeft + 200,
						300,
						0,
						0,
						200,
						75,
						WaveUi,
						0.333,
						1,
						0,
						0xFFFFFFFF);
				}
				break;
			case 1:
				if (isNextWave) {
					Novice::DrawSpriteRect(
						kScreenLeft + 200,
						300,
						200,
						0,
						200,
						75,
						WaveUi,
						0.333,
						1,
						0,
						0xFFFFFFFF);
				}
				break;
			case 2:
				if (isNextWave) {
					if (isNextWave) {
						Novice::DrawSpriteRect(
							kScreenLeft + 200,
							300,
							400,
							0,
							200,
							75,
							WaveUi,
							0.333,
							1,
							0,
							0xFFFFFFFF);
					}
				}
				break;
			}
			break;


			break;

		case GAMEOVER:

			//背景
			Novice::DrawSprite(
				gsPosX[0],
				gsPosY[0] + 20,
				GameOver,
				1,
				1,
				0,
				0xFFFFFFFF);

			//UI
			Novice::DrawSprite(
				430,
				100,
				GameOverUi,
				1,
				1,
				0,
				0xFFFFFFFF);

			//セレクトボックス
			//タイトル
			if (SelectScene == 0) {
				Novice::DrawSpriteRect(
					500,
					450,
					520,
					0,
					260,
					70,
					GameOverUis,
					0.25,
					1,
					0,
					0xFFFFFFFF);

			}
			//選択されていない
			else {
				Novice::DrawSpriteRect(
					500,
					450,
					0,
					0,
					260,
					70,
					GameOverUis,
					0.25,
					1,
					0,
					0xFFFFFFFF);
			}
			//リトライ
			if (SelectScene == 2) {
				Novice::DrawSpriteRect(
					500,
					600,
					780,
					0,
					260,
					70,
					GameOverUis,
					0.25,
					1,
					0,
					0xFFFFFFFF);
			}
			//選択されていない
			else {
				Novice::DrawSpriteRect(
					500,
					600,
					260,
					0,
					260,
					70,
					GameOverUis,
					0.25,
					1,
					0,
					0xFFFFFFFF);
			}
			break;

		case GAMECLEAR:
			//背景
			Novice::DrawSprite(
				kScreenLeft,
				kScreenTop,
				GameClear,
				1,
				1,
				0,
				0xFFFFFFFF);

			//UI
			Novice::DrawSprite(
				430,
				100,
				GameClearUi,
				1,
				1,
				0,
				0xFFFFFFFF);

			//セレクトボックス
			//タイトル

			Novice::DrawSpriteRect(
				500,
				525,
				0,
				0,
				260,
				70,
				GameClearUis,
				0.5,
				1,
				0,
				0xFFFFFFFF);


			break;
		}

#pragma region 枠の処理
		Novice::DrawSprite(0, 0, Frame, 1, 1, 0, WHITE);

		if (IsPressUp()) {
			Novice::DrawSpriteRect(125, 100, 312, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}
		else {
			Novice::DrawSpriteRect(125, 100, 0, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}

		if (IsPressDown()) {
			Novice::DrawSpriteRect(123, 269, 386, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}
		else {
			Novice::DrawSpriteRect(123, 269, 78, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}

		if (IsPressLeft()) {
			Novice::DrawSpriteRect(40, 184, 464, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}
		else {
			Novice::DrawSpriteRect(40, 184, 156, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}

		if (IsPressRight()) {
			Novice::DrawSpriteRect(210, 184, 542, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}
		else {
			Novice::DrawSpriteRect(210, 184, 232, 0, 77, 77, Keys, 0.125, 1, 0, WHITE);
		}

		if (Novice::CheckHitKey(DIK_SPACE)) {
			Novice::DrawSpriteRect(1021, 132, 190, 0, 189, 99, Space, 0.5, 1, 0, WHITE);
		}
		else {
			Novice::DrawSpriteRect(1021, 132, 0, 0, 189, 99, Space, 0.5, 1, 0, WHITE);
		}

		//プレイ時のスコア
		if (gameScene == PLAY) {
			{
				int ScoreViewNum = ToatalScore;
				int ScoreView[6] = {};

				ScoreView[0] = ScoreViewNum / 100000;
				ScoreViewNum = ScoreViewNum % 100000;

				ScoreView[1] = ScoreViewNum / 10000;
				ScoreViewNum = ScoreViewNum % 10000;

				ScoreView[2] = ScoreViewNum / 1000;
				ScoreViewNum = ScoreViewNum % 1000;

				ScoreView[3] = ScoreViewNum / 100;
				ScoreViewNum = ScoreViewNum % 100;

				ScoreView[4] = ScoreViewNum / 10;
				ScoreViewNum = ScoreViewNum % 10;

				ScoreView[5] = ScoreViewNum;

				Novice::DrawSprite(70, 550, ScoreUi, 1, 1, 0.0f, WHITE);

				for (int i = 0; i < 6; i++) {
					Novice::DrawSpriteRect(50 + i * 32, 600, ScoreView[i] * 32, 0, 32, 32, Number, 0.1, 1, 0.0f, WHITE);
				}
			}
		}

		if (gameScene == GAMECLEAR || gameScene == GAMEOVER) {
			{
				int ScoreViewNum = ToatalScore;
				int ScoreView[6] = {};

				ScoreView[0] = ScoreViewNum / 100000;
				ScoreViewNum = ScoreViewNum % 100000;

				ScoreView[1] = ScoreViewNum / 10000;
				ScoreViewNum = ScoreViewNum % 10000;

				ScoreView[2] = ScoreViewNum / 1000;
				ScoreViewNum = ScoreViewNum % 1000;

				ScoreView[3] = ScoreViewNum / 100;
				ScoreViewNum = ScoreViewNum % 100;

				ScoreView[4] = ScoreViewNum / 10;
				ScoreViewNum = ScoreViewNum % 10;

				ScoreView[5] = ScoreViewNum;

				Novice::DrawSprite(kScreenLeft + 210, 250, ScoreUi, 1, 1, 0.0f, WHITE);

				for (int i = 0; i < 6; i++) {
					Novice::DrawSpriteRect(kScreenLeft + 190 + i * 32, 300, ScoreView[i] * 32, 0, 32, 32, Number, 0.1, 1, 0.0f, WHITE);
				}
			}
		}

#pragma endregion
		// デバック用の文字を表示



		//音を鳴らす
		if (EnemyDefeatSEFlag) {
			Novice::PlayAudio(EnemyDefeatSE, 0, 0.3);
		}
		if (PlayerShotSEFlag) {
			Novice::PlayAudio(PlayerShotSE, 0, 0.02);
		}
		if (DropSEFlag) {
			Novice::PlayAudio(DropSE, 0, 0.1);
		}
		if (EnemyShotSEFlag) {
			Novice::PlayAudio(EnemyShotSE, 0, 0.15);
		}
		if (DecisionSEFlag) {
			Novice::PlayAudio(DecisionSE, 0, 0.3);
		}
		if (SelectSEFlag) {
			Novice::PlayAudio(SelectSE, 0, 0.3);
		}
		if (SmallSkillSEFlag) {
			Novice::PlayAudio(SmallSkillSE, 0, 0.3);
		}
		if (BigSkillSEFlag) {
			Novice::PlayAudio(BigSkillSE, 0, 0.3);
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
