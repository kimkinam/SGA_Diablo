#pragma once

enum E_ITEM
{
	ITEM_HEAD,
	ITEM_ARM,
	ITEM_CHEST,
	ITEM_PANTS,
	ITEM_BOOTS,
	ITEM_SHOULDER,
	ITEM_SWORD,
	ITEM_ACCESS,
	ITEM_COUNT
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
};

bool IsRectIn(RECT rc)
{
	if (PtInRect(&rc, g_ptMouse))
		return true;

	return false;
}

