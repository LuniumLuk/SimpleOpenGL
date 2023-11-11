#include "PCH.h"

#include "SimpleGL/Utility/Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace SGL::Utility {

    Camera::Camera(glm::vec3 const& eye, glm::vec3 const& at, glm::vec3 const& up) noexcept :
        eye(eye), at(at), up(up) {}

    glm::mat4 Camera::getProjView() const noexcept {
        return getProj() * getView();
    }

    glm::mat4 Camera::getView() const noexcept {
        return glm::lookAt(eye, at, up);
    }

    glm::mat3 Camera::getBasis() const noexcept {
        auto z = glm::normalize(at - eye);
        auto x = glm::normalize(glm::cross(z, up));
        auto y = glm::normalize(glm::cross(x, z));
        return glm::mat3(x, y, z);
    }

    void Camera::move(glm::vec3 const& vec, bool absolute) noexcept {
        if (absolute) {
            eye += vec;
            at += vec;
        }
        else {
            auto delta = getBasis() * vec;
            eye += delta;
            at += delta;
        }
    }

    void Camera::rotate(float pitch, float yaw, float roll) noexcept {
        auto basis = getBasis();

        glm::mat4 transform(1.0f);
        transform = glm::rotate(transform, pitch, basis[0]);
        transform = glm::rotate(transform, yaw, basis[1]);
        transform = glm::rotate(transform, roll, basis[2]);

        auto forward = glm::vec3(transform * glm::vec4(basis[2], 1.0f));

        auto cos = glm::dot(forward, up);
        if (1.0f - std::abs(cos) < 0.0001f) {
            return;
        }

        at = eye + forward;
    }

    void Camera::rotate(float yaw, float pitch, float roll, glm::vec3 const& center) noexcept {
        auto forward = center - eye;
        auto dist = glm::length(forward);
        auto right = glm::cross(forward, up);
        auto up = glm::cross(right, forward);

        glm::mat4 transform(1.0f);
        transform = glm::rotate(transform, pitch, right);
        transform = glm::rotate(transform, yaw, up);
        transform = glm::rotate(transform, roll, forward);

        forward = glm::normalize(glm::vec3(transform * glm::vec4(forward, 1.0f)));

        auto cos = glm::dot(forward, up);
        if (1.0f - std::abs(cos) < 0.0001f) {
            return;
        }

        eye = center - forward * dist;
        at = eye + forward;
    }

    OrthoCamera::OrthoCamera(float cx, float cy, float w, float aspect, float zNear, float zFar) noexcept
        : cx(cx)
        , cy(cy)
        , halfWidth(w / 2) {

        this->aspect = aspect;
        this->zNear = zNear;
        this->zFar = zFar;
    }

    glm::mat4 OrthoCamera::getProj() const noexcept {
        float left = cx - halfWidth;
        float right = cx + halfWidth;
        float bottom = cy - halfWidth * aspect;
        float top = cy + halfWidth * aspect;
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    void OrthoCamera::zoom(float zoom) noexcept {
        if (zoom > 0) {
            halfWidth /= zoom;
        }
    }

    PerspCamera::PerspCamera(glm::vec3 const& eye, glm::vec3 const& at, glm::vec3 const& up, float fov) noexcept
        : Camera(eye, at, up)
        , fov(fov) {}

    glm::mat4 PerspCamera::getProj() const noexcept {
        return glm::perspective(fov, aspect, zNear, zFar);
    }

    void PerspCamera::zoom(float zoom) noexcept {
        if (zoom > 0) {
            fov /= zoom;
            fov = std::clamp(fov, 0.0f, glm::pi<float>());
        }
    }

}
