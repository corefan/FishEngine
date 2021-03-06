aux_source_directory( ${CMAKE_CURRENT_LIST_DIR} SRCS )
AUX_SOURCE_DIRECTORY( ${CMAKE_CURRENT_LIST_DIR}/SceneManagement SceneManagement_SRCS)
SOURCE_GROUP(SceneManagement FILES ${SceneManagement_SRCS})
FILE(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.hpp)
SET(
    Serialization_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/Archive.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Archive.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/helper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/NameValuePair.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/types/list.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/types/map.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/types/vector.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/archives/binary.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/archives/text.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/archives/yaml.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/archives/yaml.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization/archives/YAMLInputArchive.hpp
    ${CMAKE_CURRENT_LIST_DIR}/generate/EngineClassSerialization.cpp
)
foreach (x ${Serialization_SRCS})
    IF(NOT EXISTS ${x})
        MESSAGE(FATAL_ERROR "source files does not exist: ${x}")
    ENDIF ()
endforeach ()
SOURCE_GROUP(Serialization FILES ${Serialization_SRCS})

foreach (x AssetBundle Resources)
    foreach (ext hpp cpp)
        set(f ${CMAKE_CURRENT_LIST_DIR}/${x}.${ext})
        SET(Asset_SRCS ${Asset_SRCS} ${f})
        IF(NOT EXISTS ${f})
            MESSAGE(FATAL_ERROR "source files does not exist: ${f}")
        ENDIF ()
    endforeach ()
endforeach()
SOURCE_GROUP(Asset FILES ${Asset_SRCS})

foreach (x Vector2 Vector3 Vector4 Matrix4x4 Quaternion Mathf Frustum IntVector Ray Bounds)
    foreach (ext hpp cpp)
        set(f ${CMAKE_CURRENT_LIST_DIR}/${x}.${ext})
        SET(Math_SRCS ${Math_SRCS} ${f})
    endforeach ()
endforeach()
SOURCE_GROUP(Math FILES ${Math_SRCS})

foreach (x Attribute ReflectEnum ReflectClass ClassID Component_gen)
    foreach (ext hpp cpp)
        set(f ${CMAKE_CURRENT_LIST_DIR}/${x}.${ext})
        SET(Reflect_SRCS ${Reflect_SRCS} ${f})
    endforeach ()
endforeach()
SOURCE_GROUP(Reflect FILES ${Reflect_SRCS})

foreach (x Collider MeshCollider PhysicsSystem SphereCollider BoxCollider CapsuleCollider Rigidbody)
    foreach (ext hpp cpp)
        set(f ${CMAKE_CURRENT_LIST_DIR}/${x}.${ext})
        SET(Physics_SRCS ${Physics_SRCS} ${f})
    endforeach ()
endforeach()
SOURCE_GROUP(Physics FILES ${Physics_SRCS})

foreach (x GL GLEnvironment Graphics Color Light Material Mesh MeshFilter MeshRenderer Pipeline QualitySettings RenderSettings RenderSystem RenderTarget RenderTexture Renderer Shader ShaderCompiler ShaderProperty ShaderVariables_gen SkinnedMeshRenderer Skybox Texture Texture2D TextureProperty Gizmos)
    foreach (ext hpp cpp)
        set(f ${CMAKE_CURRENT_LIST_DIR}/${x}.${ext})
        SET(Render_SRCS ${Render_SRCS} ${f})
    endforeach ()
endforeach()
SOURCE_GROUP(Render FILES ${Render_SRCS})


add_library (FishEngine STATIC ${SRCS} ${HEADERS} ${Serialization_SRCS} ${SceneManagement_SRCS})
