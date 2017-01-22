#pragma once

class XImage
{
public:
	XImage(void);
	XImage(int width,int height);
	XImage(LPCWSTR pFileName);
	XImage(UINT pResourceID, LPCTSTR pResourceType = RT_BITMAP, HMODULE hInstance = NULL);
	~XImage(void);
private:
	//ビットマップ
	Gdiplus::Bitmap* m_bitmap;
	//リソースバッファ
	HGLOBAL m_hResourceBuffer;
protected:
	//ビットマップを取得
	Gdiplus::Bitmap* GetBitmap(void);
public:
	//参照オペレーター
	operator Gdiplus::Bitmap*() const;
	//ファイル名を指定して画像をロード
	bool Load(LPCWSTR pFile);
	//リソースから画像をロード
	virtual bool Load(UINT pResourceID, LPCTSTR pResourseType= RT_BITMAP, HMODULE hInstance = NULL);
	//幅
	virtual int GetWidth(void);
	//高さ
	virtual int GetHeight(void);
};
