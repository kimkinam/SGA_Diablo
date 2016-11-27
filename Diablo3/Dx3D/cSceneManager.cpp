#include "stdafx.h"
#include "cSceneManager.h"
#include "cSceneObject.h"

DWORD CALLBACK loadingThread(LPVOID prc)
{
	//��ü�� �� �ʱ�ȭ
	cSceneManager::_readyScene->SetUp();

	//���� ���� ��ü�� ������ �ٲ� �ְ�
	cSceneManager::_currentScene = cSceneManager::_readyScene;

	//�ε� ������ �״ϱ� �ε��� �����ְ�
	cSceneManager::_loadingScene->Release();
	//�ε��� �ΰ�
	cSceneManager::_loadingScene = NULL;
	//��ü������� ���� �ΰ�
	cSceneManager::_readyScene = NULL;

	//������� ���������� DWORD == int 
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
		//�ʿ� ����ִ� ���� NULL�� �ƴϸ� �� ������
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
	//���� ������ �ΰ� ��ȯWD
	if (!scene) return NULL;

	//���� Ű���� �Բ� �ʿ� ��´�
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

cSceneObject * cSceneManager::addLoadingScene(string loadingSceneName, cSceneObject * scene)
{
	//���� ������ �ΰ� ��ȯ
	if (!scene) return NULL;

	//���� Ű���� �Բ� �ʿ� ��´�
	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT cSceneManager::changeScene(string sceneName)
{
	//����ü
	mapSceneIter find = _mSceneList.find(sceneName);

	//�ش� ���� ��ã������ ����
	if (find == _mSceneList.end()) return E_FAIL;

	// ã�����ϴ� ���� ������̸� ������ �ٲ�����
	if (find->second == _currentScene) return S_OK;

	//�� �ʱ�ȭ �����ϸ�
	if (SUCCEEDED(find->second->SetUp()))
	{
		//�ִ� �� �����ְ�
		if (_currentScene) _currentScene->Release();
		// ������� ��ü�� ������
		_currentScene = find->second;
		return S_OK;
	}

	return E_FAIL;
}

HRESULT cSceneManager::changeScene(string sceneName, string loadingSceneName)
{
	//�� Ű �� ã�´�
	mapSceneIter find = _mSceneList.find(sceneName);

	//������ ����
	if (find == _mSceneList.end()) return E_FAIL;

	//������̸� �ȹٲ�
	if (find->second == _currentScene) return S_OK;

	//�ε� Ű��
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	//���� ���� �ε����̸� 
	if (find == _mLoadingSceneList.end()) return changeScene(loadingSceneName);

	//���� ���������� ��ü�ؼ� �ʱ�ȭ �����ϸ�
	if (SUCCEEDED(find->second->SetUp()))
	{
		//�ִ� ���� �����ְ�
		if (_currentScene) _currentScene->Release();

		_loadingScene = findLoading->second;

		_readyScene = find->second;


		//������ �ݾ��ش�.
		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadThreadID));
	}

	return E_FAIL;
}

