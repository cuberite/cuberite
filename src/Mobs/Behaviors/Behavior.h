class cBehavior
{
public:
	virtual bool IsControlDesired();
	virtual bool ControlStarting();
	virtual bool ControlEnding();
	virtual void Tick();
	virtual void PostTick();
	virtual void PreTick();
	virtual void onRightClicked();
	virtual void Destroyed();
	virtual ~cBehavior() = 0;
};
