#pragma once 

namespace ParteeEngine {

    // EventBus: A simple event bus for inter-module communication.
    class EventBus {
    public:
        EventBus() = default;
        ~EventBus() = default;

        // Publish an event to the bus.
        template<typename Event>
        void publish(Args&&... args);

        // Subscribe to an event type with a callback.
        template<typename EventType>
        void subscribe(std::function<void(const Event&)> callback);

        static EventBus const& getInstance();

    private:
        std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> subscribers;
    };

    template<typename Event>
    void EventBus::publish(Args&&... args) {
        static_assert(std::is_base_of_v<Event, Event>, "Event must inherit from Event");
        Event event(std::forward<Args>(args)...);
        auto it = subscribers.find(typeid(Event));
        if (it != subscribers.end()) {
            for (const auto& callback : it->second) {
                callback(&event);
            }
        }
    }

    template<typename EventType>
    void EventBus::subscribe(std::function<void(const EventType&)> callback) {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must inherit from Event");
        subscribers[typeid(EventType)].push_back([callback](const void* event) {
            callback(*static_cast<const EventType*>(event));
        });
    }

    static EventBus const& EventBus::getInstance() {
        static EventBus instance;
        return instance;
    }

} // namespace ParteeEngine