/*
    Multiply permutations given in cycles-form.
    Knuth, algo A [I-1.3.3]
*/
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>

class permutation
{
public:
    struct element
    {
        char c;
        bool m = false; // marked or not
    };

public:
    explicit permutation(const std::string& s)
    {
        auto sz = s.size();
        s_.reserve(sz);
        for (std::size_t i = 0; i < sz; ++i)
            s_.push_back({s[i], false});
    }

    permutation(const std::vector<element>& s) : s_(s) {}

    void print() const noexcept
    {
        std::cout << std::endl;
        for (auto e : s_) std::cout << e.c;
    }

    operator std::string()
    {
        std::string s;
        s.resize(size());
        std::transform(s_.cbegin(), s_.cend(), s.begin(), [] (const auto& e) { return e.c; });
        return s;
    }

    std::size_t size() const noexcept
    {
        return s_.size();
    }

    auto begin()
    {
        return s_.begin();
    }

    auto begin() const
    {
        return s_.begin();
    }

    auto cbegin() const
    {
        return s_.cbegin();
    }

    auto end()
    {
        return s_.end();
    }

     auto end() const
    {
        return s_.end();
    }

    auto cend() const
    {
        return s_.cend();
    }

    permutation& operator *=(const permutation& p)
    {
        std::string s;
        std::stringstream ss;

        std::vector<permutation::element> els;
        els.reserve(size() + p.size());

        fill_elements(els, *this);
        fill_elements(els, p);

        auto start = els.begin();
        auto current = els.begin();

        for (;;)
        {
            start = std::find_if(els.begin(), els.end(), [] (auto& e) { return !e.m; });
            if (start == els.end()) break;
        
            ss << "(" << start->c;
            start->m = true;
            current = start + 1;

            auto right = els.begin();
            auto  left = current;
            for(;;)
            {
                right = std::find_if(left + 1, els.end(), [current] (auto& e)
                    { return (!e.m) && (e.c == current->c); });
                if (right == els.end())
                {
                    if (current->c != start->c)
                    {
                        ss << current->c;
                        left = std::prev(std::find_if(els.begin(), els.end(),
                            [] (auto& e) { return !e.m; }));
                        continue;
                    }
                    else break;
                }
                right->m = true;
                current = right + 1;
                left = current;
            }
            ss << ")";
        }

        ss >> s;
        s_.clear();
        s_.resize(s.size());
        std::transform(s.cbegin(), s.cend(), s_.begin(), [] (auto c) { return element{c, false}; });
        return *this;
    }

private:
    std::vector<element> s_;

    inline void fill_elements(std::vector<permutation::element>& els, const permutation& p)
    {
        char head;
        bool head_flag = false;
        for (const auto e : p)
        {
            auto c = e.c;
            switch (c)
            {
            case '(':
                head_flag = true;
                els.push_back({c, true});
                break;
            case ')':
                els.push_back({head, true});
                break;
            default:
                if (head_flag)
                {
                    head = c;
                    head_flag = false;
                }
                els.push_back({c, false});
                break;
            }
        }
    }
};

inline auto operator *(const permutation& p1, const permutation& p2)
{
    auto p = p1; p *= p2;
    return p;
}

int main()
{
    permutation p1("(acfg)");
    permutation p2("(bcd)");
    permutation p3("(aed)");
    permutation p4("(fade)");
    permutation p5("(bgfae)");

    auto s = static_cast<std::string>( ((p1*p2)*p3)*(p4*p5) );
    assert(s == "(adg)(bce)(f)");


    permutation p6("(acf)(bd)");
    permutation p7("(abd)(ef)");

    (p6*p7).print();
}
