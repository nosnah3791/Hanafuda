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

//1�b���ɃJ�E���^�[�����Z�b�g����
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

//���[�g���v������
BOOL TimeControl::Measure(){
	this->m_dwFrameCnt++;
	return this->ResetCounter();
}

//���Ԑ���
void TimeControl::TimeRegular(){
	
	//�����ݒ�
	if(this->m_bInitFlg)
	{
		this->m_dwLastMinitues = timeGetTime();
		this->m_bInitFlg = FALSE;
		this->m_bDrawFlg = TRUE;
		return;
	}
	
	this->m_dwFrameCnt++;

	DWORD frameTime = (DWORD)(this->m_dwFrameCnt * this->m_fFrameTime);
	if(this->m_bSkipFlg && timeGetTime() - this->m_dwLastMinitues > frameTime + this->m_fFrameTime){//1�t���[���ȏ�x��Ă�
		//�`��X�L�b�v
		this->m_bDrawFlg = FALSE;
		this->m_dwSkipCnt++;
	}else{
		//����
		while(timeGetTime() - this->m_dwLastMinitues <= frameTime){
			Sleep(1);//�ҋ@
		}
		this->m_bDrawFlg = TRUE;
	}

	//�J�E���^��1�b�o�ߖ��Ƀ��Z�b�g
	this->ResetCounter();
}

//FPS�ݒ�
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