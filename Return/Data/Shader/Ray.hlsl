//ｸﾞﾛｰﾊﾞﾙ変数.
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
//ｱﾌﾟﾘ側と同じﾊﾞｯﾌｧｻｲｽﾞになっている必要がある.
cbuffer per_mesh : register(b0)	//ﾚｼﾞｽﾀ番号.
{
	matrix	g_mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの変換合成行列.
	float4	g_Color;	//色(RGBA:xyzw).
};


//頂点ｼｪｰﾀﾞ.
//主にﾓﾃﾞﾙの頂点の表示位置を決定する.
float4 VS_Main( float4 Pos : POSITION ) : SV_Position
{
	Pos = mul( Pos, g_mWVP );
	return Pos;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//画面上に評されるﾋﾟｸｾﾙ(ﾄﾞｯﾄ1つ分)の色を決定する.
float4 PS_Main( float4 Pos : SV_Position ) : SV_Target
{
	return g_Color;
}