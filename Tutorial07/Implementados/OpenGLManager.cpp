#include "Encabezados/OpenGLManager.h"
#include "Encabezados/CShader.h"
#include <iostream>
#include"imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "../Encabezados/stb_image.h"


CCamera* g_CurrentCamera;
CCamera* g_SecondCamera;


bool g_WhichCamera = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
}

void OpenGLManager::InitDevice() {

    
    m_RenderTarget.InitFrameBuffer();

  
    m_CSceneManager.Init();


    m_GraphicApi.ChargeMesh("EscenaDelMaestro/Model/Scene/Scene.fbx", m_GraphicApi.m_Model, &m_CSceneManager);

    m_programShaderID = CShader::LoadShaders("GBufferVS.glsl", "GBufferPS.glsl");
    glUseProgram(m_programShaderID);

    m_Buffers.OpenGLVAO();


    GameLoop();

    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


HRESULT OpenGLManager::FrameBuffer() {

    glGenFramebuffers(1, &m_frameBufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);

 

    glGenTextures(1, &renderedTexture);

    glBindTexture(GL_TEXTURE_2D, renderedTexture);

 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

   
    glDrawBuffers(1, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return false;
}


void OpenGLManager::processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void CreateInputCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {

    g_CurrentCamera->inputs(key);
}

void MouseButtonCallBack(GLFWwindow* _window, int _button, int _action, int _mods) {

    if (_button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_PRESS) {

        g_CurrentCamera->mClickPressed = true;

        POINT mousePos;

        GetCursorPos(&mousePos);
        g_CurrentCamera->SetOriginalMousePos(mousePos.x, mousePos.y);
    }
    else if (_button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_RELEASE) {

        g_CurrentCamera->mClickPressed = false;
    }
}

int OpenGLManager::WindowGLFW() {


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);

    if (m_window == NULL) {

        std::cout << "Error al crear una ventana en GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

 
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    InitCameras();
    InitDevice();

    return 0;
}


void OpenGLManager::GameLoop() {

    float color1 = 0;
    float color2 = 0;
    float color3 = 0;
    FrameBuffer();
    BillBoard();

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(m_window, true);

 
    ImGui::StyleColorsDark();

 
    glfwSetKeyCallback(m_window, CreateInputCallBack);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallBack);

    while (!glfwWindowShouldClose(m_window)) {

        color1 += 0.02f;
        color2 = cos(color1);
        color3 = sin(color1);

        UpdateBillBoard();

        ImGui_ImplGlfwGL3_NewFrame();
        render1();
        render2();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

 
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void OpenGLManager::InitCameras()
{
    CCameraDescriptor FirstCamera;
    FirstCamera.dAt = { 0,0,0 };
    FirstCamera.dEye = { 0,0,20 };
    FirstCamera.dUp = { 0,1,0 };
    FirstCamera.dFar = 1000;
    FirstCamera.dNear = 0.01;

#ifdef D3D11
    FirstCamera.dFoV = XM_PIDIV4;
#endif

    FirstCamera.dHeight = 800;
    FirstCamera.dWidht = 800;

    m_FreeCamera.Init(FirstCamera);
    m_FPSCamera.Init(FirstCamera);

    g_CurrentCamera = &m_FPSCamera;
    g_SecondCamera = &m_FreeCamera;
}

void OpenGLManager::BillBoard() {

    glGenTextures(1, &m_TextureBillboard);
    glBindTexture(GL_TEXTURE_2D, m_TextureBillboard);

 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Billboard.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    glm::vec3 upWordSpace = g_CurrentCamera->GetMUp();
    glm::vec3 RoghtWordSpace = g_CurrentCamera->GetMRight();
    glm::vec3 pos;
    SimpleVertex billboard[4];
    billboard[0].Pos = { -0.01f, -0.01f, 0.0f };
    pos = billboard[0].Pos;
    billboard[0].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[0].Tex = { -1,1 };

    billboard[1].Pos = { 0.01f, -0.01f, 0.0f };
    pos = billboard[1].Pos;
    billboard[1].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[1].Tex = { 1, 1 };

    billboard[2].Pos = { -0.01f, 0.01f, 0.0f };
    pos = billboard[2].Pos;
    billboard[2].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[2].Tex = { -1, -1 };

    billboard[3].Pos = { 0.01f, 0.01f, 0.0f };
    pos = billboard[3].Pos;
    billboard[3].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[3].Tex = { 1, -1 };

    glGenBuffers(1, &m_vertexBufferBillBoard);

 
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);


    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * 4, &billboard, GL_STATIC_DRAW);

    WORD indexBuffer[] = { 0,1,2,1,3,2 };

    glGenBuffers(1, &m_IndexBufferBillBoard);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferBillBoard);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(WORD), &indexBuffer[0], GL_STATIC_DRAW);
}

void OpenGLManager::UpdateBillBoard()
{

    glm::vec3 upWordSpace = g_CurrentCamera->GetMUp();
    glm::vec3 RoghtWordSpace = g_CurrentCamera->GetMRight();
    glm::vec3 pos;
    SimpleVertex billboard[4];
    billboard[0].Pos = { -0.5f, -0.5f, 0.0f };
    pos = billboard[0].Pos;
    billboard[0].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[0].Tex = { 0,0 };

    billboard[1].Pos = { 0.5f, -0.5f, 0.0f };
    pos = billboard[1].Pos;
    billboard[1].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[1].Tex = { 1, 0 };

    billboard[2].Pos = { -0.5f, 0.5f, 0.0f };
    pos = billboard[2].Pos;
    billboard[2].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[2].Tex = { 0, 1 };

    billboard[3].Pos = { 0.5f, 0.5f, 0.0f };
    pos = billboard[3].Pos;
    billboard[3].Pos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[3].Tex = { 1, 1 };

 
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);


    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * 4, &billboard, GL_STATIC_DRAW);
}

void OpenGLManager::render1()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);
    glUseProgram(m_programShaderID);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    glm::mat4x4 word = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    glm::mat4x4 view = g_SecondCamera->GetView();
    view = glm::transpose(view);
    glm::mat4x4 projection = g_SecondCamera->GetProjection();
    projection = glm::transpose(projection);


    GLuint MatrixID = glGetUniformLocation(m_programShaderID, "WordMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &word[0].x);
 
    MatrixID = glGetUniformLocation(m_programShaderID, "ViewMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0].x);

 
    MatrixID = glGetUniformLocation(m_programShaderID, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0].x);

    for (int i = 0; i < m_CSceneManager.m_MeshInScene.size(); i++) {


        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_CSceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);

        glVertexAttribPointer
        (
            0,

          
            3,

          
            GL_FLOAT,

            GL_FALSE,

           
            sizeof(SimpleVertex),

           
            (void*)0
            );

        glBindBuffer(GL_ARRAY_BUFFER, m_CSceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);
        glVertexAttribPointer
        (
            1,

            
            2,

           
            GL_FLOAT,

           
            GL_FALSE,

          
            sizeof(SimpleVertex),

         
            (unsigned char*)NULL + (3 * sizeof(float))
            );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CSceneManager.m_MeshInScene[i]->GetIndexBuffer()->m_ID);

       
        GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

        glUniform1i(texID, m_CSceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        glActiveTexture(GL_TEXTURE0 + m_CSceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);


        glBindTexture(GL_TEXTURE_2D, m_CSceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);
        
        glDrawElements
        (
         
            GL_TRIANGLES,

            
            m_CSceneManager.m_MeshInScene[i]->GetIndexNum(),

            
            GL_UNSIGNED_SHORT,

           
            (void*)0
            );

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }

}

void OpenGLManager::render2()
{

    if (g_CurrentCamera->mClickPressed) {

        g_CurrentCamera->MouseRotation();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(m_programShaderID);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4x4 word = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    glm::mat4x4 view = g_CurrentCamera->GetView();
    view = glm::transpose(view);
    glm::mat4x4 projection = g_CurrentCamera->GetProjection();
    projection = glm::transpose(projection);

   
    GLuint MatrixID = glGetUniformLocation(m_programShaderID, "WordMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &word[0].x);
   
    MatrixID = glGetUniformLocation(m_programShaderID, "ViewMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0].x);

    
    MatrixID = glGetUniformLocation(m_programShaderID, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0].x);

    for (int i = 0; i < m_CSceneManager.m_MeshInScene.size(); i++) {


        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_CSceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);

        glVertexAttribPointer
        (
           
            0,

            
            3,

          
            GL_FLOAT,

            
            GL_FALSE,

        
            sizeof(SimpleVertex),

           
            (void*)0
            );

        glBindBuffer(GL_ARRAY_BUFFER, m_CSceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);
        glVertexAttribPointer
        (
          
            1,

            
            2,

           
            GL_FLOAT,

           
            GL_FALSE,

           
            sizeof(SimpleVertex),

           
            (unsigned char*)NULL + (3 * sizeof(float))
            );

        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CSceneManager.m_MeshInScene[i]->GetIndexBuffer()->m_ID);

      
        GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

        glUniform1i(texID, m_CSceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        glActiveTexture(GL_TEXTURE0 + m_CSceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

       

        glBindTexture(GL_TEXTURE_2D, m_CSceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);
       
        glDrawElements
        (
            GL_TRIANGLES,

           
            m_CSceneManager.m_MeshInScene[i]->GetIndexNum(),

           
            GL_UNSIGNED_SHORT,

          
            (void*)0
            );

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }

    {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);

        glVertexAttribPointer
        (
           
            0,

          
            3,

           
            GL_FLOAT,

          
            GL_FALSE,

         
            sizeof(SimpleVertex),

           
            (void*)0
            );

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);
        glVertexAttribPointer
        (
           
            1,

         
            2,

            
            GL_FLOAT,

           
            GL_FALSE,

       
            sizeof(SimpleVertex),

       
            (unsigned char*)NULL + (3 * sizeof(float))
            );

        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferBillBoard);

        
        GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

        glUniform1i(texID, renderedTexture);

        glActiveTexture(GL_TEXTURE0 + renderedTexture);

      

        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        
        glDrawElements
        (
          
            GL_TRIANGLES,

            6,

        
            GL_UNSIGNED_SHORT,

          
            (void*)0
            );

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }

    ImGui::Text("ChangeCamara");

  
    if (ImGui::Button("Button")) {

      
        if (g_WhichCamera == false) {

            g_WhichCamera = true;
        }
      
        else if (g_WhichCamera == true) {

            g_WhichCamera = false;
        }

       
        CCamera* Temporal = g_SecondCamera;
        g_SecondCamera = g_CurrentCamera;
        g_CurrentCamera = Temporal;
    }

    ImGui::SameLine();

  
    if (g_WhichCamera == false) {

        ImGui::Text("Primera Camara");
    }
    else if (g_WhichCamera == true) {

        ImGui::Text("Segunda Camara");
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}