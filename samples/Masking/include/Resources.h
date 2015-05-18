#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )

#define PROJECT_ROOT	"../../../../"
//#define RES_GLSL_PO_MASK_VERT    CINDER_RESOURCE( ../resources/, poMask_vert.glsl, 135, GLSL )
//#define RES_GLSL_PO_MASK_FRAG    CINDER_RESOURCE( ../resources/, poMask_frag.glsl, 136, GLSL )

#define RES_GLSL_PO_MASK_VERT    CINDER_RESOURCE( PROJECT_ROOT + "resources/poSceneResources/shaders/", poMask_vert.glsl, 135, GLSL )
#define RES_GLSL_PO_MASK_FRAG    CINDER_RESOURCE( PROJECT_ROOT + "resources/poSceneResources/shaders/", poMask_frag.glsl, 136, GLSL )
