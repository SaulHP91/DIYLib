#ifndef MESHEXPORTER_HPP
#define MESHEXPORTER_HPP

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
		class MeshExporter
		{
		public:
			DIYLIB_API ~MeshExporter(void);

			DIYLIB_API static void ExportCCode(MeshFile* file, std::string name);

		private:
			MESH_FRIENDS

			MeshExporter(void);
		};
	}
}

#endif
