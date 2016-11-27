#pragma once

class cUIImage : public cUIObject
{
protected:
	LPDIRECT3DTEXTURE9		m_pTexture;
	SYNTHESIZE(RECT, rc_image, Rc_iamge);
public:
	cUIImage();
	virtual ~cUIImage();

	virtual void SetTexture(char* szFileName);


	//cUIObect override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

