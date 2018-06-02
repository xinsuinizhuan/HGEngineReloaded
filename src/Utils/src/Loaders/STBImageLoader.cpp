#include "Loaders/STBImageLoader.hpp"
#include <stb_image.h>

UTILS_MODULE_NS::STBImageLoader::STBImageLoader()
{

}

UTILS_MODULE_NS::STBImageLoader::ResultType HG::Utils::STBImageLoader::load(const std::byte* data, std::size_t size)
{
    // Trying to load image with stbb
    int width;
    int height;
    int bpp;

    auto resultData = stbi_load_from_memory(
        reinterpret_cast<const stbi_uc*>(data),
        static_cast<int>(size),
        &width,
        &height,
        &bpp,
        4
    );

    if (resultData == nullptr)
    {
        return nullptr;
    }

    auto surface = std::make_shared<Surface>(
        [](uint8_t* data, int, int, int)
        {
            stbi_image_free(data);
        }
    );

    surface->Data = resultData;
    surface->Width = width;
    surface->Height = height;
    surface->Bpp = bpp;

    return surface;
}
