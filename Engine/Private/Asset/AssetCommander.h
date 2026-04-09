#ifndef __WTR_ASSETCOMMANDER_H__
#define __WTR_ASSETCOMMANDER_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class Asset;
	class MeshAsset;
	class TextureAsset;
	class ShaderAsset;
	class RHICommandList;
};

namespace wtr
{
	class AssetCommander
	{
	public :
		static void Load(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);
		static void Update(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);
		static void Unload(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);

	private :
		static void onLoad(Memory::RefPtr<MeshAsset> meshAsset, Memory::RefPtr<RHICommandList> cmdList);
		static void onLoad(Memory::RefPtr<TextureAsset> textureAsset, Memory::RefPtr<RHICommandList> cmdList);
		static void onLoad(Memory::RefPtr<ShaderAsset> shaderAsset, Memory::RefPtr<RHICommandList> cmdList);
		
		static void onUpdate(Memory::RefPtr<MeshAsset> meshAsset, Memory::RefPtr<RHICommandList> cmdList);
		static void onUpdate(Memory::RefPtr<TextureAsset> textureAsset, Memory::RefPtr<RHICommandList> cmdList);

		static void onUnload(Memory::RefPtr<MeshAsset> meshAsset, Memory::RefPtr<RHICommandList> cmdList);
		static void onUnload(Memory::RefPtr<TextureAsset> textureAsset, Memory::RefPtr<RHICommandList> cmdList);
		static void onUnload(Memory::RefPtr<ShaderAsset> shaderAsset, Memory::RefPtr<RHICommandList> cmdList);
	};
};
#endif // __WTR_ASSETCOMMANDER_H__