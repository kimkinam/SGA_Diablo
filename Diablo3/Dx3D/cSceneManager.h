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

	//�� �߰� �Լ�
	cSceneObject* addScene(string sceneName, cSceneObject* scene);

	//�ε� �� �߰��Լ� (�Ƹ� �Ⱦ���)
	cSceneObject* addLoadingScene(string loadingSceneName, cSceneObject* scene);

	//�� ��ȯ �Լ�
	HRESULT changeScene(string sceneName);
	//�ε����� �� ��ȯ �Լ� (�Ƹ� ���� ���� ��)
	HRESULT changeScene(string sceneName, string loadingSceneName);

	//�ε������� ����� ���������� ������� ���� ��. ���ϰ� ����� friend 
	friend DWORD CALLBACK loadingThread(LPVOID prc);

	cSceneObject* GetCurScene() { return _currentScene; }

};

