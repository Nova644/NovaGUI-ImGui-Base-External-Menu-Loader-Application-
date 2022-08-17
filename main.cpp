// NOVA GUI is simple base made with ImGui from ocornut : https://github.com/ocornut/imgui
// You can use it for literally everything it's free to use !
// NOVA GUI 2022 - Nova644 & LKS

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define DIRECTINPUT_VERSION 0x0800
#include "main.h"

int main(int, char**)
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, AppClass, NULL };
    RegisterClassEx(&wc);
    hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, AppClass, AppName, WS_POPUP, (desktop.right / 2) - (WindowWidth / 2), (desktop.bottom / 2) - (WindowHeight / 2), WindowWidth, WindowHeight, 0, 0, wc.hInstance, 0);

    //SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);

    if (CreateDeviceD3D(hwnd) < 0)
    {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    DefaultFont = io.Fonts->AddFontFromFileTTF(skCrypt("C:\\Windows\\Fonts\\seguihis.ttf"), 24.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::CustomColor();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    static bool open = true;
    DWORD dwFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        if (!open) ExitProcess(EXIT_SUCCESS);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(WindowWidth, WindowHeight), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        {
            ImGui::Begin(AppName, &open, dwFlag);
            {
                if (ImGui::BeginPopupContextItem("Credits"))
                {
                    ImGui::Separator();
                    ImGui::Text(" ImGui from Ocornut\n Special thanks to KleskBY, LKS & Sprtx\n Develop by Nova644");
                    ImGui::Separator();
                    ImGui::EndPopup();
                }
                if (currentColumn == 0)
                {
                    ImGui::Separator(); ImGui::NewLine();
                    ImGui::Text(skCrypt("\t\t\t\t\t\t\t\t\t\tWelcome to NOVA GUI !\n\t\t\t\t\t\tThe best ImGui Base for literally everything !\n\t\t\t\t\t\t\t\t\t\t\t\t\t  Enjoy !"));
                    ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();
                    if (ImGui::Button(skCrypt("    Login    "), ImVec2(186, 29)))
                    {
                        currentColumn = 1;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(skCrypt("    Credits    "), ImVec2(186, 29)))
                    {
                        ImGui::OpenPopup("Credits");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(skCrypt("    Exit    "), ImVec2(186, 29)))
                    {
                        ::exit(0);
                    }
                    ImGui::NewLine(); ImGui::Separator(); ImGui::Text(skCrypt("\t\t\t\t\t\t\t\t\t       by Nova644 & LKS")); ImGui::SameLine();
                }
                if (currentColumn == 1)
                {
                    ImGui::Separator();
                    ImGui::NewLine(); ImGui::Text("  "); ImGui::SameLine();
                    ImGui::InputText("  :  Username", username, IM_ARRAYSIZE(username));
                    ImGui::Text("  "); ImGui::SameLine();
                    ImGui::InputText("  :  Password", pass, IM_ARRAYSIZE(pass));
                    ImGui::Text("  "); ImGui::SameLine();
                    ImGui::InputText("  :  Key", key, IM_ARRAYSIZE(key));
                    ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::NewLine();
                    if (ImGui::Button(skCrypt("    Login    "), ImVec2(186, 29)))
                    {
                        //Make your login auth system her.
                        currentColumn = 2; //only for educational purposes
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(skCrypt("    Menu    "), ImVec2(186, 29)))
                    {
                        currentColumn = 0;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(skCrypt("    Exit    "), ImVec2(186, 29)))
                    {
                        ::exit(0);
                    }
                    ImGui::NewLine();
                    ImGui::Separator(); ImGui::Text(skCrypt("\t\t\t\t\t\t\t\t\t       by Nova644 & LKS")); ImGui::SameLine();
                }
                if (currentColumn == 2)
                {
                    //Arrive here you are connected with your authentication system
                }
            }
        }
        ImGui::End();

        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
