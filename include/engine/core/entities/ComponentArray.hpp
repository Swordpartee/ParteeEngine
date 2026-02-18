#pragma once

#include <vector>
#include <memory>

namespace parteeengine {

    class VirtualComponentArray {
    public:
        virtual ~VirtualComponentArray() = default;

         // Add a component to the array (entity at this row)
        virtual void addComponent(const void* component) = 0;

        // Remove component at index (swap-and-pop)
        virtual void removeComponent(size_t index) = 0;

        // Move a component from this array to another
        // Used when entity migrates to different archetype
        virtual void moveComponent(size_t fromIndex, VirtualComponentArray* toArray) = 0;

        // Get pointer to component at index (for typed access)
        virtual void* getComponent(size_t index) = 0;
    };

    template<typename T>
    class ComponentArray : public VirtualComponentArray {
    private:
        std::vector<T> data;

    public:
        void addComponent(const void* component) override;

        void removeComponent(size_t index) override;

        void moveComponent(size_t fromIndex, VirtualComponentArray* toArray) override;

        void* getComponent(size_t index) override;

        // Typed access (only use within ArchetypeManager)
        T& get(size_t index);

    };

    template<typename T>
    void ComponentArray<T>::addComponent(const void* component) {
        data.push_back(*static_cast<const T*>(component));
    }

    template<typename T>
    void ComponentArray<T>::removeComponent(size_t index) {
        // Swap-and-pop
        data[index] = std::move(data.back());
        data.pop_back();
    }

    template<typename T>
    void ComponentArray<T>::moveComponent(size_t fromIndex, VirtualComponentArray* toArray) {
        auto* otherArray = static_cast<ComponentArray<T>*>(toArray);
        otherArray->data.push_back(std::move(data[fromIndex]));
    }

    template<typename T>
    void* ComponentArray<T>::getComponent(size_t index) {
        return &data[index];
    }

    template<typename T>
    T& ComponentArray<T>::get(size_t index) {
        return data[index];
    }

} // namespace parteeengine