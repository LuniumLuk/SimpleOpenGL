#pragma once

#include "SimpleGL/Core/Types.h"
#include "SimpleGL/Core/Buffer.h"

namespace SGL {

    struct Mesh {

        unsigned int handle;
        uint32_t elementCount;
        VertexBuffer vertexBuffer;
        ElementBuffer elementBuffer;
        PrimitiveType type;

        Mesh(float* vdata, uint32_t vsize, VertexBufferLayout const& layout, uint32_t* idata, uint32_t isize, PrimitiveType type = PrimitiveType::Triangles);

        Mesh(Mesh const&) = delete;
        Mesh(Mesh&& other) noexcept
            : elementCount(other.elementCount)
            , vertexBuffer(std::move(other.vertexBuffer))
            , elementBuffer(std::move(other.elementBuffer))
            , type(other.type)
        {
            handle = other.handle;
            other.handle = 0;
        }

        Mesh& operator=(Mesh const&) = delete;
        Mesh& operator=(Mesh&& other) noexcept {
            elementCount = other.elementCount;
            vertexBuffer = std::move(other.vertexBuffer);
            elementBuffer = std::move(other.elementBuffer);
            type = other.type;
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

        void bind() const noexcept;
        void draw() const noexcept;
        void drawInstanced(uint32_t num, VertexBuffer* instanceBuffer = nullptr, unsigned int start = 0, unsigned int divisor = 0) const noexcept;

    };

    struct VertexArray {

        unsigned int handle;
        uint32_t vertexCount;
        VertexBuffer vertexBuffer;

        VertexArray(void* vdata, uint32_t vsize, VertexBufferLayout const& layout);

        VertexArray(VertexArray const&) = delete;
        VertexArray(VertexArray&& other) noexcept
            : vertexCount(other.vertexCount)
            , vertexBuffer(std::move(other.vertexBuffer))
        {
            handle = other.handle;
            other.handle = 0;
        }

        VertexArray& operator=(VertexArray const&) = delete;
        VertexArray& operator=(VertexArray&& other) noexcept {
            vertexCount = other.vertexCount;
            vertexBuffer = std::move(other.vertexBuffer);
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

        void bind() const noexcept;
        void draw(PrimitiveType type) const noexcept;
        void drawInstanced(PrimitiveType type, uint32_t num, VertexBuffer* instanceBuffer = nullptr, unsigned int start = 0, unsigned int divisor = 0) const noexcept;

    };

}
