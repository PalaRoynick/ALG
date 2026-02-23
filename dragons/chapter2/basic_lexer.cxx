#include "basic_lexer.h"

Lexer::Lexer(std::istringstream&& iss) : iss_(std::move(iss))
{
    words_["true"]  = std::make_unique<Word>("true");
    words_["false"] = std::make_unique<Word>("false");
}

std::unique_ptr<Token> Lexer::scan_step()
{
    // skip all the whitespaces
    for (; valid_read() && peek_ != iss_.eof(); peek_ = iss_.peek(), iss_.ignore())
    {
        if ( (peek_ == ' ') || (peek_ == '\t') ) continue;
        else if (peek_ == '\n')
        {  lines_++; std::cout << "NEWLINE!\n"; }
        else break;
    }
    // gather a number from digits
    if (is_digit())
    {
        int v = 0;
        for (; is_digit(); v = make_digit(peek_) + v * 10, iss_ >> peek_) {}
        return std::make_unique<Number>(v);
    }
    // gather all chars and digits to build a word
    if (is_letter())
    {
        std::ostringstream oss;
        for (; is_letter(); oss << peek_, peek_ = iss_.get()) {}
        auto str = oss.str();
        auto it = words_.find(str);
        if (it != words_.cend())
            return it->second->clone();
        words_[str] = std::make_unique<Word>(str);
        return std::make_unique<Word>(str);
    }
    if (peek_ == '{')
    {
        peek_ = ' ';
        return std::make_unique<OpenBrace>();
    }
    if (peek_ == '}')
    {
        peek_ = ' ';
        return std::make_unique<CloseBrace>();
    }
    if (peek_ == ';')
    {
        peek_ = ' ';
        return std::make_unique<Semicol>();
    }
    // the new token met
    if (valid_read())
    {
        auto new_token = std::make_unique<Token>(peek_);
        peek_ = ' ';
        return new_token;
    }
    return nullptr;
}
