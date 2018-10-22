// TransparentScene
#include <Assets/Scenes/TransparentScene.hpp>
#include <Assets/Materials/GlassMaterial.hpp>

// HG::Core
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GameObjectBuilder.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>
#include <MaterialCollection.hpp>
#include <Behaviours/Mesh.hpp>
#include <Camera.hpp>
#include <CubeMap.hpp>
#include <Behaviours/CubeMap.hpp>

// HG::Standard
#include <Behaviours/FPSCameraMovement.hpp>
#include <Behaviours/ServiceInformationOverlay.hpp>

// HG::Utils
#include <Loaders/AssimpLoader.hpp>
#include <Model.hpp>
#include <Loaders/STBImageLoader.hpp>

void TransparentScene::start()
{
    // Loading bottle model
    auto bottleModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/bottle.obj")
        .guaranteeGet();

    // Creating material
    auto glassMaterial = registerResource(
        application()->renderer()
            ->materialCollection()->getMaterial<GlassMaterial>()
    );

    // Creating cubemap object
    // and loading cubemap textures
    // (async loading)
    auto cubemap = registerResource(
        new HG::Rendering::Base::CubeMap(
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/Lake/lake2_ft.tga"),
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/Lake/lake2_bk.tga"),
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/Lake/lake2_up.tga"),
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/Lake/lake2_dn.tga"),
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/Lake/lake2_lf.tga"),
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/Lake/lake2_rt.tga")
        )
    );

    // Service GO
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Service")
            .addBehaviour(new HG::Rendering::Base::Behaviours::CubeMap(cubemap))
    );

    // Adding camera
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .setGlobalPosition({0, 0, 0})
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
    );

    glm::ivec2 amount(20, 20);

    // Adding bottles
    int i = 0;
    for (int x = -(amount.x / 2); x < (amount.x / 2); ++x)
    {
        for (int z = -(amount.y / 2); z < (amount.y / 2); ++z)
        {
            addGameObject(
                HG::Core::GameObjectBuilder()
                    .setName("Bottle #" + std::to_string(i++))
                    .setGlobalPosition({x * 3, 0, z * 3})
                    .addBehaviour(new HG::Rendering::Base::Behaviours::Mesh(
                        bottleModel->children()[0]->meshes()[0],
                        glassMaterial
                    ))
            );
        }
    }
}