#include "Scene.h"

void Scene::updateBuffers() {
	for (auto actor : actors) {
		actor->updateBuffer();
	}
}

void Scene::draw(Camera* cam) {
	for (auto actor : actors) {
		actor->draw(cam);
	}
}

void Scene::initializeMeshManager() {
	for (auto actor : actors) {
		actor->appendMeshManager(meshManager);
	 
	}

	for (auto m : staticMeshes) {
		meshManager.push_back(m);
	}
}
