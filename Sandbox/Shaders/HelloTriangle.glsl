@uniforms: {
    #version 460 core
}
@stage: vert {
    layout(location = 0) in vec3 aPosition;
    layout(location = 1) in vec3 aColor;

    layout(location = 0) out vec3 vColor;

    void main() {
        gl_Position = vec4(aPosition, 1.0);
        vColor = aColor;
    }
}
@stage: frag {
    layout(location = 0) in vec3 vColor;

    layout(location = 0) out vec4 fragColor;

    void main() {
        fragColor = vec4(vColor, 1.0);
    }
}