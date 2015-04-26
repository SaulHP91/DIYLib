#ifndef MESH_DEFINES_HPP
#define MESH_DEFINES_HPP

#define MESH_EXT ".mesh"
#define MESH_HEADER "mesh"
#define MESH_VERSION 1.0f

#define MESH_FRIENDS friend class MeshAnimatedVertex;\
friend class MeshAnimatedVertices;\
friend class MeshExporter;\
friend class MeshFace;\
friend class MeshFaces;\
friend class MeshGroup;\
friend class MeshGroups;\
friend class MeshImporter;\
friend class MeshMaterial;\
friend class MeshMaterials;\
friend class MeshMorphTarget;\
friend class MeshMorphTargets;\
friend class MeshMorphTargetVertex;\
friend class MeshMorphTargetVertices;\
friend class MeshNode;\
friend class MeshNodes;\
friend class MeshObject;\
friend class MeshObjects;\
friend class MeshFile;\
friend class MeshSkin;\
friend class MeshSkins;\
friend class MeshSkinWeight;\
friend class MeshSkinWeights;\
friend class MeshTexture;\
friend class MeshTextures;\
friend class MeshVertex;\
friend class MeshPhysics;\
friend class MeshRigidBody;\
friend class MeshRigidBodies;\
friend class MeshJoint;\
friend class MeshJoints;

/*
	version history
   1.0  base version
   1.1  introduces emissive and bump textures
   1.11 introduces per vertex tangents
   1.12 introduces per face normals and tangents
   1.2 introduces per vertex skinning and morphing information
   1.3 deprecates per face normals and tangents and introduces faces with more than three vertices
   1.4 introduces material modes
   1.5 deprecates skin and face material list
   1.51 deprecates object inverse bind matrix
   1.6 introduces comments
   1.61 deprecates node inverse bind matrix
   1.7 deprecates per object vertices, introdices per face vertices
   1.8 introduces compressed texture data
   2.0 new format
   1.0 minor changes, returns to version 1.0
*/

#define NODE_SYMMETRY_NONE 0
#define NODE_SYMMETRY_MIRROR 1
#define NODE_SYMMETRY_INVERT 2
#define NODE_SYMMETRY_POSITIVE 1.0f
#define NODE_SYMMETRY_ZERO 0.0f
#define NODE_SYMMETRY_NEGATIVE -1.0f

#define OBJECT_MIRROR_X 1
#define OBJECT_MIRROR_Y 2
#define OBJECT_MIRROR_Z 4

#define MATERIAL_CONSTANT 0
#define MATERIAL_LAMBERT 1
#define MATERIAL_PHONG 2

#define MATERIAL_ONE_SIDE 0
#define MATERIAL_TWO_SIDES 1

#define MATERIAL_NORMAL 0
#define MATERIAL_ADD 1
#define MATERIAL_SUBTRACT 2
#define MATERIAL_MULTIPLY 3

#define TEXTURE_DEPTH_TO_GL_FORMAT(depth) (depth == 1 ? GL_LUMINANCE : depth == 2 ? GL_LUMINANCE_ALPHA : depth == 3 ? GL_RGB : GL_RGBA)
#define TEXTURE_DEPTH_TO_GL_COMPONENTS(depth) (depth == 1 ? GL_LUMINANCE8 : depth == 2 ? GL_LUMINANCE8_ALPHA8 : depth == 3 ? GL_RGB8 : GL_RGBA8)
#define TEXTURE_WRAP_CLAMP 0
#define TEXTURE_WRAP_REPEAT 1
#define TEXTURE_FILTER_NEAREST 0
#define TEXTURE_FILTER_LINEAR 1
#define TEXTURE_GRAY 1
#define TEXTURE_GRAY_ALPHA 2
#define TEXTURE_RGB  3
#define TEXTURE_RGBA 4
#define TEXTURE_RED 8
#define TEXTURE_GREEN 16
#define TEXTURE_BLUE  32
#define TEXTURE_ALPHA 64
#define TEXTURE_GRAY_8 0x00000008
#define TEXTURE_GRAY_ALPHA_88 0x00000808
#define TEXTURE_RGB_565 0x00050605
#define TEXTURE_RGB_888 0x00080808
#define TEXTURE_RGB_PALETTED_8 0x00010008
#define TEXTURE_RGBA_4444 0x04040404
#define TEXTURE_RGBA_5551 0x05050501
#define TEXTURE_RGBA_8888 0x08080808
#define TEXTURE_RGB_PALETTED_8_ALPHA_8 0x01000808
#define TEXTURE_RAW 0
#define TEXTURE_COMPRESSED 1
#define TEXTURE_PALETTED 2

#define RIGIDBODY_SPHERE 0
#define RIGIDBODY_BOX 1
#define RIGIDBODY_CAPSULE 2
#define RIGIDBODY_CYLINDER 3
#define RIGIDBODY_KINEMATIC 0
#define RIGIDBODY_SIMULATED 1
#define RIGIDBODY_ALIGNED 2

#define JOINT_BALL 0
#define JOINT_HINGE 1
#define JOINT_UNIVERSAL 2

#define mov_3fv_3fv(a, b) a[0]=b[0]; a[1]=b[1]; a[2]=b[2];
#define add_eq_3fv_3fv(a, b) a[0]+=b[0]; a[1]+=b[1]; a[2]+=b[2];
#define add_eq_3fv_3fv_f(a, b, c) a[0]+=b[0]*c; a[1]+=b[1]*c; a[2]+=b[2]*c;
#define sub_eq_3fv_3fv(a, b) a[0]-=b[0]; a[1]-=b[1]; a[2]-=b[2];
#define mul_eq_3fv_f(a, b) a[0]*=b; a[1]*=b; a[2]*=b;
#define mul_eq_3fv_3fv_f(a, b, c) a[0]=b[0]*c; a[1]=b[1]*c; a[2]=b[2]*c;

#define eq_3fv_mul_9fv_3fv(a, m, b) \
   a[0] = m[0]*b[0] + m[3]*b[1] + m[6]*b[2]; \
   a[1] = m[1]*b[0] + m[4]*b[1] + m[7]*b[2]; \
   a[2] = m[2]*b[0] + m[5]*b[1] + m[8]*b[2];

#define eq_3fv_mul_16fv_3fv(a, m, b) \
   a[0] = m[0]*b[0] + m[4]*b[1] + m[8]*b[2] + m[12]; \
   a[1] = m[1]*b[0] + m[5]*b[1] + m[9]*b[2] + m[13]; \
   a[2] = m[2]*b[0] + m[6]*b[1] + m[10]*b[2] + m[14];

#define add_eq_3fv_mul_1f_mul_9fv_3fv(a, w, m, b) \
   a[0] += w * (m[0]*b[0] + m[3]*b[1] + m[6]*b[2]); \
   a[1] += w * (m[1]*b[0] + m[4]*b[1] + m[7]*b[2]); \
   a[2] += w * (m[2]*b[0] + m[5]*b[1] + m[8]*b[2]);

#define add_eq_3fv_mul_1f_mul_16fv_3fv(a, w, m, b) \
   a[0] += w * (m[0]*b[0] + m[4]*b[1] + m[8]*b[2] + m[12]); \
   a[1] += w * (m[1]*b[0] + m[5]*b[1] + m[9]*b[2] + m[13]); \
   a[2] += w * (m[2]*b[0] + m[6]*b[1] + m[10]*b[2] + m[14]);

#endif
