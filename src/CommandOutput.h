
// CommandOutput.h

// Declares various classes that process command output





/** Interface for a callback that receives command output
The Out() function is called for any output the command has produced.
Descendants override that function to provide specific processing of the output. */
class cCommandOutputCallback
{
public:
	virtual ~cCommandOutputCallback() {}  // Force a virtual destructor in subclasses

	/** Syntax sugar function, calls Out() with Printf()-ed parameters; appends a newline" */
	void Out(const char * a_Fmt, fmt::ArgList);
	FMT_VARIADIC(void, Out, const char *)

	/** Called when the command wants to output anything; may be called multiple times */
	virtual void Out(const AString & a_Text) = 0;

	/** Called when the command processing has been finished */
	virtual void Finished(void) {}
} ;





/** Class that discards all command output */
class cNullCommandOutputCallback :
	public cCommandOutputCallback
{
	// cCommandOutputCallback overrides:
	virtual void Out(const AString & a_Text) override
	{
		// Do nothing
		UNUSED(a_Text);
	}
} ;






/** Accumulates all command output into a string. */
class cStringAccumCommandOutputCallback:
	public cCommandOutputCallback
{
	typedef cCommandOutputCallback super;

public:
	// cCommandOutputCallback overrides:
	virtual void Out(const AString & a_Text) override;
	virtual void Finished(void) override {}

	/** Returns the accumulated command output in a string. */
	const AString & GetAccum(void) const { return m_Accum; }

protected:
	/** Output is stored here until the command finishes processing */
	AString m_Accum;
} ;





/** Sends all command output to a log, line by line, when the command finishes processing */
class cLogCommandOutputCallback :
	public cStringAccumCommandOutputCallback
{
public:
	// cStringAccumCommandOutputCallback overrides:
	virtual void Finished(void) override;
} ;





/** Sends all command output to a log, line by line; deletes self when command finishes processing */
class cLogCommandDeleteSelfOutputCallback :
	public cLogCommandOutputCallback
{
	typedef cLogCommandOutputCallback super;

	virtual void Finished(void) override
	{
		super::Finished();
		delete this;
	}
} ;




