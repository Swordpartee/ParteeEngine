#pragma once

namespace parteeengine {

    class EntityManager;

    // Data passed to modules each frame.
    struct ModuleInput {
        const EntityManager& entityManager;
        float dt = 0; // Delta time since last frame
    };

    // Base class for all engine modules. Modules are the primary extension point
    // for adding systems (rendering, physics, audio, etc.) to the engine.
    class Module {
    public:
        virtual ~Module() = default;

        // Called once before the main loop. Return false to abort engine startup.
        virtual bool initialize(const ModuleInput& input) = 0;
        // Called every frame. Return false to signal the engine to stop.
        virtual bool update(const ModuleInput& input) = 0;
    };

} // namespace parteeengine