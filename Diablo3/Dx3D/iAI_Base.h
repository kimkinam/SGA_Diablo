#pragma once



class iAI_Base
{
private:
	//고유 식별 번호
	SYNTHESIZE(UINT, m_ID, ID);

	//다음 유효한 식별번호(총 갯수)
	static UINT m_INextID;


public:
	iAI_Base() { }
	iAI_Base(UINT id)
	{
		SetID(id);
	}
	virtual ~iAI_Base(){ }

	virtual void Update() PURE;
};

