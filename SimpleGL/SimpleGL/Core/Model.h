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

        /*
        * Vertex attributes are passed to shader as follows:
        *   layout (location = 0) in vec3 aPosition;
        *   layout (location = 1) in vec3 aNormal;
        *   layout (location = 2) in vec2 aTexcoord;
        *   layout (location = 3) in vec3 aTangent;
        *   layout (location = 4) in vec3 aBitangent;
        * Also it will automatically bind the following textures:
        *   uniform sampler2D uDiffuseMap0;
        *   uniform sampler2D uDiffuseMap1;
        *   ...
        *   uniform sampler2D uSpecularMap0;
        *   ...
        *   uniform sampler2D uNormalMap0;
        *   ...
        *   uniform sampler2D uHeightMap0;
        *   ...
        * Meanwhile, it will set the following uniforms:
        *   uniform mat4 uModel;
        */
        void draw(Shader* shader) noexcept;
        void drawInstanced(Shader* shader, uint32_t num, VertexBuffer* instanceBuffer = nullptr, uint32_t divisor = 0) noexcept;

        static std::unique_ptr<Model> loadAssimp(std::string const& path) noexcept;
        static std::unique_ptr<Model> loadTinyObjLoader(std::string const& path) noexcept;

    };

}
