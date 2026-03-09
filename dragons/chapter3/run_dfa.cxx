#include "dfa.h"

#include <cassert>

int main()
{
    // (a|b)*abb
    DFA automat{4, 2};
    Symbol a{'a', 0}, b{'b', 1};

    automat.add_transition(0, 1, a);
    automat.add_transition(0, 0, b);
    automat.add_transition(1, 2, b);
    automat.add_transition(1, 1, a);
    automat.add_transition(2, 1, a);
    automat.add_transition(2, 3, b);
    automat.add_transition(3, 0, b);
    automat.add_transition(3, 1, a);

    automat.set_initial_state(0);
    automat.set_accepting_states({3});

    // automat.step(a);
    // assert(automat.current_state() == 1);
    
    // automat.step(b);
    // assert(automat.current_state() == 0);

    assert(automat.is_accepted("ababb"));
    assert(!automat.is_accepted("aba"));
    assert(automat.is_accepted("abca"));
}




