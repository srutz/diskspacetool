#ifndef GUARD_H
#define GUARD_H

#include <functional>

/*
 * A handler to run much like a finally block
 */
class Guard {
public:
    explicit Guard(std::function<void()> handler)
        : handler(handler), active(true) {
    }

    // no copy constructor and copy assignment
    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;

    // move semantics
    Guard(Guard&& other) noexcept : handler(std::move(other.handler)), active(other.active) {
        other.active = false;
    }

    ~Guard() {
        if (active) {
            handler();
        }
    }
    // Manually dismiss
    void dismiss() { active = false; }

private:
    std::function<void()> handler;
    bool active;
};
#endif // GUARD_H
