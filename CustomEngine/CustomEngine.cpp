// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
//Esc退出

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp



//OpenGL redirector
#include <glad/glad.h>
//UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//C++ standard lib
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
//GLFW winodw
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

//My Utilityies
#include "Camera.h"
#include "Shaders.h"
#include "Material.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CQuaternion.h"
#include "StageInfo.h"
#include "Cube.h"
#include "Scene.h"
#include "PostProcessing.h"
#include "BufferManager.h"

void processInput(GLFWwindow* window, Camera* camera);
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


//void readAfterBindTex(const char* file, bool isRGB, bool flip = true);
//
//void draw1(Cube& cube, Camera& camera);

Camera* camera00 = new Camera(0.01, Cmatrix::perspective(Cmath::radians(45.0f), 1080.0 / 720.0f, 0.1f, 100.0f));
Camera* camera01 = new Camera(0.01, Cmatrix::perspective(Cmath::radians(45.0f), 1080.0 / 720.0f, 0.1f, 100.0f));

Camera* currentCam = camera00;



// Main code
int main(int, char**)
{
    //return 0;

    
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 440";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only //要写这个！！！
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.05f, 0.05f, 0.00f, 1.00f);



    
    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN


#else
    //检查GLAD是否正确处理引导OPGL函数的指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //InstancedCube cube("./vertex1.glsl", "./fragment.glsl");
    GridPlane plane("./debugGridv.glsl", "./debugGridf.glsl");
    RegionA regionA;
    RegionB1 regionB1;
    RegionB2 regionB2;
    RegionC regionC;
    uint32_t mid = Material::registerMaterial("./stageCubeV.glsl", "./stageCubeF.glsl");
    regionA.mat = mid;
    regionB1.mat = mid;
    regionB2.mat = mid;
    regionC.mat = mid;
    regionA.SpawnMember();
    regionB1.SpawnMember();
    regionB2.SpawnMember();
    regionC.SpawnMember();
    TextureManager UTextureManager;
    std::vector<std::string> tpaths;
    tpaths.push_back("./texture/A.jpg");
 
    UTextureManager.upLoadTexture("regionA", tpaths);
    tpaths.clear();
    tpaths.push_back("./test.jpg");
    UTextureManager.upLoadTexture("regionB", tpaths);

    tpaths.clear();
    tpaths.push_back("./texture/C1_1.jpg");
    tpaths.push_back("./texture/C1_2.jpg");
    tpaths.push_back("./texture/C1_3.jpg");
    tpaths.push_back("./texture/C1_4.jpg");
    UTextureManager.upLoadTexture("regionC", tpaths);

    tpaths.clear();
    tpaths.push_back("./test.jpg");
    tpaths.push_back("./texture/B1_1L.jpg");
    tpaths.push_back("./texture/B1_2L.jpg");
    tpaths.push_back("./texture/B1_3L.jpg");
    tpaths.push_back("./texture/B1_4L.jpg");
    tpaths.push_back("./texture/B1_5L.jpg");
    tpaths.push_back("./texture/B1_6L.jpg");
    tpaths.push_back("./texture/B1_7L.jpg");
    tpaths.push_back("./texture/B1_8L.jpg");
    tpaths.push_back("./texture/B1_9L.jpg");
    tpaths.push_back("./texture/B1_1R.jpg");
    tpaths.push_back("./texture/B1_2R.jpg");
    tpaths.push_back("./texture/B1_3R.jpg");
    tpaths.push_back("./texture/B1_4R.jpg");
    tpaths.push_back("./texture/B1_5R.jpg");
    tpaths.push_back("./texture/B1_6R.jpg");
    tpaths.push_back("./texture/B1_7R.jpg");
    tpaths.push_back("./texture/B1_8R.jpg");
    tpaths.push_back("./texture/B1_9R.jpg");
    UTextureManager.upLoadTexture("regionB1", tpaths);

    float r = 5;
    Scene scene;
    for (int i = 0; i < 12; i++) {
        Actor* a = new Actor(mid);
        a->setTranslate(Cvector::CVector(cos(Cmath::radians(30 * i))*r , 5.5f, sin(Cmath::radians(30 * i))*r+5));
        a->setRotation(CRotator::CRotator(0, Cmath::radians(-30 * i), 0));
        a->name = "actor" + std::to_string(i);
        scene.appendActors(a);

        
    }
    

    for (auto m : regionA.meshes) {
        m->parent = "RegionA";
        scene.appendMeshs(m);
        m->updateBuffers(true);
    }
    for (auto m : regionB1.meshes) {
        m->parent = "RegionB1";
        scene.appendMeshs(m);
        m->updateBuffers(true);
    }
    for (auto m : regionB2.meshes) {
        m->parent = "RegionB2";
        scene.appendMeshs(m);
        m->updateBuffers(true);
    }
    for (auto m : regionC.meshes) {
        m->parent = "RegionC";
        scene.appendMeshs(m);
        m->updateBuffers(true);

    }

    scene.initializeMeshManager();
    BufferManager buffManager(window);
    ColorAttachment* renderBuffer = buffManager.getAlbedo();
    PostProcessing postRender;



    Mesh* mesh = new Mesh[10];
    //------------ debug ------------------------

    //---------------------------------------------


    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

    glViewport(0, 0, 1280, 720);
    //glEnable(GL_BLEND); //开混合模式贴图
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    
    float currenttime = 0, pretime = 0;
    int count=0;
    int n = 2;

    //stateControl
    bool key1 = false;
    bool camState = false;
    bool keyf2 = false;
    bool keyf3 = false;
    bool key3 = false;
    bool key4 = false;

    bool key7 = false;
    bool key7toggle = false;

    bool key8 = false;
    bool key8toggle = false;

    bool sActor = false;

    bool move3=false;
    bool move4 = false;
    float timer = 0;;
    bool selected = false;
    float hde=0, vde=0;

    bool activateB;
    bool activateCT;
    bool activateCR;
    Mesh* currentMesh = nullptr;

    float lastX;
    float lastY;
   
    lastX = 0;
    lastY = 0;
    while (!glfwWindowShouldClose(window))
#endif
    {
        count++;
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Console infomration");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("TranslationSpeed:%f", currentCam->ts);   
            ImGui::Text("RotationSpeed:%f", currentCam->rs);                // Display some text (you can use a format strings too)

            ImGui::Text("yaw:%f", currentCam ->yaw);
            ImGui::Text("roll:%f", currentCam ->roll);
            ImGui::Text("pitch:%f", currentCam ->pitch);

            
            if (currentCam->disableCameraMovement == true) {
                ImGui::Text("Manual CameraMovement:disable");
            }
            else {
                ImGui::Text("Manual CameraMovement:enable");
            }
            
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //ImGui::SliderInt("ChangeType", &cube.hueshift, 0, 1);

           

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        auto io = ImGui::GetIO();
        //处理相机输入
        //--legacy--
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS) {
            currentCam->ProcessMouseMovement(io.MousePos.x, io.MousePos.y);
        }
        else {
            currentCam->xpos = io.MousePos.x;
            currentCam->ypos = io.MousePos.y;
        }


        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClearColor(0.0, 0.0, 0.0, 0.0);


        pretime = currenttime;
        currenttime = glfwGetTime();

        //rendering images
        //
        //----------------------Framebuffer draw-----------------
        glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer->FBO);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glEnable(GL_STENCIL_TEST);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
        
        
        glStencilMask(0x00);
        plane.draw(currentCam);
        
        //cube.draw(&camera);
        

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            key1 = true;
        }
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE&& key1 == true){
            Cvector::CVector start, dir;
            currentCam->GetCamStartAndDir(start, dir, io.MousePos.x, io.MousePos.y);
            if (currentMesh)currentMesh->selected = false;
            currentMesh = nullptr;
            HitResult result;
            dir = dir * 1000;
            for (auto m : scene.meshManager) {
                HitResult tresult;
                tresult = m->checkIntersection(start, start + dir);
                if (!tresult.isHit)continue;
                if (tresult.distance < result.distance) {
                    result = tresult;
                }
                
            }
            if (result.isHit == true)currentMesh =  result.mesh;
            key1 = false;

        }
            processInput(window, currentCam);
            currentCam->update(currenttime - pretime);
            for (auto m : scene.meshManager) {
                m->selected = false;
            }
            

        //键位输入
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
       



        float offy = lastY - io.MousePos.y;
        float offx = io.MousePos.x - lastX;
        lastX = io.MousePos.x;
        lastY = io.MousePos.y;

        if(currentMesh) currentMesh->selected = true;
        if (currentMesh) {
            if (currentMesh->parent == "RegionC") {
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
                    StaticMesh* temp = dynamic_cast<StaticMesh*>(currentMesh);
                    if (temp) {
                        currentMesh->rotation.Yaw += 0.1;
                        temp->updateBuffers(true);
                    }
                }
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                    StaticMesh* temp = dynamic_cast<StaticMesh*>(currentMesh);
                    if (temp) {
                        currentMesh->translate = currentMesh->translate + Cvector::CVector(-offx*0.02, 0.0, 0.0);
                        temp->updateBuffers(true);
                    }
                }
            }
            if (currentMesh->parent == "RegionB1") {
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                    StaticMesh* temp = dynamic_cast<StaticMesh*>(currentMesh);
                    if (temp) {
                        currentMesh->translate = currentMesh->translate + Cvector::CVector(0.0, offy*0.02, 0.0);
                        temp->updateBuffers(true);
                    }
                }
            }
            if (currentMesh->parent == "Actor") {
                SkeletalMesh* temp = dynamic_cast<SkeletalMesh*>(currentMesh);
                temp->root->markSelect();
                sActor = true;
            }
            else {
                sActor = false;
            }
       }
        //save module

        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
            keyf2 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE && keyf2) {
            keyf2 = false;
            std::ofstream outputFile("Scene.txt");
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open the file." << std::endl;
                return 1;
            }
            for (int i = 0; i < scene.meshManager.size(); i++) {
                outputFile << i << " " << scene.meshManager[i]->scale.x << ',' << scene.meshManager[i]->scale.y << ',' << scene.meshManager[i]->scale.z << " " <<
                    scene.meshManager[i]->translate.x << ',' << scene.meshManager[i]->translate.y << ',' << scene. meshManager[i]->translate.z << " " <<
                    scene.meshManager[i]->rotation.Yaw << "\n";
                
            }
            outputFile.close();
            std::cout << "save done.";
        }

        if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
            keyf3 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE && keyf3) {
            keyf3 = false;
            std::ifstream inputFile("Scene.txt");

            if (!inputFile.is_open()) {
                std::cout << "无法打开文件" << std::endl;
            }
            else {
                int index;
                Cvector::CVector scale;
                Cvector::CVector translate;
                float yaw;
                std::string line;
                while (getline(inputFile, line)) {
                    std::stringstream ss(line);
                    ss >> index;
                    if (ss.fail()) {
                        std::cerr << "Error parsing int value." << std::endl;
                        return 1;
                    }
                    char comma;
                    ss >> scale.x >> comma >> scale.y >> comma >> scale.z;
                    ss >> translate.x >> comma >> translate.y >> comma >> translate.z;
                    ss >> yaw;

                    if (index >= scene.meshManager.size())std::cout << "warning::invalid scene file";
                    else {
    
                        scene.meshManager[index]->rotation.Yaw = yaw;
                        scene.meshManager[index]->scale = scale;
                        scene.meshManager[index]->translate = translate;
                        StaticMesh* temp = dynamic_cast<StaticMesh*>(scene.meshManager[index]);
                        temp->updateBuffers(true);
                    }
                }
            }
            std::cout << "read done.";
        }

        
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            key3 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && key3) {
            move3 = !move3;
            key3 = false;
        }
        if (move3) {
            for (auto m : regionB1.meshes) {
                m->translate = m->abstranslate + Cvector::CVector(0, std::sin(glfwGetTime()+m->abstranslate.x+ m->abstranslate.z), 0);
                StaticMesh* temp = dynamic_cast<StaticMesh*>(m);
                if (temp) {
                    temp->updateBuffers(true);
                }
            }
        }


        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            key4 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE && key4) {
            move4 = !move4;
            key4 = false;
        }
        if (move4) {
            for (auto m : regionC.meshes) {
                m->translate = m->abstranslate + Cvector::CVector(std::sin(glfwGetTime()), 0.0, 0.0);
                m->rotation.Yaw = glfwGetTime()*0.1;
                StaticMesh* temp = dynamic_cast<StaticMesh*>(m);
                if (temp) {
                    temp->updateBuffers(true);
                }
            }
        }
        if (count % 50 == 0) {
            count = 0;
            for (auto m : scene.meshManager) {
                SkeletalMesh* temp = dynamic_cast<SkeletalMesh*>(m);
                if (temp) {
                    for (auto other : scene.staticMeshes) {
                        temp->intersectionEvent(other);
                    }
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
            key8 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_RELEASE && key8) {
            key8toggle = !key8toggle;
            key8 = false;
            if (key8toggle && sActor)
            {
                SkeletalMesh* temp = dynamic_cast<SkeletalMesh*>(currentMesh);
                CRotator::CRotator r = temp->root->getRotator();
                camera01->yaw = r.Yaw;
                camera01->roll = r.Roll;
                camera01->pitch = r.Pitch;
                camera01->Position = temp->root->getTranslate() + Cvector::CVector(0, 4, 0);
                camera01->targetPos = temp->root->getTranslate() + Cvector::CVector(0, 4, 0);
                currentCam = camera01;
                key8 = false;

            }
            else {
                currentCam = camera00;
            }
        }

        if (key8toggle && sActor) {
            SkeletalMesh* temp = dynamic_cast<SkeletalMesh*>(currentMesh);
            camera01->Position = temp->root->getTranslate() + Cvector::CVector(0, 4, 0);
            camera01->targetPos = temp->root->getTranslate() + Cvector::CVector(0, 4, 0);
        }

      
  

        //glUseProgram(shaderProgram1);
        //glBindVertexArray(VAO[1]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glStencilFunc(GL_ALWAYS, 255, 255);
        UTextureManager.BindTextures("regionA");
        regionA.drawmeshes(currentCam);
        UTextureManager.BindTextures("regionB1");
        regionB1.drawmeshes(currentCam);
       
        regionB2.drawmeshes(currentCam);
        UTextureManager.BindTextures("regionC");
        regionC.drawmeshes(currentCam);

        //actor00.setRotation(CRotator::CRotator(0.0, glfwGetTime(), 0.0));
        //actor00.updateBuffer();
        //actor00.draw(&camera);
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            key7 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_RELEASE && key7) {
            key7 = false;
            key7toggle = !key7toggle;
        }

        if(key7toggle)
        {   
            timer += io.DeltaTime;
            r = 3 * (sin(timer) + 2);
            int i = 0;
            for (auto a : scene.actors) {
                a->leftLeg->rotation = CRotator::CRotator(sin(glfwGetTime() * 2), 0, 0);
                a->rightLeg->rotation = CRotator::CRotator(-sin(glfwGetTime() * 2), 0, 0);
                a->leftArm->rotation = CRotator::CRotator(-sin(glfwGetTime() * 2), 0, 0);
                a->rightArm->rotation = CRotator::CRotator(sin(glfwGetTime() * 2), 0, 0);
                a->setTranslate(Cvector::CVector(cos(Cmath::radians(30 * i)) * r, 5.f, sin(Cmath::radians(30 * i)) * r + 5));
                i++;
            }
        }


        scene.updateBuffers();
        scene.draw(currentCam);
  

        glStencilMask(0xFF); //最后记得打开，因为clear需要清零
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        postRender.render(renderBuffer->tex, renderBuffer->customTex, renderBuffer->stencilTex);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void processInput(GLFWwindow* window, Camera* camera)
{

    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->ProcessKeyboard(FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->ProcessKeyboard(BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->ProcessKeyboard(RIGHT);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->ProcessKeyboard(LEFT);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera->ProcessKeyboard(DOWN);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera->ProcessKeyboard(UP);
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
            camera->ProcessKeyboard(TSPEEDUP);
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
            camera->ProcessKeyboard(TSPEEDDOWN);
        if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
            camera->ProcessKeyboard(RSPEEDUP);
        if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
            camera->ProcessKeyboard(RSPEEDDOWN);

    }
}

