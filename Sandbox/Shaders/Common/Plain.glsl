@uniforms: {
    #version 460 core

    uniform sampler2D uDiffuseMap0;
    uniform sampler2D uSpecularMap0;
    uniform sampler2D uNormalMap0;
    uniform sampler2D uHeightMap0;

    uniform mat4 uProj;
    uniform mat4 uView;
    uniform mat4 uModel;
}
@stage: vert {
    layout(location = 0) in vec3 aPosition;
    layout(location = 1) in vec3 aNormal;
    layout(location = 2) in vec2 aTexcoord;
    layout(location = 3) in vec3 aTangent;
    layout(location = 4) in vec3 aBitangent;

    layout(location = 0) out vec2 vTexcoord;

    void main() {
        gl_Position = uProj * uView * uModel * vec4(aPosition, 1.0);
        vTexcoord = aTexcoord;
    }
}
@stage: frag {
    layout(location = 0) in vec2 vTexcoord;

    layout(location = 0) out vec4 fragColor;

    void main() {
        fragColor = texture(uDiffuseMap0, vTexcoord);
    }
}