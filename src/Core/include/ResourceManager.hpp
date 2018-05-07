#pragma once

#include <CurrentLogger.hpp>
#include "ResourceAccessor.hpp"

namespace UTILS_MODULE_NS
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
}

namespace CORE_MODULE_NS
{
    /**
     * @brief Class, that describes resource manager.
     * @tparam Accessor Files accessor type. (Can't be changed on runtime)
     */
    class ResourceManager
    {
        friend class Application;

    public:

        // Disable copying
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        /**
         * @brief Constructor.
         */
        ResourceManager();

        /**
         * @brief Destructor.
         */
        virtual ~ResourceManager();

        /**
         * @brief Method for getting resource accessor for
         * low level resources loading.
         * @return Pointer to resource accessor.
         */
        ResourceAccessor* resourceAccessor() const;

        /**
         * @brief Method for setting resource accessor for
         * resource manager. Implemented as template method
         * to derive creation to <code>ResourceManager</code>
         * class.
         * @param accessor Pointer to resource accessor
         * implementation.
         */
        template<typename Accessor>
        typename std::enable_if<
            std::is_base_of<ResourceAccessor, Accessor>::value
        >::type setResourceAccessor()
        {
            if (m_accessor != nullptr)
            {
                Warning() << "Redefining ResourceAccessor accessor is bad practice.";
                delete m_accessor;
                m_accessor = nullptr;
            }

            m_accessor = new Accessor;
        }

        /**
         * @brief Method for loading mesh with
         * using of specified mesh formatter.
         * @tparam MeshLoader Mesh formatter object.
         * @param id Resource id for resource accessor.
         * @return
         */
        template<typename MeshLoader>
        ::UTILS_MODULE_NS::MeshPtr loadMesh(const std::string& id)
        {
            if (m_accessor == nullptr)
            {
                Error() << "Trying to load \"" << id << "\" resource, without ResourceAccessor.";
                return nullptr;
            }

            auto data = m_accessor->loadRaw(id);

            if (data == nullptr)
            {
                Error() << "Can't load \"" << id << "\" mesh resource. See errors above.";

                return nullptr;
            }

            MeshLoader loader;

            return loader.load(data);
        }

    private:

        ResourceAccessor* m_accessor;
    };
}
