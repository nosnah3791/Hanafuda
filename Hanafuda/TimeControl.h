#pragma once

class TimeControl
{
public:
	TimeControl(DWORD dwFps = 30, BOOL bSkipFlg = TRUE);
	~TimeControl(void);
private:
	DWORD m_dwLastMinitues;//最終時刻
	DWORD m_dwFrameRate;//フレームレート(フレーム数/1秒)
	DWORD m_dwSkipRate;//スキップレート(スキップ数/1秒)
	float m_fFrameTime;//フレームタイム(マイクロ秒/1フレーム)
	BOOL m_bDrawFlg;//描画フラグ
	BOOL m_bSkipFlg;//スキップフラグ		
	BOOL m_bInitFlg;//初期フラグ
	DWORD m_dwFrameCnt;//フレーム計測数
	DWORD m_dwSkipCnt;//スキップ計測数
public:
	BOOL Measure();//レートを計測する
	void TimeRegular();//時間制御
	void SetupFPS(DWORD fps);//FPS設定
	BOOL ResetCounter(BOOL stopFlg=FALSE);//一秒毎にカウンタをリセットする

	void SetSkipFlg(BOOL bSkipFlg);
	DWORD GetFrameRate();
	DWORD GetSkipRate();
	BOOL GetDrawFlg();

};
