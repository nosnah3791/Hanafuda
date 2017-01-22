#include "StdAfx.h"
#include "TimeControl.h"
#pragma comment(lib,"winmm.lib")

TimeControl::TimeControl(DWORD dwFps, BOOL bSkipFlg)
{
	this->m_bInitFlg = TRUE;
	this->m_bSkipFlg = bSkipFlg;
	this->SetupFPS(dwFps);
	this->m_dwLastMinitues = 0;
	this->m_dwFrameCnt = 0;
	timeBeginPeriod(1);
}


TimeControl::~TimeControl(void)
{
	timeEndPeriod(1);
}

//1秒毎にカウンターをリセットする
BOOL TimeControl::ResetCounter(BOOL stopFlg)
{
	if(stopFlg || (timeGetTime() - this->m_dwLastMinitues >= 1000))
	{
		this->m_dwLastMinitues = timeGetTime();
		this->m_dwFrameRate = this->m_dwFrameCnt;
		this->m_dwFrameCnt = 0;
		this->m_dwSkipRate = this->m_dwSkipCnt;
		this->m_dwSkipCnt = 0;
		this->m_bDrawFlg = FALSE;
		return TRUE;
	}
	return FALSE;
}

//レートを計測する
BOOL TimeControl::Measure(){
	this->m_dwFrameCnt++;
	return this->ResetCounter();
}

//時間制御
void TimeControl::TimeRegular(){
	
	//初期設定
	if(this->m_bInitFlg)
	{
		this->m_dwLastMinitues = timeGetTime();
		this->m_bInitFlg = FALSE;
		this->m_bDrawFlg = TRUE;
		return;
	}
	
	this->m_dwFrameCnt++;

	DWORD frameTime = (DWORD)(this->m_dwFrameCnt * this->m_fFrameTime);
	if(this->m_bSkipFlg && timeGetTime() - this->m_dwLastMinitues > frameTime + this->m_fFrameTime){//1フレーム以上遅れてる
		//描画スキップ
		this->m_bDrawFlg = FALSE;
		this->m_dwSkipCnt++;
	}else{
		//調整
		while(timeGetTime() - this->m_dwLastMinitues <= frameTime){
			Sleep(1);//待機
		}
		this->m_bDrawFlg = TRUE;
	}

	//カウンタを1秒経過毎にリセット
	this->ResetCounter();
}

//FPS設定
void TimeControl::SetupFPS(DWORD fps)
{
	this->m_fFrameTime = 1000.0f/fps;
}

void TimeControl::SetSkipFlg(BOOL bSkipFlg)
{
	this->m_bSkipFlg = bSkipFlg;
}

DWORD TimeControl::GetFrameRate()
{
	return this->m_dwFrameRate;
}

DWORD TimeControl::GetSkipRate()
{
	return this->m_dwSkipRate;
}

BOOL TimeControl::GetDrawFlg()
{
	return this->m_bDrawFlg;
}