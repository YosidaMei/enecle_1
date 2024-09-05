#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

//角度をラジアンに変換する関数
float DEGtoRAD(float num) {
	float num2;
	num2 = num * (M_PI / 180);
	return num2;
}

//上移動キーを押しているか
int IsPressUp(){
	if(Novice::CheckHitKey(DIK_UP) || Novice::CheckHitKey(DIK_W)){
		return 1;
	}else{
		return 0;
	}
}

//下移動キーを押しているか
int IsPressDown(){
	if(Novice::CheckHitKey(DIK_DOWN) || Novice::CheckHitKey(DIK_S)){
		return 1;
	}else{
		return 0;
	}
}

//右移動キーを押しているか
int IsPressRight(){
	if(Novice::CheckHitKey(DIK_RIGHT) || Novice::CheckHitKey(DIK_D)){
		return 1;
	}else{
		return 0;
	}
}

//左移動キーを押しているか
int IsPressLeft(){
	if(Novice::CheckHitKey(DIK_LEFT) || Novice::CheckHitKey(DIK_A)){
		return 1;
	}else{
		return 0;
	}
}

