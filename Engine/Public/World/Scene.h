#ifndef __WTR_SCENE_H__
#define __WTR_SCENE_H__

#include <ECS/include/Object/Scene.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	struct ViewInfo
	{
		std::string name = "DefaultView";
		std::string pipeline = "DefaultPipeline";

		ECS::UUID cameraID = ECS::UUID();

		uint16_t posX = 0;
		uint16_t posY = 0;
		uint16_t width = 800;
		uint16_t height = 600;

		bool active = true;
	};

	class Scene : public ECS::Scene
	{
		public :
			Scene();
			Scene(const std::string& name);
			virtual ~Scene();

		public :
			void RegisterView(const ViewInfo& view);
			void RemoveView(const std::string name);

			const ViewInfo& GetView(const std::string& name) const;
			const wtr::HashMap<std::string, ViewInfo>& GetView() const;
		private :
			wtr::HashMap<std::string, ViewInfo> m_viewMap;
	};

	class SceneContainer
	{
		public :
			using Storage = wtr::HashMap<std::string, Scene>;

			SceneContainer();
			~SceneContainer();

		public :
			Scene& Create(const std::string& name);
			void Remove(const std::string& name);

			Scene& GetScene(const std::string& name);
			const Scene& GetScene(const std::string& name) const;

			Storage& GetScene();
			const Storage& GetScene() const;

			bool HasScene(const std::string& name) const;
			
		private :
			Storage m_storage;
	};
};

#endif // __WTR_SCENE_H__