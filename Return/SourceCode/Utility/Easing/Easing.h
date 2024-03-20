#pragma once
#include "..\..\Global.h"

/************************************************
*	�C�[�W���O�N���X.
*		���c�F�P.
**/
class CEasing final
{
public:
	CEasing();
	~CEasing();

	// �C���X�^���X�̎擾.
	static CEasing* GetInstance();

	// �C�[�W���O�֐�.
	//	 �e�C�[�W���O�̓���͈ȉ��Ŋm�F���邱�Ƃ��ł��܂�.
	//	 https://easings.net/ja.
	static float Linear(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InSine(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutSine(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutSine(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InCubic(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutCubic(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutCubic(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InQuint(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutQuint(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutQuint(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InCirc(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutCirc(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutCirc(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InQuad(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutQuad(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutQuad(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InQuart(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutQuart(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutQuart(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	
	static float InExpo(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutExpo(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutExpo(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InBack(		float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );
	static float OutBack(		float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );
	static float InOutBack(		float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );
	
	static float InBounce(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutBounce(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutBounce(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	// �C�[�W���O���X�g�̎擾.
	static std::vector<std::string> GetEasingList();

	// �C�[�W���O���X�g����C�[�W���O���s��.
	static float Easing( const std::string& Name, float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );

private:
	// 2�̐��l���������Ƃ���0�ɂȂ邩���ׂ�.
	static bool CheckZero( const float EpsilonNum, const float Num );

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CEasing( const CEasing & )				= delete;
	CEasing& operator = ( const CEasing & ) = delete;
	CEasing( CEasing && )					= delete;
	CEasing& operator = ( CEasing && )		= delete;
};