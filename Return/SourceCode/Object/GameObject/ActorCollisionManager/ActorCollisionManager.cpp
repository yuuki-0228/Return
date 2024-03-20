#include "ActorCollisionManager.h"
#include "..\Actor\Actor.h"

namespace{
	const int OBJECT_TAG_MIN = static_cast<int>( EObjectTag::None );	// �I�u�W�F�N�g�^�O�̍ŏ��l.
	const int OBJECT_TAG_MAX = static_cast<int>( EObjectTag::Max );		// �I�u�W�F�N�g�^�O�̍ő�l.
	const std::vector<bool> INIT_LIST( OBJECT_TAG_MAX, false );			// �������p���X�g.
}

CActorCollisionManager::CActorCollisionManager()
	: m_HitActorList	()
	, m_BeAHitActorList	()
	, m_SkipTag			()
	, m_SkipTagToTag	()
{
	// �I�u�W�F�N�g�^�O���z���p��.
	m_SkipTag.resize( OBJECT_TAG_MAX );
	m_SkipTagToTag.resize( OBJECT_TAG_MAX );
	for ( auto& t : m_SkipTagToTag ) t.resize( OBJECT_TAG_MAX );
}

CActorCollisionManager::~CActorCollisionManager()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CActorCollisionManager* CActorCollisionManager::GetInstance()
{
	static std::unique_ptr<CActorCollisionManager> pInstance = std::make_unique<CActorCollisionManager>();
	return pInstance.get();
}

//---------------------------.
// �����蔻��.
//---------------------------.
void CActorCollisionManager::Collision()
{
	CActorCollisionManager* pI = GetInstance();

	// �����蔻����s���I�u�W�F�N�g���Ȃ��ꍇ.
	//	�s���K�v���Ȃ����ߔ�����.
	if ( pI->m_HitActorList.empty() ||
		 pI->m_BeAHitActorList.empty() )
	{
		// ���X�g�̏�����.
		pI->m_HitActorList.clear();
		pI->m_BeAHitActorList.clear();
		return;
	}

	// ���Ă�I�u�W�F�N�g.
	for ( auto& [BaAHit_a, SortType] : pI->m_BeAHitActorList ){
		if ( BaAHit_a == nullptr		) continue;

		// �^�O��ۑ�.
		const EObjectTag BaAHit_Tag		= BaAHit_a->GetObjectTag();
		const int		 BaAHit_TagNo	= static_cast<int>( BaAHit_Tag );

		// �^�O���͈͊O/�X�L�b�v����^�O��.
		if ( BaAHit_TagNo <= OBJECT_TAG_MIN	) continue;
		if ( BaAHit_TagNo >= OBJECT_TAG_MAX	) continue;
		if ( pI->m_SkipTag[BaAHit_TagNo]	) continue;

		// ���Ă���I�u�W�F�N�g����ѕς���.
		std::vector<CActor*> HitList = pI->m_HitActorList;
		if ( SortType != ECollSort::None ) {
			// �������X�g���쐬.
			std::vector<std::pair<float, CActor*>> m_DistanceList;
			for ( auto& h : HitList ) {
				const D3DXVECTOR3& Vector = BaAHit_a->GetPosition() - h->GetPosition();
				m_DistanceList.emplace_back( std::make_pair( D3DXVec3Length( &Vector ), h ) );
			}

			// ���ѕς���.
			if (		SortType != ECollSort::Near ) std::sort( m_DistanceList.begin(), m_DistanceList.end(), std::greater<std::pair<float, CActor*>>{} );
			else if (	SortType != ECollSort::Far  ) std::sort( m_DistanceList.begin(), m_DistanceList.end() );

			// ���ѕς������X�g�Ƀ��X�g���X�V����.
			HitList.clear();
			for ( auto& [Distance, Object] : m_DistanceList ) {
				HitList.emplace_back( Object );
			}
		}

		// ���Ă���I�u�W�F�N�g.
		for ( auto& Hit_a : HitList ){
			if ( Hit_a == nullptr		) continue;

			// �������g���m�œ����蔻��͍s��Ȃ��Ă������ߔ�����.
			if ( BaAHit_a == Hit_a ) continue;

			// �^�O��ۑ�.
			const EObjectTag Hit_Tag		= Hit_a->GetObjectTag();
			const int		 Hit_TagNo	= static_cast<int>( Hit_Tag );

			// �^�O���͈͊O/�X�L�b�v����^�O��.
			if ( Hit_TagNo <= OBJECT_TAG_MIN					) continue;
			if ( Hit_TagNo >= OBJECT_TAG_MAX					) continue;
			if ( pI->m_SkipTag[Hit_TagNo]						) continue;
			if ( pI->m_SkipTagToTag[BaAHit_TagNo][Hit_TagNo]	) continue;

			// �����蔻����s��.
			BaAHit_a->Collision( Hit_a );
		}
	}
	// ���X�g�̏�����.
	pI->m_HitActorList.clear();
	pI->m_BeAHitActorList.clear();
}

//---------------------------.
// �����蔻��̃��X�g��������.
//---------------------------.
void CActorCollisionManager::Reset()
{
	CActorCollisionManager* pI = GetInstance();

	// ���X�g�Ɠ����蔻����s��Ȃ����X�g�̏�����.
	pI->m_HitActorList.clear();
	pI->m_BeAHitActorList.clear();
	pI->m_SkipTag = INIT_LIST;
	for ( auto& vt : pI->m_SkipTagToTag )
		vt = INIT_LIST;
}
