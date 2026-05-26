#include <Asset/OBJParser.h>

#include <Log/include/Log.h>
#include <Asset/AssetTypes.h>
#include <Asset/AssetStream.h>
#include <Asset/AssetSystem.h>
#include <Asset/AssetUtils.h>
#include <Memory/include/Core.h>

#include <type_traits>

namespace wtr
{
	constexpr size_t MAX_FACE_VERTICES = 3;
	constexpr int ERROR_INDEX = 0;

	bool OBJParser::Parse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[OBJ] Failed to parse the obj file, the asset is invalid";
			return false;
		}

		Memory::RefPtr<MeshAsset> mesh = Memory::Cast<MeshAsset>(asset);
		if (!mesh)
		{
			LOGINFO() << "[OBJ] Failed to parse the obj file, the asset is not the mesh asset";
			return false;
		}

		wtr::DynamicArray<uint8_t> fileBuffer = ReadBuffer(asset);
		if (fileBuffer.Empty())
		{
			LOGINFO() << "[OBJ] Failed to read the obj file : " << asset->path;

			return false;
		}

		OBJMesh objTotalMesh;
		OBJGroups objGroups;
		OBJMaterials objMaterials;

		const uint8_t* curr = fileBuffer.Data();
		const uint8_t* end = curr + fileBuffer.Size();

		while (curr != end)
		{
			const uint8_t* lineEnd = curr;
			while (lineEnd != end && *lineEnd != '\n' && *lineEnd != '\r')
			{
				lineEnd++;
			}

			std::string_view line(reinterpret_cast<const char*>(curr), static_cast<size_t>(lineEnd - curr));

			if (curr < end && (*curr == '\r' || *curr == '\n'))
			{
				curr++; 
				continue;
			}

			AssetStream lineStream(line);
			
			std::string_view tag;
			lineStream >> tag;

			if (tag == "mtllib")
			{
				std::string mtlPath = AssetUtils::GetPath(asset->path) + "/";
				lineStream >> mtlPath;

				objMaterials.Insert(mtlPath);
			}
			else if (tag == "v")
			{
				fvec3 pos;
				lineStream >> pos.x >> pos.y >> pos.z;

				objTotalMesh.pos.PushBack(pos);
			}
			else if (tag == "vt")
			{
				fvec2 uv;
				lineStream >> uv.x >> uv.y;

				objTotalMesh.uv.PushBack(uv);
			}
			else if (tag == "vn")
			{
				fvec3 nor;
				lineStream >> nor.x >> nor.y >> nor.z;

				objTotalMesh.nor.PushBack(nor);
			}
			else if (tag == "vp")
			{
				fvec3 free;
				lineStream >> free.x >> free.y >> free.z;

				objTotalMesh.free.PushBack(free);
			}
			else if (tag == "g")
			{
				OBJGroup group;
				lineStream >> group.name;

				objGroups.EmplaceBack(std::move(group));
			}
			else if (tag == "usemtl")
			{
				if (objGroups.Empty())
				{
					curr = lineEnd;
					continue;
				}

				auto& group = objGroups.Back();

				lineStream >> group.material;
			}
			else if (tag == "f")
			{
				if (objGroups.Empty())
				{
					curr = lineEnd;
					continue;
				}

				auto& group = objGroups.Back();

				OBJFace face;
				std::string_view faceView;
				do
				{
					lineStream >> faceView;
					AssetStream faceStream(faceView);

					OBJVertex vertex { ERROR_INDEX, ERROR_INDEX, ERROR_INDEX, ERROR_INDEX };

					faceStream >> vertex.pos;
					faceStream.Skip('/');
					faceStream >> vertex.uv;
					faceStream.Skip('/');
					faceStream >> vertex.nor;
					faceStream.Skip('/');
					faceStream >> vertex.free;

					face.vertices.PushBack(vertex);
				}
				while (!lineStream.Empty());

				if (face.vertices.Size() > 3)
				{
					LOGINFO() << "[OBJ] The face has more than 3 vertices, which is not supported : " << std::string(line);
					return false;
				}

				group.faces.EmplaceBack(std::move(face));
			}
			else
			{
				if (tag != "#" && !tag.empty())
				{
					LOGINFO() << "[OBJ] Unsupported tag in the obj file : " << std::string(tag);
				}
			}

			curr = lineEnd;
		}

		if (ParseInternal(mesh, objTotalMesh, objGroups, objMaterials))
		{
			LOGINFO() << "[OBJ] Succeed to parsed the obj file : " << mesh->path;

			return true;
		}
		else
		{
			LOGERROR() << "[OBJ] Failed to parser the obj file : " << mesh->path;

			return false;
		}
	}

	bool OBJParser::ParseInternal(Memory::RefPtr<MeshAsset> mesh, const OBJMesh& totalMesh, const OBJGroups& groups, const OBJMaterials& materials)
	{
		if (!mesh)
		{
			LOGERROR() << "[OBJ] Failed to parse the obj file, the mesh asset is invalid";
			return false;
		}

		if (groups.Empty())
		{
			LOGERROR() << "[OBJ] Failed to parse the obj file, the group is empty";
			return false;
		}

		if (totalMesh.pos.Empty() && totalMesh.uv.Empty() && totalMesh.nor.Empty() && totalMesh.free.Empty())
		{
			LOGERROR() << "[OBJ] Failed to the parse the obj file, the total mesh's buffer is empty";
			return false;
		}

		for (const auto& fileName : materials)
		{
			const std::string path = AssetUtils::GetPath(mesh->path) + "/" + fileName;
			Memory::RefPtr<const Asset> asset = AssetSystem::Load(path);
			if (!asset)
			{
				LOGINFO() << "[OBJ] Failed to load the material file : " << path;
				continue;
			}

			Memory::RefPtr<const MaterialAsset> material = Memory::Cast<const MaterialAsset>(asset);
			if (material)
			{
				mesh->materials[material->name] = material;
			}
		}

		wtr::HashMap<OBJVertex, uint32_t, OBJVertexHash> vertexMap;
		vertexMap.Reserve(totalMesh.pos.Size());

		OBJMesh finalMesh;
		finalMesh.pos.Reserve(totalMesh.pos.Size());
		finalMesh.uv.Reserve(totalMesh.uv.Size());
		finalMesh.nor.Reserve(totalMesh.nor.Size());
		finalMesh.free.Reserve(totalMesh.free.Size());
		finalMesh.groups.Resize(groups.Size());

		for (size_t index = 0; index < groups.Size(); index++)
		{
			const auto& group = groups[index];

			vertexMap.Clear();
			finalMesh.index.Reserve(finalMesh.index.Size() + group.faces.Size() * MAX_FACE_VERTICES);

			auto& section = finalMesh.groups[index];
			section.indexOffset = static_cast<uint32_t>(finalMesh.index.Size() * sizeof(decltype(finalMesh.index)::ValueType));
			section.name = group.name;
			section.materialName = group.material;

			uint32_t vertexCount = 0;
			for (const auto& face : group.faces)
			{
				for (const auto& vertex : face.vertices)
				{
					auto itr = vertexMap.Find(vertex);
					if (itr == vertexMap.End())
					{
						vertexMap.Insert(std::make_pair(vertex, vertexCount));

						if (vertex.pos != ERROR_INDEX)
						{
							const size_t index = (vertex.pos > 0) ? vertex.pos - 1 : totalMesh.pos.Size() + vertex.pos;

							finalMesh.pos.PushBack(totalMesh.pos[index]);

							section.minVertex.x = std::min(section.minVertex.x, finalMesh.pos.Back().x);
							section.minVertex.y = std::min(section.minVertex.y, finalMesh.pos.Back().y);
							section.minVertex.z = std::min(section.minVertex.z, finalMesh.pos.Back().z);

							section.maxVertex.x = std::max(section.maxVertex.x, finalMesh.pos.Back().x);
							section.maxVertex.y = std::max(section.maxVertex.y, finalMesh.pos.Back().y);
							section.maxVertex.z = std::max(section.maxVertex.z, finalMesh.pos.Back().z);
						}

						if (vertex.uv != ERROR_INDEX)
						{
							const size_t index = (vertex.uv > 0) ? vertex.uv - 1 : totalMesh.uv.Size() + vertex.uv;
							finalMesh.uv.PushBack(totalMesh.uv[index]);
						}

						if (vertex.nor != ERROR_INDEX)
						{
							const size_t index = (vertex.nor > 0) ? vertex.nor - 1 : totalMesh.nor.Size() + vertex.nor;
							finalMesh.nor.PushBack(totalMesh.nor[index]);
						}

						if (vertex.free != ERROR_INDEX)
						{
							const size_t index = (vertex.free > 0) ? vertex.free - 1 : totalMesh.free.Size() + vertex.free;
							finalMesh.free.PushBack(totalMesh.free[index]);
						}

						finalMesh.index.PushBack(vertexCount);
						vertexCount++;
					}
					else
					{
						finalMesh.index.PushBack(itr->second);
					}

					section.minVertexIndex = std::min(section.minVertexIndex, finalMesh.index.Back());
					section.maxVertexIndex = std::max(section.maxVertexIndex, finalMesh.index.Back());
					section.indexCount++;
				}
			}
		}

		return Convert(mesh, std::move(finalMesh));
	}

	bool OBJParser::Convert(Memory::RefPtr<MeshAsset> mesh, OBJMesh&& objMesh)
	{
		if (!mesh)
		{
			LOGERROR() << "[OBJ] Failed to convert the obj mesh, the mesh asset is invalid";
			return false;
		}

		auto& rawBuffers = mesh->rawBuffers;

		if (!objMesh.pos.Empty())
		{
			using ValueType = decltype(objMesh.pos)::ValueType;

			FormattedBuffer posBuffer;
			Memory::RefPtr<ArrayData<ValueType>> arrayData = Memory::MakeRef<ArrayData<ValueType>>();
			arrayData->data = std::move(objMesh.pos);

			posBuffer.bulkData = arrayData;
			posBuffer.componentType = eDataType::eFloat;
			posBuffer.numComponents = ValueType::length();
			posBuffer.count = static_cast<uint32_t>(arrayData->data.Size());

			VertexKey posKey{ eVertexSemantic::ePosition, 0 };
			rawBuffers[posKey] = posBuffer;
		}

		if (!objMesh.nor.Empty())
		{
			using ValueType = decltype(objMesh.nor)::ValueType;

			FormattedBuffer norBuffer;
			Memory::RefPtr<ArrayData<ValueType>> arrayData = Memory::MakeRef<ArrayData<ValueType>>();
			arrayData->data = std::move(objMesh.nor);

			norBuffer.bulkData = arrayData;
			norBuffer.componentType = eDataType::eFloat;
			norBuffer.numComponents = ValueType::length();
			norBuffer.count = static_cast<uint32_t>(arrayData->data.Size());

			VertexKey norKey{ eVertexSemantic::eNormal, 0 };
			rawBuffers[norKey] = norBuffer;
		}

		if (!objMesh.uv.Empty())
		{
			using ValueType = decltype(objMesh.uv)::ValueType;

			FormattedBuffer uvBuffer;
			Memory::RefPtr<ArrayData<ValueType>> arrayData = Memory::MakeRef<ArrayData<ValueType>>();
			arrayData->data = std::move(objMesh.uv);

			uvBuffer.bulkData = arrayData;
			uvBuffer.componentType = eDataType::eFloat;
			uvBuffer.numComponents = decltype(objMesh.uv)::ValueType::length();
			uvBuffer.count = static_cast<uint32_t>(arrayData->data.Size());

			VertexKey uvKey{ eVertexSemantic::eTexCoord, 0 };
			rawBuffers[uvKey] = uvBuffer;
		}

		if (!objMesh.free.Empty())
		{
			using ValueType = decltype(objMesh.free)::ValueType;

			FormattedBuffer freeBuffer;
			Memory::RefPtr<ArrayData<ValueType>> arrayData = Memory::MakeRef<ArrayData<ValueType>>();
			arrayData->data = std::move(objMesh.free);

			freeBuffer.bulkData = arrayData;
			freeBuffer.componentType = eDataType::eFloat;
			freeBuffer.numComponents = decltype(objMesh.free)::ValueType::length();
			freeBuffer.count = static_cast<uint32_t>(arrayData->data.Size());

			VertexKey freeKey{ eVertexSemantic::eGeneric, 0 };
			rawBuffers[freeKey] = freeBuffer;
		}

		if (!objMesh.index.Empty())
		{
			using ValueType = decltype(objMesh.index)::ValueType;

			FormattedBuffer indexBuffer;
			Memory::RefPtr<ArrayData<ValueType>> arrayData = Memory::MakeRef<ArrayData<ValueType>>();
			arrayData->data = std::move(objMesh.index);

			indexBuffer.bulkData = arrayData;
			indexBuffer.componentType = eDataType::eUInt;
			indexBuffer.numComponents = 1;
			indexBuffer.count = static_cast<uint32_t>(arrayData->data.Size());

			mesh->rawIndex = indexBuffer;
			mesh->drawMode = eDrawMode::eTriangles;
		}

		if (!objMesh.groups.Empty())
		{
			mesh->sections = std::move(objMesh.groups);
		}

		return true;
	}
}