#include "SimpleGL.h"
#include "glad/glad.h"

namespace sgl = SGL;

struct MyApp : sgl::Application {

    MyApp(sgl::WindowOption const& opt): sgl::Application(opt) {}

    virtual void init() noexcept override {

    }

    virtual void update(double deltaTime) noexcept override {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Begin("Hello World!");
        ImGui::End();
    }

    virtual void fixedUpdate() noexcept override {

    }

};

int main() {
    auto opt = sgl::WindowOption();
    opt.width = 1280;
    opt.height = 720;
    opt.name = "Sandbox";
    opt.resizable = true;
    //opt.fullScreen = true;
    auto app = MyApp(opt);
    app.run();

    return 0;
}
