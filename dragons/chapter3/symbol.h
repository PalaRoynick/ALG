#pragma once

class Symbol
{
public:
    Symbol(char c, std::size_t id)
        : value_(c), id_(id)
    {
        if (check(c, id))
            symbols_table_.push_back(*this);
    }

    std::size_t id() const { return id_; }
    char value() const { return value_; }

    static auto get_symbol(const char c)
    {
        auto it = std::find_if(symbols_table_.cbegin(), symbols_table_.cend(),
            [c] (auto&& s) { return s.value() == c; });
        if (it == symbols_table_.cend())
            throw std::runtime_error(std::string("A wrong symbol met : ") + c);
        return *it;
    }

    static const auto& table()
    {
        return symbols_table_;
    }

    static auto unique_symbols()
    {
        auto copy = symbols_table_;

        // no epsilon-symbols (' ')
        copy.erase(std::remove_if(copy.begin(), copy.end(),
            [] (auto& s) { return s.value() == ' '; }), copy.end());

        // no repeating symbols
        std::sort(copy.begin(), copy.end(),
            [] (auto& s1, auto& s2) { return s1.value() < s2.value(); });
        copy.erase(std::unique(copy.begin(), copy.end(),
            [] (auto& s1, auto& s2) { return s1.value() == s2.value(); }), copy.end());

        return copy;
    }

protected:
    Symbol() {}

private:
    bool check(char c, std::size_t id) const
    {
        // auto it = std::find_if(symbols_table_.cbegin(), symbols_table_.cend(),
        //     [c, id] (auto&& s) { return /*s.value() == c ||*/ s.id() == id; });
        // if (it != symbols_table_.cend())
        //     throw std::runtime_error(
        //         std::string("Cannot create a new symbol with value = ")
        //         + c
        //         + " and id = "
        //         + std::to_string(id));
       return true;
    }

protected:
    char value_;
    std::size_t id_ = 0;

    static std::vector<Symbol> symbols_table_;
};

std::vector<Symbol> Symbol::symbols_table_;

class EpsSymbol : public Symbol
{
public:
    // can only be created inside of NFA::add_epsilon_transition
    EpsSymbol()
    {
        value_ = ' ';
        id_ = total_count++;
    }

    static void set_shift(std::size_t shift)
    {
        total_count = shift;
    }

    static std::size_t count()
    {
        return total_count;
    }

private:
    static std::size_t total_count;
};

std::size_t EpsSymbol::total_count;