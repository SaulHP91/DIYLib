#ifndef XFILEUT_HPP
#define XFILEUT_HPP

#include <vector>

#include <diy/x/xNode.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XFile;

		DIYLIB_API XNode XGetParentNode(XFile& x_file, XNode child);

		DIYLIB_API XNode XGetFirstChildNodeByType(XFile& x_file, XNode parent, XNode::NodeType type);

		DIYLIB_API void XGetChildrenNodeListByType(std::vector<XNode>& children, XFile& x_file, XNode parent, XNode::NodeType type);

		DIYLIB_API void XGetSiblingsNodeListByType(std::vector<XNode>& siblings, XFile& x_file, XNode sibling, XNode::NodeType type);
	}
}

#endif
