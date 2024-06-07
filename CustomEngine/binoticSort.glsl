#version 430

struct Data {
    vec3 position;
    int value;
};

// Sort 16 sortable elements
//#define N_ELEMENTS 8

layout(local_size_x =  512 , local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer InputBuffer {
    Data inputData[];
};

uniform int count;

// Workgroup local memory. We use this to minimise round-trips to global memory.
// Allows us to evaluate a sorting network of up to 1024 with a single shader invocation.
shared Data local_value[1024];

void local_compare_and_swap(ivec2 idx){
	//local_value[idx.x].value > local_value[idx.y].value
	if (local_value[idx.x].position.x*64 + local_value[idx.x].position.z
	>local_value[idx.y].position.x*64 +local_value[idx.y].position.z){
		Data tmp = local_value[idx.x];
		local_value[idx.x] = local_value[idx.y];
		local_value[idx.y] = tmp;
	}
}

void local_swap(ivec2 idx){
		Data tmp = local_value[idx.x];
		local_value[idx.x] = local_value[idx.y];
		local_value[idx.y] = tmp;
}

void global_compare_and_swap(ivec2 idx){
	if (inputData[idx.x].value < inputData[idx.y].value ) {
		Data tmp = inputData[idx.x];
		//inputData[idx.x] = tmp;
		inputData[idx.y] = tmp;
	}
}


void do_flip(int h){
	uint t = gl_LocalInvocationID.x;
	int q = (int(t*2)/h)* h;

	ivec2 indices;
	if(int(((2*t)%h)/(h/2))>=1){
		int offset = int(t%(h/4));
		indices = q+ ivec2(offset+h/2, h-offset-1);
		local_swap(indices);
	}
	barrier();
	
}


void do_disperse(int h){
	uint t = gl_LocalInvocationID.x;
	int q = (int(2*t)/h) * h;
	ivec2 indices = q + ivec2( t % (h/2), (t % (h/2)) + (h / 2) );
	local_compare_and_swap(indices);
	//global_compare_and_swap(indices);
}




void main(){
	uint t = gl_LocalInvocationID.x;

	// Each local worker must save two elements to local memory, as there
	// are twice as many elments as workers.
	local_value[t*2]   = inputData[t*2];
	local_value[t*2+1] = inputData[t*2+1];
 
 	int n = count;
	do_disperse(2);
	barrier();
	
	for ( int h = 4; h <=n; h *= 2 ) {
		barrier();
		do_flip(h);
		for (int hh = h ; hh > 1 ; hh /= 2 ) {
			barrier();
			do_disperse(hh);			
		}
	}
	barrier();

	// Write local memory back to buffer
	inputData[t*2]   = local_value[t*2];
	inputData[t*2+1] = local_value[t*2+1];
	//inputData[gl_GlobalInvocationID.x].value = int(gl_LocalInvocationID.x);
}