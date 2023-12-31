#include "PCH.h"

#include "SimpleGL/Core/Mesh.h"
#include "glad/glad.h"

namespace SGL {

    Mesh::Mesh(float* vdata, uint32_t vsize, VertexBufferLayout const& layout, uint32_t* idata, uint32_t isize, PrimitiveType _type)
        : elementCount(isize / sizeof(uint32_t))
        , vertexBuffer(vdata, vsize, layout)
        , elementBuffer(idata, isize)
        , type(_type)
    {
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.handle);

        vertexBuffer.bindAttributes();
        glBindVertexArray(0);
    }

    void Mesh::bind() const noexcept {
        glBindVertexArray(handle);
    }

    void Mesh::draw() const noexcept {
        switch (type) {
        case PrimitiveType::Points:
            glDrawElements(GL_POINTS, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::Lines:
            glDrawElements(GL_LINES, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::LineStrip:
            glDrawElements(GL_LINE_STRIP, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::LineStripAdjacency:
            glDrawElements(GL_LINE_STRIP_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::LinesAdjacency:
            glDrawElements(GL_LINES_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::TriangleStrip:
            glDrawElements(GL_TRIANGLE_STRIP, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::TriangleFan:
            glDrawElements(GL_TRIANGLE_FAN, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::Triangles:
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::TriangleStripAdjacency:
            glDrawElements(GL_TRIANGLE_STRIP_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::TrianglesAdjacency:
            glDrawElements(GL_TRIANGLES_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0); break;
        case PrimitiveType::Patches:
            glDrawElements(GL_PATCHES, elementCount, GL_UNSIGNED_INT, 0); break;
        }
    }

    void Mesh::drawInstanced(uint32_t num, VertexBuffer* instanceBuffer, unsigned int start, unsigned int divisor) const noexcept {
        if (instanceBuffer) {
            instanceBuffer->bindInstanceAttributes(start, divisor);
        }

        switch (type) {
        case PrimitiveType::Points:
            glDrawElementsInstanced(GL_POINTS, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::Lines:
            glDrawElementsInstanced(GL_LINES, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::LineStrip:
            glDrawElementsInstanced(GL_LINE_STRIP, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::LineStripAdjacency:
            glDrawElementsInstanced(GL_LINE_STRIP_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::LinesAdjacency:
            glDrawElementsInstanced(GL_LINES_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::TriangleStrip:
            glDrawElementsInstanced(GL_TRIANGLE_STRIP, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::TriangleFan:
            glDrawElementsInstanced(GL_TRIANGLE_FAN, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::Triangles:
            glDrawElementsInstanced(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::TriangleStripAdjacency:
            glDrawElementsInstanced(GL_TRIANGLE_STRIP_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0, num); break;
        case PrimitiveType::TrianglesAdjacency:
            glDrawElementsInstanced(GL_TRIANGLES_ADJACENCY, elementCount, GL_UNSIGNED_INT, 0, num); break;
        }
    }

    VertexArray::VertexArray(void* vdata, uint32_t vsize, VertexBufferLayout const& layout)
        : vertexCount(vsize / layout.stride)
        , vertexBuffer(vdata, vsize, layout)
    {
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.handle);

        vertexBuffer.bindAttributes();
        glBindVertexArray(0);
    }

    void VertexArray::bind() const noexcept {
        glBindVertexArray(handle);
    }

    void VertexArray::draw(PrimitiveType type) const noexcept {
        switch (type) {
        case PrimitiveType::Points:
            glDrawArrays(GL_POINTS, 0, vertexCount); break;
        case PrimitiveType::Lines:
            glDrawArrays(GL_LINES, 0, vertexCount); break;
        case PrimitiveType::LineStrip:
            glDrawArrays(GL_LINE_STRIP, 0, vertexCount); break;
        case PrimitiveType::LineStripAdjacency:
            glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, vertexCount); break;
        case PrimitiveType::LinesAdjacency:
            glDrawArrays(GL_LINES_ADJACENCY, 0, vertexCount); break;
        case PrimitiveType::TriangleStrip:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount); break;
        case PrimitiveType::TriangleFan:
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount); break;
        case PrimitiveType::Triangles:
            glDrawArrays(GL_TRIANGLES, 0, vertexCount); break;
        case PrimitiveType::TriangleStripAdjacency:
            glDrawArrays(GL_TRIANGLE_STRIP_ADJACENCY, 0, vertexCount); break;
        case PrimitiveType::TrianglesAdjacency:
            glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, vertexCount); break;
        case PrimitiveType::Patches:
            glDrawArrays(GL_PATCHES, 0, vertexCount); break;
        }
    }

    void VertexArray::drawInstanced(PrimitiveType type, uint32_t num, VertexBuffer* instanceBuffer, unsigned int start, unsigned int divisor) const noexcept {
        if (instanceBuffer) {
            instanceBuffer->bindInstanceAttributes(start, divisor);
        }

        switch (type) {
        case PrimitiveType::Points:
            glDrawArraysInstanced(GL_POINTS, 0, vertexCount, num); break;
        case PrimitiveType::Lines:
            glDrawArraysInstanced(GL_LINES, 0, vertexCount, num); break;
        case PrimitiveType::LineStrip:
            glDrawArraysInstanced(GL_LINE_STRIP, 0, vertexCount, num); break;
        case PrimitiveType::LineStripAdjacency:
            glDrawArraysInstanced(GL_LINE_STRIP_ADJACENCY, 0, vertexCount, num); break;
        case PrimitiveType::LinesAdjacency:
            glDrawArraysInstanced(GL_LINES_ADJACENCY, 0, vertexCount, num); break;
        case PrimitiveType::TriangleStrip:
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, vertexCount, num); break;
        case PrimitiveType::TriangleFan:
            glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, vertexCount, num); break;
        case PrimitiveType::Triangles:
            glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, num); break;
        case PrimitiveType::TriangleStripAdjacency:
            glDrawArraysInstanced(GL_TRIANGLE_STRIP_ADJACENCY, 0, vertexCount, num); break;
        case PrimitiveType::TrianglesAdjacency:
            glDrawArraysInstanced(GL_TRIANGLES_ADJACENCY, 0, vertexCount, num); break;
        }
    }

}
