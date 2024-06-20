/*                                           NOTEPAD_1.0 - Created By 3spectrx                                                                */

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <commctrl.h>
#include <ShellAPI.h>

#define IDM_FILE_OPC 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_EXIT 4
#define ABRIR 5

HWND File;
HWND hTexto;

void SaveToFile(HWND hwnd, LPCWSTR szFileName) {
    FILE* file = _wfopen(szFileName, L"w, ccs=UTF-8");
    if (file != NULL) {
        int len = GetWindowTextLengthW(hTexto);
        if (len > 0) {
            wchar_t* buffer = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
            if (buffer) {
                GetWindowTextW(hTexto, buffer, len + 1);
                fputws(buffer, file);
                free(buffer);
            }
        }
        fclose(file);
    } else {
        MessageBoxW(hwnd, L"Erro ao salvar o arquivo", L"Erro", MB_ICONERROR);
    }
}

void AddControls(HWND hwnd){
     hTexto = CreateWindowW(L"Edit", L" ", WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_NOHIDESEL , 5, 5, 950, 480, hwnd, NULL, NULL, NULL);
}

HMENU createMenu(){
    HMENU hmenu = CreateMenu();
    HMENU hsubMenu = CreatePopupMenu();
    AppendMenuW(hsubMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hsubMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
    AppendMenuW(hsubMenu, MF_STRING, IDM_FILE_EXIT, L"&Exit");
    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR)hsubMenu, L"&Opções");

    return hmenu;

}


LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, PSTR lCmdLine, int nCmdShow){

    WNDCLASS wc = {0};
    wc.hInstance = hinst;
    wc.lpszClassName = _T("NotepadClass");
    wc.lpfnWndProc = WinProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);


    if(!RegisterClass(&wc)){
        MessageBoxW(NULL, L"Falha ao Registrar a Classe!", L"Erro", MB_ICONERROR);

        return EXIT_FAILURE;
    }

    HWND hwnd  = CreateWindowEx(0, _T("NotepadClass"), _T("Notepad_1.0"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                                                                         CW_USEDEFAULT, 980, 550, NULL, NULL, hinst, NULL);
    if(hwnd == NULL){
        MessageBoxW(NULL, L"Erro ao criar a janela", L"Erro", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    HMENU hmenu = createMenu();
    AddControls(hwnd);
    SetMenu(hwnd, hmenu);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    

    return (int)msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){
        case WM_COMMAND:
            switch(LOWORD(wp)){
                case IDM_FILE_OPEN: {
                    OPENFILENAME ofn;
                    wchar_t szFileName[MAX_PATH] = {0};
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = szFileName;
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrFilter = L"Text files (*.txt)\0*.txt\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrInitialDir = TEXT("C:\\");
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    if (GetOpenFileName(&ofn)) {
                        printf("Arquivo Selecionado: %ls", szFileName);

                        FILE* file = _wfopen(szFileName, L"r, ccs=UTF-8");
                        
                        if (file != NULL) {
                            SetWindowTextW(hTexto, L" ");
                            wchar_t buffer[4096];
                            while (fgetws(buffer, sizeof(buffer) / sizeof(wchar_t), file)) {
                                SendMessageW(hTexto, EM_REPLACESEL, TRUE, (LPARAM)buffer);
                            }
                            fclose(file);
                        
                        } else {
                            MessageBoxW(NULL, L"Erro ao abrir o arquivo", L"Erro", MB_ICONERROR);
                            return EXIT_FAILURE;
                        }
                    }
                }
                break;
                case IDM_FILE_SAVE: {
                    // Implementação para salvar arquivo
                    OPENFILENAME ofn;
                    wchar_t szFileName[MAX_PATH] = {0};
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = szFileName;
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrFilter = L"Text files (*.txt)\0*.txt\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrInitialDir = TEXT("C:\\");
                    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

                    if (GetSaveFileName(&ofn)) {
                        SaveToFile(hwnd, szFileName);
                    }
                }
                break;
                case IDM_FILE_EXIT:
                    PostQuitMessage(0);
                    break;
                default:
                    return DefWindowProc(hwnd, msg, wp, lp);
                    break;
            }
            break;
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            SetWindowPos(hTexto, NULL, 0, 0, rect.right, rect.bottom, SWP_NOZORDER);
        } 
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }



    return DefWindowProc(hwnd, msg, wp, lp);
}