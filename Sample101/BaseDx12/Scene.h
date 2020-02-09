#pragma once

class Scene :public SceneBase{
public:
	Scene():SceneBase(){}
	virtual ~Scene() {}
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
};

