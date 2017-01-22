#pragma once
#include "scene.h"
#include "layer.h"

class LastScene :
	public Scene
{
private:
	LastScene(void);
	LastScene(const LastScene& lastScene) {}
	LastScene& operator=(const LastScene& lastScene) {}
public:
	static LastScene* GetInstance(void){
		static LastScene lastScene;
		return &lastScene;
	}
	~LastScene(void);
private:
	enum{
		FADECNT = 40 //フェードアウトフレーム数
	};
	//フレームNo.
	UINT m_uiFrameNo;
	//レイヤー
	Layer* m_layer;
private:
	//再生
	BOOL Play(void);
public:
	//初期化
	void Init(void);
	//リセット
	void Reset(void);
	// マウス移動時の処理
	BOOL MouseMove(double x, double y);
	// マウスクリック時の処理
	BOOL MouseClick(double x, double y);
	// 札を動かす
	void Move(void);
	// 描画
	void Paint(void);
};
