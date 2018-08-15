#include <Common/MeshDataProcessor.hpp>
#include <Common/MeshData.hpp>
#include <Behaviours/Mesh.hpp>
#include <Mesh.hpp>

bool HG::Rendering::OpenGL::Common::MeshDataProcessor::setup(HG::Rendering::Base::RenderData* renderData)
{
    auto meshBehaviour = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(renderData);

    if (meshBehaviour->mesh() == nullptr)
    {
        Error() << "Can't setup render without set mesh.";
        return false;
    }

    Common::MeshData* data = nullptr;

    if ((data = meshBehaviour->specificData<Common::MeshData>()) == nullptr)
    {
        data = meshBehaviour->setSpecificData<Common::MeshData>();
    }

    // Binding vertex array
    data->VAO.bind();

    // Binding vertex buffer object
    data->VBO.bind(GL_ARRAY_BUFFER);

    auto mesh = meshBehaviour->mesh();

    // Loading data into VBO
    data->VBO.set_data(
        mesh->Vertices.size() * sizeof(HG::Utils::Vertex),
        mesh->Vertices.data()
    );

    // Binding element buffer object
    data->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    data->EBO.set_data(
        mesh->Indices.size() * sizeof(uint32_t),
        mesh->Indices.data()
    );

    // Binding vertex buffer
    data->VAO.set_vertex_buffer(0, data->VBO, 0, sizeof(HG::Utils::Vertex));
    data->VAO.set_vertex_buffer(1, data->VBO, 0, sizeof(HG::Utils::Vertex));
    data->VAO.set_vertex_buffer(2, data->VBO, 0, sizeof(HG::Utils::Vertex));

    // Enabling attributes
    data->VAO.set_attribute_enabled(0, true);
    data->VAO.set_attribute_enabled(1, true);
    data->VAO.set_attribute_enabled(2, true);
    data->VAO.set_attribute_enabled(3, true);
    data->VAO.set_attribute_enabled(4, true);

    // Setting
    data->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, position)));
    data->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, normal)));
    data->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, uv)));
    data->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, tangent)));
    data->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, bitangent)));

    return true;
}

std::size_t HG::Rendering::OpenGL::Common::MeshDataProcessor::getTarget()
{
    return HG::Rendering::Base::Behaviours::Mesh::Id;
}
