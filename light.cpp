﻿//=============================================================================
//
// ライト処理 [light.cpp]
// Author :  GP11B132 14 ゴショウケン
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;


//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{

	//ライト初期化
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Light[i].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		g_Light[i].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		g_Light[i].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Light[i].Attenuation = 300.0f;	// 減衰距離
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ライトのタイプ
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	g_Light[0].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	g_Light[0].Diffuse = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.0f);
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;
	g_Light[0].Enable = true;
	SetLight(0, &g_Light[0]);



	// フォグの初期化
	g_Fog.FogStart = 100.0f;
	g_Fog.FogEnd = 12000.0f;
	g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetFog(&g_Fog);
	SetFogEnable(true);

}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{



}


//=============================================================================
// ライトの設定
// Typeによってセットするメンバー変数が変わってくる
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// フォグの設定
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}



