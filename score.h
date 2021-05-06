﻿//=============================================================================
//
// スコア処理 [score.h]
// Author : GP11B132 14 ゴショウケン
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX			(99)		// スコアの最大値
#define SCORE_DIGIT			(2)			// 桁数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore(int add);
void AddCoin(int add);
void AddMiss(int add);
int GetScore();
int GetMiss();

void showSign(int i);


