aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MATH_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/double DOUBLE_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/fraction FRACTION_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/linear LINEAR_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/long LONG_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/polynomial POLY_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../steps STEPS_DIR)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/finite_field FINITE_FIELD_DIR)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/libgmp_6.2.1-1_msvc16/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../steps
)

add_library(MathLib ${MATH_DIR} ${DOUBLE_DIR} ${FRACTION_DIR} ${LINEAR_DIR} ${POLY_DIR} ${LONG_DIR} ${STEPS_DIR} ${FINITE_FIELD_DIR})