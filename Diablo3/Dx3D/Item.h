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

struct ST_PLUS_STAT
{
	float	fHp;		// 방어구가 증가
	float	fMp;		// 방어구가 증가
	float	fAtk;		// 무기가 증가
	float	fSpeed;		// 부츠가 증가

	ST_PLUS_STAT(){ fHp = fMp = fAtk = fSpeed = 0.0f; }
	ST_PLUS_STAT(float _hp, float _mp, float _atk, float _speed)
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

	ST_PLUS_STAT	stPlusStat;

	ST_ITEM()
	{
		eItemType = E_ITEM::ITEM_COUNT;
		szFileName = NULL;
		stPlusStat = ST_PLUS_STAT(0, 0, 0, 0);
	}

	ST_ITEM(E_ITEM _type, char* _fileName, ST_PLUS_STAT _plusStat)
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

