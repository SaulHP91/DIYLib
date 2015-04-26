#ifndef MESHFILE_HPP
#define MESHFILE_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace anim
	{
		class AnimKeyFrame;
	}

	namespace pose
	{
		class PosePose;
	}

	namespace mesh
	{
		class MeshFile
		{
		public:
			DIYLIB_API MeshFile(std::string name = std::string());
			DIYLIB_API ~MeshFile(void);

			DIYLIB_API void SetName(std::string name);
			DIYLIB_API std::string GetName(void);

			DIYLIB_API void SetComment(std::string comment);
			DIYLIB_API std::string GetComment(void);

			DIYLIB_API MeshNodes* GetNodes(void);
			DIYLIB_API MeshObjects* GetObjects(void);
			DIYLIB_API MeshMaterials* GetMaterials(void);
			DIYLIB_API MeshTextures* GetTextures(void);
			DIYLIB_API MeshPhysics* GetPhysics(void);

			DIYLIB_API void Clear(void);

			DIYLIB_API bool LoadFromFile(std::string path);
			DIYLIB_API void SaveToFile(std::string path);

			DIYLIB_API void ExportAndroidData(std::string path);

			DIYLIB_API void Combine(MeshFile* file);

			DIYLIB_API void Update(anim::AnimKeyFrame* anim_key_frame);
			DIYLIB_API void Update(pose::PosePose* pose_pose);

		private:
			MESH_FRIENDS

			std::string mName;
			std::string mComment;

			MeshNodes* mNodes;
			MeshObjects* mObjects;
			MeshMaterials* mMaterials;
			MeshTextures* mTextures;
			MeshPhysics* mPhysics;
		};
	}
}

#endif
