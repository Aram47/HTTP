#ifndef __CUSTOM_ERROR__
#define __CUSTOM_ERROR__

#include <string>
#include <exception>

class CustomError : public std::exception {
public:
	CustomError(const std::string&);
public:
	const char* what() const
#ifdef __APPLE__
	_NOEXCEPT
#else
	_GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
#endif
;
private:
	CustomError& operator=(const CustomError&) { return *this; };
private:
	const std::string& message;
};

#endif
