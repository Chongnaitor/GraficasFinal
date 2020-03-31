/****************************************************************************
 * Creado por Juan Pablo Chong                                              *
 *                                                                          *
 * Este es el main del peqeño motor grafico que he creado.                  *
 *                                                                          *
 *   Este motorgrafico fue diseñado por los alumnos de IDV 5 con ayuda del  *                                                                    
 *   maestro Lehi, y esta hecho con el proposito de que funcione tanto con  *                                                                  
 *   Directx(11) y OpenGL.                                                  *
 *                                                                          *
 *                                                                          *
 *    Cabe destacar que esto ha sido basado en el tutorial 7 de DirecX 11.  *
 *                                                                          *
 *                                                                          *  
 ****************************************************************************/
#include "resource.h"
#include "Encabezados/Defines.h"
//#define DirectX
#define OPenGL
 //!
 //!Inclusion de librerias
 //!
#include "Encabezados/CCamera.h"
#include "Encabezados/FirstCamera.h"
#include <fstream>
#include <iostream>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <vector>
#include "Encabezados/CDevice.h"
#include "Encabezados/CDeviceContext.h"
#include "Encabezados/CSwapChain.h"
#include "Encabezados/CRenderTargetView.h"
#include "Encabezados/CRenderTarget.h"
#include "Encabezados/CSampleState.h"
#include "Encabezados/CDepthStencil.h"
#include "Encabezados/CViewPort.h"
#include "Encabezados/CShader.h"
#include "Encabezados/CVertexBuffer.h"
#include "Encabezados/CTexture2D.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Encabezados/GraphicApi.h"
#include "Encabezados/CSceneManager.h"

//!
//!GLFW
//!
#if _WIN32 || _WIN64
#if _WIN64
#include <GLFWInclude/x64/glfw3.h>
#else
#include "Encabezados/OpenGLManager.h"
#endif
#endif

//!
//!Funcion importada de ImGui para detectar los inputs
//!

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//!
//! Estructuras basicas para el funcionamiento del programa
//!
struct CBNeverChanges
{
    //!
    //!Matriz de vista
    //!
    glm::mat4x4 mView;
};
struct CBChangeOnResize
{
    //!
    //!Matriz de proyeccion
    //!

    glm::mat4x4 mProjection;
};

//!
//!Variables para ventana y deteccion de input DirectX con Windows
//!
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;

#ifdef D3D11
//!
//! PixelShader DirectX
//!
ID3D11PixelShader*          g_pPixelShader  = NULL;

//!
//!Shader Resorces DirectX
//!
ID3D11ShaderResourceView*   g_pTextureRV    = NULL;
ID3D11ShaderResourceView*   G_InactiveSRV   = NULL;
#endif

//!
//!Matriz de mundo
//!
glm::mat4x4            g_World;

//!
//!Vector de colores para modelos 
//!
glm::vec4             g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);

//!
//!Variables Globales
//!
CRenderTargetView           CRendTarView;
CRenderTarget               CRendTar;
CSampleState               Sampler;

CDepthStencil           DepthStencilView;
CViewPort               ViewPort;
CShader                 Shader;

CDevice                 Device;
CDeviceContext         DeviceContext;
CSwapChain              CSwap;

CBuffer                 FCNeverChange;
CBuffer                 FCOnResize;
CBuffer                 FCChangeEveryFrame;

CVertexBuffer           VertexBuffer;
CVertexBuffer          IndexBuffer;

CCamera                      FreeCamera;
FirstCamera                 FPSCamera;

CBuffer                 FPCNeverChange;
CBuffer                 FPCOnResize;
CBuffer                 FPCChangeEveryFrame;

CTexture2D              InactiveTexture;
CTexture2D              G_DepthStencil;

CRenderTargetView       InactiveRTV;

//!
//!Camaras
//!
CCamera*                     MainCamera      = NULL;
CCamera*                     SecondCamera    = NULL;
CCamera                      CamaraLibre;
CCamera                      CamaraPrimeraPersona;

//!
//! Carga de modelos
//!
CSceneManager                G_CSceneManager;
GraphicApi                  G_GraphicApi;

//!
//!Punteros para usar con otros punteros de tipo void
//!
#if defined(D3D11)
ID3D11Device*               ptrDEV;
IDXGISwapChain*             ptrSwap;
ID3D11DeviceContext*        ptrDevCont;
#endif

//!
//Variables ára generar mapa y el cambio de camara
//!
int                         LevelCubes[10][10] = { 0 };
int                         Rows = 0;
int                         Columns = 0;
int                         SwitchCamera = -1;

//!
//!Variables de ancho y alto de la pantalla
//!
UINT                        width;
UINT                        height;

//!
//!Variables de OpenGL
//!
unsigned int g_ColorShaderID;
OpenGLManager g_OpenGlObj;

//!
//!Funcion para reescalara ventana DirectX
//!
void Resize(){

#ifdef D3D11

    if (DeviceContext.g_pImmediateContextD3D11 != nullptr) {

        //!
        //! Obtener dimensiones de la ventana
        //!
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        //!
        //! Pasar matriz de mundo a matriz de identidad
        //!
        g_World = glm::mat4x4();

        //!
        //! Establecer el largo y ancho de la camara
        //!
        MainCamera->SetHeight(height);
        MainCamera->SetWidht(width);

        //!
        //! Actualizar la matriz de vista con nuevos parametros
        //!
        MainCamera->UpdateVM();

        //!
        //! Update CB
        //!
        CBChangeOnResize cbChangesOnResize;
        cbChangesOnResize.mProjection = MainCamera->GetProjection();
        ptrDevCont->UpdateSubresource(MainCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize, 0, 0);

        //!
        //!inactive camera
        //!
        SecondCamera->SetHeight(height);
        SecondCamera->SetWidht(width);
        SecondCamera->UpdateVM();

        CBChangeOnResize cbChangesOnResize2;
        cbChangesOnResize2.mProjection = SecondCamera->GetProjection();
        ptrDevCont->UpdateSubresource(SecondCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize2, 0, 0);

        if (ptrSwap) {

            HRESULT h;

            //!
            //!Release inactive camera texture, SRV and RTV
            //!
            InactiveTexture.m_TextureD3D11->Release();
            G_InactiveSRV->Release();
            InactiveRTV.g_pRenderTargetViewD3D11->Release();
	
            //!
            //!Resize inactive camera texture	
            //!
            Texture2Desc TD;
            ZeroMemory(&TD, sizeof(TD));
            TD.Width = width;
            TD.Height = height;
            TD.MipLevels = TD.ArraySize = 1;
            TD.Format = FORMAT_R8G8B8A8_UNORM;
            TD.SampleDesc.My_Count = 1;
            TD.Usage = USAGE_DEFAULT;
            TD.BindFlags = 8 | 32;		
            TD.CPUAccessFlags = 65536;	
            TD.MiscFlags = 0;
            InactiveTexture.Init(TD);

            h = ptrDEV->CreateTexture2D(&InactiveTexture.m_TextDescD3D11, NULL, &InactiveTexture.m_TextureD3D11);

            RenderTargetViewDesc RTVD;
            ZeroMemory(&RTVD, sizeof(RTVD));
            RTVD.Format = TD.Format;
            RTVD.ViewDimension = RTV_DIMENSION_TEXTURE2D;
            RTVD.Texture2D.My_MipSlice = 0;
            InactiveRTV.Init(RTVD);

            h = ptrDEV->CreateRenderTargetView(InactiveTexture.m_TextureD3D11, &InactiveRTV.m_renderTVD3D11, &InactiveRTV.g_pRenderTargetViewD3D11);

            D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
            ZeroMemory(&SRV, sizeof(SRV));
            SRV.Format = (DXGI_FORMAT)TD.Format;
            SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRV.Texture2D.MostDetailedMip = 0;
            SRV.Texture2D.MipLevels = 1;

            h = ptrDEV->CreateShaderResourceView(InactiveTexture.m_TextureD3D11, &SRV, &G_InactiveSRV);

            //!
            //! Camara activa
            //!
           DeviceContext.g_pImmediateContextD3D11->OMSetRenderTargets(0, 0, 0);
            CRendTarView.g_pRenderTargetViewD3D11->Release();

            h = ptrSwap->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

            CBuffer tempBack;
            h = ptrSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tempBack.m_BufferD3D11);
            h = ptrDEV->CreateRenderTargetView(tempBack.m_BufferD3D11, NULL, &CRendTarView.g_pRenderTargetViewD3D11);
            tempBack.m_BufferD3D11->Release();

            Texture2Desc DepthDesc;
            ZeroMemory(&DepthDesc, sizeof(DepthDesc));
            DepthDesc.Width = width;
            DepthDesc.Height = height;
            DepthDesc.MipLevels = 1;
            DepthDesc.ArraySize = 1;
            DepthDesc.Format = FORMAT_D24_UNORM_S8_UINT;
            DepthDesc.SampleDesc.My_Count = 1;
            DepthDesc.SampleDesc.My_Quality = 0;
            DepthDesc.Usage = USAGE_DEFAULT;
            DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            DepthDesc.CPUAccessFlags = 0;
            DepthDesc.MiscFlags = 0;

            G_DepthStencil.Init(DepthDesc);

            h = ptrDEV->CreateTexture2D(&G_DepthStencil.m_TextDescD3D11, NULL, &G_DepthStencil.m_TextureD3D11);

            DepthStencilViewDesc DSVD;
            ZeroMemory(&DSVD, sizeof(DSVD));
            DSVD.Format = DepthStencilView.m_DepthDesc.Format;
            DSVD.ViewDimension = DSV_DIMENSION_TEXTURE2D;
            DSVD.Texture2D.My_MipSlice = 0;

            DepthStencilView.g_pDepthStencilViewD3D11->Release();

            DepthStencilView.Init(DSVD);

            h = ptrDEV->CreateDepthStencilView(G_DepthStencil.m_TextureD3D11, &DepthStencilView.descDepthViewD3D11, &DepthStencilView.g_pDepthStencilViewD3D11);

            ptrDevCont->OMSetRenderTargets(1, &CRendTarView.g_pRenderTargetViewD3D11, DepthStencilView.g_pDepthStencilViewD3D11);

            ViewportDesc VD;
            ZeroMemory(&VD, sizeof(VD));
            VD.Width = width;
            VD.Height = height;
            VD.MinDepth = 0.f;
            VD.MaxDepth = 1.f;
            VD.TopLeftX = 0;
            VD.TopLeftY = 0;

            CViewPort ViewPort;
            ViewPort.Init(VD);
            ptrDevCont->RSSetViewports(1, &ViewPort.vpD3D11);
        }
    }
#endif
}

//!
//! Enun para carga de niveles
//!
enum LevelStuff {

    Wall = 1,
    Void = 2,
    Pilares = 4,
    NotColideWalls = 5
};

//!
//!Funcion que lee un archivo para generar mapa
//!
void LevelMap(std::string FileLevelName) {

    std::ifstream File;
    File.open(FileLevelName);

    File >> Columns >> Rows;

    for (int i = 0; i < Rows; i++) {

        for (int j = 0; j < Columns; j++) {

            File >> LevelCubes[i][j];
        }
    }

    for (int i = 0; i < Rows; i++) {

        for (int j = 0; j < Columns; j++) {

            std::cout << LevelCubes[i][j] << " ";
        }
        std::cout << std::endl;
    }
    File.close();
}

//!
//! Funcion para inicializar camaras
//!
void InitCameras() {

    CCameraDescriptor FreeCam;
  FreeCam.dAt = { 0,0,0 };
  FreeCam.dEye = { 0,0,-1 };
  FreeCam.dUp = { 0,1,0 };
  FreeCam.dFar = 1000;
  FreeCam.dNear = 0.01;

#ifdef D3D11
    FreeCam.dFoV = XM_PIDIV4;
#endif

    FreeCam.dHeight = height;
    FreeCam.dWidht = width;

    CCameraDescriptor FirstPersonCam;
    FirstPersonCam.dAt = { 0,0,0 };
    FirstPersonCam.dEye = { 0,0,-6 };
    FirstPersonCam.dUp = { 0,1,0 };
    FirstPersonCam.dFar = 1000;
    FirstPersonCam.dNear = 0.01;

#ifdef D3D11
   FirstPersonCam.dFoV = XM_PIDIV4;
#endif

   FirstPersonCam.dHeight = height;
   FirstPersonCam.dWidht = width;

    FreeCamera.Init(FreeCam);
    FPSCamera.Init(FirstPersonCam);

    MainCamera = &FPSCamera;
    SecondCamera = &FreeCamera;
}

//!
//!Consola
//!
void activateConsole()
{
    //!
    //! Crea una consola 
    //!
    AllocConsole();

    //!
    //! Obtiene el STDOUT handle
    //!
    HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
    FILE* COutputHandle = _fdopen(SystemOutput, "w");

    //!
    //!Obtiene STDERR handle
    //!
    HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
    int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
    FILE* CErrorHandle = _fdopen(SystemError, "w");

    //!
    //! Obtiene STDIN handle
    //!
    HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
    FILE* CInputHandle = _fdopen(SystemInput, "r");

    //!
    //!make cout, wcout, cin, wcin, wcerr, cerr, wclog y clog point para la consola
    //!
    std::ios::sync_with_stdio(true);

    //!
    //! Redirecciona los CRT standard input, output, y error handles a la consola
    //!
    freopen_s(&CInputHandle, "CONIN$", "r", stdin);
    freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
    freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

   
   
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();

}


//!
//!Forward declarations 
//!
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();



//!
//! Punto de entrada del programa, es donde esta el main loop y se redirije a la bomba de mensajes
//!
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(InitWindow(hInstance, nCmdShow)))
        return 0;

#ifdef D3D11
    if (FAILED(InitDevice()))
    {
        CleanupDevice();
        return 0;
    }
#endif 

    //!
    //! Main message loop
    //!
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
#ifdef D3D11
            //!
            //!Inicializacion de amgui y colocacion de botones virtuales
            //!
            ImVec2 ScreenImGui(200, 200);

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();

            ImGui::NewFrame();
            ImGui::Begin("Cambio Cameras");
            if (ImGui::Button("Click"))
            {
                CCamera* Temporal = SecondCamera;
                SecondCamera = MainCamera;
                MainCamera = Temporal;
            }
            ImGui::End();

            ImGui::Begin("ShaderCamera");
            ImGui::Image(G_InactiveSRV, ScreenImGui);
            ImGui::GetIO().FontGlobalScale;
            ImGui::End();
            Render();
#endif 
        }
    }

    CleanupDevice();
    LevelMap("Laberinto.txt");
    return (int)msg.wParam;
}

//!
//! Crear ventana
//!
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow){
    //!
    //!Llamar a la funcion de la consola
    //!

    activateConsole();
#ifdef DirectX


    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;
    //!
    //! Crear ventana
    //!
    g_hInst = hInstance;
    RECT rc = { 0, 0, 800, 800 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
        NULL);
    if (!g_hWnd)
        return E_FAIL;
    
    ShowWindow(g_hWnd, nCmdShow);
    
    LevelMap("Laberinto.txt");
#endif // D3D11
#ifdef OPenGL

    g_OpenGlObj.WindowGLFW();
#endif
    return S_OK;
}

//!
//! Compilar shaders de DirectX
//!
#ifdef D3D11
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )

    //!
    //! Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    //!

    //!
    //! Setting this flag improves the shader debugging experience, but still allows the shaders to be optimized and to run exactly the way they will run in 
    //!

    //!
    //! the release configuration of this program.
    //!
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    //!
    //! Guardamos el resultado del shader.
    //!
    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);

    if (FAILED(hr))
    {
        if (pErrorBlob != NULL)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}
#endif

//!
//! Funcion para crear el input layout
//!
#ifdef D3D11
HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
    //!
    //! Reflejar la informacion de los shaders
    //!
    ID3D11ShaderReflection* pVertexShaderReflection = NULL;
    if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
    {
        return S_FALSE;
    }

    //!
    //! Obtener la informacion de los shaders
    //!
    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc(&shaderDesc);

    //!
    //! Leer el input layout
    //!
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

    int offset = 0;

    for (int i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

        //!
        //! Llenar el input element desc
        //!
        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = offset;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        //!
        //! Determinar el formato DXGI
        //!
        if (paramDesc.Mask == 1)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 15)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12;
        }
        else if (paramDesc.Mask <= 7)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

      
        inputLayoutDesc.push_back(elementDesc);
    }

    //!
    //! Crear el input layout
    //!
    HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);


    pVertexShaderReflection->Release();
    return hr;
}
#endif

//!
//! Crear Direct3D device y swap chain
//!
#ifdef D3D11
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    //#ifdef D3D11
    UINT createDeviceFlags = 0;
    //#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //#endif
     //!
     //!Elegir el tipo de bandera depende el hardware 
     //!
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    //!
    //! Llenar los descriptores para el device
    //!
    DeviceDescriptor objDev;
    objDev.g_driverType = DRIVER_TYPE_NULL;
    Device.g_pd3dDeviceD3D11 = NULL;
    objDev.s_createDeviceFlags = 2;
    objDev.s_numFeatureLevels = 3;

    //!
    //! Inicializamos el Device
    //!
    Device.Init(objDev);

    //!
    //! Llenar los descriptores para el swapchain
    //!
    SwapChainDescriptor objSd;
    objSd.BufferCount = 1;
    objSd.BufferDesc.My_Width = width;
    objSd.BufferDesc.My_Height = height;
    objSd.BufferDesc.My_Format = FORMAT_R8G8B8A8_UNORM;
    objSd.BufferDesc.My_RefreshRate.My_Numerator = 60;
    objSd.BufferDesc.My_RefreshRate.My_Denominator = 1;
    objSd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    objSd.OutputWindow = g_hWnd;
    objSd.SampleDesc.My_Count = 1;
    objSd.SampleDesc.My_Quality = 0;
    objSd.Windowed = TRUE;
    //!
    //! Inicializamos el swapchain
    //!
    CSwap.Init(objSd);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        Device.g_driverTypeD3D11 = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(NULL, Device.g_driverTypeD3D11, NULL, objDev.s_createDeviceFlags, featureLevels, objDev.s_numFeatureLevels,
            D3D11_SDK_VERSION, &CSwap.sdD3D11, &CSwap.g_pSwapChainD3D11, &Device.g_pd3dDeviceD3D11, &Device.g_featureLevelD3D11, &DeviceContext.g_pImmediateContextD3D11);
        if (SUCCEEDED(hr))
            break;
    }

    //!
    //!Casteo para uso de punteros void
    //!
    ptrDEV = static_cast<ID3D11Device*>(Device.GetDev());
    ptrDevCont = static_cast<ID3D11DeviceContext*>(DeviceContext.GetDevCont());
    ptrSwap = static_cast<IDXGISwapChain*>(CSwap.GetSwap());

    if (FAILED(hr))
        return hr;

    //!
    //! Creamos un render target view
    //!
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = CSwap.g_pSwapChainD3D11->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = Device.g_pd3dDeviceD3D11->CreateRenderTargetView(pBackBuffer, NULL, &CRendTarView.g_pRenderTargetViewD3D11);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    //!
    //! Creamos textura de depth stencil
    //!
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = Device.g_pd3dDeviceD3D11->CreateTexture2D(&descDepth, NULL, &CRendTar.g_pDepthStencilD3D11);

    if (FAILED(hr))
        return hr;
 
    //!
    //! Creamos el depth stencil view
    //!
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = Device.g_pd3dDeviceD3D11->CreateDepthStencilView(CRendTar.g_pDepthStencilD3D11, &descDSV, &DepthStencilView.g_pDepthStencilViewD3D11);

    if (FAILED(hr))
        return hr;

   DeviceContext.g_pImmediateContextD3D11->OMSetRenderTargets(1, &CRendTarView.g_pRenderTargetViewD3D11, DepthStencilView.g_pDepthStencilViewD3D11);

    //!
    //! Iniciamos el viewport
    //!
    ViewportDesc  vp2;
    vp2.Width = (FLOAT)width;
    vp2.Height = (FLOAT)height;
    vp2.MinDepth = 0.0f;
    vp2.MaxDepth = 1.0f;
    vp2.TopLeftX = 0;
    vp2.TopLeftY = 0;
    ViewPort.Init(vp2);

   DeviceContext.g_pImmediateContextD3D11->RSSetViewports(1, &ViewPort.vpD3D11);

    //!
    //! Compilamos el vertex shader
    //!
    hr = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &Shader.m_pVSBlobD3D11);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    //!
    //! Creamos el vertex shader
    //!
    hr = Device.g_pd3dDeviceD3D11->CreateVertexShader(Shader.m_pVSBlobD3D11->GetBufferPointer(), Shader.m_pVSBlobD3D11->GetBufferSize(), NULL, &Shader.g_pVertexShaderD3D11);
    if (FAILED(hr))
    {
        Shader.m_pVSBlobD3D11->Release();
        return hr;
    }

    //!
    //! Definimos el inputlayout
    //!

    //!
    //! Creamos el input layout del compilado de VS
    //!
    hr = CreateInputLayoutDescFromVertexShaderSignature(Shader.m_pVSBlobD3D11, Device.g_pd3dDeviceD3D11, &Shader.LayoutD3D11);
    if (FAILED(hr))
        return hr;

    //!
    //! Creamos el input layout
    //!

    //!
    //! Establecemos el inmput layout
    //!
   DeviceContext.g_pImmediateContextD3D11->IASetInputLayout(Shader.LayoutD3D11);

    //!
    //! Compilamos el vertex shader
    //!
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    //!
    //! Creamos el pixelshader
    //!
    hr = Device.g_pd3dDeviceD3D11->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

    //!
    //! Creamos el vertex buffer
    //!
    SimpleVertex vertices[] =
    {                                     
        {  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
        {  glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec2(1.0f, 0.0f) },
        {  glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(1.0f, 1.0f) },
        {  glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },
                                          
        {  glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec2(0.0f, 0.0f) },
        {  glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
        {  glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        {  glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
                                          
        {  glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
        {  glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec2(1.0f, 0.0f) },
        {  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
        {  glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },
                                          
        {  glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
        {  glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
        {  glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec2(1.0f, 1.0f) },
        {  glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(0.0f, 1.0f) },
                                          
        {  glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec2(0.0f, 0.0f) },
        {  glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
        {  glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec2(1.0f, 1.0f) },
        {  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
                                          
        {  glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
        {  glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        {  glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(1.0f, 1.0f) },
        {  glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },
    };

    BufferDescriptor bd2;
    ZeroMemory(&bd2, sizeof(bd2));
    bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(SimpleVertex) * 24;
    bd2.BindFlags = BIND_VERTEX_BUFFER;
    bd2.CPUAccessFlags = 0;

    SUBRESOURCE_DATA initData2;
    ZeroMemory(&initData2, sizeof(initData2));
    initData2.My_pSysMem = vertices;

    VertexBuffer.Init(initData2, bd2);

    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&VertexBuffer.m_Buffer.m_BufferDescD3D11, &VertexBuffer.m_SubDataD3D11, &VertexBuffer.m_Buffer.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    //!
    //! Establecemos el vertex buffer
    //!
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
   DeviceContext.g_pImmediateContextD3D11->IASetVertexBuffers(0, 1, &VertexBuffer.m_Buffer.m_BufferD3D11, &stride, &offset);

    //!
    //! Creamos el index buffer
    //!
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(WORD) * 36;
    bd2.BindFlags = BIND_INDEX_BUFFER;
    bd2.CPUAccessFlags = 0;
    initData2.My_pSysMem = indices;

   IndexBuffer.Init(initData2, bd2);

    //!
    //! Establecemos el index buffer
    //!
    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&IndexBuffer.m_Buffer.m_BufferDescD3D11, &IndexBuffer.m_SubDataD3D11, &IndexBuffer.m_Buffer.m_BufferD3D11);
    if (FAILED(hr))

   DeviceContext.g_pImmediateContextD3D11->IASetIndexBuffer(IndexBuffer.m_Buffer.m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);
 
    //!
    //! Establecemos  topologia primitiva
    //!
   DeviceContext.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
    //!
    //! Creamos buffers constantes
    //!
    bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(CBNeverChanges);
    bd2.BindFlags = BIND_CONSTANT_BUFFER;
    bd2.CPUAccessFlags = 0;
    FreeCamera.g_pCBNeverChangesCamera.Init(bd2);

    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&FreeCamera.g_pCBNeverChangesCamera.m_BufferDescD3D11, NULL, &FreeCamera.g_pCBNeverChangesCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    //!
    //! Inicialización NeverChange para la camara en primera persona
    //!
    FPSCamera.g_pCBNeverChangesCamera.Init(bd2);
    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&FPSCamera.g_pCBNeverChangesCamera.m_BufferDescD3D11, NULL, &FPSCamera.g_pCBNeverChangesCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;


    bd2.ByteWidth = sizeof(CBChangeOnResize);
    FreeCamera.g_pCBChangeOnResizeCamera.Init(bd2);
    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&FreeCamera.g_pCBChangeOnResizeCamera.m_BufferDescD3D11, NULL, &FreeCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    //!
    //! Inicialización Resize de la camara de primera persona
    //!
    FPSCamera.g_pCBChangeOnResizeCamera.Init(bd2);
    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&FPSCamera.g_pCBChangeOnResizeCamera.m_BufferDescD3D11, NULL, &FPSCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    
    bd2.ByteWidth = sizeof(CBChangesEveryFrame);
    FreeCamera.g_pCBChangesEveryFrameCamera.Init(bd2);
    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&FreeCamera.g_pCBChangesEveryFrameCamera.m_BufferDescD3D11, NULL, &FreeCamera.g_pCBChangesEveryFrameCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    //!
    //! Inicialización ChangeEveryFrame de la camara de primera persona
    //!
    FPSCamera.g_pCBChangesEveryFrameCamera.Init(bd2);
    hr = Device.g_pd3dDeviceD3D11->CreateBuffer(&FPSCamera.g_pCBChangesEveryFrameCamera.m_BufferDescD3D11, NULL, &FPSCamera.g_pCBChangesEveryFrameCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    //!
    //! Se carga la textura
    //!
    hr = D3DX11CreateShaderResourceViewFromFile(Device.g_pd3dDeviceD3D11, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
    if (FAILED(hr))
        return hr;
 
    //!
    //! Se crea el sampler
    //!
    SampleStateDesc sampDesc2;


    sampDesc2.Filter = FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc2.AddressU = TEXTURE_ADDRESS_WRAP;
    sampDesc2.AddressV = TEXTURE_ADDRESS_WRAP;
    sampDesc2.AddressW = TEXTURE_ADDRESS_WRAP;
    sampDesc2.ComparisonFunc = COMPARISON_NEVER;
    sampDesc2.MinLOD = 0;
    sampDesc2.MaxLOD = D3D11_FLOAT32_MAX;
    Sampler.Init(sampDesc2);

    hr = Device.g_pd3dDeviceD3D11->CreateSamplerState(&Sampler.sampDescD3D11, &Sampler.g_pSamplerLinearD3D11);
    if (FAILED(hr))
        return hr;

    
    //!
    //!Inicializamos matriz de mundo
    //!
    g_World = glm::mat4x4();
 
    //!
    //! Inicializamos la matriz de vista
    //!
    glm::vec4 Eye(0.0f, 3.0f, -6.0f, 0.0f);
    glm::vec4 At(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 Up(0.0f, 1.0f, 0.0f, 0.0f);



    InitCameras();

    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = FPSCamera.GetView();

    //#ifdef D3D11
   DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(FPSCamera.g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cbNeverChanges, 0, 0);
    //#endif

    cbNeverChanges.mView = FreeCamera.GetView();

    //#ifdef D3D11
   DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(FreeCamera.g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cbNeverChanges, 0, 0);
    //#endif

    //!
    //! Inicializamos la matriz de proyeccion
    //!
    CBChangeOnResize cbChangesOnResize;
    cbChangesOnResize.mProjection = FPSCamera.GetProjection();
    //#ifdef D3D11
   DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(FPSCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize, 0, 0);
    //#endif

    cbChangesOnResize.mProjection = FreeCamera.GetProjection();

    //#ifdef D3D11
   DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(FreeCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize, 0, 0);
    //#endif

    Texture2Desc T;
    ZeroMemory(&T, sizeof(T));
    T.Width = width;
    T.Height = height;
    T.MipLevels = T.ArraySize = 1;
    T.Format = FORMAT_R8G8B8A8_UNORM;
    T.SampleDesc.My_Count = 1;
    T.Usage = USAGE_DEFAULT;
    T.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
    //#ifdef D3D11
    T.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //#endif
    T.MiscFlags = 0;

    InactiveTexture.Init(T);

    //#ifdef D3D11
    hr = Device.g_pd3dDeviceD3D11->CreateTexture2D(&InactiveTexture.m_TextDescD3D11, NULL, &InactiveTexture.m_TextureD3D11);
    //#endif
    if (FAILED(hr)) {

        return hr;
    }

    RenderTargetViewDesc RTVDesc;
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    RTVDesc.Format = T.Format;
    RTVDesc.ViewDimension = RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.My_MipSlice = 0;

    InactiveRTV.Init(RTVDesc);

    //#ifdef D3D11
    hr = Device.g_pd3dDeviceD3D11->CreateRenderTargetView(InactiveTexture.m_TextureD3D11, &InactiveRTV.m_renderTVD3D11, &InactiveRTV.g_pRenderTargetViewD3D11);
    //#endif
    if (FAILED(hr)) {

        return hr;
    }

    //#ifdef D3D11
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    SRVDesc.Format = (DXGI_FORMAT)T.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;
    //#endif

    //#ifdef D3D11
    hr = Device.g_pd3dDeviceD3D11->CreateShaderResourceView(InactiveTexture.m_TextureD3D11, &SRVDesc, &G_InactiveSRV);
    //#endif
    if (FAILED(hr)) {

        return hr;
    }

    //#ifdef D3D11
    //!
    //!Inicilizamos imgui
    //!
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(Device.g_pd3dDeviceD3D11,DeviceContext.g_pImmediateContextD3D11);
    ImGui::StyleColorsDark();
    //#endif
    InitCameras();

    //!
    //! Cargarmos modelo a la escena
    //!
    
    G_GraphicApi.ChargeMesh("SimpleMan/source/SimpleMan2.6.fbx", &G_CSceneManager, G_GraphicApi.m_Model,DeviceContext, G_GraphicApi.m_Importer, &Device);

    return S_OK;
}
#endif // D3D11

//!
//! Limpiamos buffers
//!
void CleanupDevice()
{
#ifdef D3D11
    if (DeviceContext.g_pImmediateContextD3D11)DeviceContext.g_pImmediateContextD3D11->ClearState();
    if (Sampler.g_pSamplerLinearD3D11) Sampler.g_pSamplerLinearD3D11->Release();
    if (g_pTextureRV) g_pTextureRV->Release();
    if (FCNeverChange.m_BufferD3D11)FCNeverChange.m_BufferD3D11->Release();
    if (FCOnResize.m_BufferD3D11)FCOnResize.m_BufferD3D11->Release();
    if (FCChangeEveryFrame.m_BufferD3D11)FCChangeEveryFrame.m_BufferD3D11->Release();
    if (VertexBuffer.m_Buffer.m_BufferD3D11)VertexBuffer.m_Buffer.m_BufferD3D11->Release();
    if (IndexBuffer.m_Buffer.m_BufferD3D11)IndexBuffer.m_Buffer.m_BufferD3D11->Release();
    if (Shader.LayoutD3D11) Shader.LayoutD3D11->Release();
    if (Shader.g_pVertexShaderD3D11) Shader.g_pVertexShaderD3D11->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (CRendTar.g_pDepthStencilD3D11) CRendTar.g_pDepthStencilD3D11->Release();
    if (DepthStencilView.g_pDepthStencilViewD3D11) DepthStencilView.g_pDepthStencilViewD3D11->Release();
    if (CRendTarView.g_pRenderTargetViewD3D11) CRendTarView.g_pRenderTargetViewD3D11->Release();
    if (CSwap.g_pSwapChainD3D11) CSwap.g_pSwapChainD3D11->Release();
    if (DeviceContext.g_pImmediateContextD3D11)DeviceContext.g_pImmediateContextD3D11->Release();
    if (Device.g_pd3dDeviceD3D11) Device.g_pd3dDeviceD3D11->Release();
    if (FPCNeverChange.m_BufferD3D11)FPCNeverChange.m_BufferD3D11->Release();
    if (FPCOnResize.m_BufferD3D11)FPCOnResize.m_BufferD3D11->Release();
    if (FPCChangeEveryFrame.m_BufferD3D11)FPCChangeEveryFrame.m_BufferD3D11->Release();
    if (InactiveTexture.m_TextureD3D11)  InactiveTexture.m_TextureD3D11->Release();
    if (InactiveRTV.g_pRenderTargetViewD3D11) InactiveRTV.g_pRenderTargetViewD3D11->Release();
    if (G_InactiveSRV)G_InactiveSRV->Release();
#endif
}

//!
//! Bomba de mensajes
//!
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse){

        ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
        return true;
    }

    PAINTSTRUCT ps;
    HDC hdc;
    POINT Temp;

    switch (message)
    {
        case WM_PAINT:

            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:

            PostQuitMessage(0);
            break;

        //!
        //! Mensaje para saber cuando se reescala una ventana.
        //!
        case WM_SIZE: {

            Resize();
            ImGui::GetStyle().ScaleAllSizes(1);
            break;
        }

        //!
        //! Mensaje cuando se aprieta una tecla
        //!
        case WM_KEYDOWN: {

            MainCamera->inputs(wParam);

            CBNeverChanges cb;
            cb.mView = MainCamera->GetView();

#if defined(D3D11)
           DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
#endif
            break;
        }

        //!
        //! Mensaje cuando se presiona el boton izquierdo del mouse dentro del cliente
        //!
        case WM_LBUTTONDOWN:

            GetCursorPos(&Temp);
            MainCamera->OriginalMousePos =glm::vec2(Temp.x, Temp.y);
            MainCamera->mClickPressed = true;
            break;

        case WM_MOUSEMOVE:

#ifdef D3D11
            if (MainCamera->mClickPressed)
            {
                MainCamera->SetOriginalMousePos(MainCamera->OriginalMousePos.x, MainCamera->OriginalMousePos.y);
                MainCamera->MouseRotation();
                CBNeverChanges cb;
                cb.mView = MainCamera->GetView();
               DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
            }
#endif // D3D11            
            break;

        //!
        //! Mensaje cuando se deja presionar el boton izquierdo del mouse dentro del cliente
        //!
        case WM_LBUTTONUP:

            MainCamera->mClickPressed = false;
            break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//!
//! El render de un cuadro
//!
#ifdef D3D11
void Render()
{
    G_CSceneManager.Update(); 
    
    //!
    //! Actualizamos el tiempo
    //!
    static float t = 0.0f;
    if (Device.m_DescDevice.g_driverType == D3D_DRIVER_TYPE_REFERENCE){

        t += (float)XM_PI * 0.0125f;
    }
    else{

        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }
    glm::vec3 T = { 3, 0, 0 };


    //!
    //! Modificamos el color
    //!
    g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

    //!
    //! Limpiamos el backbuffer
    //!
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

   DeviceContext.g_pImmediateContextD3D11->OMSetRenderTargets       (1, &InactiveRTV.g_pRenderTargetViewD3D11, DepthStencilView.g_pDepthStencilViewD3D11);
   DeviceContext.g_pImmediateContextD3D11->ClearRenderTargetView    (InactiveRTV.g_pRenderTargetViewD3D11, ClearColor);
    
    //!
    //! Limpiamos el depth buffer a 1.0 (max depth)
    //!
   DeviceContext.g_pImmediateContextD3D11->ClearDepthStencilView    (DepthStencilView.g_pDepthStencilViewD3D11, D3D11_CLEAR_DEPTH, 1.0f, 0);



    //!
    //!Renderizamos el cubo
    //!
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
   DeviceContext.g_pImmediateContextD3D11->IASetVertexBuffers     (0, 1, &VertexBuffer.m_Buffer.m_BufferD3D11, &stride, &offset);
   DeviceContext.g_pImmediateContextD3D11->IASetIndexBuffer       (IndexBuffer.m_Buffer.m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);
   DeviceContext.g_pImmediateContextD3D11->VSSetShader            (Shader.g_pVertexShaderD3D11, NULL, 0);
   DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers   (0, 1, &MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11);
   DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers   (1, 1, &MainCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11);
   DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers   (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
   DeviceContext.g_pImmediateContextD3D11->PSSetShader            (g_pPixelShader, NULL, 0);
   DeviceContext.g_pImmediateContextD3D11->PSSetConstantBuffers   (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
   DeviceContext.g_pImmediateContextD3D11->PSSetShaderResources   (0, 1, &g_pTextureRV);
   DeviceContext.g_pImmediateContextD3D11->PSSetSamplers          (0, 1, &Sampler.g_pSamplerLinearD3D11);

   //!
   //!Dibujamos el laberinto de cubos
   //!
    int DistanceX = 0;
    int DistanceY = 0;
    for (int i = 0; i < Rows; i++){

        for (int j = 0; j < Columns; j++){

            if (LevelCubes[i][j]){

                DistanceX += 2.5;
            }
            else if (LevelCubes[i][j] == Pilares){

                DistanceX += 2.5;
            }
            else{

                DistanceX += 2.5;
            }
            if (LevelCubes[i][j] != 0){

                CBChangesEveryFrame cb;
                cb.mWorld =glm::transpose(g_World);
               cb.vMeshColor = g_vMeshColor;

               DeviceContext.g_pImmediateContextD3D11->UpdateSubresource      (SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
               DeviceContext.g_pImmediateContextD3D11->VSSetShader            (Shader.g_pVertexShaderD3D11, NULL, 0);
               DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers   (0, 1, &SecondCamera->g_pCBNeverChangesCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers   (1, 1, &SecondCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers   (2, 1, &SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->PSSetShader            (g_pPixelShader, NULL, 0);
               DeviceContext.g_pImmediateContextD3D11->PSSetConstantBuffers   (2, 1, &SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->PSSetShaderResources   (0, 1, &g_pTextureRV);
               DeviceContext.g_pImmediateContextD3D11->PSSetSamplers          (0, 1, &Sampler.g_pSamplerLinearD3D11);
               DeviceContext.g_pImmediateContextD3D11->DrawIndexed            (36, 0, 0);
            }
            T = { DistanceX, 0, DistanceY };
            g_World = glm::translate(T);
        }
        DistanceY += 2.5;
        DistanceX = 0;
    }

    CBChangesEveryFrame m_MeshData;
    m_MeshData.mWorld = {
        1,0,0,MainCamera->GetEye().x,
        0,1,0,MainCamera->GetEye().y,
        0,0,1,MainCamera->GetEye().z,
       0,0,0,1
    };

   m_MeshData.vMeshColor = { 1,1,1,1 };
   DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &m_MeshData, 0, 0);
   DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);

    for (size_t i = 0; i < G_CSceneManager.m_MeshInScene.size(); i++)
    {
        DeviceContext.g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &G_CSceneManager.m_MeshInScene[i]->m_Materials->m_TexDif);
         UINT stride = sizeof(SimpleVertex);
         UINT offset = 0;
    
        DeviceContext.g_pImmediateContextD3D11->IASetVertexBuffers
         (
             0,

             //!
             //! Numero de buffers usados
             //!
             1,

             //!
             //! Puntero a la lista de punteros 
             //!
             &G_CSceneManager.m_MeshInScene[i]->m_VertexBuffer->m_BufferD3D11,

             //!
             //! Indica el tamaño de cada vertice
             //!
             &stride,
             &offset
         );
    
        DeviceContext.g_pImmediateContextD3D11->IASetIndexBuffer
         (
             G_CSceneManager.m_MeshInScene[i]->m_Index->m_BufferD3D11,
             DXGI_FORMAT_R16_UINT,
             0
         );


         //!
         //!Dibuja el vertex buffer en el back buffer
         //!
        DeviceContext.g_pImmediateContextD3D11->DrawIndexed(G_CSceneManager.m_MeshInScene[i]->m_IndexNum, 0, 0);
    }
   DeviceContext.g_pImmediateContextD3D11->OMSetRenderTargets   (1, &CRendTarView.g_pRenderTargetViewD3D11, DepthStencilView.g_pDepthStencilViewD3D11);
   DeviceContext.g_pImmediateContextD3D11->ClearRenderTargetView(CRendTarView.g_pRenderTargetViewD3D11, ClearColor);
   DeviceContext.g_pImmediateContextD3D11->ClearDepthStencilView(DepthStencilView.g_pDepthStencilViewD3D11, D3D11_CLEAR_DEPTH, 1.0f, 0);
 
   DeviceContext.g_pImmediateContextD3D11->IASetVertexBuffers(0, 1, &VertexBuffer.m_Buffer.m_BufferD3D11, &stride, &offset);
   DeviceContext.g_pImmediateContextD3D11->IASetIndexBuffer(IndexBuffer.m_Buffer.m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);


    DistanceX = 0;
    DistanceY = 0;

    for (int i = 0; i < Rows; i++){

        for (int j = 0; j < Columns; j++){

            if (LevelCubes[i][j]){

                DistanceX += 2.5;
            }
            else if (LevelCubes[i][j] == Pilares){

                DistanceX += 2.5;
            }
            else{

                DistanceX += 2.5;
            }
            if (LevelCubes[i][j] != 0){

                CBChangesEveryFrame cb;
                cb.mWorld = glm::transpose(g_World);
                cb.vMeshColor = g_vMeshColor;
                
               DeviceContext.g_pImmediateContextD3D11->UpdateSubresource    (MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
               DeviceContext.g_pImmediateContextD3D11->VSSetShader          (Shader.g_pVertexShaderD3D11, NULL, 0);
               DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers (0, 1, &MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers (1, 1, &MainCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->PSSetShader          (g_pPixelShader, NULL, 0);
               DeviceContext.g_pImmediateContextD3D11->PSSetConstantBuffers (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
               DeviceContext.g_pImmediateContextD3D11->PSSetShaderResources (0, 1, &G_InactiveSRV);
               DeviceContext.g_pImmediateContextD3D11->PSSetSamplers        (0, 1, &Sampler.g_pSamplerLinearD3D11);
               DeviceContext.g_pImmediateContextD3D11->DrawIndexed          (36, 0, 0);
            }
            T = { DistanceX, 0, DistanceY };
            g_World = glm::translate(T);
        }   
        DistanceY += 2.5;
        DistanceX = 0;
    }

    m_MeshData.mWorld = {
        1,0,0,SecondCamera->GetEye().x,
        0,1,0,SecondCamera->GetEye().y,
        0,0,1,SecondCamera->GetEye().z,
       0,0,0,1
    };

    m_MeshData.vMeshColor = { 1,1,1,1 };
   DeviceContext.g_pImmediateContextD3D11->UpdateSubresource(MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &m_MeshData, 0, 0);
   DeviceContext.g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);

    for (size_t i = 0; i < G_CSceneManager.m_MeshInScene.size(); i++)
    {
       DeviceContext.g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &G_CSceneManager.m_MeshInScene[i]->m_Materials->m_TexDif);
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;

       DeviceContext.g_pImmediateContextD3D11->IASetVertexBuffers
        (
            0,
      
            1,

      
            &G_CSceneManager.m_MeshInScene[i]->m_VertexBuffer->m_BufferD3D11,

            &stride,
            &offset
        );

       DeviceContext.g_pImmediateContextD3D11->IASetIndexBuffer
        (
            G_CSceneManager.m_MeshInScene[i]->m_Index->m_BufferD3D11,
            DXGI_FORMAT_R16_UINT,
            0
        );


       DeviceContext.g_pImmediateContextD3D11->DrawIndexed(G_CSceneManager.m_MeshInScene[i]->m_IndexNum, 0, 0);
    }

    
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    CSwap.g_pSwapChainD3D11->Present(0, 0);
}
#endif