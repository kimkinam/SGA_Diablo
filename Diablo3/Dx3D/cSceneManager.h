#pragma once
#include <string>	
#include <map>	

class cSceneObject;

#define g_pSceneManager cSceneManager::GetInstance()

class cSceneManager
{
private:
	SINGLETONE(cSceneManager);

public:
	typedef map<string, cSceneObject*> mapSceneList;
	typedef map<string, cSceneObject*>::iterator mapSceneIter;

private:
	static cSceneObject* _currentScene;
	static cSceneObject* _loadingScene;
	static cSceneObject* _readyScene;

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadThreadID;

public:
	void Destroy();
	void Update();
	void Render();

	//씬 추가 함수
	cSceneObject* addScene(string sceneName, cSceneObject* scene);

	//로딩 씬 추가함수 (아마 안쓸것)
	cSceneObject* addLoadingScene(string loadingSceneName, cSceneObject* scene);

	//씬 교환 함수
	HRESULT changeScene(string sceneName);
	//로딩씬과 씬 교환 함수 (아마 쓰지 않을 것)
	HRESULT changeScene(string sceneName, string loadingSceneName);

	//로딩씬에서 사용할 쓰레드지만 사용하지 않을 것. 편하게 쓰라고 friend 
	friend DWORD CALLBACK loadingThread(LPVOID prc);

	cSceneObject* GetCurScene() { return _currentScene; }

};

