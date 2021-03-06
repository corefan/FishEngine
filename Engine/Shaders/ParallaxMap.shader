uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale = 0.1;

struct VS_OUT
{
    vec3 positionInWorld;
    vec3 positionInTangent;
    vec3 viewDirInTangent;
    vec3 lightDirInTangent;
    vec2 uv;
};

@vertex
{
    #include <AppData.inc>
    #include <CG.inc>

    out VS_OUT vs_out;

    void vs_main(AppData appdata)
    {
        //vs_out.normal  = normalize(mat3(MATRIX_IT_M) * appdata.normal);
        //vs_out.tangent = normalize(mat3(MATRIX_IT_M) * appdata.tangent);
        vec3 T = normalize(appdata.tangent);
        vec3 N = normalize(appdata.normal);
        vec3 B = normalize(cross(N, T));
        mat3 TBN = transpose(mat3(T, B, N));    // world to tangent space
        vs_out.uv = appdata.uv;
        vs_out.positionInWorld   = vec3(MATRIX_M * appdata.position);
        vs_out.positionInTangent = normalize(TBN * vs_out.positionInWorld);
        vs_out.lightDirInTangent = normalize(TBN * WorldSpaceLightDir(vs_out.positionInWorld));
        vs_out.viewDirInTangent  = normalize(TBN * WorldSpaceViewDir(vs_out.positionInWorld)); 
        gl_Position = MATRIX_MVP * appdata.position;
    }
}

@fragment
{
    in  VS_OUT vs_out;
    out vec4 fragColor;

    // vec2 ParallaxMapping(vec2 uv, vec3 viewDir)
    // {
    //     float height = texture(depthMap, uv).r;
    //     vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
    //     return uv - p;
    // }
    vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
    { 
        // float height =  texture(depthMap, texCoords).r;     
        // return texCoords - viewDir.xy * (height * heightScale);        
        
        // number of depth layers
        const float minLayers = 8;
        const float maxLayers = 32;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
        // calculate the size of each layer
        float layerDepth = 1.0 / numLayers;
        // depth of current layer
        float currentLayerDepth = 0.0;
        // the amount to shift the texture coordinates per layer (from vector P)
        vec2 P = viewDir.xy / viewDir.z * heightScale; 
        vec2 deltaTexCoords = P / numLayers;
      
        // get initial values
        vec2  currentTexCoords     = texCoords;
        float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
          
        while(currentLayerDepth < currentDepthMapValue)
        {
            // shift texture coordinates along direction of P
            currentTexCoords -= deltaTexCoords;
            // get depthmap value at current texture coordinates
            currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
            // get depth of next layer
            currentLayerDepth += layerDepth;  
        }
        
        // -- parallax occlusion mapping interpolation from here on
        // get texture coordinates before collision (reverse operations)
        vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

        // get depth after and before collision for linear interpolation
        float afterDepth  = currentDepthMapValue - currentLayerDepth;
        float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
     
        // interpolation of texture coordinates
        float weight = afterDepth / (afterDepth - beforeDepth);
        vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

        return finalTexCoords;
        // return currentTexCoords;
    }

    void main()
    {
        vec3 viewDir = normalize(vs_out.viewDirInTangent);
        vec2 uv = ParallaxMapping(vs_out.uv, viewDir);
        if (uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0)
            discard;

        vec3 diffuse = texture(diffuseMap, uv).rgb;
        vec3 N = texture(normalMap, uv).xyz;
        N = normalize(N * 2.0 - 1.0);
        float NDotL = dot(N, vs_out.lightDirInTangent);
        NDotL = clamp(NDotL, 0.0, 1.0);
        fragColor = vec4( diffuse * NDotL, 1.0);
    }
}
