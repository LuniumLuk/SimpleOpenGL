#pragma once

#include "SimpleGL/Core/Texture.h"
#include "SimpleGL/Core/Shader.h"
#include "SimpleGL/Core/Mesh.h"

namespace SGL {

    struct MeshTexture {

        std::string assetName;
        std::string mapName;

    };

    struct Model {

        glm::mat4 transform = glm::mat4(1.0f);
        /*
        *   Textures of the model, shared among all nodes
        */
        std::unordered_map<std::string, Texture*> textures;

        struct Node {

            Node() = default;
            ~Node() noexcept;

            PrimitiveType type;
            std::vector<Node*> children;
            std::vector<Mesh*> meshes;
            /*
            *   Textures of node meshes, indexing on the textures of the model
            */
            std::vector<std::pair<std::string, Texture*>> textures;

        };

        std::string directory;
        Node* rootNode = nullptr;

        Model() = default;
        ~Model() noexcept;

        Model(Model const&) = delete;
        Model(Model&& other) = delete;

        Model& operator=(Model const&) = delete;
        Model& operator=(Model&& other) = delete;

        void draw(Shader* shader) noexcept;
        void drawInstanced(Shader* shader, uint32_t num, VertexBuffer* instanceBuffer = nullptr, uint32_t divisor = 0) noexcept;

        static std::unique_ptr<Model> loadAssimp(std::string const& path) noexcept;
        static std::unique_ptr<Model> loadTinyObjLoader(std::string const& path) noexcept;

    };

}
