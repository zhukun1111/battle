#ifndef _H_WMTIME_H_
#define _H_WMTIME_H_

class CWMTime
{
public:
	CWMTime(void);
	CWMTime(const CWMTime& rhs);
	CWMTime& operator = (const CWMTime& rhs);

	friend class OctetsStream;

private:
	void _SetTime(UINT64 qwTimeMsec);
	UINT64 m_qwTimeMsec;
};

#endif