#include "basic_scope.h"
#include "basic_lexer.h"

#include <stack>
#include <vector>

// The task is described in 2.7.2 "Dragons book" (about compilers)
int main()
{
    std::string input = "{ int x; char y; { bool y; x; y; } x; y; }";
    std::istringstream iss(input);

    Lexer lex(std::move(iss));
    std::vector<std::unique_ptr<Token>> tokens;
    for (;;)
    {
        auto t = lex.scan_step();
        if (!t) break;
        tokens.push_back(std::move(t));
    }
    assert(tokens.size() == 21);

    std::stack<std::unique_ptr<Scope>> scopes;
    scopes.push(nullptr);

    for (auto it = tokens.cbegin(); it != tokens.cend() - 1; ++it)
    {
        auto&& t  = (*it)->clone();
        auto&& nt = (*(it + 1))->clone();
        if (t->tag == Tag::OPEN_BRACE)
        {
            auto scope = std::make_unique<Scope>(scopes.top().get());
            scopes.push(std::move(scope));
            std::cout << " { ";
        }
        else if (t->tag == Tag::CLOSE_BRACE)
        {
            scopes.pop();
            std::cout << " } ";
        }
        // two IDs in a row => "type id;" construction
        else if (t->tag == Tag::ID && nt->tag == Tag::ID)
        {
            auto type = static_cast<Word*>(t.get())->lexeme;
            auto   id = static_cast<Word*>(nt.get())->lexeme;
            Symbol s{type};
            scopes.top()->set(id, s);
            ++it;
        }
        // just single identifier "id;"
        else if (t->tag == Tag::ID && nt->tag == Tag::SEMICOL)
        {
            auto id = static_cast<Word*>(t.get())->lexeme;
            auto s = scopes.top()->get(id);
            std::cout << id;
            std::cout << ":";
            std::cout << s.type;
            std::cout << "; ";
        }
    }
    std::cout << "}";
}