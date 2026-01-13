#ifndef __WTR_WORLDSCENE_H__
#define __WTR_WORLDSCENE_H__

#include <Container/include/HashMap.h>
#include <ECS/include/Object/Scene.h>

namespace wtr
{
	class SceneContainer
	{
		public :
			using Storage = wtr::HashMap<std::string, ECS::Scene>;

			SceneContainer();
			~SceneContainer();

		public :
			ECS::Scene& Create(const std::string& name);
			void Remove(const std::string& name);

			ECS::Scene& GetScene(const std::string& name);
			const ECS::Scene& GetScene(const std::string& name) const;

			Storage& GetScene();
			const Storage& GetScene() const;

			bool HasScene(const std::string& name) const;
			
		private :
			Storage m_storage;
	};
};

#endif // __WTR_WORLDSCENE_H__