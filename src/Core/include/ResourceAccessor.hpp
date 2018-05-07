#pragma once

#include <cstddef>
#include <string>

namespace CORE_MODULE_NS
{
    /**
     * @brief Interface for classes, that
     * has to provide access to resources.
     */
    class ResourceAccessor
    {
    public:

        class Data
        {
        public:

            /**
             * @brief Method for getting received data
             * size.
             * @return Size in bytes.
             */
            virtual std::size_t size() = 0;

            /**
             * @brief Method for getting data.
             * @return Data.
             */
            virtual std::byte* data() const = 0;

        };

        using DataPtr = std::shared_ptr<Data>;

        /**
         * @brief Virtual destructor.
         */
        virtual ~ResourceAccessor() = default;

        /**
         * @brief Method for loading raw data.
         * @param id ID of resource.
         * @return Loaded data.
         */
        virtual DataPtr loadRaw(const std::string& id) = 0;
    };
}

