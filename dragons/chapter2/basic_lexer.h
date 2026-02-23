#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <iostream>
#include <cctype>
#include <unordered_map>
#include <cassert>

// Basic lexer to work with the following tokens:
//   1. num, e.g. <num, 31>
//   2.  id, e.g. <id, "count">
//   3. some reserved keywords like "if", "while"
//   4. boolean keywords "true", "false"

enum Tag
{
    NUM   = 256,
    ID    = 257,

    TRUE  = 258,
    FALSE = 259,

    OPEN_BRACE = 260,
    CLOSE_BRACE = 261,

    SEMICOL = 262
};

struct Token
{
    int tag;

    Token (int t) : tag(t) {}

    virtual std::unique_ptr<Token> clone() const
    {
        return std::make_unique<Token>(*this);
    }
};

struct Number : Token
{
    int value;
    Number(int v) : Token(NUM), value(v) {}

    std::unique_ptr<Token> clone() const override
    {
        return std::make_unique<Number>(*this);
    }
};

struct Word : Token
{
    std::string lexeme;
    Word(const std::string& s) : Token(ID), lexeme(s) {}

    std::unique_ptr<Token> clone() const override
    {
        return std::make_unique<Word>(*this);
    }
};

struct OpenBrace : Token
{
    OpenBrace() : Token(OPEN_BRACE) {}
    std::unique_ptr<Token> clone() const override
    {
        return std::make_unique<OpenBrace>(*this);
    }
};

struct CloseBrace : Token
{
    CloseBrace() : Token(CLOSE_BRACE) {}
    std::unique_ptr<Token> clone() const override
    {
        return std::make_unique<CloseBrace>(*this);
    }
};

struct Semicol : Token
{
    Semicol() : Token(SEMICOL) {}
    std::unique_ptr<Token> clone() const override
    {
        return std::make_unique<Semicol>(*this);
    }
};

class Lexer
{
public:
    Lexer(std::istringstream&& iss);

    std::unique_ptr<Token> scan_step();

    int lines() const noexcept
    {
        return lines_;
    }

private:
    bool valid_read() const { return !iss_.eof() && iss_.good(); }

    bool is_digit() const { return valid_read() && std::isdigit(peek_); }

    bool is_letter() const { return valid_read() && (std::isalpha(peek_) || std::isdigit(peek_)); }

    int make_digit(char c)
    {
        assert(std::isdigit(c));
        switch (c)
        {
        case '0': return 0; break;
        case '1': return 1; break;
        case '2': return 2; break;
        case '3': return 3; break;
        case '4': return 4; break;
        case '5': return 5; break;
        case '6': return 6; break;
        case '7': return 7; break;
        case '8': return 8; break;
        case '9': return 9; break;
        }
        return -1;
    }

    char peek_ = ' ';
    int lines_ = 0;
    std::istringstream iss_;

    std::unordered_map<std::string, std::unique_ptr<Token>> words_;
};

inline auto print(std::unique_ptr<Token> tok)
{
    if (tok == nullptr)
        throw std::runtime_error("Null token given for print!\n");
    switch (tok->tag)
    {
    case Tag::NUM:
        std::cout << static_cast<Number*>(tok.get())->value << std::endl;
        break;
    case Tag::ID:
    case Tag::TRUE:
    case Tag::FALSE:
        std::cout << static_cast<Word*>(tok.get())->lexeme << std::endl;
        break;
    case Tag::OPEN_BRACE:
        std::cout << "{" << std::endl;
        break;
    case Tag::CLOSE_BRACE:
        std::cout << "}" << std::endl;
        break;
    case Tag::SEMICOL:
        std::cout << ";" << std::endl;
        break;
    default:
        std::cout << static_cast<char>(tok.get()->tag) << std::endl;
        break;
    }
}
