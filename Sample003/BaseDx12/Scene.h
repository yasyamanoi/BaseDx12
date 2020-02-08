#pragma once

class Scene :public SceneBase{
public:
	Scene():SceneBase(){}
	virtual ~Scene() {}
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
	virtual void OnKeyDown(UINT8 /*key*/);
	virtual void OnKeyUp(UINT8 /*key*/);
};

