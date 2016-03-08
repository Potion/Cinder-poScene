# TextSampleAdvanced
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE on )

get_filename_component( CINDER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE )
include( ${CINDER_DIR}/linux/cmake/Cinder.cmake )

project( TextSampleAdvanced )

get_filename_component( PO_SCENE_BLOCK_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../src" ABSOLUTE )
get_filename_component( SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src" ABSOLUTE )
get_filename_component( INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" ABSOLUTE )

if( NOT TARGET cinder${CINDER_LIB_SUFFIX} )
    find_package( cinder REQUIRED
        PATHS ${PROJECT_SOURCE_DIR}/../../../../../linux/${CMAKE_BUILD_TYPE}/${CINDER_OUT_DIR_PREFIX}
        $ENV{Cinder_DIR}/linux/${CMAKE_BUILD_TYPE}/${CINDER_OUT_DIR_PREFIX}
    )
endif()

# Use PROJECT_NAME since CMAKE_PROJET_NAME returns the top-level project name.
set( EXE_NAME ${PROJECT_NAME} )

set( PO_SCENE_SRC ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poNode.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poNodeContainer.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poMatrixSet.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poImage.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poEvents.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poEventCenter.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poTextBox.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poShape.cpp
                  ${PO_SCENE_BLOCK_SRC_DIR}/poScene/poScene.cpp
                  )
set( SRC_FILES
    ${SRC_DIR}/TextSampleAdvancedApp.cpp
    ${SRC_DIR}/TextSampleAdvanced.cpp
    ${SRC_DIR}/TextComponent.cpp
    ${SRC_DIR}/Scroller.cpp
    ${PO_SCENE_SRC}
)

add_executable( "${EXE_NAME}" ${SRC_FILES} )

target_include_directories(
	"${EXE_NAME}"
    PUBLIC ${INC_DIR} 
           ${SRC_DIR}
           ${PO_SCENE_BLOCK_SRC_DIR}
           ${PO_SCENE_BLOCK_SRC_DIR}/poScene
           
)

target_link_libraries( "${EXE_NAME}" cinder${CINDER_LIB_SUFFIX} )
