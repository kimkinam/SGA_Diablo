#pragma once

class cObject
{
private:
	ULONG m_nRefCount;

public:
	cObject(void);
	virtual ~cObject(void);

	virtual void AddRef();
	virtual void Release();
};

