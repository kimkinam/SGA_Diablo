#pragma once
class cUIInventory :public cUIObject
{
private:
	ST_ITEM			item_;

public:
	cUIInventory();
	~cUIInventory();



	void Setup();
	void Update();
	void Render();
};

