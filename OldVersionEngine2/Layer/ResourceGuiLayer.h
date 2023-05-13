#ifndef __RESOURCEGUILAYER_H__
#define __RESOURCEGUILAYER_H__

#include "IGuiLayer.h"
#include "ResourceManager.h"
#include "PropertyBrowserGuiLayer.h"

class ResourceGuiLayer : public IGuiLayer
{
	public :
		ResourceGuiLayer();
		ResourceGuiLayer( const ResourceGuiLayer& Other );
		virtual ~ResourceGuiLayer();

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height );
		virtual void Destroy();
		virtual void Update();
		virtual void Render();
		void RenderTypeCategory();
		void RenderAddCategory();
		void SetPropertyBrowserGuiLayer( PropertyBrowserGuiLayer* PropertyBrowserGui );

		template< class T >
		void RenderTree()
		{
			std::string Typename( typeid(T).name() );
			Typename.erase( Typename.begin(), Typename.begin()+6 );

			if ( ImGui::TreeNode( Typename.c_str() ) )
			{
				ImGui::Spacing();
				if ( !ResourceManager::GetMap<T>().empty() )
				{
					for ( auto itr : ResourceManager::GetMap<T>() )
					{
						IResource* Resource = ( itr.second );
						ImGui::PushID( Resource->GetUUID().c_str() );
						ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
						ImGui::Text( Resource->GetUUID().substr( 0, 6 ).c_str() );
						ImGui::PopStyleColor();
						ImGui::SameLine();
						if ( ImGui::Selectable( Resource->GetName().c_str() ) )
						{
							m_PropertyBrowserGuiLayer->SetResource( Resource );
						}
						ImGui::PopID();
					}
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

	private :
		PropertyBrowserGuiLayer* m_PropertyBrowserGuiLayer;
		char m_NewResource[MAX_LENGTH];
		int m_SelectedType;
};

#endif // __RESOURCEGUILAYER_H__