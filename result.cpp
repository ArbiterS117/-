//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : GP11B132 14 ゴショウケン
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "setSprite.h"
#include "gameScene.h"
#include "score.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(12)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(400)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(90)			// 

#define TEXTURE_WIDTH_RANKMARKS     (400)			// ランクマークサイズ
#define TEXTURE_HEIGHT_RANKMARKS    (400)			// 

#define TEXTURE_WIDTH_BLOCK			(64)	        // ブロック背景サイズ
#define TEXTURE_HEIGHT_BLOCK		(64)	        // 
#define BLOCK_MOVE_TIME             (TEXTURE_HEIGHT_BLOCK * 2)

#define TEXTURE_WIDTH_TEXT			(24)	        // 数字のテクスチャサイズ
#define TEXTURE_HEIGHT_TEXT			(48)	        // 
#define NUN_MAX			            (99)		    // 数字の最大値
#define NUN_DIGIT			         (2)			// 桁数

#define CONFIRM_TIME                (75)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/Brown.png",
	"data/TEXTURE/result/clearTime.png",
	"data/TEXTURE/result/TFP.png",
	"data/TEXTURE/result/rank.png",
	"data/TEXTURE/result/rankS.png",
	"data/TEXTURE/result/rankA.png",
	"data/TEXTURE/result/rankB.png",
	"data/TEXTURE/result/rankC.png",
	"data/TEXTURE/result/rankD.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/Dust Particle.png",
	"data/TEXTURE/pressbutton.png"
};


static float					g_w, g_h;					// 幅と高さ
static D3DXVECTOR3				g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

//Anim
int re_blockMoveTimer = 0;

//system
int confirmStep = 0; // 1 => clearTime ; 2 => deaths count ; 3 => rank ; 4 => Please Press button (SetMode Enable)
int confirmTimer = 0;
bool re_canInput = true;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーの初期化
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_TexNo = 0;

	//背景

	// BGM再生
	//PlaySound(SOUND_LABEL_BGM_sample002);

	//system
	re_blockMoveTimer = 0;
	confirmStep = 0;
	confirmTimer = 0;
	re_canInput = true;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	//ブラック移動
	re_blockMoveTimer += 1;
	if (re_blockMoveTimer >= BLOCK_MOVE_TIME) {
		re_blockMoveTimer = 0;
	}

	//自動コンフォーム
	confirmTimer += 1;
	if (confirmTimer >= CONFIRM_TIME) {
		confirmTimer = 0;
		confirmStep += 1;
		
	}

	//input
	if (re_canInput == true) {
		if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_B) || IsButtonTriggered(0, BUTTON_X) || IsMouseLeftTriggered() || IsMouseRightTriggered())
		{
			confirmTimer = 0;
			confirmStep += 1;

		}
	}
	// Enter押したら、ステージを切り替える
	
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START))
	{
			
		SetFade(FADE_OUT, MODE_TITLE);
			
	}
	

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{

	// 2D描画なので深度無効
	SetDepthEnable(false);
	SetLightEnable(false);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ブロックの背景を描画
	{
		for (int j = -1; j < SCREEN_HEIGHT / TEXTURE_HEIGHT_BLOCK + 2; j++) {
			for (int i = 0; i < SCREEN_WIDTH / TEXTURE_WIDTH_BLOCK + 1; i++) {
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSprite(g_VertexBuffer, TEXTURE_WIDTH_BLOCK * i, TEXTURE_HEIGHT_BLOCK*j + re_blockMoveTimer / 2, TEXTURE_WIDTH_BLOCK, TEXTURE_HEIGHT_BLOCK, 0.0f, 0.0f, 1.0f, 1.0f);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}

	// デス数を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 150, 1259*0.5, 186*0.5, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// デス数数字を描画
	if (confirmStep > 1)
	{
		// テクスチャ設定
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);
		g_Pos.x = SCREEN_WIDTH / 2 + 100;
		g_Pos.y = 150;
		g_w = TEXTURE_WIDTH_TEXT;
		g_h = TEXTURE_HEIGHT_TEXT;
		// 桁数分処理する


		int num = GetMiss();

		for (int i = 0; i < NUN_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = float(num % 10);

			// 次の桁へ
			num /= 10;

			// スコアの位置やテクスチャー座標を反映
			float px = g_Pos.x - g_w * i;	// プレイヤーの表示位置X
			float py = g_Pos.y;			// プレイヤーの表示位置Y
			float pw = g_w;				// プレイヤーの表示幅
			float ph = g_h;				// プレイヤーの表示高さ

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		//SetVertex(px, py, pw, ph, tx, ty, tw, th);
			SetSprite(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// ポリゴン描画
			//GetDeviceContext()->Draw(4, 0);

		}

	}


	// ランクを描画
	{
		// テクスチャ設定
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2 - 100, 250, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		//GetDeviceContext()->Draw(4, 0);
	}

	// ランクマークを描画
	if (confirmStep > 2)
	{
		// テクスチャ設定
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);
		g_Pos.x = SCREEN_WIDTH / 2 + 100;
		g_Pos.y = 250;
		g_w = TEXTURE_WIDTH_TEXT;
		g_h = TEXTURE_HEIGHT_TEXT;
		// 桁数分処理する


		int num = GetScore();

		for (int i = 0; i < NUN_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = float(num % 10);

			// 次の桁へ
			num /= 10;

			// スコアの位置やテクスチャー座標を反映
			float px = g_Pos.x - g_w * i;	// プレイヤーの表示位置X
			float py = g_Pos.y;			// プレイヤーの表示位置Y
			float pw = g_w;				// プレイヤーの表示幅
			float ph = g_h;				// プレイヤーの表示高さ

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		//SetVertex(px, py, pw, ph, tx, ty, tw, th);
			SetSprite(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// ポリゴン描画
			//GetDeviceContext()->Draw(4, 0);

		}
	}

	// エンターキー/スタートボタンを描画
	if (confirmStep > 3)
	{
		// テクスチャ設定
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		if (re_blockMoveTimer <= BLOCK_MOVE_TIME / 2) {
			float a = (float)re_blockMoveTimer / (float)(BLOCK_MOVE_TIME / 2.0f);
			SetSpriteColor(g_VertexBuffer, SCREEN_WIDTH / 2 - 120, 400, TEXTURE_WIDTH_LOGO * 0.8f, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, a));
		}
		else {
			float a = (float)re_blockMoveTimer / (float)(BLOCK_MOVE_TIME / 2.0f) - 1.0f;
			SetSpriteColor(g_VertexBuffer, SCREEN_WIDTH / 2 - 120, 400, TEXTURE_WIDTH_LOGO * 0.8f, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1 - a));
		}

		// ポリゴン描画
		//GetDeviceContext()->Draw(4, 0);
	}

	//Particle

	SetLightEnable(true);
	SetDepthEnable(true);
}
