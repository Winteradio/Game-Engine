#ifndef __WTR_OBJPARSER_H__
#define __WTR_OBJPARSER_H__

#include <Asset/AssetParser.h>
#include <Framework/Math/MathTypes.h>
#include <Container/include/HashSet.h>

namespace wtr
{
	class MeshAsset;
};

namespace wtr
{
	class OBJParser : public AssetParser
	{
	private:
		struct OBJMesh
		{
			wtr::DynamicArray<wtr::fvec3> pos;
			wtr::DynamicArray<wtr::fvec2> uv;
			wtr::DynamicArray<wtr::fvec3> nor;
			wtr::DynamicArray<wtr::fvec3> free;
		};

		struct OBJVertex
		{
			int pos;
			int uv;
			int nor;
			int free;

			bool operator==(const OBJVertex& other) const
			{
				return pos == other.pos && uv == other.uv && nor == other.nor && free == other.free;
			}
		};

		struct OBJVertexHash
		{
			size_t operator()(const OBJVertex& vertex) const
			{
				size_t seed = 0;
				auto combine = [](size_t& hash, int value)
				{
					hash ^= std::hash<int>{}(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				};
				combine(seed, vertex.pos);
				combine(seed, vertex.uv);
				combine(seed, vertex.nor);
				combine(seed, vertex.free);

				return seed;
			}
		};

		struct OBJFace
		{
			wtr::DynamicArray<OBJVertex> vertices;
		};

		struct OBJGroup
		{
			std::string name;
			std::string material;

			wtr::DynamicArray<OBJFace> faces;
		};

		using OBJGroups = wtr::DynamicArray<OBJGroup>;
		using OBJMaterials = wtr::HashSet<std::string>;

	public :
		OBJParser() = default;
		virtual ~OBJParser() = default;

	public :
		virtual void Parse(Memory::RefPtr<Asset> asset) override;

	private :
		virtual bool ParseInternal(Memory::RefPtr<MeshAsset> mesh, const OBJMesh& totalMesh, const OBJGroups& groups, const OBJMaterials& materials);
	};
};

#endif // __WTR_OBJPARSER_H__