#include <windows.h>
#include <windowsx.h>
#include "key_acsII.h"

#define ROW_NUM 5    //行数
#define COL_NUM 17  //列数
#define WM_CHILDWIN  WM_USER+1    //主窗口通知子窗口的自定义消息

BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;                                  //全程变量——实例句柄
int        x, y;
TCHAR Childsz[10];

/*typedef struct tagkeyinfo
{
	char upper;
	char lower;
	bool bshow;
}keyinfo, *pkeyinfo;
keyinfo keys[2][2] = {
	{'A','a',TRUE},
	{'B','b',TRUE},
}*/ //课上讲的方法
int WINAPI WinMain(HINSTANCE hInstance,         //当前实例句柄
	HINSTANCE hPrevInstance,     //前一个实例句柄
	PSTR szCmdLine,              //命令行字符
	int iCmdShow)                //窗口显示方式
{
	MSG msg;

	if (!InitWindow(hInstance, iCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL InitWindow(HINSTANCE hInstance, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("ChildWin");		  //应用程序名称
	HWND hwnd;                                    //窗口句柄
	WNDCLASS wcMainWnd;                           //窗口类结构

	wcMainWnd.style = CS_VREDRAW | CS_HREDRAW;
	wcMainWnd.lpfnWndProc = WinProc;
	wcMainWnd.cbClsExtra = 0;
	wcMainWnd.cbWndExtra = 0;
	wcMainWnd.hInstance = hInstance;
	wcMainWnd.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcMainWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcMainWnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcMainWnd.lpszMenuName = NULL;
	wcMainWnd.lpszClassName = szAppName;

	if (!RegisterClass(&wcMainWnd))
	{
		MessageBox(NULL, TEXT("注册窗口类失败！"), szAppName,
			MB_ICONERROR);
		return 0;
	}
	hInst = hInstance;
	hwnd = CreateWindow(szAppName, 						//窗口类名称
		TEXT("Windows 软键盘"),  //窗口标题
		WS_OVERLAPPEDWINDOW,				//窗口风格
		100,								//窗口位置的x坐标
		100,								//窗口位置的y坐标
		400,								//窗口的宽度
		300,								//窗口的高度
		NULL,								//父窗口句柄
		NULL,								//菜单句柄
		hInstance,							//应用程序实例句柄
		NULL);								//窗口创建数据指针

	if (!hwnd) return FALSE;

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	static	HWND	hWndEdit;

	static HWND hChildWnd[17][ROW_NUM];  //子窗口句柄
	static int  nXBox, nYBox;                 //子窗口宽度和高度
	static WORD nChildWin_ID;                 //被命中的子窗口标识
	static int  nRow, nCol;                   //被命中的子窗口位置（即行列号）

	char		c = NULL;
	WNDCLASS    wcChild;                      //子窗口类
	TCHAR szChildName[] = TEXT ("ChildWin_Child");    //子窗口类名称

	switch (message)
	{
	case WM_CREATE:
		hWndEdit = CreateWindow(TEXT("edit"),
			NULL,
			WS_CHILD|WS_VISIBLE|
			WS_HSCROLL|WS_VSCROLL|WS_BORDER|ES_LEFT|
			ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
			0,0,0,0, hWnd, (HMENU)1, 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);

		wcChild.style         = CS_VREDRAW | CS_HREDRAW;
		wcChild.lpfnWndProc   = ChildWndProc;
		wcChild.cbClsExtra    = 0;
		wcChild.cbWndExtra    = 5*sizeof (long);
		wcChild.hInstance     = hInst;
		wcChild.hIcon         = NULL;
		wcChild.hCursor       = LoadCursor (NULL, IDC_ARROW);
		wcChild.hbrBackground = (HBRUSH)GetStockObject (LTGRAY_BRUSH);
		wcChild.lpszMenuName  = NULL;
		wcChild.lpszClassName = szChildName ;

		if (!RegisterClass (&wcChild))
		{
			MessageBox (NULL, TEXT ("注册子窗口类失败！"), szChildName,
				MB_ICONERROR);
			return 0;
		}

		//创建类似“画图”程序中绘图工具栏的各工具子窗口
		for (y=0; y<1; y++)
		{
			for (x=1; x<13; x++)
			{
				nChildWin_ID = y<<4|x;             //子窗口标识值
				hChildWnd[x][y] = CreateWindow (szChildName, NULL,
					WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
					0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
					hInst, (LPVOID)c);
			}
		}
		for (y=1; y<2; y++)
		{
			for (x=1; x<14; x++)
			{
				nChildWin_ID = y<<4|x;             //子窗口标识值
				hChildWnd[x][y] = CreateWindow (szChildName, NULL,
					WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
					0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
					hInst, (LPVOID)c);
			}
		}

			for (y=2; y<3; y++)
			{
				for (x=1; x<12; x++)
				{
					nChildWin_ID = y<<4|x;             //子窗口标识值
					hChildWnd[x][y] = CreateWindow (szChildName, NULL,
						WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
						0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
						hInst, NULL);
				}
			}

			for (y=3; y<4; y++)
			{
				for (x=1; x<11; x++)
				{
					nChildWin_ID = y<<4|x;             //子窗口标识值
					hChildWnd[x][y] = CreateWindow (szChildName, NULL,
						WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
						0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
						hInst, NULL);
				}
			}

			//令0号子窗口为当前命中的子窗口
			nChildWin_ID = 0;
			nRow = 0;
			nCol = 0;
			return 0;

	case WM_SETFOCUS:   
		SetFocus(hWndEdit);   
		return 0;

	case WM_SIZE:	
		nXBox = LOWORD (lParam)/ROW_NUM/3; // 宽
		nYBox = HIWORD (lParam)/COL_NUM; // 高

		//主窗口大小改变时，重新确定各子窗口在主窗口客户区的位置
		for (y=0; y<1; y++){
			for (x=1; x<13; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}
		}

		for (y=1; y<2; y++)
		{
			for (x=1; x<14; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}
		}
		
		for (y=2; y<3; y++)
			for (x=1; x<12; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}

		for (y=3; y<4; y++)
			for (x=1; x<11; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}
		
		MoveWindow(hWndEdit, 0, 0,
			LOWORD(lParam), HIWORD(lParam) * 2 / 3, TRUE);

			return 0;

	case WM_COMMAND:                //子窗口被命中的通知消息
		//当子窗口被命中时发送此消息通知主窗口，以便由主窗口确定是否命中另一个子窗口，
		//若是，则发送消息通知子窗口函数改变前一次被命中子窗口的状态，
		if (!(LOWORD (wParam) == nChildWin_ID))
		{
			SendMessage ((HWND)hChildWnd[nCol][nRow], WM_CHILDWIN, 0, 0L);

			//记录新的被命中的子窗口
			nChildWin_ID = LOWORD (wParam);
			nRow = nChildWin_ID/16;
			nCol = nChildWin_ID%16;
		}

		if (lParam)        //编辑控件通知消息      
			if ((LOWORD (wParam) == 1) 
				&&(HIWORD(wParam) == EN_ERRSPACE             
				|| HIWORD (wParam) == EN_MAXTEXT)) 
				MessageBox (hWnd,TEXT ("编辑控件内存溢出"), 
					TEXT ("菜单示例程序"), MB_OK|MB_ICONSTOP);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

} //函数 WinProc 结束

LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	LONG l=0;
	static int xy;
	TCHAR win1;

	switch (message)
	{
	case WM_CREATE:
		xy ++;
		//置各子窗口命中标志为0，（0号子窗口则为1）
		if (GetWindowLong (hWnd, GWL_ID) == 0)
			l=SetWindowLong (hWnd, 0, 1);
		else
			l=SetWindowLong (hWnd, 0, 0);

		SetWindowLong(hWnd, 1, print_abc[xy - 1]);
		SetWindowLong(hWnd, 3, abc[xy - 1]);
		return 0;

	case WM_LBUTTONUP:
		//新的子窗口被命中
		win1 = GetWindowLong(hWnd, 3);
			//置该子窗口命中标志为1
			//SetWindowLong (hWnd, 0, 1);

			//发送消息通知主窗口
			SendMessage (GetParent (hWnd), WM_COMMAND, 
				GetWindowLong (hWnd, GWL_ID),
				(LPARAM) hWnd);
			//VK_RETURN;
			keybd_event(win1, 0, 0, 0); //该句向父窗口发送WM_CHAR消息
		return 0;

	case WM_PAINT:
		hdc = BeginPaint (hWnd, &ps);

		//反相绘制被命中的子窗口
		
		GetClientRect(hWnd, &rect);
		
		wsprintf(Childsz, L"%c", GetWindowLong(hWnd, 1));    //显示ID
		DrawText(hdc, Childsz, -1, &rect, 
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint (hWnd, &ps);
		return 0;

	case WM_DESTROY: 
		PostQuitMessage (0);
		return 0;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);

} //函数 ChildWinProc 结束