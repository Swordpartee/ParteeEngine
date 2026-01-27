#pragma once 

#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

namespace ParteeEngine {

    template<typename EventType>
    struct Subscription {
        EventType filter;
        std::function<void(const EventType&)> callback;
    };

    // EventBus: A simple event bus for inter-module communication.
    class EventBus {
    public:

        // Publish an event to the bus.
        template<typename EventType>
        static void publish(const EventType& event) {
            for (const auto& subscriber : getSubscribers<EventType>()) {
                if (!(subscriber.filter == event)) {
                    continue;
                }
                subscriber.callback(event);
            }
        };

        template<typename EventType>
        static void subscribe(EventType filter, std::function<void(const EventType&)> callback) {
            filter.subscribed();
            getSubscribers<EventType>().emplace_back(Subscription<EventType>{filter, callback});
        };

    private:
        template<typename EventType>
        static std::vector<Subscription<EventType>>& getSubscribers() {
            static std::vector<Subscription<EventType>> subscribers;
            return subscribers;
        }
    };

} // namespace ParteeEngine