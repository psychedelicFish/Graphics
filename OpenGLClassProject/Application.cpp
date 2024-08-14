#include "Application.h"
#include <iostream>
#include "Gizmos.h"
#include "Instance.h"
#include "Scene.h"
#include "imgui_glfw3.h"

Application* Application::instance = nullptr;

bool Application::Startup()
{
    //Intitialise glfw
    if (glfwInit() == false) {
        return false;
    }
    //Create a window
    window = glfwCreateWindow(windowWidth, windowHeight, "Application Thing", nullptr, nullptr);

    //make sure window was created
    if (window == nullptr) {
        glfwTerminate();
        return false;
    }
    //make the window our current context
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);

    aie::Gizmos::create(10000, 10000, 0, 0);

    aie::ImGui_Init(window, true); //init gui window

    //Set up our mouse position callback + we will set up our instance
    instance = this;
    glfwSetCursorPosCallback(window, &Application::SetMousePosition);
    


    quadTexture.load("./Textures/numbered_grid.tga");


    view = glm::lookAt(glm::vec3(10, 10,10), glm::vec3(0), glm::vec3(0,1,0));
    
    projection = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);

    normalPhongShader.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/normalPhong.vert");
    normalPhongShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shader/Fragment/normalPhong.frag");
    
    simplePhongShader.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/phong.vert");
    simplePhongShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shader/Fragment/phong.frag");

    phongNoTextureShader.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/phongNoTexture.vert");
    phongNoTextureShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shader/Fragment/phongNoTexture.frag");

    skyboxShader.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/simple.vert");
    skyboxShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shader/Fragment/simple.frag");

    if (normalPhongShader.link() == false) {
        printf("Shader Error: %s\n", normalPhongShader.getLastError());
        return false;
    }
    if (simplePhongShader.link() == false) {
        printf("Shader Error: %s\n", simplePhongShader.getLastError());
        return false;
    }
    if (phongNoTextureShader.link() == false) {
        printf("Shader Error: %s\n", phongNoTextureShader.getLastError());
        return false;
    }
    if (skyboxShader.link() == false) {
        printf("Shader Error: %s\n", skyboxShader.getLastError());
        return false;
    }

    vertexOnlyMesh.IntialiseQuad();
    vertexOnlyMesh.CreateMaterial(glm::vec3(1), glm::vec3(1), glm::vec3(1), "./Textures/Grass2.tga");
    //quad transform - Make it 10 units wide & high
    glm::mat4 quadTransform = { 10, 0, 0, 0,
                      0, 10, 0, 0,
                      0, 0, 10, 0,
                      0, 0,  0, 1 };

   
   
    bunnyMesh.InitialiseFromFile("./Models/Bunny.obj");
    bunnyMesh.LoadMaterial("./Models/Bunny.mtl");

    glm::mat4 bunnyTransform = { 0.5,0,0,0,
                                 0,0.5,0,0,
                                 0,0,0.5,0,
                                 0,0,0,1 };

    


    std::vector<std::string> faces{
    "./Textures/stormydays_ft.tga",
    "./Textures/stormydays_bk.tga",
    "./Textures/stormydays_up.tga",
    "./Textures/stormydays_dn.tga",
    "./Textures/stormydays_rt.tga",
    "./Textures/stormydays_lf.tga",
    };

    skybox.IntialiseCubeMap();
    cubemapTexture = skybox.LoadCubeMap(faces);
    Light light(glm::normalize(glm::vec3(-1)), glm::vec3{ 1, 1, 1 } , 1);
    ambientLight = { 0.25,0.25,0.25 };

    activeScene = new Scene(&camera, glm::vec2(windowWidth, windowHeight), light, ambientLight, cubemapTexture);
    
    activeScene->AddLight(new Light(glm::vec3{ 5,3,0 }, glm::vec3{ 1,0,0 }, 50));
    activeScene->AddLight(new Light(glm::vec3{ -5,3,0 }, glm::vec3{ 0,1,0 }, 50));

    soulSpear.InitialiseFromFile("./Models/soulspear.obj");
    soulSpear.LoadMaterial("./Models/soulspear.mtl");
    
    //activeScene->AddInstance(new Instance(glm::vec3(0), glm::vec3(0), glm::vec3(100, 0, 100), &vertexOnlyMesh, &simplePhongShader));
    activeScene->AddInstance(new Instance(glm::vec3(0), glm::vec3(0, 90, 0), glm::vec3(0.8f), &soulSpear, &normalPhongShader));
    



  
    //activeScene->AddInstance(new Instance(bunnyTransform, &bunnyMesh, &phongNoTextureShader));
    return true;
}

bool Application::Update()
{
    //time since application started 
    float time = glfwGetTime();

    activeScene->Update();

    camera.Update(0.1f, window);

    lastMousePosition = mousePosition;

    if (glfwWindowShouldClose(window) == true || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return false;
    }
    return true;
}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));
    //Everything goes in here related to drawing

    glm::vec4 white(1);
    glm::vec4 black(0, 0, 0, 1);

    for (int i = 0; i < 21; i++) {
        aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
        aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
    }

    aie::Gizmos::draw(activeScene->GetProjectionView());

    skyboxShader.bind();
    skyboxShader.bindUniform("P", activeScene->GetProjection());
    skyboxShader.bindUniform("V", activeScene->GetView());
    
    //skyboxShader.bindUniform("skybox", (int)cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    //
    skybox.Draw();
    glDepthMask(true);

    activeScene->Draw();

   

    glfwSwapBuffers(window);
    glfwPollEvents();

}

void Application::Shutdown()
{
    aie::ImGui_Shutdown();

    aie::Gizmos::destroy();

    delete activeScene;
  
    glfwDestroyWindow(window);
    glfwTerminate();

}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    instance->mousePosition.x = (float)x;
    instance->mousePosition.y = (float)y;
}
