#ifndef H_INCLUDE_COMMANDER_EXCEPTIONS
#define H_INCLUDE_COMMANDER_EXCEPTIONS

#include <exception>
#include <iostream>

namespace Commander {

class UnknownException : public std::exception {
public:
    explicit UnknownException(){};
    explicit UnknownException(const std::string &message) : message(message){};
    virtual const char *what() const noexcept { return this->message.c_str(); }
    void update_message(const std::string &message) { this->message = message; }

protected:
    std::string message;

};

class NotFoundException : public UnknownException {
public:
    explicit NotFoundException() : UnknownException(){};
    explicit NotFoundException(const std::string &message)
        : UnknownException(message){};
};

class InvalidLabelException : public UnknownException {
public:
    explicit InvalidLabelException() : UnknownException(){};
    explicit InvalidLabelException(const std::string &message)
        : UnknownException(message){};
};

class InvalidAdditionException : public UnknownException {
public:
    explicit InvalidAdditionException() : UnknownException(){};
    explicit InvalidAdditionException(const std::string &message)
        : UnknownException(message){};
};

class InvalidPositionalArgumentsNumberException : public UnknownException {
public:
    explicit InvalidPositionalArgumentsNumberException() : UnknownException(){};
    explicit InvalidPositionalArgumentsNumberException(
        const std::string &message
    ) : UnknownException(message){};
};

class InvalidOptionLabelException : public InvalidLabelException {
public:
    explicit InvalidOptionLabelException() : InvalidLabelException(){};
    explicit InvalidOptionLabelException(const std::string &message)
        : InvalidLabelException(message){};
};

class OptionParsingException : public UnknownException {
public:
    explicit OptionParsingException() : UnknownException(){};
    explicit OptionParsingException(const std::string &message)
        : UnknownException(message){};
};
} // namespace Commander

#endif
