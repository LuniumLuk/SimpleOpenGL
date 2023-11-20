#pragma once

#include "SimpleGL/Core/Window.h"
#include "SimpleGL/Utility/Camera.h"

namespace SGL::Utility {

    struct CameraController {

        Camera* camera;

        CameraController(Camera* camera) noexcept;

        virtual void step(double deltaTime) noexcept = 0;

    };

    struct HoveringCameraController : CameraController {

        float rotateSpeed = 0.4f;
        float moveSpeed = 5.0f;

        HoveringCameraController(Camera* camera)
            : CameraController(camera) {}

        void step(double deltaTime) noexcept override;

    };

}
