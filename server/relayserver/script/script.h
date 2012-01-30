#ifndef _H_SCRIPT_H_
#define _H_SCRIPT_H_

class CScript
{
public:
	explicit CScript(UINT32 dwScriptID, const char* pszScriptPath) : 
		m_dwScriptID(dwScriptID), m_strScriptPath(pszScriptPath)
		{

		}

		virtual ~CScript()
		{

		}

		UINT32 GetScriptID() const 
		{
			return m_dwScriptID;
		}

		const std::string& GetScriptPath() const
		{
			return m_strScriptPath;
		}

		bool CallFunction(const char* pszFuncName, bool bHasReturn, const char* pszArgFormat, ...);
		bool GetReturn(const char* pszReturnFormat, ...);

		static bool CallGlobalFunction(const char* pszFuncName, bool bHasReturn, const char* pszArgFormat, ...);
		static bool GetGlobalReturn(const char* pszReturnFormat, ...);

private:
	UINT32 m_dwScriptID;
	std::string m_strScriptPath;
};

#endif