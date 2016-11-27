#include "stdafx.h"
#include "cSceneManager.h"
#include "cSceneObject.h"

DWORD CALLBACK loadingThread(LPVOID prc)
{
	//교체될 씬 초기화
	cSceneManager::_readyScene->SetUp();

	//현재 씬을 교체될 씬으로 바꿔 주고
	cSceneManager::_currentScene = cSceneManager::_readyScene;

	//로딩 끝났을 테니까 로딩씬 날려주고
	cSceneManager::_loadingScene->Release();
	//로딩씬 널값
	cSceneManager::_loadingScene = NULL;
	//교체대기중인 씬도 널값
	cSceneManager::_readyScene = NULL;

	//쓰레드는 관습적으로 DWORD == int 
	return 0;
}

cSceneObject* cSceneManager::_currentScene = NULL;
cSceneObject* cSceneManager::_loadingScene = NULL;
cSceneObject* cSceneManager::_readyScene = NULL;


cSceneManager::cSceneManager()
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;
}


cSceneManager::~cSceneManager()
{
}

void cSceneManager::Destroy()
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		//맵에 담겨있는 씬이 NULL이 아니면 즉 있으면
		if (miSceneList->second != NULL)
		{
			//miSceneList->second->Release();

			if (miSceneList->second == _currentScene) miSceneList->second->Release();
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else
		{
			++miSceneList;
		}
	}

	_mSceneList.clear();
}

void cSceneManager::Update()
{
	if (_currentScene) _currentScene->Update();
}

void cSceneManager::Render()
{
	if (_currentScene) _currentScene->Render();
}

cSceneObject * cSceneManager::addScene(string sceneName, cSceneObject * scene)
{
	//씬이 없으면 널값 반환WD
	if (!scene) return NULL;

	//씬을 키값과 함께 맵에 담는다
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

cSceneObject * cSceneManager::addLoadingScene(string loadingSceneName, cSceneObject * scene)
{
	//씬이 없으면 널값 반환
	if (!scene) return NULL;

	//씬을 키값과 함께 맵에 담는다
	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT cSceneManager::changeScene(string sceneName)
{
	//씬교체
	mapSceneIter find = _mSceneList.find(sceneName);

	//해당 씬을 못찾았으면 실패
	if (find == _mSceneList.end()) return E_FAIL;

	// 찾으려하는 씬이 현재씬이면 오케이 바꾸지마
	if (find->second == _currentScene) return S_OK;

	//씬 초기화 성공하면
	if (SUCCEEDED(find->second->SetUp()))
	{
		//있던 씬 날려주고
		if (_currentScene) _currentScene->Release();
		// 현재씬은 교체될 씬으로
		_currentScene = find->second;
		return S_OK;
	}

	return E_FAIL;
}

HRESULT cSceneManager::changeScene(string sceneName, string loadingSceneName)
{
	//씬 키 값 찾는다
	mapSceneIter find = _mSceneList.find(sceneName);

	//없으면 실패
	if (find == _mSceneList.end()) return E_FAIL;

	//현재씬이면 안바꿈
	if (find->second == _currentScene) return S_OK;

	//로딩 키값
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	//현재 씬이 로딩씬이면 
	if (find == _mLoadingSceneList.end()) return changeScene(loadingSceneName);

	//씬을 성공적으로 교체해서 초기화 성공하면
	if (SUCCEEDED(find->second->SetUp()))
	{
		//있던 씬을 날려주고
		if (_currentScene) _currentScene->Release();

		_loadingScene = findLoading->second;

		_readyScene = find->second;


		//쓰레드 닫아준다.
		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadThreadID));
	}

	return E_FAIL;
}

