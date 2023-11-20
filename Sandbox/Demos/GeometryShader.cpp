#include "Demos/Common.h"

namespace SGL::Demo {

    struct GeometryShaderData {
        std::unique_ptr<sgl::Shader> shader;
        std::unique_ptr<sgl::VertexArray> vao;
    };

    GeometryShader::GeometryShader() noexcept {
        sgl::WindowOption opt = {};
        opt.name = "GeometryShader";
        opt.width = 800;
        opt.height = 600;
        opt.vsync = true;
        opt.fullscreen = false;
        opt.resizable = true;

        createWindow(opt);

        window->windowData.framebufferSizeCallbacks.push_back(
            [](int width, int height) {
                glViewport(0, 0, width, height);
            });

        data = new GeometryShaderData();
    }

    GeometryShader::~GeometryShader() noexcept {
        auto userData = (GeometryShaderData*)data;
        delete userData;
    }

    void GeometryShader::init() noexcept {
        auto userData = (GeometryShaderData*)data;

        float vertices[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
        };

        userData->shader = std::make_unique<sgl::Shader>(getDemoShaderPath("GeometryShader.glsl"));
        userData->vao = std::make_unique<sgl::VertexArray>(
            &vertices[0], (uint32_t)sizeof(vertices),
            sgl::VertexBufferLayout{
                { sgl::DataType::Float2 }, { sgl::DataType::Float3 }
        });
    }

    void GeometryShader::update(double deltaTime) noexcept {
        auto userData = (GeometryShaderData*)data;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        userData->shader->bind();
        userData->vao->bind();
        userData->vao->draw(sgl::PrimitiveType::Points);

        ImGui::Begin("Hello World!");
        ImGui::End();
    }

    void GeometryShader::fixedUpdate() noexcept {

    }

}
