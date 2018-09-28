// HG::Core
#include <Application.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>
#include <Camera.hpp>
#include <MaterialCollection.hpp>
#include <Behaviours/CubeMap.hpp>
#include <CubeMap.hpp>

// HG::Rendering::OpenGL
#include <Forward/CubeMapRenderer.hpp>
#include <Materials/SkyboxMaterial.hpp>
#include <Common/ShaderData.hpp>
#include <Common/CubeMapTextureData.hpp>

// gl
#include <gl/auxiliary/glm_uniforms.hpp>

HG::Rendering::OpenGL::Forward::CubeMapRenderer::CubeMapRenderer() :
    m_vao(0),
    m_vbo(0)
{

}

HG::Rendering::OpenGL::Forward::CubeMapRenderer::~CubeMapRenderer()
{
    deinit();
}

void HG::Rendering::OpenGL::Forward::CubeMapRenderer::deinit()
{
    Info() << "Deinitialializing cubemap renderer";

    m_vao = std::move(gl::vertex_array(0));
    m_vbo = std::move(gl::buffer(0));

    delete m_skyboxMaterial;
    m_skyboxMaterial = nullptr;
}

void HG::Rendering::OpenGL::Forward::CubeMapRenderer::init()
{
    Info() << "Initialializing cubemap renderer";

    static float skyboxVertices[] = {
        // positions
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
    };

    // Loading skybox material
    m_skyboxMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<Materials::SkyboxMaterial>();

    // Setting up VAO/VBO
    m_vao = std::move(gl::vertex_array());
    m_vbo = std::move(gl::buffer());

    m_vao.bind();
    m_vbo.bind(GL_ARRAY_BUFFER);

    m_vbo.set_data(
        sizeof(skyboxVertices),
        skyboxVertices,
        GL_STATIC_DRAW
    );

    m_vao.set_vertex_buffer(0, m_vbo, 0, sizeof(float) * 3);
    m_vao.set_attribute_enabled(0, true);
    m_vao.set_attribute_format(0, 3, GL_FLOAT);
}

void HG::Rendering::OpenGL::Forward::CubeMapRenderer::render(HG::Rendering::Base::RenderBehaviour* renderBehaviour)
{
    auto cubemapBehaviour = static_cast<HG::Rendering::Base::Behaviours::CubeMap*>(renderBehaviour);
    auto cubemap = cubemapBehaviour->cubeMap();

    if (cubemap == nullptr)
    {
        return;
    }

    if (application()->renderer()->needSetup(cubemap))
    {
        // If can't setup renderer - skip it
        if (!application()->renderer()->setup(cubemap))
        {
            return;
        }
    }

    auto cubemapExternalData = static_cast<Common::CubeMapTextureData*>(cubemap->specificData());

    auto* program = &static_cast<Common::ShaderData*>(
        m_skyboxMaterial
        ->shader()
        ->specificData()
    )->Program;

    program->use();

    GLint location;

    cubemapExternalData->Texture.set_active(0);
    cubemapExternalData->Texture.bind();

    static std::string skyboxName = "skybox";
    if ((location = program->uniform_location(skyboxName)) != -1)
    {
        program->set_uniform(location, 0);
    }

    static std::string projectionName = "projection";
    if ((location = program->uniform_location(projectionName)) != -1)
    {
        program->set_uniform(
            location,
            application()->renderer()->activeCamera()->projectionMatrix()
        );
    }

    static std::string viewName = "view";
    if ((location = program->uniform_location(viewName)) != -1)
    {
        program->set_uniform(
            location,
            glm::mat4(glm::mat3(application()->renderer()->activeCamera()->viewMatrix()))
        );
    }

    m_vao.bind();

    gl::draw_arrays(
        GL_TRIANGLES,
        0, 36
    );

    m_vao.unbind();
    cubemapExternalData->Texture.unbind();
}

size_t HG::Rendering::OpenGL::Forward::CubeMapRenderer::getTarget()
{
    return HG::Rendering::Base::Behaviours::CubeMap::RenderBehaviourId;
}
