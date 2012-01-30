#ifndef _H_DATA_H_
#define _H_DATA_H_

class CData
{
public:
	CData(){}
	virtual ~CData(){}

	virtual void Destroy(void) const
	{
		delete this;
	}

	friend OctetsStream& operator << (OctetsStream& os, const CData& roData);
	friend OctetsStream& operator >> (const OctetsStream& os, CData& roData);

	virtual CData* Clone() const = 0;
	virtual CData& operator = (const CData& roData) = 0;

protected:
	virtual OctetsStream& _Marshal(OctetsStream& os) const = 0;
	virtual const OctetsStream& _Unmarshal(const OctetsStream& os) = 0;
};

inline OctetsStream& operator << (OctetsStream&os, const CData& roData)
{
	return roData._Marshal(os);
}

inline const OctetsStream& operator >> (const OctetsStream& os, CData& roData)
{
	return roData._Unmarshal(os);
}

#endif