#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XNode XGetParentNode(XFile& x_file, XNode child)
		{
			if (child.ParentID != -1)
			{
				return *(x_file.Nodes.begin() + child.ParentID);
			}

			return XNode();
		}

		XNode XGetFirstChildNodeByType(XFile& x_file, XNode parent, XNode::NodeType type)
		{
			for (std::vector<XNode>::iterator i = x_file.Nodes.begin(); i != x_file.Nodes.end(); ++i)
			{
				if (i->ParentID == parent.ID && i->Type == type)
				{
					return *i;
				}
			}

			return XNode();
		}

		void XGetChildrenNodeListByType(std::vector<XNode>& children, XFile& x_file, XNode parent, XNode::NodeType type)
		{
			children.clear();

			for (std::vector<XNode>::iterator i = x_file.Nodes.begin(); i != x_file.Nodes.end(); ++i)
			{
				if (i->ParentID == parent.ID && i->Type == type)
				{
					children.push_back(*i);
				}
			}
		}

		void XGetSiblingsNodeListByType(std::vector<XNode>& siblings, XFile& x_file, XNode sibling, XNode::NodeType type)
		{
			siblings.clear();

			for (std::vector<XNode>::iterator i = x_file.Nodes.begin(); i != x_file.Nodes.end(); ++i)
			{
				if (i->ID != sibling.ID && i->ParentID == sibling.ParentID && i->Type == type)
				{
					siblings.push_back(*i);
				}
			}
		}
	}
}
