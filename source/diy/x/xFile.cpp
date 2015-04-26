#include <diy/x/x.hpp>

#include <fstream>
#include <stack>

namespace diy
{
	namespace x
	{
		XFile::XFile(void)
		{
			;
		}

		XFile::~XFile(void)
		{
			Clear();
		}

		void XFile::Clear(void)
		{
			Name.clear();

			Nodes.clear();
			Nodes.swap(std::vector<XNode>(Nodes));

			AnimTicksPerSecond.clear();
			AnimTicksPerSecond.swap(std::vector<XAnimTicksPerSecond>(AnimTicksPerSecond));

			Frames.clear();
			Frames.swap(std::vector<XFrame>(Frames));

			FrameTransformMatrices.clear();
			FrameTransformMatrices.swap(std::vector<XFrameTransformMatrix>(FrameTransformMatrices));

			Meshes.clear();
			Meshes.swap(std::vector<XMesh>(Meshes));

			MeshNormals.clear();
			MeshNormals.swap(std::vector<XMeshNormals>(MeshNormals));

			MeshTextureCoords.clear();
			MeshTextureCoords.swap(std::vector<XMeshTextureCoords>(MeshTextureCoords));

			MeshMaterialList.clear();
			MeshMaterialList.swap(std::vector<XMeshMaterialList>(MeshMaterialList));

			Materials.clear();
			Materials.swap(std::vector<XMaterial>(Materials));

			SkinMeshHeaders.clear();
			SkinMeshHeaders.swap(std::vector<XSkinMeshHeader>(SkinMeshHeaders));

			SkinWeights.clear();
			SkinWeights.swap(std::vector<XSkinWeights>(SkinWeights));

			TextureFileNames.clear();
			TextureFileNames.swap(std::vector<XTextureFileName>(TextureFileNames));

			AnimationSets.clear();
			AnimationSets.swap(std::vector<XAnimationSet>(AnimationSets));

			Animations.clear();
			Animations.swap(std::vector<XAnimation>(Animations));

			AnimationKeys.clear();
			AnimationKeys.swap(std::vector<XAnimationKey>(AnimationKeys));
		}

		bool XFile::ParseFromFile(std::string filename)
		{
			Clear();

			std::ifstream in(filename.c_str());
			if (in.is_open())
			{
				char header[256];
				in.getline(header, 255);
				if
					(
					_stricmp(header, "xof 0302txt 0032") &&
					_stricmp(header, "xof 0302txt 0064") &&
					_stricmp(header, "xof 0303txt 0032") &&
					_stricmp(header, "xof 0303txt 0064")
					)
				{
					in.close();
					return false;
				}

				char buffer[256];
				_splitpath(filename.c_str(), 0, 0, buffer, 0);
				Name = buffer;

				std::stack<XNode> xNodeStack;
				XName name;
				while (!in.eof())
				{
					if (name.ParseFromFStream(in))
					{
						if (!_stricmp(name.Text.c_str(), ";"))
						{
							;
						}
						else if (!_stricmp(name.Text.c_str(), "{"))
						{
							xNodeStack.push(xNodeStack.top());
						}
						else if (!_stricmp(name.Text.c_str(), "template"))
						{
							in.ignore(256, '}');
						}
						else if (!_stricmp(name.Text.c_str(), "AnimTicksPerSecond"))
						{
							if (name.ParseFromFStream(in))
							{
								AnimTicksPerSecond.push_back(XAnimTicksPerSecond());
								if (AnimTicksPerSecond.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::AnimTicksPerSecond;
									Nodes.back().Index = AnimTicksPerSecond.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "Frame"))
						{
							if (name.ParseFromFStream(in))
							{
								if (!_stricmp(name.Text.c_str(), "{"))
								{
									sprintf(buffer, "frame%i", Frames.size());
									name.Text = buffer;
								}
								else
								{
									in.ignore(256, '{');
								}

								Frames.push_back(XFrame());

								Nodes.push_back(XNode());
								Nodes.back().Name = name;
								Nodes.back().ID = Nodes.size() - 1;
								Nodes.back().Type = XNode::Frame;
								Nodes.back().Index = Frames.size() - 1;
								if (!xNodeStack.empty())
								{
									Nodes.back().ParentID = xNodeStack.top().ID;
									Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
								}
								xNodeStack.push(Nodes.back());
							}
						}
						else if (!_stricmp(name.Text.c_str(), "FrameTransformMatrix"))
						{
							if (name.ParseFromFStream(in))
							{
								if (!_stricmp(name.Text.c_str(), "{"))
								{
									sprintf(buffer, "frameTransfromMatrix%i", FrameTransformMatrices.size());
									name.Text = buffer;
								}
								else
								{
									in.ignore(256, '{');
								}

								FrameTransformMatrices.push_back(XFrameTransformMatrix());

								if (FrameTransformMatrices.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().Name = name;
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::FrameTransformMatrix;
									Nodes.back().Index = FrameTransformMatrices.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "Mesh"))
						{
							if (name.ParseFromFStream(in))
							{
								if (!_stricmp(name.Text.c_str(), "{"))
								{
									sprintf(buffer, "mesh%i", Meshes.size());
									name.Text = buffer;
								}
								else
								{
									in.ignore(256, '{');
								}

								Meshes.push_back(XMesh());
								if (Meshes.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().Name = name;
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::Mesh;
									Nodes.back().Index = Meshes.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "MeshNormals"))
						{
							if (name.ParseFromFStream(in))
							{
								MeshNormals.push_back(XMeshNormals());
								if (MeshNormals.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::MeshNormals;
									Nodes.back().Index = MeshNormals.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "MeshTextureCoords"))
						{
							if (name.ParseFromFStream(in))
							{
								MeshTextureCoords.push_back(XMeshTextureCoords());
								if (MeshTextureCoords.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::MeshTextureCoords;
									Nodes.back().Index = MeshTextureCoords.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "MeshMaterialList"))
						{
							if (name.ParseFromFStream(in))
							{
								MeshMaterialList.push_back(XMeshMaterialList());
								if (MeshMaterialList.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::MeshMaterialList;
									Nodes.back().Index = MeshMaterialList.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "Material"))
						{
							if (name.ParseFromFStream(in))
							{
								if (!_stricmp(name.Text.c_str(), "{"))
								{
									sprintf(buffer, "material%i", Materials.size());
									name.Text = buffer;
								}
								else
								{
									in.ignore(256, '{');
								}

								Materials.push_back(XMaterial());
								if (Materials.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().Name = name;
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::Material;
									Nodes.back().Index = Materials.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "TextureFileName"))
						{
							if (name.ParseFromFStream(in))
							{
								TextureFileNames.push_back(XTextureFileName());
								if (TextureFileNames.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::TextureFileName;
									Nodes.back().Index = TextureFileNames.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "XSkinMeshHeader"))
						{
							if (name.ParseFromFStream(in))
							{
								SkinMeshHeaders.push_back(XSkinMeshHeader());
								if (SkinMeshHeaders.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::SkinMeshHeader;
									Nodes.back().Index = SkinMeshHeaders.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "SkinWeights"))
						{
							if (name.ParseFromFStream(in))
							{
								SkinWeights.push_back(XSkinWeights());
								if (SkinWeights.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::SkinWeights;
									Nodes.back().Index = SkinWeights.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "AnimationSet"))
						{
							if (name.ParseFromFStream(in))
							{
								if (!_stricmp(name.Text.c_str(), "{"))
								{
									sprintf(buffer, "animation_set%i", AnimationSets.size());
									name.Text = buffer;
								}
								else
								{
									in.ignore(256, '{');
								}

								AnimationSets.push_back(XAnimationSet());

								Nodes.push_back(XNode());
								Nodes.back().Name = name;
								Nodes.back().ID = Nodes.size() - 1;
								Nodes.back().Type = XNode::AnimationSet;
								Nodes.back().Index = AnimationSets.size() - 1;
								if (!xNodeStack.empty())
								{
									Nodes.back().ParentID = xNodeStack.top().ID;
									Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
								}
								xNodeStack.push(Nodes.back());
							}
						}
						else if (!_stricmp(name.Text.c_str(), "Animation"))
						{
							if (name.ParseFromFStream(in))
							{
								Animations.push_back(XAnimation());

								Nodes.push_back(XNode());
								Nodes.back().ID = Nodes.size() - 1;
								Nodes.back().Type = XNode::Animation;
								Nodes.back().Index = Animations.size() - 1;
								if (!xNodeStack.empty())
								{
									Nodes.back().ParentID = xNodeStack.top().ID;
									Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
								}
								xNodeStack.push(Nodes.back());
							}
						}
						else if (!_stricmp(name.Text.c_str(), "AnimationKey"))
						{
							if (name.ParseFromFStream(in))
							{
								AnimationKeys.push_back(XAnimationKey());
								if (AnimationKeys.back().ParseFromFStream(in))
								{
									Nodes.push_back(XNode());
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Type = XNode::AnimationKey;
									Nodes.back().Index = AnimationKeys.size() - 1;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}
									xNodeStack.push(Nodes.back());
								}
							}
						}
						else if (!_stricmp(name.Text.c_str(), "}"))
						{
							if (!xNodeStack.empty())
							{
								xNodeStack.pop();
							}
						}
						else
						{
							std::vector<XNode>::iterator x_object;
							for (x_object = Nodes.begin(); x_object != Nodes.end(); ++x_object)
							{
								if (!_stricmp(name.Text.c_str(), x_object->Name.Text.c_str()))
								{
									Nodes.push_back(*x_object);
									Nodes.back().ID = Nodes.size() - 1;
									Nodes.back().Instance = true;
									if (!xNodeStack.empty())
									{
										Nodes.back().ParentID = xNodeStack.top().ID;
										Nodes.back().Indentation = xNodeStack.top().Indentation + 1;
									}

									break;
								}
							}
							if (x_object == Nodes.end())
							{
								if (!in.eof())
								{
									int ident = 0;
									while (name.ParseFromFStream(in))
									{
										if (!_stricmp(name.Text.c_str(), "{"))
										{
											++ident;
										}
										else if (!_stricmp(name.Text.c_str(), "}"))
										{
											--ident;
											if (ident == 0)
											{
												break;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						in.ignore();
					}
				}
				in.close();
			}

			return true;
		}
	}
}
