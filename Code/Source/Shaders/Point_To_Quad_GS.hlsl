#pragma pack_matrix(row_major)

struct GSInput
{
	float4 svPos : SV_POSITION;
};

struct GSOutput
{
	float4 uvwH : UV;
	float4 nrmH : NORMAL;
	float4 colH : COLOR;
	float4 posH : POSITION;
	float4 svPos : SV_POSITION;
};

cbuffer Matrix : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
}

[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream< GSOutput > output)
{
	GSOutput verts[4] =
	{
		float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f),
		float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f),
		float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f),
		float4(1.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f),
	};

	verts[0].svPos.xyz = input[0].svPos.xyz;
	verts[0].svPos.x -= 60.0f;
	verts[0].svPos.z += 60.0f;
	verts[0].posH.xyz = input[0].svPos.xyz;
	verts[0].posH.x -= 60.0f;
	verts[0].posH.z += 60.0f;

	verts[1].svPos.xyz = input[0].svPos.xyz;
	verts[1].svPos.x += 60.0f;
	verts[1].svPos.z += 60.0f;
	verts[1].posH.xyz = input[0].svPos.xyz;
	verts[1].posH.x += 60.0f;
	verts[1].posH.z += 60.0f;

	verts[2].svPos.xyz = input[0].svPos.xyz;
	verts[2].svPos.x -= 60.0f;
	verts[2].svPos.z -= 60.0f;
	verts[2].posH.xyz = input[0].svPos.xyz;
	verts[2].posH.x -= 60.0f;
	verts[2].posH.z -= 60.0f;

	verts[3].svPos.xyz = input[0].svPos.xyz;
	verts[3].svPos.x += 60.0f;
	verts[3].svPos.z -= 60.0f;
	verts[3].posH.xyz = input[0].svPos.xyz;
	verts[3].posH.x += 60.0f;
	verts[3].posH.z -= 60.0f;

		for (unsigned int i = 0; i < 4; i++)
		{
			verts[i].svPos = mul(verts[i].svPos, viewMatrix);
			verts[i].svPos = mul(verts[i].svPos, projMatrix);
		}

	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
	output.Append(verts[3]);
}