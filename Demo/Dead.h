
//////////////////À¿Õˆ∂Øª≠º”‘ÿ

class dead{

public:
	static void DeleteImage();
	static BOOL LoadImage();

	bool Draw(CDC* pDC, bool bPause);
	//CDead(CPoint ptPos);
	virtual ~CExplosion();

private:
	int m_nProcess;

	static CImageList m_Images;

}