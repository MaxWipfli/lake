set(LAKE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_sources(lake
    PRIVATE
        dummy.cpp
    PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    BASE_DIRS ${LAKE_INCLUDE_DIR}
    FILES
        "${LAKE_INCLUDE_DIR}/lake/array.hpp"
        "${LAKE_INCLUDE_DIR}/lake/extras.hpp"
        "${LAKE_INCLUDE_DIR}/lake/fixed_array.hpp"
        "${LAKE_INCLUDE_DIR}/lake/hash.hpp"
        "${LAKE_INCLUDE_DIR}/lake/iterator.hpp"
        "${LAKE_INCLUDE_DIR}/lake/optional.hpp"
        "${LAKE_INCLUDE_DIR}/lake/span.hpp"
        "${LAKE_INCLUDE_DIR}/lake/string_view.hpp"
        "${LAKE_INCLUDE_DIR}/lake/types.hpp"
        "${LAKE_INCLUDE_DIR}/lake/type_traits.hpp"
        "${LAKE_INCLUDE_DIR}/lake/unique_ptr.hpp"
        "${LAKE_INCLUDE_DIR}/lake/vector.hpp"
)
