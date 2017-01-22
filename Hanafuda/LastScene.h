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
		FADECNT = 40 //�t�F�[�h�A�E�g�t���[����
	};
	//�t���[��No.
	UINT m_uiFrameNo;
	//���C���[
	Layer* m_layer;
private:
	//�Đ�
	BOOL Play(void);
public:
	//������
	void Init(void);
	//���Z�b�g
	void Reset(void);
	// �}�E�X�ړ����̏���
	BOOL MouseMove(double x, double y);
	// �}�E�X�N���b�N���̏���
	BOOL MouseClick(double x, double y);
	// �D�𓮂���
	void Move(void);
	// �`��
	void Paint(void);
};
