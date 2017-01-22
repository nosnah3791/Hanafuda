#include "StdAfx.h"
#include "Scene.h"

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

//Œø‰Ê‰¹
void Scene::Sound(BOOL playFlg)
{
	if(this->m_sound){
		if(playFlg) this->m_sound->Play();
		else this->m_sound->Stop();
	}
}

//ˆêŽž’âŽ~
BOOL Scene::Stop(void)
{
	if(this->m_uiStopCnt){
		this->m_uiStopCnt--;
		return TRUE;
	}
	return FALSE;
}
