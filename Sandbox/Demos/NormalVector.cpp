#include "Demos/Common.h"

namespace SGL::Demo {

    struct NormalVectorData {
        std::unique_ptr<sgl::Shader> plainShader;
        std::unique_ptr<sgl::Shader> normalShader;
        std::unique_ptr<sgl::Utility::Camera> camera;
        std::unique_ptr<sgl::Utility::HoveringCameraController> cameraController;
        std::unique_ptr<sgl::Model> model;
    };

    NormalVector::NormalVector() noexcept {
        sgl::WindowOption opt = {};
        opt.name = "NormalVector";
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

        data = new NormalVectorData();
    }

    NormalVector::~NormalVector() noexcept {
        auto userData = (NormalVectorData*)data;
        delete userData;
    }

    void NormalVector::init() noexcept {
        auto userData = (NormalVectorData*)data;

        userData->plainShader = std::make_unique<sgl::Shader>(getDemoShaderPath("Common/Plain.glsl"));
        userData->normalShader = std::make_unique<sgl::Shader>(getDemoShaderPath("NormalVector.glsl"));
        userData->camera = std::make_unique<sgl::Utility::PerspCamera>(
            glm::vec3(0.0, 0.0, 5.0),
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0),
            (float)sgl::PI() / 3);
        userData->cameraController = std::make_unique<sgl::Utility::HoveringCameraController>(userData->camera.get());
        userData->model = sgl::Model::loadAssimp(getDemoAssetPath("backpack/backpack.obj"));
    }

    void NormalVector::update(double deltaTime) noexcept {
        auto userData = (NormalVectorData*)data;

        userData->camera->aspect = (float)window->getWidth() / (float)window->getHeight();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        userData->cameraController->step(deltaTime);

        userData->plainShader->bind();
        userData->plainShader->setMat4("uProj", userData->camera->getProj());
        userData->plainShader->setMat4("uView", userData->camera->getView());
        userData->model->draw(userData->plainShader.get());

        userData->normalShader->bind();
        userData->normalShader->setMat4("uProj", userData->camera->getProj());
        userData->normalShader->setMat4("uView", userData->camera->getView());
        userData->normalShader->setFloat("uVectorMagnitude", 0.1f);
        userData->model->draw(userData->normalShader.get());

        ImGui::Begin("Hello World!");
        ImGui::End();
    }

    void NormalVector::fixedUpdate() noexcept {

    }

}
