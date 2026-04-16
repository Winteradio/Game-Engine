#ifndef __WTR_SYSTEM_H__
#define __WTR_SYSTEM_H__

#include <ECS/include/Object/System.h>
#include <ECS/include/Utils.h>
#include <World/Node.h>

namespace wtr
{
	class InputStorage;
};

namespace wtr
{
	class BaseSystem : public ECS::System<BaseNode>
	{
		GENERATE(BaseSystem);

	public :
		BaseSystem() = default;
		virtual ~BaseSystem() = default;
	};

	template<typename T>
	class TypedSystem : public BaseSystem
	{
		static_assert(ECS::Utils::IsNode<T> && "Invalid the node type");

		GENERATE(TypedSystem);

	public:
		using NodeType = T;
		using ContainerType = ECS::Container<NodeType, BaseNode>;

		TypedSystem()
			: BaseSystem()
			, m_nodeType(Reflection::TypeInfo::Get<NodeType>())
		{}

		virtual ~TypedSystem() = default;

	public:
		void Update(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ECS::BaseContainer<BaseNode>> container) override
		{
			Memory::ObjectPtr<ContainerType> nodeContainer = Memory::Cast<ContainerType>(container);
			
			UpdateInternal(timeStep, nodeContainer);
		}

		const Reflection::TypeInfo* GetNodeType() const override
		{
			return m_nodeType;
		}

	protected:
		virtual void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) = 0;

	private :
		const Reflection::TypeInfo* m_nodeType;
	};

	template<typename T>
	class InputSystem : public TypedSystem<T>
	{
		static_assert(ECS::Utils::IsNode<T> && "Invalid the node type");

		GENERATE(InputSystem);

	public:
		InputSystem()
			: TypedSystem<T>()
			, m_inputStorage(nullptr)
		{}

		virtual ~InputSystem() = default;

	public:
		void SetInputStorage(const Memory::RefPtr<InputStorage> inputStorage)
		{
			m_inputStorage = inputStorage;
		}

		Memory::RefPtr<const InputStorage> GetInputStorage() const
		{
			return m_inputStorage;
		}

	private:
		Memory::RefPtr<InputStorage> m_inputStorage;
	};
};

#endif // __WTR_SYSTEM_H__