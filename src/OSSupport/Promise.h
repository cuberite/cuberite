#pragma once

class cCombinedPromise;


class cPromise {
	public:
		cPromise() {}
		virtual ~cPromise () {}
		cPromise * WaitFor(cPromise * a_Promise);
		cPromise * CancelOn(volatile bool& cancelationtoken);
		void Wait();
		virtual bool IsCompleted() = 0;
		//TODO:Expose Events for waiting on
};

class cCombinedPromise : public cPromise {
public:
	cCombinedPromise(cPromise*, cPromise*);
	~cCombinedPromise();
	virtual bool IsCompleted();
private:
	cPromise* m_left;
	cPromise* m_right;
};

class cCancelablePromise : public cPromise {
public:
	cCancelablePromise(cPromise*, volatile bool&);
	~cCancelablePromise();
	virtual bool IsCompleted();
private:
	volatile bool& m_cancel;
	cPromise* m_wrapped;
};



