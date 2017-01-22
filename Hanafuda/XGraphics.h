#pragma once

class XGraphics
{
public:
	XGraphics(void);
	XGraphics(HWND hWnd);
	XGraphics(HWND hWnd, int width, int height);
	~XGraphics(void);
private:
	//ウィンドウ幅
	UINT m_uiWidth;
	//ウィンドウ高さ
	UINT m_uiHeight;
	//オンスクリーン
	Graphics* m_gOnScreen;
	//オフスクリーンビットマップ
	Bitmap* m_offScreenBmp;
public:
	enum{
		BLEND_ALPHA = 1//ブレンド種別(アルファ)
	};
	//ウィンドウハンドル
	HWND m_hWnd;
	//オフスクリーン
	Graphics* m_gOffScreen;
private:
	//ブレンド定義関数
	byte (XGraphics::*m_funcBlend)(UINT,UINT,UINT);
	//アルファブレンドする
	byte BlendAlpha(UINT dstByte, UINT srcByte, UINT alpha);
public:
	//画面更新
	Status Update(HDC hdc);
	//詳細設定
	void SetProperty(HWND hWnd);
	//二つの画像をブレンドする
	void BlendImages(INT x, INT y, int blendFunc,Bitmap* srcBitmap, Bitmap* dstBitmap=NULL);

};
