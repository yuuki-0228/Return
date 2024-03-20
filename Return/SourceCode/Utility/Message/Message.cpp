#include "Message.h"

CMessage::CMessage()
	: m_MsgList	()
{
}

CMessage::~CMessage()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CMessage* CMessage::GetInstance()
{
	static std::unique_ptr<CMessage> pInstance = std::make_unique<CMessage>();
	return pInstance.get();
}

//---------------------------.
// �X�V.
//---------------------------.
void CMessage::Update( const float& DeltaTime )
{
	CMessage* pI = GetInstance();

	auto Itr = pI->m_MsgList.begin();
	while ( Itr != pI->m_MsgList.end() ) {
		std::string Key	 = Itr->first;
		bool		Flag = Itr->second;

		// ���b�Z�[�W�̍폜.
		if (		Flag == false								) Itr = pI->m_MsgList.erase( Itr );
		else if (	Key.substr( 0, 3 ) == Msg::ONE_FRAME_MSG	) Itr = pI->m_MsgList.erase( Itr );
		else Itr++;
	}
}

//---------------------------.
// ���b�Z�[�W�𑗂�.
//---------------------------.
void CMessage::Send( const std::string& Message, const Msg::EMsgType Type )
{
	CMessage* pI = GetInstance();

	Msg::EMsgType	NowType = Msg::EMsgType::None;
	std::string		EndMsg	= Message;

	// ���b�Z�[�W�̖��O�Ɏ��ʕ������܂܂�邩.
	const std::string MsgType = EndMsg.substr( 0, 3 );
	if (		MsgType == Msg::ONE_FRAME_MSG	) NowType = Msg::EMsgType::OneFrameMsg;
	else if (	MsgType == Msg::CHECK_FRAME_MSG ) NowType = Msg::EMsgType::CheckFrameMsg;
	else if (	MsgType == Msg::CHECK_MSG		) NowType = Msg::EMsgType::CheckMsg;
	else if (	MsgType == Msg::NEW_MSG			) NowType = Msg::EMsgType::NewMsg;
	else if (	MsgType == Msg::DELETE_MSG		) NowType = Msg::EMsgType::DeleteMsg;
	else if (	MsgType == Msg::AUTO_MSG		) {
		NowType = Msg::EMsgType::AutoMsg;
		EndMsg	= Msg::ONE_FRAME_MSG + EndMsg.substr( 3 );
	}

	// ���ʕ������܂܂�Ă��Ȃ��ꍇ�O�ɕt����.
	if ( NowType == Msg::EMsgType::None ) {
		std::string AddName = "";
		if (		Type == Msg::EMsgType::OneFrameMsg		) AddName = Msg::ONE_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckFrameMsg	) AddName = Msg::CHECK_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckMsg			) AddName = Msg::CHECK_MSG;
		else if (	Type == Msg::EMsgType::NewMsg			) AddName = Msg::NEW_MSG;
		else if (	Type == Msg::EMsgType::DeleteMsg		) AddName = Msg::DELETE_MSG;
		else												  AddName = Msg::ONE_FRAME_MSG;
		EndMsg = AddName + EndMsg;
	}

	// �f���[�g���b�Z�[�W�̏ꍇ�A�j���[���b�Z�[�W���폜����.
	if ( NowType == Msg::EMsgType::DeleteMsg || Type == Msg::EMsgType::DeleteMsg ) {
		NewMsgDelete( EndMsg );
		return;
	}

	// ���b�Z�[�W�̒ǉ�.
	pI->m_MsgList[EndMsg] = true;
}

//---------------------------.
// ���b�Z�[�W�̊m�F.
//---------------------------.
bool CMessage::Check( const std::string& Message, const Msg::EMsgType Type )
{
	CMessage* pI = GetInstance();

	Msg::EMsgType	NowType = Msg::EMsgType::None;
	std::string		EndMsg	= Message;

	// ���b�Z�[�W�̖��O�Ɏ��ʕ������܂܂�邩.
	const std::string MsgType = EndMsg.substr( 0, 3 );
	if (		MsgType == Msg::ONE_FRAME_MSG	) NowType = Msg::EMsgType::OneFrameMsg;
	else if (	MsgType == Msg::CHECK_FRAME_MSG ) NowType = Msg::EMsgType::CheckFrameMsg;
	else if (	MsgType == Msg::CHECK_MSG		) NowType = Msg::EMsgType::CheckMsg;
	else if (	MsgType == Msg::NEW_MSG			) NowType = Msg::EMsgType::NewMsg;
	else if (	MsgType == Msg::DELETE_MSG		) NowType = Msg::EMsgType::DeleteMsg;
	else if (	MsgType == Msg::AUTO_MSG		) NowType = Msg::EMsgType::AutoMsg;

	// ���ʕ������܂܂�Ă��Ȃ��ꍇ�O�ɕt����.
	if ( NowType == Msg::EMsgType::None ) {
		std::string AddName = "";
		if (		Type == Msg::EMsgType::OneFrameMsg		) AddName = Msg::ONE_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckFrameMsg	) AddName = Msg::CHECK_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckMsg			) AddName = Msg::CHECK_MSG;
		else if (	Type == Msg::EMsgType::NewMsg			) AddName = Msg::NEW_MSG;
		else if (	Type == Msg::EMsgType::DeleteMsg		) AddName = Msg::DELETE_MSG;
		else if (	Type == Msg::EMsgType::AutoMsg			) AddName = Msg::AUTO_MSG;
		else												  AddName = Msg::ONE_FRAME_MSG;
		EndMsg = AddName + EndMsg;
	}

	// �`�F�b�N�t���[�����b�Z�[�W�̏ꍇ�A�����t���[�����b�Z�[�W�ɕύX����.
	if ( NowType == Msg::EMsgType::CheckFrameMsg || Type == Msg::EMsgType::CheckFrameMsg ) {
		return CheckFrameMsgChange( EndMsg );
	}

	// �`�F�b�N���b�Z�[�W�̏ꍇ�A�m�F�チ�b�Z�[�W���폜����.
	if ( NowType == Msg::EMsgType::CheckMsg || Type == Msg::EMsgType::CheckMsg ) {
		return CheckMsgDelete( EndMsg );
	}

	// �I�[�g���b�Z�[�W�̏ꍇ�A�S�Ă̎��ʕ����Œ��ׂ�.
	if ( NowType == Msg::EMsgType::AutoMsg || Type == Msg::EMsgType::AutoMsg ) {
		return AutoMsgCheck( EndMsg );
	}

	// ���b�Z�[�W���͂��Ă��邩.
	return pI->m_MsgList[EndMsg];
}

//---------------------------.
// �j���[���b�Z�[�W���폜.
//---------------------------.
bool CMessage::NewMsgDelete( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	std::string NewMsg = Msg::NEW_MSG + CheckMsg.substr( 3 );
	pI->m_MsgList.erase( NewMsg );
	return true;
}

//---------------------------.
// �`�F�b�N�t���[�����b�Z�[�W�̕ύX.
//---------------------------.
bool CMessage::CheckFrameMsgChange( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	const bool Flag = pI->m_MsgList[CheckMsg];
	if ( Flag ) {
		const std::string Msg = CheckMsg.substr( 3 );
		pI->m_MsgList.erase( CheckMsg );
		pI->m_MsgList[Msg::ONE_FRAME_MSG + Msg] = true;
	}
	return Flag;
}

//---------------------------.
// �`�F�b�N���b�Z�[�W���폜.
//---------------------------.
bool CMessage::CheckMsgDelete( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	const bool Flag = pI->m_MsgList[CheckMsg];
	pI->m_MsgList.erase( CheckMsg );
	return Flag;
}

//---------------------------.
// �I�[�g���b�Z�[�W�̊m�F.
//---------------------------.
bool CMessage::AutoMsgCheck( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	const std::string Msg = CheckMsg.substr( 3 );
	if( pI->m_MsgList[Msg::ONE_FRAME_MSG	+ Msg] ) return true;
	if( pI->m_MsgList[Msg::CHECK_FRAME_MSG	+ Msg] ) return CheckFrameMsgChange( Msg::CHECK_FRAME_MSG + Msg );
	if( pI->m_MsgList[Msg::CHECK_MSG		+ Msg] ) return CheckMsgDelete( Msg::CHECK_MSG + Msg );
	if( pI->m_MsgList[Msg::NEW_MSG			+ Msg] ) return true;
	return false;
}
