

template<class T>
class Option
{
public:
	Option(T a_value);
	static Option<T> None();
	template<class U, class F>
	U To(F a_mapper, U a_default) const;
	bool HasValue();
	T GetValue();
};
