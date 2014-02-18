
#pragma once
class cSocketSet
{
public:

	cSocketSet();
	cSocketSet(const std::vector<cSocket> & a_Sockets);
	
	void Add(cSocket a_Socket);
	
	bool IsSet(cSocket & a_Socket);
	
	void Reset();
	
	class cEventIterator
	{
	public:
		cEventIterator() {}
		~cEventIterator() {}

		cEventIterator & operator= (const cEventIterator & other);

		bool operator== (const cEventIterator & other);

		bool operator!= (const cEventIterator & other);


		cEventIterator & operator++();

		cEventIterator operator++(int)
		{
			cEventIterator tmp(*this);
			++(*this);
			return(tmp);
		}

		cSocket & operator* ();

		cSocket * operator-> ();

	};
	
	cEventIterator Eventbegin();
	cEventIterator Eventend();
	
private:
	std::vector<cSocket> m_Sockets;
	cSocket	Highest;
	
};
