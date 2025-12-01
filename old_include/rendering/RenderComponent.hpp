#pragma once

#include "Component.hpp"
#include <iostream>
#include <typeindex>

namespace ParteeEngine {
    class Entity; // Forward declaration
    class TransformComponent; // Forward declaration
    class Renderer; // Forward declaration
    
    class RenderComponent : public Component {
        public:
            void requireDependencies(Entity &owner) override;

            Mesh getRenderMesh() const {
                return 0;
            }

            // Rendering properties
            bool visible = true;
            enum RenderType { SQUARE, CUBE } type = SQUARE;        
    };
}