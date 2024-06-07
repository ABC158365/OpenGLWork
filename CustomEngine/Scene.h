#pragma once
#include "Mesh.h"
#include "Camera.h"
class Scene
{
public:
	std::vector<Actor*> actors;
	std::vector<StaticMesh*> staticMeshes;
	std::vector<Mesh*>meshManager;


	inline void appendActors(Actor* actor) {
		actors.push_back(actor);
	}

	inline void appendMeshs(StaticMesh* m) {
		staticMeshes.push_back(m);
	}
	void updateBuffers();
	void draw(Camera* cam);
	void initializeMeshManager();

	Scene() {
		actors.clear();
		staticMeshes.clear();
	}
};


