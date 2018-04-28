#pragma once

#include "Component.h"
#include <assert.h>
#include <stdlib.h>

class Drawer
{
  public:
    template <typename TComp> static bool addComponent(Handle& handle)
    {
        // No dead objects available
        if (Component<TComp>::aliveSize >= TComp::maxSize)
        {
            // Failed
            return false;
        }

        // Make the first dead handle point to the first dead component
        Component<TComp>::handles[Component<TComp>::firstDeadHandle].idx = Component<TComp>::aliveSize;

        // Make the external handle point to the internal one
        handle.idx = Component<TComp>::firstDeadHandle;
        handle.generation = Component<TComp>::handles[Component<TComp>::firstDeadHandle].generation;

        // Increment amount of alive components
        Component<TComp>::aliveSize++;

        // Update firstDeadHandle to the next one
        while (Component<TComp>::handles[++Component<TComp>::firstDeadHandle].idx != INVALID_HANDLE_IDX &&
               Component<TComp>::firstDeadHandle < TComp::maxSize)
            ;

        // Succeeded
        return true;
    };

    template <typename TComp> static void fillComponents()
    {
        Handle h;
        while (addComponent<T>(h))
            ;
    }

    template <typename TComp> static void removeComponent(const Handle& handle)
    {
        // Impossible idx for a handle of this type T
        assert(handle.idx != INVALID_HANDLE_IDX);
        assert(handle.idx < TComp::maxSize);

        // Internal handle's idx is wrong
        assert(Component<TComp>::handles[handle.idx].idx != INVALID_HANDLE_IDX);
        assert(Component<TComp>::handles[handle.idx].idx < Component<TComp>::aliveSize);

        // Invalid or expired handle
        if (handle.idx == INVALID_HANDLE_IDX || (handle.generation & GEN_MASK) != (Component<TComp>::handles[handle.idx].generation))
        {
            return;
        }

        // Dead internal handles should not match generations with its external handles
        assert(Component<TComp>::handles[handle.idx].idx != INVALID_HANDLE_IDX);

        // Invalidate handles to the object being killed
        Component<TComp>::handles[handle.idx].generation++;

        // Update firstDeadHandle if necessary
        Component<TComp>::firstDeadHandle = handle.idx < Component<TComp>::firstDeadHandle ? handle.idx : Component<TComp>::firstDeadHandle;

        // Assign the last component to the one being killed and decrement aliveSize
        Component<TComp>::objects[Component<TComp>::handles[handle.idx].idx] = Component<TComp>::objects[--Component<TComp>::aliveSize];

        // Update the internal handle of the moved component
        int handleIdx = 0;
        for (; Component<TComp>::handles[handleIdx].idx != Component<TComp>::aliveSize; ++handleIdx)
            ;

        Component<TComp>::handles[handleIdx].idx = Component<TComp>::handles[handle.idx].idx;

        // Kill the handle
        Component<TComp>::handles[handle.idx].idx = INVALID_HANDLE_IDX;
    };

    template <typename TComp> inline static TComp& getComponent(const Handle& handle)
    {
        assert(handle.idx != INVALID_HANDLE_IDX);                                        // Invalid handle, check isValidHandle() before using it
        assert(handle.idx < TComp::maxSize);                                             // Wrong handle index
        assert(Component<TComp>::handles[handle.idx].idx < Component<TComp>::aliveSize); // Wrong internal handle index
        assert(Component<TComp>::handles[handle.idx].idx !=
               INVALID_HANDLE_IDX); // Expired handle (points to a dead component), check isValidHandle() before using it
        assert(handle.generation == (Component<TComp>::handles[handle.idx]
                                         .generation)); // Expired handle (points to a reused component), check isValidHandle() before using it

        return Component<TComp>::objects[Component<TComp>::handles[handle.idx].idx];
    };

    template <typename TComp> inline static Handle getRandomHandle()
    {
        int idx = rand() % Component<TComp>::aliveSize;

        while (Component<TComp>::handles[idx].idx == INVALID_HANDLE_IDX)
        {
            ++idx;
            idx %= Component<TComp>::aliveSize;
        }

        return Component<TComp>::handles[idx];
    };

    template <typename TComp> inline static bool isHandleValid(const Handle& handle)
    {
        assert(handle.idx != INVALID_HANDLE_IDX);                                        // Invalid handle
        assert(handle.idx < TComp::maxSize);                                             // Wrong handle index
        assert(Component<TComp>::handles[handle.idx].idx < Component<TComp>::aliveSize); // Wrong internal handle index

        return handle.idx != INVALID_HANDLE_IDX                                   // Invalid handle
               && Component<TComp>::handles[handle.idx].idx != INVALID_HANDLE_IDX // Expired handle (points to a dead component)
               &&
               (handle.generation & GEN_MASK) == (Component<TComp>::handles[handle.idx].generation); // Expired handle (points to a reused component)
    };

    template <typename TComp> inline static bool isHandleValidNA(const Handle& handle)
    {
        return handle.idx != INVALID_HANDLE_IDX                                   // Invalid handle
               && Component<TComp>::handles[handle.idx].idx != INVALID_HANDLE_IDX // Expired handle (points to a dead component)
               &&
               (handle.generation & GEN_MASK) == (Component<TComp>::handles[handle.idx].generation); // Expired handle (points to a reused component)
    };

    template <typename TComp> inline static TComp& getSingleComponent()
    {
        assert(TComp::maxSize == 1); // Component is not single

        return Component<TComp>::objects[0];
    };

    template <typename TComp> inline static unsigned int getNumAvailableComponents() { return TComp::maxSize - Component<TComp>::aliveSize; };
};
