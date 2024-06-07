#version 430

struct Data {
    vec3 position;
    int value;
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer InputBuffer {
    Data inputData[];
};

void main() {
    uint index = gl_GlobalInvocationID.x;
    inputData[index].value = (int)index;      // Double the value
    inputData[index].position.y = 100;
}