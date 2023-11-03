#pragma once

namespace SGL {

    enum struct TextureType {
        None,
        Texture2D,
        TextureCube,
        RenderTarget,
    };

    enum struct InternalFormat {
        Default,
        RED,
        RGB,
        RGBA,
        FloatRED,
        FloatRGB,
        FloatRGBA,
        Depth,
    };

    enum struct DataType {
        None,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool,
        Mat3,
        Mat4,
    };

    constexpr inline uint32_t getDataTypeSize(DataType type) noexcept {
        switch (type) {
        case DataType::None:    return 0;
        case DataType::Float:   return 4;
        case DataType::Float2:  return 4 * 2;
        case DataType::Float3:  return 4 * 3;
        case DataType::Float4:  return 4 * 4;
        case DataType::Int:     return 4;
        case DataType::Int2:    return 4 * 2;
        case DataType::Int3:    return 4 * 3;
        case DataType::Int4:    return 4 * 4;
        case DataType::Bool:    return 1;
        case DataType::Mat3:    return 4 * 3 * 3;
        case DataType::Mat4:    return 4 * 4 * 4;
        default:                return 0;
        }
    }

    enum struct AttachmentType {
        Color,
        ColorCubemap,
        ColorMSAA,
        Depth,
        DepthArray,
        DepthCubemap,
        Stencil,
        DepthStencil,
        DepthStencilMSAA,
        ColorRenderBuffer,
        ColorMSAARenderBuffer,
        DepthRenderBuffer,
        StencilRenderBuffer,
        DepthStencilRenderBuffer,
        DepthStencilMSAARenderBuffer,
    };

    enum struct ShaderModuleType {
        Geometry,
        Vertex,
        Fragment,
        Compute,
        TessellationControl,
        TessellationEvaluation,
    };

    enum struct BufferUsageType {
        Static,
        Dynamic,
        Stream,
    };

    enum struct PrimitiveType {
        Points,
        LineStrip,
        Lines,
        LineStripAdjacency,
        LinesAdjacency,
        TriangleStrip,
        TriangleFan,
        Triangles,
        TriangleStripAdjacency,
        TrianglesAdjacency,
        Patches,
    };

}
