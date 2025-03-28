# Agregar todos los archivos .c en el directorio src
file(GLOB PROJECT_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# Especificar la carpeta de includes
SET(PROJECT_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)