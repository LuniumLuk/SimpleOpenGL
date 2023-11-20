#include "PCH.h"

#include "SimpleGL/Core/Model.h"
#include "SimpleGL/Core/Texture.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "tinyobjloader/tiny_obj_loader.h"

namespace SGL {

    Model::~Model() noexcept {
        delete rootNode;
        for (auto& texture : textures) {
            delete texture.second;
        }
    }

    Model::Node::~Node() noexcept {
        for (auto& mesh : meshes) {
            delete mesh;
        }
        for (auto& child : children) {
            delete child;
        }
    }

    static void drawNode(Shader* shader, Model::Node* node) noexcept {
        for (uint32_t i = 0; i < node->textures.size(); ++i) {
            node->textures[i].second->bind(shader, node->textures[i].first, i);
        }

        for (uint32_t i = 0; i < node->meshes.size(); ++i) {
            node->meshes[i]->bind();
            node->meshes[i]->draw();
        }

        for (uint32_t i = 0; i < node->children.size(); ++i) {
            drawNode(shader, node->children[i]);
        }
    };

    static void drawNodeInstanced(Shader* shader, Model::Node* node, uint32_t num, VertexBuffer* instanceBuffer, uint32_t divisor) noexcept {
        for (uint32_t i = 0; i < node->textures.size(); ++i) {
            node->textures[i].second->bind(shader, node->textures[i].first, i);
        }

        for (uint32_t i = 0; i < node->meshes.size(); ++i) {
            node->meshes[i]->bind();
            node->meshes[i]->drawInstanced(num, instanceBuffer, 0, divisor);
        }

        for (uint32_t i = 0; i < node->children.size(); ++i) {
            drawNode(shader, node->children[i]);
        }
    };

    void Model::draw(Shader* shader) noexcept {
        shader->setMat4("uModel", transform);
        drawNode(shader, rootNode);
    }

    void Model::drawInstanced(Shader* shader, uint32_t num, VertexBuffer* instanceBuffer, uint32_t divisor) noexcept {
        shader->setMat4("uModel", transform);
        drawNodeInstanced(shader, rootNode, num, instanceBuffer, divisor);
    }

    static inline std::string getAssimpTextureFilename(aiMaterial* mat, uint32_t i, aiTextureType type) noexcept {
        aiString filename;
        mat->GetTexture(type, i, &filename);
        return std::string(filename.C_Str());
    }

    static std::vector<std::pair<std::string, Texture*>> processAssimpMaterialTextures(Model* model, aiMaterial* mat, aiTextureType type, std::string const& name) noexcept {
        std::vector<std::pair<std::string, Texture*>> textures;
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            auto filename = getAssimpTextureFilename(mat, i, type);
            if (model->textures.find(filename) == model->textures.end()) {
                Texture* texture = new Texture2D(model->directory + filename);
                model->textures.insert(std::make_pair(filename, texture));
            }
            textures.emplace_back(std::make_pair(name + std::to_string(i), model->textures[filename]));
        }
        return textures;
    }

    static void processAssimpMesh(Model* model, Model::Node* mNode, aiMesh* mesh, const aiScene* scene) noexcept {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texcoord;
            glm::vec3 tangent;
            glm::vec3 bitangent;
        };

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<std::pair<std::string, Texture*>> textures;

        for (size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex = {};
            glm::vec3 vector = {};
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            // normals
            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            // texture coordinates
            // here we only use TEXCOORD_0
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texcoord = vec;
                // tangent
                if (mesh->HasTangentsAndBitangents()) {
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.bitangent = vector;
                }
                else {
                    vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
                    vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else {
                vertex.texcoord = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        auto diffuseMaps = processAssimpMaterialTextures(model, material, aiTextureType_DIFFUSE, "uDiffuseMap");
        mNode->textures.insert(mNode->textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        auto specularMaps = processAssimpMaterialTextures(model, material, aiTextureType_SPECULAR, "uSpecularMap");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        auto normalMaps = processAssimpMaterialTextures(model, material, aiTextureType_NORMALS, "uNormalMap");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        auto heightMaps = processAssimpMaterialTextures(model, material, aiTextureType_HEIGHT, "uHeightMap");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        mNode->meshes.push_back(new Mesh(
            (float*)vertices.data(),
            static_cast<uint32_t>(vertices.size() * sizeof(Vertex)),
            VertexBufferLayout{
                {DataType::Float3},
                {DataType::Float3},
                {DataType::Float2},
                {DataType::Float3},
                {DataType::Float3}
            },
            indices.data(),
            static_cast<uint32_t>(indices.size() * sizeof(uint32_t)),
            PrimitiveType::Triangles));
    }

    static void processAssimpNode(Model* model, Model::Node* mNode, aiNode* node, const aiScene* scene) noexcept {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processAssimpMesh(model, mNode, mesh, scene);
        }

        for (size_t i = 0; i < node->mNumChildren; i++) {
            auto newNode = new Model::Node();
            mNode->children.push_back(newNode);
            processAssimpNode(model, newNode, node->mChildren[i], scene);
        }
    }

    std::unique_ptr<Model> Model::loadAssimp(std::string const& path) noexcept {
        auto model = std::make_unique<Model>();
        model->directory = path.substr(0, path.find_last_of('/') + 1);
        model->rootNode = new Node();

        Assimp::Importer importer;
        aiScene const* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            SGL_LOG_ERROR("Failed to load model: {0}", importer.GetErrorString());
            return model;
        }

        processAssimpNode(model.get(), model->rootNode, scene->mRootNode, scene);

        return model;
    }

    std::unique_ptr<Model> Model::loadTinyObjLoader(std::string const& path) noexcept {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texcoord;
            glm::vec3 tangent;
            glm::vec3 bitangent;
        };

        auto model = std::make_unique<Model>();
        model->directory = path.substr(0, path.find_last_of('/') + 1);
        model->rootNode = new Node();

        tinyobj::ObjReaderConfig readerConfig;
        readerConfig.mtl_search_path = model->directory;
        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(path, readerConfig)) {
            SGL_LOG_ERROR("Failed to load model: {0}", reader.Error());
            return model;
        }

        if (!reader.Warning().empty()) {
            SGL_LOG_WARN(reader.Warning());
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        size_t verticesNum = attrib.vertices.size() / 3;
        for (size_t i = 0; i < verticesNum; i++) {
            Vertex vertex = {};
            vertex.position = glm::vec3(
                attrib.vertices[i * 3 + 0],
                attrib.vertices[i * 3 + 1],
                attrib.vertices[i * 3 + 2]
            );
            vertices.push_back(vertex);
        }

        for (size_t s = 0; s < shapes.size(); s++) {
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                if (fv != 3) continue;
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    int vidx = shapes[s].mesh.indices[index_offset + v].vertex_index;

                    if (idx.normal_index >= 0) {
                        vertices[vidx].normal = glm::vec3(
                            attrib.normals[3 * size_t(idx.normal_index) + 0],
                            attrib.normals[3 * size_t(idx.normal_index) + 1],
                            attrib.normals[3 * size_t(idx.normal_index) + 2]
                        );
                    }

                    if (idx.texcoord_index >= 0) {
                        vertices[vidx].texcoord = glm::vec2(
                            attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
                            attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
                        );
                    }

                    indices.push_back(vidx);
                }
                index_offset += fv;
            }
        }

        model->rootNode->meshes.push_back(new Mesh(
            (float*)vertices.data(),
            static_cast<uint32_t>(vertices.size() * sizeof(Vertex)),
            VertexBufferLayout{
                {DataType::Float3},
                {DataType::Float3},
                {DataType::Float2},
                {DataType::Float3},
                {DataType::Float3}
            },
            indices.data(),
            static_cast<uint32_t>(indices.size() * sizeof(uint32_t)),
            PrimitiveType::Triangles));

        return model;
    }

}
