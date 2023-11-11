#pragma once

#include "glm/glm.hpp"

namespace SGL::Utility {

    struct Camera {

        glm::vec3 eye = { 0.0f, 0.0f, -1.0f };
        glm::vec3 at = { 0.0f, 0.0f, 0.0f };
        glm::vec3 up = { 0.0f, 1.0f, 0.0f };
        float aspect = 1.0f;
        float zNear = 0.01f;
        float zFar = 1000.0f;

        Camera() noexcept {};
        Camera(glm::vec3 const& eye, glm::vec3 const& at, glm::vec3 const& up) noexcept;

        glm::mat4 getProjView() const noexcept;
        glm::mat4 getView() const noexcept;
        virtual glm::mat4 getProj() const noexcept = 0;

        glm::mat3 getBasis() const noexcept;

        /*
        * Move the camera, if absolute is true, the move is w.r.t. the world
        * if absolute is false, the move is w.r.t. camera itself
        */
        void move(glm::vec3 const& offset, bool absolute = false) noexcept;

        /*
        * Rotate the camera, if a center is provided, the rotation will be w.r.t. the center,
        * otherwise, it will be w.r.t. itself
        */
        void rotate(float pitch, float yaw, float roll) noexcept;
        void rotate(float pitch, float yaw, float roll, glm::vec3 const& center) noexcept;

        /*
        * Zoom the camera, for perspective camera, this will adjust its fov,
        * for orthogonal camera, this will adjust its frsutum size
        */
        virtual void zoom(float zoom) noexcept = 0;

    };

    struct OrthoCamera : Camera {

        float cx, cy;
        float halfWidth;

        OrthoCamera(float cx, float cy, float w, float aspect, float zNear = -1.0f, float zFar = 1.0f) noexcept;

        glm::mat4 getProj() const noexcept override;

        void zoom(float zoom) noexcept override;

    };

    struct PerspCamera : Camera {

        float fov;

        PerspCamera(glm::vec3 const& eye, glm::vec3 const& at, glm::vec3 const& up, float fov) noexcept;

        glm::mat4 getProj() const noexcept override;

        void zoom(float zoom) noexcept override;

    };

}
