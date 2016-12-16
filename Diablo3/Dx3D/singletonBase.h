#pragma once

template <typename T>
class singletonBase
{
protected:
	//싱글톤 인스턴스 선언
	static T* singleton;

	singletonBase(void) {};
	~singletonBase(void) {};
public:
	static T* getSingleton(void);
	void releaseSingleton(void);

};

//싱글톤 초기화
template<typename T>
T* singletonBase<T>::singleton = 0;

//싱글톤 값 가져온다
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	//싱글톤이 생성되지 않았다면 생성해줘라
	if (!singleton) singleton = new T;

	return singleton;
}

template <typename T>
void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;

		singleton = 0;
	}
}