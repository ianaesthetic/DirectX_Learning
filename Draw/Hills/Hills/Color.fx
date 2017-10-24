cbuffer gMatrix {
	float4x4 gWVP;
};

struct VertexIn{
	float3 PosL:POSITION;
	float4 Color:COLOR;
};

struct VertexOut {
	float4 PosH:SV_POSITION;
	float4 Color:COLOR;
};

VertexOut VS(VertexIn Vin) {
	VertexOut Vout;
	Vout.PosH = mul(float4(Vin.PosL, 1.0f), gWVP);
	Vout.Color = Vin.Color;
	return Vout;
}

float4 PS(VertexOut Pin) :SV_Target
{
	return Pin.Color;
}

technique11 ColorTech {
	pass P0 {
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}