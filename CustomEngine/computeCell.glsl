#version 430

struct Data {
    vec3 position;
    int value;
};

struct Args{
    vec3 gridCenter;
    int gridRes;
    float gridSize;
};

layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer InputBuffer {
    Data inputData[];
};
layout(std430, binding = 1)buffer argBuffer{
   Args arg;  
};

vec2 worldSpacetoGridSpace(float x, float z, float gridSize){
    return vec2(x+gridSize/2.0, z+gridSize/2.0);
}

ivec2 gridSpaceToidx2d(vec2 gridPos){
    return ivec2(int(gridPos.x/arg.gridRes), int(gridPos.y/arg.gridRes));
}

int idxto1d(ivec2 idx2d){
    return idx2d.x + (idx2d.y * arg.gridRes);
}


void main() {
    uint index = gl_GlobalInvocationID.x;
    //inputData[index].value = int(gl_WorkGroupID.x);      // Double the value
    inputData[index].position.y = 5;

    vec3 pos = inputData[index].position;
    pos = pos -  arg.gridCenter;
    vec2 gridPos = worldSpacetoGridSpace(pos.x, pos.z, arg.gridSize);
    ivec2 gridXY = gridSpaceToidx2d(gridPos);
    int idx = idxto1d(gridXY);
    if(gridXY.x >= arg.gridRes || gridXY.y >=arg.gridRes ||
        gridPos.x < 0.0 || gridPos.y <0.0
    ){
        //inputData[index].value = 0;
    }else{
        //inputData[index].value = idx;
    }
    inputData[index].value = int(gl_GlobalInvocationID.x);
}