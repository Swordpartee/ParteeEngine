#pragma once

namespace ParteeEngine
{   

    struct Event {
        virtual ~Event() = default;

        virtual void subscribed() {}  // Called when event is subscribed to
    
    protected:
        Event() = default;
    };

} // namespace ParteeEngine
