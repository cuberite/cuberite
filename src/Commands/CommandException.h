#pragma once
class cCommandParseException : public std::exception
{
public:
	explicit cCommandParseException(const char * a_Message)
		: m_Msg(a_Message) {}

	explicit cCommandParseException(const AString & a_Message)
		: m_Msg(a_Message) {}

	virtual const char * what() const noexcept override
	{
		return m_Msg.c_str();
	}
private:
	const AString m_Msg;
};

class cCommandExecutionException : public cCommandParseException
{
	public:
	explicit cCommandExecutionException(const AString & a_Message)
		: cCommandParseException(a_Message) {}
};
