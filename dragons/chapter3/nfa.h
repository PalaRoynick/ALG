#pragma once

#include <set>
#include <stack>
#include <numeric>

#include <iostream>

#include "dfa.h"

class StateDFA
{
public:
    StateDFA(const std::set<std::size_t> s)
        : marked_(false), state_ids_(s)
    {}

    std::size_t id() const { return id_; }
    
    void set_id(std::size_t id) { id_ = id; }

    void set_marked(bool b) { marked_ = b; }

    bool marked() const { return marked_; }

    auto begin()       { return state_ids_.begin();  }
    auto begin() const { return state_ids_.cbegin(); }
    auto end()         { return state_ids_.end();    }
    auto end()   const { return state_ids_.cend();   }

    void print() const
    {
        printf("{");
        for (auto e : state_ids_) std::cout << e << ", ";
        printf("}\n");
    }

    friend bool operator==(const StateDFA& s1, const StateDFA& s2);
private:
    std::size_t id_;
    bool marked_ = false;
    std::set<std::size_t> state_ids_;
};

bool operator==(const StateDFA &s1, const StateDFA &s2)
{
    return s1.state_ids_ == s2.state_ids_;
}

class NFA : public DFA
{
public:
    NFA(std::size_t n_states, std::size_t n_edges)
    {
        eps_symbol_ids_.resize(n_states, std::vector<std::size_t>(0));
        trans_table_.resize(n_states, std::vector<std::size_t>(n_edges, 100500));
    }

    DFA convert_to_dfa() const
    {
        const auto& unique_symbols = Symbol::unique_symbols();
        DFA dfa{trans_table_.size(), unique_symbols.size()};

        std::size_t n_states = 0;
        std::vector<StateDFA> dfa_states;
        dfa_states.reserve(trans_table_.size());

        // initial state
        dfa_states.emplace_back( epsilon_closure(initial_state_) );
        dfa_states[0].set_id(n_states++);

        for (;;)
        {
            auto it = std::find_if(dfa_states.begin(), dfa_states.end(),
                [] (const auto& s) { return !s.marked(); });
            if (it == dfa_states.end()) break;
            it->set_marked(true);
            auto& t = *it;   // below we do dfa_states.push_back(u) => invalidation if use ref, if not reserved!

            // assume we have only one symbol table in the whole program
            for (auto i = 0ull; i < unique_symbols.size(); ++i)
            {
                const auto& sym = unique_symbols[i];

                auto u = StateDFA{ epsilon_closure(sym_move(t, sym)) };
                auto itt = std::find(dfa_states.cbegin(), dfa_states.cend(), u);
                if(itt == dfa_states.cend())
                {
                    u.set_id(n_states++);
                    dfa_states.push_back(u);
                }
                else
                {
                    u = *itt;
                }
                dfa.add_transition(t.id(), u.id(), Symbol{sym.value(), i});
            }
        }

        for (auto& st : dfa_states)
        {
            st.print();
        }

        return dfa;
    }

    void add_epsilon_transition(std::size_t s1, std::size_t s2)
    {
        eps_symbol_ids_[s1].push_back(EpsSymbol::count());
        add_transition(s1, s2, EpsSymbol{});
    }

// private:
    std::set<std::size_t> sym_move(const StateDFA& s, const Symbol& sym) const
    {
        std::set<std::size_t> move_set;
        for (auto sid : s)
        {
            if (trans_table_[sid][sym.id()] != 100500)  // meh...
                move_set.insert(trans_table_[sid][sym.id()]);
        }
        return move_set;
    }

    std::set<std::size_t> epsilon_closure(std::size_t s) const
    {
        std::stack<std::size_t> st; st.push(s);
        std::set<std::size_t> closure{s};

        while (!st.empty())
        {
            auto t = st.top(); st.pop();

            for (std::size_t e : eps_symbol_ids_[t])
            {
                auto u = trans_table_[t][e];
                if (!closure.contains(u))
                {
                    closure.insert(u);
                    st.push(u);
                }
            }
        }

        return closure;
    }

    std::set<std::size_t> epsilon_closure(const std::set<std::size_t>& ss) const
    {
        auto make_set_union = [] (
            const std::set<std::size_t>& s1,
            const std::set<std::size_t>& s2)
        {
            std::set<std::size_t> common;
            std::set_union(
                s1.cbegin(), s1.cend(),
                s2.cbegin(), s2.cend(),
                std::inserter(common, common.begin())
            );
            return common;
        };

        return std::transform_reduce(ss.cbegin(), ss.cend(), std::set<std::size_t>{},
            [make_set_union] (const auto& s1, const auto& s2) { return make_set_union(s1, s2); },
            [this] (const auto& s) { return epsilon_closure(s); }
        );
    }

private:
    std::vector<std::vector<std::size_t>> eps_symbol_ids_;
};