@uniforms: {
    #version 460 core
}
@stage: vert {
    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec3 a_color;

    layout(location = 0) out vec3 v_color;

    void main() {
        gl_Position = vec4(a_position, 1.0);
        v_color = a_color;
    }
}
@stage: frag {
    layout(location = 0) in vec3 v_color;

    layout(location = 0) out vec4 fragColor;

    void main() {
        fragColor = vec4(v_color, 1.0);
    }
}