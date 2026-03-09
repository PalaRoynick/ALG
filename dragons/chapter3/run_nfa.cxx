#include "nfa.h"

#include <cassert>

int main()
{
    // (a|b)*abb
    auto n_states = 10ull;
    auto n_edges  = 13ull;

    NFA automat{n_states, n_edges};
    std::vector<std::size_t> ss;
    ss.reserve(n_states);
    for (auto i = 0; i < n_states; ++i)
        ss.push_back(i);

    // need explicitly enumerate all the symbols with different ids
    // (maybe the same values, see the scheme for all the edges)
    // need to set initial shift for non-epsilon symbols
    Symbol a{'a', 0}, b{'b', 1};
    EpsSymbol::set_shift(2);

    automat.add_epsilon_transition(ss[0], ss[1]);
    automat.add_epsilon_transition(ss[1], ss[2]);
    automat.add_epsilon_transition(ss[1], ss[4]);
    automat.add_epsilon_transition(ss[3], ss[6]);
    automat.add_epsilon_transition(ss[5], ss[6]);
    automat.add_epsilon_transition(ss[6], ss[7]);
    automat.add_epsilon_transition(ss[6], ss[1]);
    automat.add_epsilon_transition(ss[0], ss[7]);

    automat.add_transition(ss[2], ss[3], a);
    automat.add_transition(ss[4], ss[5], b);
    automat.add_transition(ss[7], ss[8], a);
    automat.add_transition(ss[8], ss[9], b);
    automat.add_transition(ss[9], ss[10], b);

    automat.set_initial_state(ss[0]);
    automat.set_accepting_states({ss[10]});

    automat.convert_to_dfa();

    // auto t = std::vector<State>{ss[3], ss[8]};
    // for (auto s : automat.epsilon_closure(t))
    //     printf("%d -> ", s);
    // printf("\n");
    // for (auto s : automat.epsilon_closure(ss[1]))
    //     printf("%d -> ", s);
    // printf("\n");
    // for (auto s : automat.epsilon_closure(ss[6]))
    //     printf("%d -> ", s);
    // printf("\n");
    // for (auto s : automat.epsilon_closure(ss[9]))
    //     printf("%d -> ", s);

    // automat.step(a);
    // assert(automat.current_state().id() == s1.id());
    
    // automat.step(b);
    // assert(automat.current_state().id() == s0.id());

    // assert(automat.is_accepted("ababb"));
    // assert(!automat.is_accepted("aba"));
    // assert(automat.is_accepted("abca"));
}
