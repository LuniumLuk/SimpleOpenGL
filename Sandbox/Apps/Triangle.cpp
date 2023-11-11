#include "Apps/Triangle.h"

DemoTriangle::DemoTriangle() noexcept {
    sgl::WindowOption opt = {};
    opt.name = "Triangle";
    opt.width = 800;
    opt.height = 600;
    opt.vsync = true;
    opt.fullscreen = false;
    opt.resizable = true;

    createWindow(opt);
}

void DemoTriangle::init() noexcept {

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    shader = new sgl::Shader(getShaderPath("Triangle.glsl"));
    vao = new sgl::VertexArray(&vertices[0], sizeof(vertices),
        sgl::VertexBufferLayout{
            { sgl::DataType::Float3 }, { sgl::DataType::Float3 }
        });
}

void DemoTriangle::update(double deltaTime) noexcept {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window->getWidth(), window->getHeight());
    shader->bind();
    vao->bind();
    vao->draw(sgl::PrimitiveType::Triangles);

    ImGui::Begin("Hello World!");
    ImGui::End();
}

void DemoTriangle::fixedUpdate() noexcept {

}
