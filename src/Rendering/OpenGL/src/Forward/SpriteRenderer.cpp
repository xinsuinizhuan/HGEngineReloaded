// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>
#include <HG/Core/CountStatistics.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Behaviours/Sprite.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Shader.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Materials/SpriteMaterial.hpp>
#include <HG/Rendering/OpenGL/Forward/SpriteRenderer.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Common/MeshData.hpp>

// HG::Utils
#include <HG/Utils/Surface.hpp>
#include <HG/Utils/Mesh.hpp>

// GLM
#include <gl/auxiliary/glm_uniforms.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Rendering::OpenGL::Forward::SpriteRenderer::SpriteRenderer() :
    m_spriteMaterial(nullptr),
    m_spriteData(nullptr)
{

}

HG::Rendering::OpenGL::Forward::SpriteRenderer::~SpriteRenderer()
{
    deinit();
}

void HG::Rendering::OpenGL::Forward::SpriteRenderer::deinit()
{
    Info() << "Deinitializing sprite renderer";

    delete m_spriteMaterial;
    delete m_spriteData;

    m_spriteMaterial = nullptr;
    m_spriteData = nullptr;
}

void HG::Rendering::OpenGL::Forward::SpriteRenderer::init()
{
    Info() << "Initializing sprite renderer";

    m_spriteMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<Materials::SpriteMaterial>();

    // Initializing MeshData
    m_spriteData = new Common::MeshData();

    // Binding vertex array
    m_spriteData->VAO.bind();

    // Binding vertex buffer object
    m_spriteData->VBO.bind(GL_ARRAY_BUFFER);

    HG::Utils::Mesh mesh;

    float scale = 0.01f;

    // Image is flipped by y
    mesh.Vertices = {
        {{-0.5f * scale, -0.5f * scale,  0}, {1.0f,  1.0f}},
        {{ 0.5f * scale,  0.5f * scale,  0}, {0.0f,  0.0f}},
        {{ 0.5f * scale, -0.5f * scale,  0}, {0.0f,  1.0f}},
        {{ 0.5f * scale,  0.5f * scale,  0}, {0.0f,  0.0f}},
        {{-0.5f * scale, -0.5f * scale,  0}, {1.0f,  1.0f}},
        {{-0.5f * scale,  0.5f * scale,  0}, {1.0f,  0.0f}},
    };

    mesh.Indices = {
        0, 1, 2, 3, 4, 5
    };

    // Loading data into VBO
    m_spriteData->VBO.set_data(
        mesh.Vertices.size() * sizeof(HG::Utils::Vertex),
        mesh.Vertices.data()
    );

    // Binding element buffer object
    m_spriteData->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    m_spriteData->EBO.set_data(
        mesh.Indices.size() * sizeof(uint32_t),
        mesh.Indices.data()
    );

    // Binding vertex buffer
    m_spriteData->VAO.set_vertex_buffer(0, m_spriteData->VBO, 0, sizeof(HG::Utils::Vertex));
    m_spriteData->VAO.set_vertex_buffer(1, m_spriteData->VBO, 0, sizeof(HG::Utils::Vertex));
    m_spriteData->VAO.set_vertex_buffer(2, m_spriteData->VBO, 0, sizeof(HG::Utils::Vertex));

    // Enabling attributes
    m_spriteData->VAO.set_attribute_enabled(0, true);
    m_spriteData->VAO.set_attribute_enabled(1, true);
    m_spriteData->VAO.set_attribute_enabled(2, true);
    m_spriteData->VAO.set_attribute_enabled(3, true);
    m_spriteData->VAO.set_attribute_enabled(4, true);

    // Setting
    m_spriteData->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, position)));
    m_spriteData->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, normal)));
    m_spriteData->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, uv)));
    m_spriteData->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, tangent)));
    m_spriteData->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, bitangent)));
}

void HG::Rendering::OpenGL::Forward::SpriteRenderer::render(HG::Rendering::Base::RenderBehaviour *renderBehaviour)
{
    auto spriteBehaviour = static_cast<HG::Rendering::Base::Behaviours::Sprite*>(renderBehaviour);

    auto program = static_cast<Common::ShaderData*>(m_spriteMaterial->shader()->specificData());

    program->Program.use();

    GLint location;

    if ((location = program->Program.uniform_location("model")) != -1)
    {
        program->Program.set_uniform(
            location,
            spriteBehaviour->gameObject()->transform()->localToWorldMatrix()
        );
    }

    if ((location = program->Program.uniform_location("view")) != -1)
    {
        program->Program.set_uniform(
            location,
            application()->renderer()->activeCamera()->viewMatrix()
        );
    }

    if ((location = program->Program.uniform_location("projection")) != -1)
    {
        program->Program.set_uniform(
            location,
            application()->renderer()->activeCamera()->projectionMatrix()
        );
    }

    if ((location = program->Program.uniform_location("size")) != -1)
    {
        program->Program.set_uniform(
            location,
            glm::vec2(
                spriteBehaviour->texture()->surface()->Width,
                spriteBehaviour->texture()->surface()->Height
            )
        );
    }


    if ((location = program->Program.uniform_location("tex")) != -1)
    {
        program->Program.set_uniform_1i(location, 0);

        auto textureData = static_cast<Common::Texture2DData*>(spriteBehaviour->texture()->specificData());

        if (application()->renderer()->needSetup(spriteBehaviour->texture()))
        {
            if (!application()->renderer()->setup(spriteBehaviour->texture()))
            {
                // FALLBACK HERE
                return;
            }

            textureData = static_cast<Common::Texture2DData*>(spriteBehaviour->texture()->specificData());
        }

        textureData->Texture.set_active(0);
        textureData->Texture.bind();
    }

    m_spriteData->VAO.bind();

    gl::set_blending_enabled(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(6),
        static_cast<GLsizei>(6),
        GL_UNSIGNED_INT
    );

    if (application()->countStatistics()->hasCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices))
    {
        application()->countStatistics()->add(
            HG::Core::CountStatistics::CommonCounter::NumberOfVertices,
            6
        );
    }

    m_spriteData->VAO.unbind();
}

size_t HG::Rendering::OpenGL::Forward::SpriteRenderer::getTarget()
{
    return HG::Rendering::Base::Behaviours::Sprite::RenderBehaviourId;
}
