//==================================
// ���b�V���X�t�B�A�̍쐬
// Author: ������
//==================================
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPHERE_RADIUS	(100.0f)													// �����a
#define SPHERE_SIZE		(D3DXVECTOR3(SPHERE_RADIUS,SPHERE_RADIUS,SPHERE_RADIUS))	// ���T�C�Y

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CMeshSphere : public CScene, public CMesh
{
public:
	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMeshSphere(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshSphere();

	//--------------------------------------------
	// ���\�b�h
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// �ݒ�E�擾����
	//--------------------------------------------
	void SetScale(D3DXVECTOR3 scale)				{ m_scale = scale; }				// �X�P�[���̐ݒ�
	D3DXVECTOR3 GetScale()							{ return m_scale; }

	bool SphereCollisionSphere(float fRadius, CScene *pScene);
	static CMeshSphere *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical,bool bInsideDraw, CScene::OBJTYPE objtype);	// �V�[��3D�̐���
	bool LineCollisionMesh(CScene *pScene, const int *nVtx);

private:
	D3DXVECTOR3 m_scale;										// �X�P�[��
	bool m_bInsideDraw;											// �����ɕ`�悷�邩
	CScene::OBJTYPE objtype;									// �I�u�W�F�N�g�^�C�v
};


#endif


