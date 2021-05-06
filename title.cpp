//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11B132 14 ゴショウケン
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "setSprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(9)				// テクスチャの数
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_WIDTH_BLOCK			(64)	        // ブロック背景サイズ
#define TEXTURE_HEIGHT_BLOCK		(64)	        // 
#define TEXTURE_BLOCK_SCALE		    (96)	        // 


#define TEXTURE_WIDTH_LOGO			(400)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(154)			// 

#define BLOCK_MOVE_TIME             (TEXTURE_HEIGHT_BLOCK * 2)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/Blue.png",
	"data/TEXTURE/Gray.png",
	"data/TEXTURE/Green.png",
	"data/TEXTURE/Brown.png",
	"data/TEXTURE/Pink.png",
	"data/TEXTURE/Purple.png",
	"data/TEXTURE/Yellow.png",
	"data/TEXTURE/LOGO1.png",
	"data/TEXTURE/pressbutton.png"
};


static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static D3DXVECTOR3				g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

// other
int blockMoveTimer = 0;
int blocktype = 3;
int scalesize = TEXTURE_HEIGHT_BLOCK;

bool ti_canInput = true;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
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
	g_Use = true;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_TexNo = 0;

	//other
	ti_canInput = true;

	// BGM再生
	//PlaySound(SOUND_LABEL_BGM_sample000);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	blockMoveTimer += 1;
	if (blockMoveTimer >= BLOCK_MOVE_TIME) {
		blockMoveTimer = 0;
		blocktype += 1;
		if (blocktype > 6)blocktype = 0;
		// scale
		scalesize = TEXTURE_BLOCK_SCALE;
	}
	if (scalesize > TEXTURE_HEIGHT_BLOCK) {
		scalesize -= 4;
	}


	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START))
	{// Enter押したら、ステージを切り替える
		if (ti_canInput == true) {
			ti_canInput = false;
			SetFade(FADE_OUT, MODE_TUTORIAL);
			//PlaySound(SOUND_LABEL_SE_start);
		}
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
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
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[blocktype]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSprite(g_VertexBuffer, TEXTURE_WIDTH_BLOCK * i, TEXTURE_HEIGHT_BLOCK*j + blockMoveTimer / 2, scalesize, scalesize, 0.0f, 0.0f, 1.0f, 1.0f);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// エンターキー/スタートボタンを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		if (blockMoveTimer <= BLOCK_MOVE_TIME / 2) {
			float a = (float)blockMoveTimer / (float)(BLOCK_MOVE_TIME / 2.0f);
			SetSpriteColor(g_VertexBuffer, SCREEN_WIDTH / 2, 380, TEXTURE_WIDTH_LOGO, 90.0f, 0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, a));
		}
		else {
			float a = (float)blockMoveTimer / (float)(BLOCK_MOVE_TIME / 2.0f) - 1.0f;
			SetSpriteColor(g_VertexBuffer, SCREEN_WIDTH / 2, 380, TEXTURE_WIDTH_LOGO, 90.0f, 0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1 - a));
		}

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	SetLightEnable(true);
	SetDepthEnable(true);
}

