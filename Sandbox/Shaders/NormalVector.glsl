@uniforms: {
    #version 460 core

    layout(location = 0) uniform mat4 uProj;
    layout(location = 1) uniform mat4 uView;
    layout(location = 2) uniform mat4 uModel;
    layout(location = 3) uniform float uVectorMagnitude;
}
@stage: vert {
    layout(location = 0) in vec3 aPosition;
    layout(location = 1) in vec3 aNormal;

    layout(location = 0) out vec3 vNormal;

    void main() {
        gl_Position = uView * uModel * vec4(aPosition, 1.0);
        mat3 normalMatrix = mat3(transpose(inverse(uView * uModel)));
        vNormal = normalize(normalMatrix * aNormal);
    }
}
@stage: geom {
    layout (triangles) in;
    layout (line_strip, max_vertices = 6) out;

    layout(location = 0) in vec3 vNormal[];

    layout(location = 0) out vec3 fColor;

    void generateLine(int index) {
        gl_Position = uProj * gl_in[index].gl_Position;
        EmitVertex();
        gl_Position = uProj * (gl_in[index].gl_Position + vec4(vNormal[index], 0.0) * uVectorMagnitude);
        EmitVertex();
        EndPrimitive();
    }

    void main() {
        generateLine(0);
        generateLine(1);
        generateLine(2);
    }
}
@stage: frag {
    layout(location = 0) out vec4 fragColor;

    void main() {
        fragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
}