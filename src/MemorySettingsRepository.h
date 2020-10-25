
#pragma once

#include "SettingsRepositoryInterface.h"

class cMemorySettingsRepository : public cSettingsRepositoryInterface
{
public:

	virtual bool KeyExists(const AString keyname) const override;

	virtual bool HasValue(const AString & a_KeyName, const AString & a_ValueName) const override;

	virtual int AddKeyName(const AString & keyname) override;

	virtual bool AddKeyComment(const AString & keyname, const AString & comment) override;

	virtual AString GetKeyComment(const AString & keyname, const int commentID) const override;

	virtual bool DeleteKeyComment(const AString & keyname, const int commentID) override;

	virtual void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value) override;
	void AddValue (const AString & a_KeyName, const AString & a_ValueName, const Int64 a_Value);
	void AddValue (const AString & a_KeyName, const AString & a_ValueName, const bool a_Value);

	virtual std::vector<std::pair<AString, AString>> GetValues(AString a_keyName) override;

	virtual AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const override;


	virtual AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "") override;
	virtual int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0) override;
	virtual Int64   GetValueSetI(const AString & keyname, const AString & valuename, const Int64     defValue = 0) override;
	virtual bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false) override;

	virtual bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true) override;
	virtual bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true) override;

	virtual bool DeleteValue(const AString & keyname, const AString & valuename) override;

	virtual bool Flush() override;

	void SetReadOnly()
	{
		m_Writable = false;
	}

private:

	bool m_Writable = true;

	struct sValue
	{
		sValue(AString value):
			#ifdef _DEBUG
				m_Type(eType::String),
			#endif
			m_stringValue (std::move(value))
		{
		}

		sValue(Int64 value):
			#ifdef _DEBUG
				m_Type(eType::Int64),
			#endif
			m_intValue(value)
		{
		}

		sValue(bool value):
			#ifdef _DEBUG
				m_Type(eType::Bool),
			#endif
			m_boolValue(value)
		{
		}

		AString getStringValue() const { ASSERT(m_Type == eType::String); return m_stringValue; }
		Int64   getIntValue()    const { ASSERT(m_Type == eType::Int64);  return m_intValue;    }
		bool    getBoolValue()   const { ASSERT(m_Type == eType::Bool);   return m_boolValue;   }

	private:

		#ifdef _DEBUG
			enum class eType
			{
				String,
				Int64,
				Bool
			} m_Type;
		#endif

		AString m_stringValue;
		union
		{
			Int64 m_intValue;
			bool m_boolValue;
		};
	};

	std::unordered_map<AString, std::unordered_multimap<AString, sValue>> m_Map{};
};

