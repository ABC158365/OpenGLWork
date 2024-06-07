#pragma once
 
#include <glad/glad.h>
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>

struct Agent {
	glm::vec3 location;
	int cellID;
};

struct Args {
	glm::vec3 gridCenter;
	int gridRes;
	float gridSize;
	
};

struct SimulationParams {
	float gridWidth;
};

class Agents
{
private:
	std::vector<Agent> agents;
	uint32_t computeCellProgram;
	uint32_t binoticSort;

public:
	Agents() {
		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 32; j++) {
				agents.push_back(Agent{ glm::vec3(i * 1.1, 0, j * 1.1), -1 });
			}
		}
		std::random_device rd;
		std::default_random_engine rng(rd());
		std::shuffle(agents.begin(), agents.end(), rng);

		compileComputeShaders();
	}

	std::vector<Agent>& getAgents() {
		return agents;
	}


	void compileComputeShaders();

	void computeCells(int iteration) {
		uint32_t inputBuffer[2];
		glGenBuffers(2, inputBuffer);
		// Bind the buffer object to the binding point
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer[0]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Agent)*agents.size(), agents.data(), GL_DYNAMIC_COPY);
		
		Args arg;
		arg.gridCenter = glm::vec3(25.0, 0.0, 25.0);
		arg.gridRes = 5;
		arg.gridSize = 50;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, inputBuffer[1]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Args), &arg, GL_DYNAMIC_COPY);
		
		
		glUseProgram(computeCellProgram);

	
		glDispatchCompute(32, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


		glUseProgram(binoticSort);
		
		glUniform1i(glGetUniformLocation(binoticSort, "count"), iteration);
		glDispatchCompute(2, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		// Retrieve the results
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer[0]);
		Agent* resultData = (Agent*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		agents.assign(resultData, resultData + 1024);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glDeleteBuffers(2, inputBuffer);
	}

};
