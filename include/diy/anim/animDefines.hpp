#ifndef ANIMDEFINES_HPP
#define ANIMDEFINES_HPP

#define ANIM_EXT ".anim"

#define ANIM_HEADER "anim"
#define ANIM_VERSION 1.0f

#define ANIM_FRIENDS friend class AnimAnimation; \
friend class AnimAnimations; \
friend class AnimFile; \
friend class AnimKeyFrame; \
friend class AnimKeyFrames; \
friend class AnimMorphTarget; \
friend class AnimMorphTargets; \
friend class AnimNode; \
friend class AnimNodes; \
friend class AnimObject; \
friend class AnimObjects;

#define ANIMATION_STEP 0
#define ANIMATION_RAMP 1
#define ANIMATION_SINUSOID 2
#define ANIMATION_EXPONENTIAL 3

#endif
