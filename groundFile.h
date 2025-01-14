﻿/*******************************************************************************
* タイトル:		groundFile
* プログラム名:	groundFile.h
* 作成者:		GP11B132 14 ゴショウケン
*******************************************************************************/
#ifndef FILE_H
#define FILE_H

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "platform.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/



/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct	GROUNDSAVEDATA				// セーブデータの構造体
{
	// プレイヤーデータを保存
	//PLAYER player[MAX_PLAYER];	// プレイヤーのMAX人数分セーブする

	// エネミーデータを保存
	//ENEMY   enemy[MAX_ENEMY];	// エネミーのMAX人数分セーブする

	// 何かセーブしたい物があればここに追加してみよう
	PLATFORM platform[MAX_PLATFORM];

};



/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void SaveGroundData(void);
bool LoadGroundData(void);


#endif
