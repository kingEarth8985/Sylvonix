// ============================================================
// TONEMAP COMPUTE SHADER
// ============================================================

// HDR input (float4)
[[vk::binding(0, 0)]]
RWTexture2D<float4> hdrInput;

[[vk::binding(1, 0)]]
RWTexture2D<float4> sdrOutput;

[[vk::binding(2, 0)]]
cbuffer TonemapParams
{
    float exposure;
}

// ACES-inspired tonemap curve (same as your fragment shader)
float3 ACESFilmO(float3 x)
{
    return (x * (2.51 * x + 0.03)) /
           (x * (2.43 * x + 0.59) + 0.14);
}

// ============================================================
// ACES FILMIC TONE MAPPING
// ============================================================

float3 ACESFilm(float3 color)
{
    // Fitted ACES curve.
    float3 numerator =
        color * (2.51 * color + 0.03);

    float3 denominator =
        color * (2.43 * color + 0.59) + 0.14;

    return saturate(numerator / max(denominator, 0.000001));
}

[numthreads(8,8,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    // Read HDR pixel
    float3 hdr = hdrInput[id.xy].rgb;

    // Apply exposure
    hdr *= exposure;

    // Apply ACES-inspired curve
    float3 mapped = ACESFilm(hdr);

    // Clamp to SDR range
    mapped = saturate(mapped);

    // Gamma correction (same as your fragment shader)
    mapped = pow(mapped, 1.0 / 2.2);

    // Write SDR pixel
    sdrOutput[id.xy] = float4(mapped, 1.0);
}
