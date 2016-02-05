
#pragma once

struct HTTPRequest;





// tolua_begin
class cWebPlugin
{
public:
	// tolua_end

	struct cTab
	{
		AString m_Title;
		AString m_SafeTitle;
		int m_UserData;

		cTab(const AString & a_Title, int a_UserData):
			m_Title(a_Title),
			m_SafeTitle(cWebPlugin::SafeString(a_Title)),
			m_UserData(a_UserData)
		{
		}
	};

	typedef SharedPtr<cTab> cTabPtr;
	typedef std::list<cTabPtr> cTabPtrs;
	typedef std::list<std::pair<AString, AString>> cTabNames;


	cWebPlugin();

	virtual ~cWebPlugin();

	// tolua_begin

	/** Returns the title of the plugin, as it should be presented in the webadmin's pages tree. */
	virtual const AString GetWebTitle(void) const = 0;

	/** Sanitizes the input string, replacing spaces with underscores. */
	static AString SafeString(const AString & a_String);

	// tolua_end

	virtual AString HandleWebRequest(const HTTPRequest & a_Request) = 0;

	/** Adds a new web tab with the specified contents. */
	void AddNewWebTab(const AString & a_Title, int a_UserData);

	/** Removes all the tabs. */
	void ClearTabs(void);

	/** Returns all the tabs that this plugin has registered. */
	const cTabPtrs & GetTabs(void) const { return m_Tabs; }

	/** Returns all of the tabs that this plugin has registered. */
	cTabNames GetTabNames(void) const;  // Exported in ManualBindings.cpp

	/** Returns the tab that has the specified SafeTitle.
	Returns nullptr if no such tab. */
	cTabPtr GetTabBySafeTitle(const AString & a_SafeTitle) const;

	std::pair<AString, AString> GetTabNameForRequest(const HTTPRequest & a_Request);

private:
	/** All tabs that this plugin has registered.
	Protected against multithreaded access by m_CSTabs. */
	cTabPtrs m_Tabs;

	/** Protects m_Tabs against multithreaded access. */
	mutable cCriticalSection m_CSTabs;
};  // tolua_export




