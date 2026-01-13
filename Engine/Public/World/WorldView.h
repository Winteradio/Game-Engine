#ifndef __WTR_WORLDVIEW_H__
#define __WTR_WORLDVIEW_H__

#include <Container/include/HashMap.h>
#include <ECS/include/UUID/UUID.h>

namespace wtr
{
	struct View
	{
		std::string name;

		ECS::UUID cameraID;
		ECS::UUID sceneID;

		uint16_t posX;
		uint16_t posY;
		uint16_t width;
		uint16_t height;

		bool active;

		View();
		View(const std::string& name, const ECS::UUID& cameraID, const ECS::UUID& sceneID);
	};

	class ViewContainer
	{
		public :
			using Storage = wtr::HashMap<std::string, View>;

			ViewContainer();
			~ViewContainer();

		public :
			View& Create(const std::string& name, const ECS::UUID& cameraID, const ECS::UUID& sceneID);
			void Remove(const std::string& name);

			View& GetView(const std::string& name);
			const View& GetView(const std::string& name) const;

			Storage& GetView();
			const Storage& GetView() const;

			bool HasView(const std::string& name) const;
			
		private :
			Storage m_storage;
	};
};

#endif // __WTR_WORLDVIEW_H__