#pragma once
#include "..\Actor.h"
#include "..\..\..\..\Global.h"
#include "..\..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	�ۉe(�X�v���C�g)�N���X.
*		���c�F�P.
**/
class CRoundShadow final
	: public CActor
{
public:
	CRoundShadow( CActor* pObj, CStaticMesh* pModel, float vScale = 1.0f );
	virtual ~CRoundShadow();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

private:
	// �����̉e�̃T�C�Y���擾.
	D3DXSCALE3 GetShadowScale( CStaticMesh* pModel, float vScale );

private:
	CSprite*			m_pSpriteShadow;	// �ۉe�̉摜.
	SSpriteRenderState	m_ShadowState;		// �ۉe�̏��.
	CActor*				m_pObject;			// �e��t����I�u�W�F�N�g.

private:
	// �f�t�H���g�R���X�g���N�^�֎~.
	CRoundShadow() = delete;
	CRoundShadow( const CRoundShadow& ) = delete;
	CRoundShadow& operator = ( const CRoundShadow& rhs ) = default;
};