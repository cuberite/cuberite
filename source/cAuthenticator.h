
// cAuthenticator.h

// Interfaces to the cAuthenticator class representing the thread that authenticates users against the official MC server
// Authentication prevents "hackers" from joining with an arbitrary username (possibly impersonating the server admins)
// For more info, see http://wiki.vg/Session#Server_operation
// In MCS, authentication is implemented as a single thread that receives queued auth requests and dispatches them one by one.





#pragma once
#ifndef CAUTHENTICATOR_H_INCLUDED
#define CAUTHENTICATOR_H_INCLUDED

#include "cIsThread.h"





// fwd: "cRoot.h"
class cRoot;





class cAuthenticator :
	public cIsThread
{
	typedef cIsThread super;
	
public:
	cAuthenticator(void);
	~cAuthenticator();

	/// (Re-)read server and address from INI:
	void ReadINI(void);

	/// Queues a request for authenticating a user. If the auth fails, the user is kicked
	void Authenticate(const AString & iUserName, const AString & iServerHash);

	// Stops the authenticator thread
	void Stop(void);
	
private:

	class cUser
	{
	public:
		AString mName;
		AString mServerHash;
		
		cUser(const AString & iName, const AString & iServerHash) : mName(iName), mServerHash(iServerHash) {}
	} ;
	
	typedef std::deque<cUser> cUserList;
	
	cCriticalSection mCS;
	cUserList        mQueue;
	cEvent           mQueueNonempty;
	
	AString mServer;
	AString mAddress;
	bool    mShouldAuthenticate;
	
	// cIsThread override:
	virtual void Execute(void) override;
	
	// Returns true if the user authenticated okay, false on error; iLevel is the recursion deptht (bails out if too deep)
	bool AuthFromAddress(const AString & iServer, const AString & iAddress, const AString & iUserName, int iLevel = 1);
};





#endif  // CAUTHENTICATOR_H_INCLUDED




