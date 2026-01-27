#pragma once

namespace ParteeEngine
{   

    struct Event {
        virtual ~Event() = default;

        virtual void subscribed() {}  // Called when event is subscribed to

        // virtual bool operator==(const Event& other) const = 0;
    
    protected:
        Event() = default;
    };

} // namespace ParteeEngine
