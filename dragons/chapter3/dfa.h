#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <algorithm>

#include "state.h"
#include "symbol.h"

class DFA
{
public:
    DFA(std::size_t n_states, std::size_t n_symbols)
    {
        trans_table_.resize(n_states, std::vector<std::size_t>(n_symbols));
    }

    void set_initial_state(std::size_t s) { initial_state_ = s; current_state_ = s; }

    void set_accepting_states(const std::vector<std::size_t>& ss)
    {
        accepting_states_ = ss;
    }

    void set_accepting_states(std::vector<std::size_t>&& ss)
    {
        accepting_states_ = std::move(ss);
    }

    // trans_table[state_id][symbol_id] -> state
    void add_transition(std::size_t from, std::size_t to, Symbol s)
    {
        // trans_table_.resize(from + 1, std::vector<std::size_t>(s.id() + 1));
        trans_table_[from][s.id()] = to;
    }

    std::size_t current_state() const
    {
        return current_state_;
    }

    virtual void step(const Symbol& s)
    {
        current_state_ = trans_table_[current_state_][s.id()];
    }

    virtual bool is_accepted(std::string_view input)
    {
        std::for_each(input.cbegin(), input.cend(),
            [this] (const char c) { auto s = Symbol::get_symbol(c); step(s); });
        auto it = std::find_if(accepting_states_.cbegin(), accepting_states_.cend(),
            [this] (auto&& s) { return s == current_state_; });
        if (it == accepting_states_.cend())
            return false;
        else
            return true;
    }

protected:
    DFA() {}

    std::size_t initial_state_;
    std::size_t current_state_;

    std::vector<std::size_t> accepting_states_;

    std::vector<std::vector<std::size_t>> trans_table_;
};