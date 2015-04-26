#ifndef MESHIMPORTER_HPP
#define MESHIMPORTER_HPP

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
	namespace mesh
	{
		class MeshImporter
		{
		public:
			DIYLIB_API ~MeshImporter(void);

			DIYLIB_API static MeshFile* Import(obj::ObjFile& objFile);
			DIYLIB_API static MeshFile* Import(x::XFile& xFile);
			DIYLIB_API static MeshFile* Import(smd::SmdFile& smdFile);
			DIYLIB_API static MeshFile* Import(pmd::PmdFile& pmdFile);
			DIYLIB_API static MeshFile* Import(mqo::MqoFile& mqoFile);
			DIYLIB_API static MeshFile* Import(tds::TdsFile& tdsFile);
			DIYLIB_API static MeshFile* Import(std::string name);

		private:
			MESH_FRIENDS

			MeshImporter(void);
		};
	}
}

#endif
