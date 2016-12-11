#pragma once

class cObject
{
private:
	SYNTHESIZE(ULONG, m_nRefCount, RefCount);

public:
	cObject(void);
	virtual ~cObject(void);

	virtual void AddRef();
	virtual void Release();

	cObject* AutoRelease();
};

