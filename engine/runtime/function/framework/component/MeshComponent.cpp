#include "MeshComponent.h"

#include "backend/GLUtils.h"

Primitive::Primitive(const Geometry &geometry,
                     std::shared_ptr<MaterialInstance> material)
    : geometry(geometry), matInstance(material) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    auto &vertices = geometry.vertices;
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    auto &indices = geometry.indices;
    if (indices.size()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int), &indices[0],
                     GL_STATIC_DRAW);
    }

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Bitangent));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, MAX_BONE_INFLUENCE, GL_INT, GL_FALSE,
                          sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));

    glBindVertexArray(0);
}

void Primitive::Draw() {
    {
        /*
        if (rs.depthFunc == RasterState::DepthFunc::A && !rs.depthWrite) {
            gl.disable(GL_DEPTH_TEST);
        } else {
            gl.enable(GL_DEPTH_TEST);
            gl.depthFunc(getDepthFunc(rs.depthFunc));
            gl.depthMask(GLboolean(rs.depthWrite));
        }
        */
        matInstance->depthTest() ? glEnable(GL_DEPTH_TEST)
                                 : glDisable(GL_DEPTH_TEST);
        // opengl 开关深度写入接口,名字是mask，但功能仅仅是开关
        matInstance->depthWrite() ? glDepthMask(GL_TRUE)
                                  : glDepthMask(GL_FALSE);

        // set Culling State
        auto cullingMode = matInstance->cullingMode();
        if (cullingMode == CullingMode::NONE) {
            glDisable(GL_CULL_FACE);
        } else {
            glEnable(GL_CULL_FACE);
            glCullFace(getCullingMode(cullingMode));
        }
    }

    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    auto &textures = geometry.textures;
    for (unsigned int i = 0; i < textures.size(); i++) {
        // active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(matInstance->shaderID(),
                                         (name + number).c_str()),
                    i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    auto indices = geometry.indices;
    if (indices.size())
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                       GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0,
                     static_cast<unsigned int>(geometry.vertices.size()));

    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

// void MeshComponent::postLoadResource(std::weak_ptr<Object> parent_object) {
//     Component::postLoadResource(parent_object);
// }

void MeshComponent::tick(float delta_time) {
    for (auto &sub_mesh : m_primitives) {
        sub_mesh.Draw();
    }
}
