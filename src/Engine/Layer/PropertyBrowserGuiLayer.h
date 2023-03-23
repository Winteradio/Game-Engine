#ifndef __PROPERTYBROWSERGUILAYER_H__
#define __PROPERTYBROWSERGUILAYER_H__

#include "IBrowserGuiLayer.h"

#include "ResourceManager.h"

class PropertyBrowserGuiLayer : public IBrowserGuiLayer
{
	public :
		PropertyBrowserGuiLayer();
		PropertyBrowserGuiLayer( const PropertyBrowserGuiLayer& Other );
		virtual ~PropertyBrowserGuiLayer();

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height );
		virtual void Destroy();
		virtual void Update();
		virtual void Render();
		virtual void SetPath( FS::path Path );
		void RenderCommon();
		void RenderType();
		void RenderScene();
		void RenderComponent();
		void RenderCamera();
		void RenderLight();
		void RenderMesh();
		void RenderScript();
		void RenderShader();
		void RenderTexture();

		void RenderXMFLOAT2( XMFLOAT2& Info );
		void RenderXMFLOAT3( XMFLOAT3& Info );
		void RenderXMFLOAT4( XMFLOAT4& Info );
		void RenderColor( XMFLOAT4& Info );

		void SetResource( IResource* Resource );
		void Clear();

		template< class MainT, class T >
		void RenderPart( MainT* MapData )
		{
			std::string Typename( typeid(T).name() );
			Typename.erase( Typename.begin(), Typename.begin()+6 );

			if ( ImGui::TreeNode( Typename.c_str() ) )
			{
				if ( ImGui::TreeNode("Selected") )
				{
					if ( !MapData->GetIDs<T>().empty() )
					{
						for ( auto ITRID : MapData->GetIDs<T>() )
						{
							IResource* Resource = ResourceManager::GetResource<T>( ITRID );
							ImGui::PushID( Resource->GetUUID().c_str() );
							if ( ImGui::TreeNode( Resource->GetName().c_str() ) )
							{
								if ( ImGui::Selectable("Go") )
								{
									m_Resource = Resource;
								}
								ImGui::Spacing();
								if ( ImGui::Selectable("Get Out") )
								{
									((MainT*)m_Resource)->GetoutResource<T>( Resource->GetUUID() );
								}
								ImGui::TreePop();
							}
							ImGui::PopID();
						}
					}
					ImGui::TreePop();
				}

				ImGui::Spacing();

				if ( ImGui::TreeNode("Non-Selected") )
				{
					if ( !ResourceManager::GetMap<T>().empty() )
					{
						for ( auto itr : ResourceManager::GetMap<T>() )
						{
							IResource* Resource = itr.second;
							ImGui::PushID( Resource->GetUUID().c_str() );
							if ( ImGui::Selectable( Resource->GetName().c_str() ) )
							{
								MapData->InputResource<T>( Resource->GetUUID() );
							}
							ImGui::PopID();
						}
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}

	private :
		IResource* m_Resource;
};

#endif // __PROPERTYBROWSERGUILAYER_H__