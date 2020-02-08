#include "stdafx.h"
#include "Scene.h"
#include "GameStage.h"

void Scene::OnInit(){
	auto ptr = ResetActiveStage<GameStage>();
}
void Scene::OnUpdate() {
	GetActiveStage<GameStage>()->OnUpdate();
}
void Scene::OnRender() {
	GetActiveStage<GameStage>()->OnRender();

}
void Scene::OnDestroy() {
	GetActiveStage<GameStage>()->OnDestroy();
}

