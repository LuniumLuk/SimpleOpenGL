#include "PCH.h"

#include "SimpleGL/Utility/CameraController.h"

namespace SGL::Utility {

    CameraController::CameraController(Camera* camera) noexcept
        :camera(camera) {}

    void HoveringCameraController::step(double deltaTime) noexcept {
        auto const& state = InputState::instance();
        if (state.rightButtonPressed) {
            camera->rotate(
                -state.mouseDeltaY * rotateSpeed * (float)deltaTime,
                -state.mouseDeltaX * rotateSpeed * (float)deltaTime,
                0.0f
            );
        }

        glm::vec3 move(0.0f);
        if (state.keyPressed[KEY_A]) {
            move.x -= moveSpeed * (float)deltaTime;
        }
        if (state.keyPressed[KEY_D]) {
            move.x += moveSpeed * (float)deltaTime;
        }
        if (state.keyPressed[KEY_S]) {
            move.z -= moveSpeed * (float)deltaTime;
        }
        if (state.keyPressed[KEY_W]) {
            move.z += moveSpeed * (float)deltaTime;
        }

        camera->move(move);
    }

}
