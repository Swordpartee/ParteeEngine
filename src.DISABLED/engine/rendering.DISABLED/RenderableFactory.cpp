// #include "engine/rendering//RenderableFactory.hpp"
// #include "engine/entities/Entity.hpp"
// #include "engine/entities/components/RenderComponent.hpp"
// #include "engine/Engine.hpp"

// namespace ParteeEngine {

//     Entity& RenderableFactory::createSquare(Engine& engine) {
//         Entity& entity = create(engine);

//         RenderData data;
//         data.sprite.textureID = 1; // Placeholder texture ID

//         entity.getComponent<RenderComponent>()->setRenderData(data);

//         return entity;
//     }

//     Entity& RenderableFactory::createCube(Engine& engine) {
//         Entity& entity = create(engine);

//         RenderData data;
//         data.mesh;

//         entity.getComponent<RenderComponent>()->setRenderData(data);

//         return entity;
//     }

//     Entity& RenderableFactory::create(Engine& engine) {
//         Entity& entity = engine.createEntity();
//         entity.addComponent<RenderComponent>();
//         return entity;
//     }

// } // namespace ParteeEngine