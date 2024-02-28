#include "header.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
std::vector<int> nums;
HMENU hmenu;
HWND Butt[16], ListBox, Edit, SpinC, Progrs;
int f;
int range;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    // создаём главное окно приложения на основе модального диалога
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void StartGame(HWND hwnd) {
    srand(time(0));
    int num, s[16];
    wchar_t* tnum;
    for (size_t i = 0; i < 16; i++)
    {
        tnum = new wchar_t[4];
        num = rand() % 100;
        s[i] = num;
        nums.push_back(s[i]);
        _itow(s[i], tnum, 10);
        SetWindowText(Butt[i], tnum);
    }
    sort(nums.begin(), nums.end());
    reverse(nums.begin(), nums.end());
    tnum = new wchar_t[4];
    GetWindowText(Edit, tnum, 4);
    range = _wtoi(tnum);
    if (tnum == L"0") {

    }
    else {
        SendMessage(Progrs, PBM_SETRANGE, 0, MAKELPARAM(0, range));
    }
    delete[] tnum;
}
void GameOver(HWND hwnd) {
    KillTimer(hwnd, 1);
    f = 0;
    nums.clear();
    SendMessage(ListBox, LB_RESETCONTENT, 0, 0);
    for (size_t i = 0; i < 16; i++)
    {
        SetWindowText(Butt[i], TEXT("0"));
    }
    SendMessage(Progrs, PBM_SETPOS, 0, 0);
    MessageBox(hwnd, TEXT("Game Over"), TEXT("Game Over!"), MB_OK);
}
void Check(int btn, HWND hwnd) {
    wchar_t* tnum = new wchar_t[4];
    GetWindowText(Butt[btn - 1001], tnum, 4);
    if (nums.back() == _wtoi(tnum)) {
        nums.pop_back();
        SendMessage(ListBox, LB_ADDSTRING, 0, LPARAM(tnum));
    }
    else {
        GameOver(hwnd);
    }
    if (SendMessage(ListBox, LB_GETCOUNT, 0, 0) == 16) {
        KillTimer(hwnd, 1);
        f = 0;
        nums.clear();
        SendMessage(ListBox, LB_RESETCONTENT, 0, 0);
        for (size_t i = 0; i < 16; i++)
        {
            SetWindowText(Butt[i], TEXT("0"));
        }
        SendMessage(Progrs, PBM_SETPOS, 0, 0);
        MessageBox(hwnd, TEXT("Win!"), TEXT("Win!"), MB_OK);
    }
    delete[] tnum;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    switch (message)
    {
    case WM_CLOSE: {
        EndDialog(hwnd, 0);
        return TRUE;
        break;
    }
    case WM_INITDIALOG: {
        hmenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
        SetMenu(hwnd, hmenu);
        ListBox = GetDlgItem(hwnd, IDC_LIST1);
        SpinC = GetDlgItem(hwnd, IDC_SPIN1);
        Progrs = GetDlgItem(hwnd, IDC_PROGRESS1);
        Edit = GetDlgItem(hwnd, IDC_EDIT1);
        for (size_t i = 0; i < 16; i++)
        {
            Butt[i] = GetDlgItem(hwnd, 1001 + i);
            SetWindowText(Butt[i], TEXT("0"));
        }
        SetWindowText(Edit, L"60");
        SendMessage(SpinC, UDM_SETBUDDY, WPARAM(Edit), 0);
        
        f = 0;
        break;
    }
    case WM_TIMER: {
        f++;
        SendMessage(Progrs, PBM_SETPOS, f, 0);
        if (range == SendMessage(Progrs, PBM_GETPOS, 0, 0)) {
            GameOver(hwnd);
        }
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wp)) {
        case ID_NewGame: {
            StartGame(hwnd);
            SetTimer(hwnd, 1, 1000, NULL);
            break;
        }
        case ID_Exit: {
            EndDialog(hwnd, 0);
            return TRUE;
            break;
        }
        }
        if (LOWORD(wp) > 1000 && LOWORD(wp) < 1017) {
            Check(LOWORD(wp), hwnd);
        }
        break;
    }
    }
    return FALSE;
}