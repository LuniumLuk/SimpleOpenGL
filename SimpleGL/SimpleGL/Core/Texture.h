#pragma once

#include "SimpleGL/Core/Types.h"

namespace SGL {

    struct Shader;

    struct Texture {

        uint32_t handle;
        TextureType type;

        Texture();
        ~Texture();

        Texture(Texture const&) = delete;
        Texture(Texture&& other) noexcept
            : handle(other.handle)
            , type(other.type) {
            other.handle = 0;
        }

        Texture& operator=(Texture const&) = delete;
        Texture& operator=(Texture&& other) noexcept {
            handle = other.handle;
            type = other.type;
            other.handle = 0;
        }

        void bind(uint32_t binding) const noexcept;
        void bind(Shader* shader, std::string const& name, uint32_t binding) const noexcept;

    };

    struct Texture2D : public Texture {
        Texture2D(std::string const& filename, bool hdr = false, bool genMipmap = false, bool flipY = true) noexcept;
        Texture2D(uint32_t width, uint32_t height, InternalFormat format, void* data = NULL) noexcept;
    };

    struct TextureCube : public Texture {
        TextureCube(std::vector<std::string> const& filenames, bool genMipmap = true, bool flipY = false) noexcept;
    };

    void saveSnapshot(std::string const& path, int w, int h) noexcept;

}
