#include "Demos/Common.h"

namespace SGL::Demo {

    struct HelloTriangleData {
        std::unique_ptr<sgl::Shader> shader;
        std::unique_ptr<sgl::VertexArray> vao;
    };

    HelloTriangle::HelloTriangle() noexcept {
        sgl::WindowOption opt = {};
        opt.name = "HelloTriangle";
        opt.width = 800;
        opt.height = 600;
        opt.vsync = true;
        opt.fullscreen = false;
        opt.resizable = true;

        /*
        * Create Window here allows you to explicitly set the window option
        * otherwise a default window option will be used.
        */
        createWindow(opt);

        window->windowData.framebufferSizeCallbacks.push_back(
            [](int width, int height) {
                glViewport(0, 0, width, height);
            });

        data = new HelloTriangleData();
    }

    HelloTriangle::~HelloTriangle() noexcept {
        auto userData = (HelloTriangleData*)data;
        delete userData;
    }

    void HelloTriangle::init() noexcept {
        auto userData = (HelloTriangleData*)data;

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
        };

        userData->shader = std::make_unique<sgl::Shader>(getDemoShaderPath("HelloTriangle.glsl"));
        userData->vao = std::make_unique<sgl::VertexArray>(
            &vertices[0], (uint32_t)sizeof(vertices),
            sgl::VertexBufferLayout{
                { sgl::DataType::Float3 }, { sgl::DataType::Float3 }
        });
    }

    void HelloTriangle::update(double deltaTime) noexcept {
        auto userData = (HelloTriangleData*)data;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        userData->shader->bind();
        userData->vao->bind();
        userData->vao->draw(sgl::PrimitiveType::Triangles);

        ImGui::Begin("Hello World!");
        ImGui::End();
    }

    void HelloTriangle::fixedUpdate() noexcept {

    }

}
