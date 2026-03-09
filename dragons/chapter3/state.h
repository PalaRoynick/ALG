#pragma once

class State
{
public:
    State() = default;
    State(std::size_t id) : id_(id) {}

    std::size_t id() const { return id_; }
private:
    std::size_t id_ = 0;
};
