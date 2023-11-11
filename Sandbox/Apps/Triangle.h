#pragma once

#include "Apps/Common.h"

struct DemoTriangle : sgl::Application {

    sgl::Shader* shader = nullptr;
    sgl::VertexArray* vao = nullptr;

    DemoTriangle() noexcept;

    virtual void init() noexcept override;
    virtual void update(double deltaTime) noexcept override;
    virtual void fixedUpdate() noexcept override;

};

