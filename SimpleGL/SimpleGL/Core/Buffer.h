#pragma once

#include "SimpleGL/Core/Types.h"

namespace SGL {

    struct Shader;

    struct Buffer {

        unsigned int handle;

        Buffer();
        ~Buffer();

        Buffer(Buffer const&) = delete;
        Buffer(Buffer&& other) noexcept
            : handle(other.handle)
        {
            other.handle = 0;
        }

        Buffer& operator=(Buffer const&) = delete;
        Buffer& operator=(Buffer&& other) noexcept {
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

        void copyTo(Buffer const& other, uint32_t rOffset, uint32_t wOffset, uint32_t size) const noexcept;

    };

    struct BufferElementItem {

        DataType type;
        uint32_t offset;

        BufferElementItem() = delete;
        BufferElementItem(DataType _type, uint32_t _offset = 0)
            : type(_type), offset(_offset)
        {}

    };

    struct VertexBufferLayout {

        uint32_t stride = 0;
        std::vector<BufferElementItem> elements;
        VertexBufferLayout(std::initializer_list<BufferElementItem> const& list);

    };

    struct VertexBuffer : public Buffer {

        VertexBufferLayout layout;

        VertexBuffer(void* data, uint32_t size, VertexBufferLayout const& layout, BufferUsageType behavior = BufferUsageType::Static);

        VertexBuffer(VertexBuffer const&) = delete;
        VertexBuffer(VertexBuffer&& other) noexcept
            : layout(other.layout)
        {
            handle = other.handle;
            other.handle = 0;
        }

        VertexBuffer& operator=(VertexBuffer const& other) = delete;
        VertexBuffer& operator=(VertexBuffer&& other) noexcept {
            layout = other.layout;
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

        void bindAttributes() const noexcept;
        void bindInstanceAttributes(unsigned int start, unsigned int divisor) const noexcept;

    };

    struct ElementBuffer : public Buffer {

        ElementBuffer(void* data, uint32_t size, BufferUsageType behavior = BufferUsageType::Static);

        ElementBuffer(ElementBuffer const&) = delete;
        ElementBuffer(ElementBuffer&& other) noexcept {
            handle = other.handle;
            other.handle = 0;
        }

        ElementBuffer& operator=(ElementBuffer const& other) = delete;
        ElementBuffer& operator=(ElementBuffer&& other) noexcept {
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

    };

    struct UniformBuffer : public Buffer {

        uint32_t size;

        UniformBuffer(void* data, uint32_t _size, BufferUsageType behavior = BufferUsageType::Static);

        UniformBuffer(UniformBuffer const&) = delete;
        UniformBuffer(UniformBuffer&& other) noexcept {
            size = other.size;
            handle = other.handle;
            other.handle = 0;
        }

        UniformBuffer& operator=(UniformBuffer const& other) = delete;
        UniformBuffer& operator=(UniformBuffer&& other) noexcept {
            size = other.size;
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

        void bind(unsigned int binding) const noexcept;
        void bind(Shader* shader, std::string const& name, unsigned int binding) const noexcept;
        void update(void* data) const noexcept;

        // deprecate
        void update(void* data, uint32_t offset, uint32_t _size) const noexcept;

    };

    struct StorageBuffer : public Buffer {

        uint32_t size;

        StorageBuffer(void* data, uint32_t _size, BufferUsageType behavior = BufferUsageType::Static);

        StorageBuffer(StorageBuffer const&) = delete;
        StorageBuffer(StorageBuffer&& other) noexcept {
            size = other.size;
            handle = other.handle;
            other.handle = 0;
        }

        StorageBuffer& operator=(StorageBuffer const& other) = delete;
        StorageBuffer& operator=(StorageBuffer&& other) noexcept {
            size = other.size;
            handle = other.handle;
            other.handle = 0;
            return *this;
        }

        void bind(unsigned int binding) const noexcept;
        void bind(Shader* shader, std::string const& name, unsigned int binding) const noexcept;
        void update(void* data) const noexcept;

        // deprecate
        void update(void* data, uint32_t offset, uint32_t _size) const noexcept;

    };

    struct AttachmentElement {

        AttachmentType type;
        InternalFormat format = InternalFormat::Default;
        union {
            unsigned int levels = 1;
            unsigned int samples;
        };
        unsigned int handle = 0;

    };

    struct FrameBufferLayout {

        FrameBufferLayout(std::initializer_list<AttachmentElement> const& list);
        std::vector<AttachmentElement> elements;

    };

    struct FrameBuffer {

        unsigned int handle;
        uint32_t width;
        uint32_t height;
        std::vector<AttachmentElement> colorAttachments;
        AttachmentElement depthAttachment;

        FrameBuffer(FrameBufferLayout const& layout, uint32_t w = 0, uint32_t h = 0);
        ~FrameBuffer();

        FrameBuffer(FrameBuffer const&) = delete;
        FrameBuffer(FrameBuffer&& other) noexcept {
            handle = other.handle;
            width = other.width;
            height = other.height;
            other.handle = 0;
        }

        FrameBuffer& operator=(FrameBuffer const& other) = delete;
        FrameBuffer& operator=(FrameBuffer&& other) noexcept {
            handle = other.handle;
            width = other.width;
            height = other.height;
            other.handle = 0;
            return *this;
        }

        void bind() const noexcept;
        void attach(AttachmentType type, InternalFormat format = InternalFormat::Default, uint32_t binding = 0, unsigned int samples = 1) noexcept;
        void bindColorAttachmentAsTexture(unsigned int attachment, unsigned int binding) const noexcept;
        void bindDepthAttachmentAsTexture(unsigned int binding) const noexcept;
        void bindColorMipmapLevel(unsigned int attachment, unsigned int level) const noexcept;
        void setViewport(unsigned int level = 0) const noexcept;

        static void blitFrameBuffer(FrameBuffer* tar, FrameBuffer* src) noexcept;

    };

}


