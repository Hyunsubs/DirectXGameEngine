#ifndef _StdShader
#define _StdShader

struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


// Vertex Shader Stage
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;
    
    return output;
}


float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    
    return input.color;
}


#endif
