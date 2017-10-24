cbuffer cbPerObject{
	float4x4 gWorldViewProj;
}

struct Vin {
	float3 cPosL: POSITION;
	float4 cColor: COLOR;
};

struct Vout {
	float4 cPosH: SV_POSITION;
	float4 cColor: COLOR;
};

Vout VS(Vin vin) {
	Vout vout;
	vout.cPosH = mul(float4(vin.cPosL, 1.0f), gWorldViewProj);
	vout.cColor = vin.cColor; 
	return vout;
}

float4 PS(Vout vout):SV_Target
{
	return vout.cColor;
}

technique11 ColorTech {
	pass P0 {
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS())); 
	}
}