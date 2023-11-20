@uniforms: {
    #version 460 core
}
@stage: vert {
    layout(location = 0) in vec3 aPosition;
    layout(location = 1) in vec3 aColor;

    layout(location = 0) out vec3 vColor;

    void main() {
        gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
        vColor = aColor;
    }
}
@stage: geom {
    layout (points) in;
    layout (triangle_strip, max_vertices = 5) out;

    layout(location = 0) in vec3 vColor[];

    layout(location = 0) out vec3 fColor;

    void buildHouse(vec4 position) {
        gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left
        EmitVertex();   
        gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
        EmitVertex();
        gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
        EmitVertex();
        gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
        EmitVertex();
        gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
        fColor = vec3(1.0, 1.0, 1.0);
        EmitVertex();
        EndPrimitive();
    }

    void main() {
        fColor = vColor[0];
        buildHouse(gl_in[0].gl_Position);
    }
}
@stage: frag {
    layout(location = 0) in vec3 fColor;

    layout(location = 0) out vec4 fragColor;

    void main() {
        fragColor = vec4(fColor, 1.0);
    }
}