#include <CustomError.hpp>

CustomError::CustomError(const std::string& msg) : message{msg} {};

const char* CustomError::what() const 
#ifdef __APPLE__
	_NOEXCEPT
#else
	_GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
#endif 
{
	return this->message.c_str();
}
