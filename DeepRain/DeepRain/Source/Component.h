#pragma once

#define INVALID_HANDLE_IDX (unsigned short)~0

// Handles are a tool to reference objects. They are like pointers, but better, as they
// remain valid even if the object is moved in memory. This is key, since we will move
// objects when creating or destroying objects in order to keep the alive ones well
// packed in memory. We will also be sorting them.

// The way handles work is this:
//
//           [External logic]                     [Drawer]
//
//                handle:                       component<T>:
//               --------               ----------------------------
//              | idx: 1 |             |   handles:     objects:    |
//              | gen: 3 |             |   -------    -----------   |
//               --------              | 0| idx:2 | 0|           |  |
//                                     |  | gen:4 |  |   T obj   |  |
//   Our handle refers to the          |  |-------|  |           |  |
//   second handle in the internal     | 1| idx:0 |  |-----------|  |
//   array of handles, which           |  | gen:3 | 1|           |  |
//   refers to the first object.       |  |-------|  |   T obj   |  |
//   If we move the objects we         | 2| idx:1 |  |           |  |
//   update the internal handles       |  | gen:5 |  |-----------|  |
//   and the external ones remain      |  |-------| 2|           |  |
//   valid.                            | 3| idx:1 |  |   T obj   |  |
//   If we destroy the first           |  | gen:5 |  |           |  |
//   object and both the object        |  |-------|  |-----------|  |
//   and the internal handle are       |  |   .   |  |     .     |  |
//   reused by a new object, the       |  |   .   |  |     .     |  |
//   new internal handle will have     |                            |
//   its gen increased and thus         ----------------------------
//   the external handle will be
//   invalid because the gens will
//   not match.

struct Handle
{
    unsigned short idx = INVALID_HANDLE_IDX;
    unsigned short generation = 0;

    Handle(unsigned short aIdx, unsigned short aGeneration)
        : idx(aIdx)
        , generation(aGeneration){};

    Handle(){};

    inline bool operator==(const Handle& h) { return h.idx == idx && h.generation == generation; };
    inline bool operator!=(const Handle& h) { return h.idx != idx || h.generation != generation; };
};

template <typename T> struct Component
{
    static Handle handles[T::maxSize];
    static T objects[T::maxSize];
    static unsigned int aliveSize;
    static unsigned int firstDeadHandle;
};

// Static fields definition:

template <typename T> Handle Component<T>::handles[T::maxSize];

template <typename T> T Component<T>::objects[T::maxSize];

template <typename T> unsigned int Component<T>::aliveSize = 0;

template <typename T> unsigned int Component<T>::firstDeadHandle = 0;

// Component macros:

// GET

#define GET_SINGLE_COMP(COMP_TYPE, COMP_VAR) COMP_TYPE& COMP_VAR = Drawer::getSingleComponent<COMP_TYPE>();

#define GET_COMP(COMP_TYPE, COMP_VAR, HANDLE) COMP_TYPE& COMP_VAR = Drawer::getComponent<COMP_TYPE>(HANDLE);

#define GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, DO)                                                                                              \
                                                                                                                                                     \
    if (!drawer.isHandleValid<COMP_TYPE>(HANDLE))                                                                                                    \
    {                                                                                                                                                \
        DO                                                                                                                                           \
    }                                                                                                                                                \
    COMP_TYPE& COMP_VAR = Drawer::getComponent<COMP_TYPE>(HANDLE);

#define GET_COMP_OR_BREAK(COMP_TYPE, COMP_VAR, HANDLE) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, break;)
#define GET_COMP_OR_RETURN(COMP_TYPE, COMP_VAR, HANDLE) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, return;)
#define GET_COMP_OR_CONTINUE(COMP_TYPE, COMP_VAR, HANDLE) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, continue;)
#define GET_JOB_COMP_OR_RETURN(COMP_TYPE, COMP_VAR) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, handle, return;)

#define GET_COMP_OR_ASSERT_BREAK(COMP_TYPE, COMP_VAR, HANDLE) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); break;)
#define GET_COMP_OR_ASSERT_RETURN(COMP_TYPE, COMP_VAR, HANDLE) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); return;)
#define GET_COMP_OR_ASSERT_CONTINUE(COMP_TYPE, COMP_VAR, HANDLE) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); continue;)
#define GET_JOB_COMP_OR_ASSERT_RETURN(COMP_TYPE, COMP_VAR) GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, handle, assert(false); return;)

// ADD

#define ADD_COMP(COMP_TYPE, HANDLE) Drawer::addComponent<COMP_TYPE>(HANDLE)

#define ADD_COMP_OR_DO(COMP_TYPE, HANDLE, DO)                                                                                                        \
                                                                                                                                                     \
    if (!ADD_COMP(COMP_TYPE, HANDLE))                                                                                                                \
    {                                                                                                                                                \
        DO                                                                                                                                           \
    }

#define ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, DO)                                                                                      \
                                                                                                                                                     \
    ADD_COMP_OR_DO(COMP_TYPE, HANDLE, DO)                                                                                                            \
    COMP_TYPE& COMP_VAR = Drawer::getComponent<COMP_TYPE>(HANDLE);

#define ADD_AND_GET_COMP_OR_BREAK(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, break;)
#define ADD_AND_GET_COMP_OR_RETURN(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, return;)
#define ADD_AND_GET_COMP_OR_CONTINUE(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, continue;)

#define ADD_AND_GET_COMP_OR_ASSERT_BREAK(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); break;)
#define ADD_AND_GET_COMP_OR_ASSERT_RETURN(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); return;)
#define ADD_AND_GET_COMP_OR_ASSERT_CONTINUE(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_GET_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); continue;)

#define ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, DO)                                                                                \
                                                                                                                                                     \
    ADD_COMP_OR_DO(COMP_TYPE, HANDLE, DO)                                                                                                            \
    COMP_VAR = Drawer::getComponent<COMP_TYPE>(HANDLE);

#define ADD_AND_OVERWRITE_COMP_OR_BREAK(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, break;)
#define ADD_AND_OVERWRITE_COMP_OR_RETURN(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, return;)
#define ADD_AND_OVERWRITE_COMP_OR_CONTINUE(COMP_TYPE, COMP_VAR, HANDLE) ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, continue;)

#define ADD_AND_OVERWRITE_COMP_OR_ASSERT_BREAK(COMP_TYPE, COMP_VAR, HANDLE)                                                                          \
    ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); break;)
#define ADD_AND_OVERWRITE_COMP_OR_ASSERT_RETURN(COMP_TYPE, COMP_VAR, HANDLE)                                                                         \
    ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); return;)
#define ADD_AND_OVERWRITE_COMP_OR_ASSERT_CONTINUE(COMP_TYPE, COMP_VAR, HANDLE)                                                                       \
    ADD_AND_OVERWRITE_COMP_OR_DO(COMP_TYPE, COMP_VAR, HANDLE, assert(false); continue;)

// REMOVE

#define REMOVE_COMP(COMP_TYPE, HANDLE) Drawer::removeComponent<COMP_TYPE>(HANDLE);

// OUTPUT

#define ADD_OUTPUT(JOB) output[outputSize++] = JOB;
