#pragma once


class cWindow;
class cBlockEntity;





class cWindowOwner
{
public:
	cWindowOwner() : m_Window( NULL ) {}
	void CloseWindow() { m_Window = NULL; }
	void OpenWindow( cWindow* a_Window ) { m_Window = a_Window; }

	cWindow* GetWindow() { return m_Window; }

	void SetEntity(cBlockEntity *a_Entity) { m_Entity = a_Entity; }
	cBlockEntity *GetEntity() { return m_Entity; }
private:
	cWindow* m_Window;
	cBlockEntity *m_Entity;
};




