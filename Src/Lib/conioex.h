#pragma once
/**
* @file	conioex.h
* @brief	Win32 コンソール I/O 拡張 for Visual C++/Borland C++
*
* @date	2019/10/15 2019年度初版
* @version	1.0
*		2022/10/01
*			グローバル変数の名前の先頭には'g_'を付加するように変更した。
*			「拡張文字列処理(マルチバイト/Unicode対応)」追加
*				半角⇒全角変換関数（HanToZen）追加。
*				書式指定で全角文字出力する関数(PrintStringF)の追加。
* @version	1.1
*		2023/12/1
*           ClearFrameBuffer()
*           PrintFrameBuffer()
*       
* @note
*  コンソールウィンドウのプロパティの設定を以下に変更すること
*  ・「従来のコンソールを使う」のチェックを外す
*  ・「簡易編集モード」のチェックを外す
*
* コンソール関数について
* https://learn.microsoft.com/ja-jp/windows/console/console-functions
*/

#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <mbstring.h>
#include <process.h>
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <digitalv.h>
#include <xinput.h>
#include <CRTDBG.H>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xinput.lib")
#pragma warning(disable:4996)

#define NOCURSOR		0
#define SOLIDCURSOR		1
#define NORMALCURSOR	2

#define DEF_FONTSIZE_X		16
#define DEF_FONTSIZE_Y		16

// https://en.wikipedia.org/wiki/Color_Graphics_Adapter
enum COLORS {
	BLACK,			/* #000000	黒				*/
	BLUE,			/* #0000AA	青				*/
	GREEN,			/* #00AA00	緑				*/
	CYAN,			/* #00AAAA	シアン			*/
	RED,			/* #AA0000	赤				*/
	MAGENTA,		/* #AA00AA	マゼンタ		*/
	BROWN,			/* #AA5500	茶				*/
	LIGHTGRAY,		/* #AAAAAA	明るい灰色		*/
	DARKGRAY,		/* #555555	暗い灰色		*/
	LIGHTBLUE,		/* #5555FF	明るい青		*/
	LIGHTGREEN,		/* #55FF55	明るい緑		*/
	LIGHTCYAN,		/* #55FFFF	明るいシアン	*/
	LIGHTRED,		/* #FF5555	明るい赤		*/
	LIGHTMAGENTA,	/* #FF55FF	明るいマゼンタ	*/
	YELLOW,			/* #FFFF55	黄				*/
	WHITE			/* #FFFFFF	白				*/
};

/* キーコード */
#define PK_ENTER				VK_RETURN
#define PK_ESC					VK_ESCAPE
#define PK_F1					VK_F1
#define PK_F2					VK_F2
#define PK_F3					VK_F3
#define PK_F4					VK_F4
#define PK_F5					VK_F5
#define PK_F6					VK_F6
#define PK_F7					VK_F7
#define PK_F8					VK_F8
#define PK_F9					VK_F9
#define PK_F10					VK_F10
#define PK_F11					VK_F11
#define PK_F12					VK_F12
#define PK_SP					VK_SPACE
#define PK_UP					VK_UP
#define PK_DOWN					VK_DOWN
#define PK_RIGHT				VK_RIGHT
#define PK_LEFT					VK_LEFT
#define PK_SHIFT				VK_SHIFT
#define PK_CTRL					VK_CONTROL
#define PK_ALT					VK_MENU
#define PK_BS					VK_BACK
#define PK_PAUSE				VK_PAUSE
#define PK_INS					VK_INSERT
#define PK_DEL					VK_DELETE
#define PK_TAB					VK_TAB
#define PK_NFER					VK_KANA		/* [無変換]	*/
#define PK_XFER					VK_CONVERT	/* [変換]	*/
#define PK_0					0x30
#define PK_1					0x31
#define PK_2					0x32
#define PK_3					0x33
#define PK_4					0x34
#define PK_5					0x35
#define PK_6					0x36
#define PK_7					0x37
#define PK_8					0x38
#define PK_9					0x39
#define PK_NUMPAD0				VK_NUMPAD0	/* テンキーの[0]	*/
#define PK_NUMPAD1				VK_NUMPAD1	/* テンキーの[1]	*/
#define PK_NUMPAD2				VK_NUMPAD2	/* テンキーの[2]	*/
#define PK_NUMPAD3				VK_NUMPAD3	/* テンキーの[3]	*/
#define PK_NUMPAD4				VK_NUMPAD4	/* テンキーの[4]	*/
#define PK_NUMPAD5				VK_NUMPAD5	/* テンキーの[5]	*/
#define PK_NUMPAD6				VK_NUMPAD6	/* テンキーの[6]	*/
#define PK_NUMPAD7				VK_NUMPAD7	/* テンキーの[7]	*/
#define PK_NUMPAD8				VK_NUMPAD8	/* テンキーの[8]	*/
#define PK_NUMPAD9				VK_NUMPAD9	/* テンキーの[9]	*/
#define PK_A					0x41
#define PK_B					0x42
#define PK_C					0x43
#define PK_D					0x44
#define PK_E					0x45
#define PK_F					0x46
#define PK_G					0x47
#define PK_H					0x48
#define PK_I					0x49
#define PK_J					0x4A
#define PK_K					0x4B
#define PK_L					0x4C
#define PK_M					0x4D
#define PK_N					0x4E
#define PK_O					0x4F
#define PK_P					0x50
#define PK_Q					0x51
#define PK_R					0x52
#define PK_S					0x53
#define PK_T					0x54
#define PK_U					0x55
#define PK_V					0x56
#define PK_W					0x57
#define PK_X					0x58
#define PK_Y					0x59
#define PK_Z					0x5A

#define PK_LT					0x0BC		/* [,]	*/
#define PK_GT					0x0BE		/* [.]	*/
#define PK_SLUSH				0x0BF		/* [?]	*/
#define PK_DOT					VK_DECIMAL	/* テンキーの[.]	*/
#define PK_DIV					VK_DIVIDE	/* テンキーの[/]	*/
#define PK_BSLUSH				0x0E2		/* [_]	*/

#define PK_SEMICOLON			0x0BB		/* [;]	*/
#define PK_ADD					VK_ADD		/* テンキーの[+]	*/
#define PK_COLON				0x0BA		/* [:]	*/
#define PK_MUL					VK_MULTIPLY	/* テンキーの[*]	*/
#define PK_RBRACE				0x0DD		/* []]	*/

#define PK_ATMARK				0x0C0		/* [@]	*/
#define PK_LBRACE				0x0DB		/* [[]	*/

#define PK_MINUS				0x0BD		/* [-]	*/
#define PK_SUB					VK_SUBTRACT	/* テンキーの[-]	*/
#define PK_XOR					0x0DE		/* [^]	*/
#define PK_YEN					0x0DC		/* [\]	*/

#define PK_KANJI				0x0F3		/* [半角/全角]	*/
#define PK_CAPS					0x0F0		/* [英数][ひらがな]	*/

#define PM_LEFT					VK_LBUTTON
#define PM_MID					VK_MBUTTON
#define PM_RIGHT				VK_RBUTTON
#define PM_CURX					0x0101
#define PM_CURY					0x0102

#define PJ1_XPOS				0x0200
#define PJ1_YPOS				0x0201
#define PJ1_ZPOS				0x0202
#define PJ1_BTNS				0x0203
#define PJ2_XPOS				0x0210
#define PJ2_YPOS				0x0211
#define PJ2_ZPOS				0x0212
#define PJ2_BTNS				0x0213
#define PJ3_XPOS				0x0220
#define PJ3_YPOS				0x0221
#define PJ3_ZPOS				0x0222
#define PJ3_BTNS				0x0223
#define PJ4_XPOS				0x0230
#define PJ4_YPOS				0x0231
#define PJ4_ZPOS				0x0232
#define PJ4_BTNS				0x0233
#define PJ_XPOS					PJ1_XPOS
#define PJ_YPOS					PJ1_YPOS
#define PJ_ZPOS					PJ1_ZPOS
#define PJ_BTNS					PJ1_BTNS

#define PJX1_LXPOS				0x0200
#define PJX1_LYPOS				0x0201
#define PJX1_LTRG				0x0202
#define PJX1_RXPOS				0x0203
#define PJX1_RYPOS				0x0204
#define PJX1_RTRG				0x0205
#define PJX1_BTNS				0x0206
#define PJX2_LXPOS				0x0210
#define PJX2_LYPOS				0x0211
#define PJX2_LTRG				0x0212
#define PJX2_RXPOS				0x0213
#define PJX2_RYPOS				0x0214
#define PJX2_RTRG				0x0215
#define PJX2_BTNS				0x0216
#define PJX3_LXPOS				0x0220
#define PJX3_LYPOS				0x0221
#define PJX3_LTRG				0x0222
#define PJX3_RXPOS				0x0223
#define PJX3_RYPOS				0x0224
#define PJX3_RTRG				0x0225
#define PJX3_BTNS				0x0226
#define PJX4_LXPOS				0x0230
#define PJX4_LYPOS				0x0231
#define PJX4_LTRG				0x0232
#define PJX4_RXPOS				0x0233
#define PJX4_RYPOS				0x0234
#define PJX4_RTRG				0x0235
#define PJX4_BTNS				0x0236
#define PJX_LXPOS				PJX1_LXPOS
#define PJX_LYPOS				PJX1_LYPOS
#define PJX_LTRG				PJX1_LTRG
#define PJX_RXPOS				PJX1_RXPOS
#define PJX_RYPOS				PJX1_RYPOS
#define PJX_RTRG				PJX1_RTRG
#define PJX_BTNS				PJX1_BTNS

#define	NUM_KEYS	(256)	//キーバッファに読み取るキーの数
#define	NUM_PALETTE	(16)	//カラーパレット数（画面の色数：４ビットカラー）
#define	NUM_ANSI_PAL	(256)	//ANIS 256 colors.

//LoadBmpで読み込んだ画像データを格納する構造体。
struct Bmp {
	int width;		//画像の横方向ピクセル数
	int height;		//画像の縦方向ピクセル数
	int colbit;		//各ピクセルの色ビット数
	int numpal;		//パレットの色数
	int numpix;		//画像データのバイト数
	bool swapRB;	//RとBが入れ替えられている場合は'TRUE'になる
	COLORREF* pal;		//パレット（16色）データへのポインタ※無い場合はNULL
	char* pixel;	//画像データへのポインタ
	//以下はBiitmap文字用
	int aaLv;		//諧調数（アンチエイリアスレベル）Win32APIのGetGlyphOutline関数参照
	//GGO_BITMAP：２色：２値の場合は{0x0|0xF}⇒０が透明、Fが不透明
	//GGO_GRAY2_BITMAP：５色
	//GGO_GRAY4_BITMAP：１７色
	//GGO_GRAY8_BITMAP：６５色
	wchar_t	wch;	//変換元の文字
	//Bmp* _next;	//リスト構造用＜工事中＞
};
//----------------------------------------------------------------
// 初期化
void	InitConio(int _width, int _height);	//コンソール I / O 初期化
void	InitConioEx(int _width, int _height, int _font_w, int _font_h);	//コンソール I / O 初期化（拡張版）
void	InitConioEx(int _width, int _height, int _font_w, int _font_h, bool _init_wbuf = true);	//コンソール I / O 初期化（拡張版）
void	InitConioEx(int _width, int _height, int _font_w, int _font_h, const wchar_t* _font_face_name, const COLORREF* _pal16, bool _init_wbuf = true);	//コンソール I / O 初期化（拡張版）
void	EndConioEx(void);	//conioexの終了処理
int		InitDoubleBuffer(void);	//ダブルバッファ初期化
void	FlipScreen(void);		//ダブルバッファ時の描画面切替
void	SetScreenFontSize(int width, int height);	//フォントサイズ変更
void	ClearScreen(void);				//画面（スクリーンバッファ）消去
//----------------------------------------------------------------
//ウィンドウ

void CCC(char*, int, int);

LONG	FixWin(void);	//ウィンドウサイズ固定(変更前の状態を返す)
HANDLE	GetCurrentHandle(void);	//現在アクセス中のバッファのハンドル。
void	SetCaption(const char* title);	//コンソールウィンドウのタイトルバーにテキストを設定
void	SetCaptionF(const char* _format, ...);	//コンソールウィンドウのタイトルバーに書式指定してテキストを設定
int		GetCaption(char* title, int len);	//コンソールウィンドウのタイトルバーに表示されるテキストを取得
RECT& GetConWinSize(RECT& _r);	//クライアント領域のサイズとフォントサイズの取得
//----------------------------------------------------------------
//カーソル
int		GetCursorX(void);				//水平方向のカーソル位置を取得
int		GetCursorY(void);				//垂直方向のカーソル位置を取得
void	SetCursorPosition(int x, int y);	//カーソル位置の移動
void	SetCursorType(int type);		//カーソルタイプ設定
POINT	GetCursorMousePos(POINT* _mp);	//マウス座標(文字単位)の取得
//----------------------------------------------------------------
//文字列描画
void	PrintStringA(const char* _srcbuf, int _size);	//文字列の出力（マルチバイト文字用）
void	PrintStringW(const wchar_t* _srcbuf, int _size);	//文字列の出力（Unicode文字用）
#ifdef UNICODE
void	PrintString(const char* _srcbuf, int _size);	// 文字列の出力(マルチバイト⇒Unicode変換版)
#else
#define	PrintString	PrintStringA
#endif // UNICODE
//文字全体
void	SetHighVideoColor(void);		//文字色高輝度化
void	SetLowVideoColor(void);			//文字色低輝度化
void	SetNormalVideoColor(void);		//既定文字色設定
//文字属性指定
void	SetTextBackColor(int color);	//文字背景色設定
void	SetConsoleTextColor(int color);	//文字色設定
void	SetTextAttribute(int attribute);	//文字色背景色同時設定
//行操作
void	ClearLine(void);				//行末まで消去
void	InsertLine(void);	//現在行に挿入
void	DeleteLine(void);	//現在行の削除
//----------------------------------------------------------------
//拡張文字列描画
char* HanToZenA(const char* _src);	//半角文字を全角文字に変換する（マルチバイト版）
wchar_t* HanToZenW(const wchar_t* _src);	//半角文字を全角文字に変換する（Unicode版）
void	VPrintStringFA(bool _zenkaku, const char* _format, va_list _ap);	//書式指定付文字列描画（引数リスト版）（マルチバイト文字用）
void	VPrintStringFW(bool _zenkaku, const wchar_t* _format, va_list _ap);	//書式指定付文字列描画（引数リスト版）（Unicode文字用）
void	PrintStringFA(bool _zenkaku, const char* _format, ...);	//書式指定付文字列描画（マルチバイト文字用）
void	PrintStringFW(bool _zenkaku, const wchar_t* _format, ...);	//書式指定付文字列描画（Unicode文字用）
void	PosPrintStringFA(int _xp, int _yp, bool _zenkaku, const char* _format, ...);	//位置指定＆書式指定付文字列描画（マルチバイト文字用）
void	PosPrintStringFW(int _xp, int _yp, bool _zenkaku, const wchar_t* _format, ...);	//位置指定＆書式指定付文字列描画（Unicode文字用）
void	DrawStringFA(int _x, int _y, bool _zenkaku, const char* _format, ...);	//座標指定（画面左上隅が(0, 0)座標）＋書式指定付文字列描画（マルチバイト文字用）
void	DrawStringFW(int _x, int _y, bool _zenkaku, const wchar_t* _format, ...);	//座標指定（画面左上隅が(0, 0)座標）＋書式指定付文字列描画（Unicode文字用）
// 以下の「#define」はプロジェクトが(マルチバイト／Unicode)に関わらず、
// 常にマルチバイト版（末尾'A'）の関数名を使うようにする為の定義。
// ※Unicode版を使うときは明示的に末尾'W'の関数名を呼び出す。
#ifdef UNICODE
#define	HanToZen	HanToZenA
#define	PrintStringF	PrintStringFA
#define	PosPrintStringF	PosPrintStringFA
#define	DrawStringF		DrawStringFA
#else	// UNICODE
#define	HanToZen	HanToZenA
#define	PrintStringF	PrintStringFA
#define	PosPrintStringF	PosPrintStringFA
#define	DrawStringF		DrawStringFA
#endif // UNICODE
//----------------------------------------------------------------
// パレット関係
COLORREF	RGBQtoCREF(RGBQUAD rgb);	//RGBQUAD: {B, G, B, 0}型をCOLORREF:0x00BBGGRR型に変換
RGBQUAD		CREFtoRGBQ(COLORREF ref);	//COLORREF:0x00BBGGRR型をRGBQUAD : {B, G, B, 0}型に変換
COLORREF* ConvRGBQtoCREF(const RGBQUAD* _rgb, COLORREF* _cref);	//パレット変換：RGBQ[16]->COLORREF[16]
RGBQUAD* ConvCREFtoRGBQ(const COLORREF* _cref, RGBQUAD* _rgb);	//パレット変換：RGBQ[16]->COLORREF[16]
void	SetPalette(const COLORREF* _pal16, int _p1, int _p2);	//１６色パレット設定
void	SetPalette16(const COLORREF* _pal16);	//１６色パレット設定
void	SetPalette(const Bmp* _pBmp);	//Bmpから１６色パレット設定
//----------------------------------------------------------------
//フレームバッファ画像描画
extern char* g_FrameBuffer4bit;
void	ClearScreenBuffer(const char _code = 0);	//スクリーンバッファの消去
void	ClearFrameBuffer(char* buf);	//「フレームバッファ(16色)」クリア
inline void	ClearFrameBuffer(void) { ClearFrameBuffer(g_FrameBuffer4bit); }	//「フレームバッファ(16色)」クリア
void	PrintFrameBuffer(const char* buf);	//「フレームバッファ(16色)」の一括転送
inline void	PrintFrameBuffer(void) { PrintFrameBuffer(g_FrameBuffer4bit); }	//「フレームバッファ(16色)」の一括転送
void	DrawPixel(int _x, int _y, int _c);	//点を打つ
void	DrawBmp(int _xp, int _yp, const Bmp* _bmp, bool _tr = true);	//Bmp画像の出力
inline void	DrawBmpCP(int _cx, int _cy, const Bmp* _bmp, bool _tr = true) {
	int xp = _cx - _bmp->width / 2;
	int yp = _cy - _bmp->height / 2;
	DrawBmp(xp, yp, _bmp, _tr);;
}	//Bmp画像の中心座標で出力
//----------------------------------------------------------------
// ビットマップ(bmp)ファイル
Bmp* LoadBmp(const char* _file_name, bool _palset_or_swap24RB = false);	//BMPファイルの読み込み。
void	DeleteBmp(Bmp** _p_bmp);	//使い終わったBMP画像の削除。
void	Bmp24SwapRB(Bmp* _bmp);	//BRGのピクセルをRGBに変換する。
//----------------------------------------------------------------
//ビットマップ文字（文字をBmp画像として生成）
Bmp* CreateBmpChar(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, const TCHAR* _text);	//文字列をBmp画像の配列として生成する。
Bmp* CreateBmpString(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, const TCHAR* _text);	//文字列を１枚の画像として生成する。
Bmp* CreateBmpStringF(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, bool _zenkaku, const TCHAR* _format, ...);	//書式指定した文字列を１枚の画像として生成する。
//----------------------------------------------------------------
// キー入力関係
void	ResetKeyMap(void);	//キー情報リセット
int		InputKeyMouse(int port);	//キーボード・マウス入力
//----------------------------------------------------------------
// 拡張キー入力
SHORT	GetKey(int _vk);	//単一キーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
SHORT	WaitKey(int _vk);	//キー入力を待つ。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
int		GetKeyEx(int _vk, int _chktype);	//全てのキーの入力と判定
void	GetKeyAll(void);	//全てのキーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
int		ChkKeyEdge(int _vk);	//Edgeキー入力判定：GetKeyAll()で入力したキー情報についてEdgeキー（トリガーキー）入力判定する
int		ChkKeyPress(int _vk);	//Pressキー入力判定：GetKeyAll()で入力したキー情報についてPressキー（押下キー）入力判定する
//----------------------------------------------------------------
// ジョイパッド入力
int		InputJoystick(int port);	//ジョイパッド入力
#if FALSE
int		InputJoystickX(int port);					// ゲームパッド入力(XInput対応)
#else	//FALSE
int		InputJoystickX(int id, int port);			// ゲームパッド入力(XInput対応)
#endif // FALSE
//----------------------------------------------------------------
// サウンド再生
int* MciOpenSound(const char* path);	//サウンド ファイルを開く
void	MciCloseSound(int* sound_id);	//サウンド ファイルを閉じる
void	MciPlaySound(int* sound_id, int repeat);	//サウンドを再生する
void	MciStopSound(int* sound_id);	//サウンド再生を停止する
int		MciCheckSound(int* sound_id);	//サウンド再生状態の取得
void	MciUpdateSound(int* sound_id);	//ループ再生の強制更新
void	MciSetVolume(int* sound_id, int percent);	//再生音量を設定する
//----------------------------------------------------------------
// 24ビット色画像
extern char* g_FrameBufferFull;
void	PrintImage(const char* _buf);	//フルカラー(24ビット/Pixel)画像の表示（画面のサイズと同じ画像を出力する）
inline void	PrintImage(void) { PrintImage(g_FrameBufferFull); };	//フルカラー(24ビット/Pixel)画像の表示（画面のサイズと同じ画像を出力する）
void	ClearFrameBufferFull(char* buf);	//フレームバッファ(フルカラー(24ビット/Pixel))クリア
inline void	ClearFrameBufferFull(void) { ClearFrameBufferFull(g_FrameBufferFull); }	//フレームバッファ(フルカラー(24ビット/Pixel))クリア
////----------------------------------------------------------------
////　256色画像【工事中】仕様が変わる可能性が大きい
//void	InitPaletteANSI256(void);	//２５６パレット初期化
//void	SetPixel256(int _x, int _y, int _palidx);	//１ピクセル/２５６色描画
//void	SetPixelBuffer256(int _x, int _y, int _palidx);
//void	DrawPixelBuffer256(int _xp, int _yp, int _w, int _h, BYTE* _buf);

/**
* @copyright (c) 2018-2023 HAL Osaka College of Technology & Design (Ihara, H.)
*/
