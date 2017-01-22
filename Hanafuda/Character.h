#pragma once

class Character:public XImage
{
public:
	Character(void):  m_bExistFlg(FALSE)
					, m_bInvisibleFlg(FALSE)
					, _x(0)
					, _y(0)
					, _width(0)
					, _height(0)
					, m_dScale(1.0)
    {
		this->m_uiCID = ++Character::uiCID;

	}
	Character(int width,int height);
	~Character(void);
private:
	// 存在フラグ
	BOOL m_bExistFlg;
	//不可視フラグ
	BOOL m_bInvisibleFlg;
	//拡大縮小指数
	double m_dScale;
	//幅
	UINT _width;
	//高さ
	UINT _height;
public:
	static UINT uiCID;//シリアルキャラクターID
	//座標
	UINT _x;
	UINT _y;
protected:
	//キャラクターID
	UINT m_uiCID;
	//RGBA値
	float m_fRGBA[4];
	//存在フラグ
	BOOL GetExistFlg(void);
	//特殊描画
	void ExtraPaint(Graphics* g,float red,float green,float blue,float alpha);
	//ブレンド描画
	void BlendPaint(void);
public:
	//リソースから画像をロード
	bool Load(UINT pResourceID, LPCTSTR pResourseType= RT_BITMAP, HMODULE hInstance = NULL);
	//画像矩形
	RECT returnRect(void);
	// 座標設定
	void SetPos(Point pt);
	// 座標＆サイズ設定
	void SetPos(Point pt,double scale);
	// 座標取得
	Point GetPos(void);
	//幅
	int GetWidth(void);
	//高さ
	int GetHeight(void);	
	// 初期化
	virtual void Init(void);
	// 初期化
	virtual void Init(UINT _x, UINT _y);
	// 初期化
	virtual void Init(Point pt);
	// 描画
	void Paint(Graphics* g);
	//中央に配置
	virtual void Centering(UINT _x,UINT _y,UINT width=0,UINT height=0);
	// リセット
	virtual void Reset(void);
	// 同位置に置き換える
	virtual void Trace(Character* lpch);
	//可視状態を切り替え
	void SetupInvisible(BOOL flag);
	//拡大縮小指数
	void SetScale(double scale);
	//RGBA値を設定
	void SetRGBA(float red,float green,float blue,float alpha);
};
