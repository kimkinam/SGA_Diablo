#pragma once



class iAI_Base
{
private:
	//���� �ĺ� ��ȣ
	SYNTHESIZE(UINT, m_ID, ID);

	//���� ��ȿ�� �ĺ���ȣ(�� ����)
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

