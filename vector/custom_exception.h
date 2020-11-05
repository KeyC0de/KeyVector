#include <sstream>
#include <string>
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#if defined(_UNICODE) || defined(UNICODE)
#	define unicode
#	if defined(_MSC_VER) || defined (_WIN32) || defined(_WIN64)
#		define __function__ __FUNCTIONW__
#	elif defined(GNUC) || defined(__linux__) || defined (__unix__) || defined (GNUG)
#		define __function__ __PRETTY_FUNCTION__
#	endif
#else
#	define ascii
#	if defined(_MSC_VER) || defined (_WIN32) || defined(_WIN64)
#		define __function__ __FUNCSIG__
#	elif defined(GNUC) || defined(__linux__) || defined(__unix__) || defined(GNUG)
#		define __function__ __PRETTY_FUNCTION__
#	endif
#endif
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#if defined(_UNICODE) || defined(UNICODE)	// it's defined

class Exception
{
	std::wstring m_msg;
public:
	Exception(const std::wstring &arg, const std::wstring &file,
		const std::wstring &function, int line)
	{
		std::wostringstream woss;
		woss << arg << ' '
			<< file << ", "
			<< function << "@line:#"
			<< line << ": ";
		m_msg = woss.str();
	}

	const std::wstring what() const noexcept
	{
		return m_msg;
	}
};

#define throwException(msg) throw Exception(L ## msg, __FILEW__, __function__, __LINE__);
/////////////////////////////////////////////////////////////////////////////////////////
#else
/////////////////////////////////////////////////////////////////////////////////////////
/*
class Exception : public std::runtime_error // or std::logic_error
{
	std::string m_msg;
public:
	Exception(const std::string &arg, const char *file, const char* function, int line) :
		std::runtime_error(arg)
	{
		std::ostringstream oss;
		oss << file << '!'
			<< function << " @line:#"
			<< line << ": "
			<< arg;
		m_msg = oss.str();
	}
	~Exception() override;
	Exception(const Exception& ex) = default;
	Exception& operator=(const Exception& ex) = default;

	virtual const char* what() const noexcept override {
		return m_msg.c_str();
	}
};

#define throwException(arg) throw Exception(arg, __FILE__, __function__, __LINE__);
*/
class Exception
{
	std::string m_msg;
public:
	Exception(const std::string &arg, const std::string &file,
		const std::string &function, int line)
	{
		std::ostringstream oss;
		oss << arg << ' '
			<< file << " !"
			<< function << " @line:#"
			<< line << ": ";
		m_msg = oss.str();
	}

	const std::string what() const noexcept
	{
		return m_msg;
	}
};

#define throwException(msg) throw Exception(msg, __FILE__, __function__, __LINE__);
#endif