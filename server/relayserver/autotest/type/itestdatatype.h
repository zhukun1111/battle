#ifndef _H_TEST_DATA_TYPE_H_
#define _H_TEST_DATA_TYPE_H_

class ITestDataType
{
public:
	virtual bool Init() = 0;
	virtual std::string GetTypeName() = 0;
	virtual bool UnMarshal(const std::string  & rstrDataValue, void * pDataValue) = 0;
	virtual bool Marshal(std::string& rstrDataValue, const void * pDataValue) = 0;
};

inline INT32 FindNextData(const std::string & rstrDataValue, INT32 nStart)
{

}
#endif