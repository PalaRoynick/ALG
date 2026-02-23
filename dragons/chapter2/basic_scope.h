#pragma once

#include <string>
#include <cassert>
#include <unordered_map>

struct Symbol
{
    std::string type;
};

class Scope
{
private:
    Scope* prev_ = nullptr;
    std::unordered_map<std::string, Symbol> table_;

public:
    Scope(Scope* p = nullptr) : prev_(p) {}

    Symbol get(std::string id) const
    {
        const auto* e = this;
        for (; e; e = e->prev_)
        {
            auto it = e->table_.find(id);
            if (it != e->table_.end())
                return it->second;
        }
        return {"dummy_type"};
    }

    void set(const std::string& id, Symbol sym)
    {
        table_[id] = sym;
    }
};

