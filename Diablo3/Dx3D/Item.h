#pragma once

enum E_ITEM
{
	ITEM_HEAD,     // 0
	ITEM_ARM,      // 1
	ITEM_CHEST,    // 2
	ITEM_PANTS,    // 3
	ITEM_BOOTS,    // 4
	ITEM_SHOULDER, // 5
	ITEM_SWORD,    // 6
	ITEM_ACCESS,   // 7
	ITEM_COUNT     // 8
};

struct ST_PLAYET_STAT
{
	float	fHp;		// ���� ����
	float	fMp;		// ���� ����
	float	fAtk;		// ���Ⱑ ����
	float	fSpeed;		// ������ ����

	ST_PLAYET_STAT(){ fHp = fMp = fAtk = fSpeed = 0.0f; }
	ST_PLAYET_STAT(float _hp, float _mp, float _atk, float _speed)
	{
		fHp = _hp;
		fMp = _mp;
		fAtk = _atk;
		fSpeed = _speed;
	}
};

struct ST_ITEM
{
	E_ITEM	eItemType;
	char*	szFileName;

	ST_PLAYET_STAT	stPlusStat;

	ST_ITEM()
	{
		eItemType = E_ITEM::ITEM_COUNT;
		szFileName = NULL;
		stPlusStat = ST_PLAYET_STAT(0, 0, 0, 0);
	}

	ST_ITEM(E_ITEM _type, char* _fileName, ST_PLAYET_STAT _plusStat)
	{
		eItemType = _type;
		szFileName = _fileName;
		stPlusStat = _plusStat;
	}
};

bool IsRectIn(RECT rc)
{
	if (PtInRect(&rc, g_ptMouse))
		return true;

	return false;
}

