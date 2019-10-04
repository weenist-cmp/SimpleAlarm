/*==============================================================================
SimpleAlarm
20190929...Start
20191003...화면보호기 상태일때 계속 연주, 파일이 없을때 기본 경고음 연주안함
==============================================================================*/
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <Mmsystem.h>
#include "SimpleAlarm_res.h"
//==============================================================================
const int def_Alarm_Time = 600; const int def_Alarm_Modify = 300; const int def_Alarm_Min = 300; const int def_Alarm_Max = 36000; const char *def_Alarm_Sound = "Simple_drum2.wav";
char Alarm_Sound[999]; HWND hwnd = NULL, hti; HINSTANCE hInst; int WindowWidth=200,WindowHeight=58, Alarm_Time, Alarm_Modify, Alarm_Min, Alarm_Max, pwav;
DWORD WindowStyle = WS_CAPTION | WS_POPUPWINDOW;
time_t Time_CheckStart;
//==============================================================================
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void PreMain(void);
void EndMain(void);
void ShowAlarm(void);
void StartWait(void);
void StopWait(void);
void CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );
//==============================================================================
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
  RECT r; MSG msg; WNDCLASSEX wc;

  hInst = hThisInstance; r.left=0; r.top=0; r.right = WindowWidth; r.bottom = WindowHeight; AdjustWindowRect(&r,WindowStyle,FALSE); wc.hInstance = hThisInstance; wc.lpszClassName = P_Name; wc.lpfnWndProc = WindowProcedure; wc.style = CS_DBLCLKS; wc.cbSize = sizeof (WNDCLASSEX); wc.hIcon = LoadIcon (NULL, IDI_APPLICATION); wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION); wc.hCursor = LoadCursor (NULL, IDC_ARROW); wc.lpszMenuName = NULL; wc.cbClsExtra = 0; wc.cbWndExtra = 0; wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255,255,0));
  if(!RegisterClassEx (&wc)) return 0;
  hwnd = CreateWindowEx(0,P_Name,F_Description,WindowStyle,0,0,r.right-r.left,r.bottom-r.top,HWND_DESKTOP,NULL,hThisInstance,NULL);

  PreMain(); while(GetMessage (&msg, NULL, 0, 0)){ TranslateMessage(&msg); DispatchMessage(&msg); } EndMain(); UnregisterClass( P_Name, hThisInstance ); return msg.wParam;
}
//==============================================================================
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message){
    case WM_CREATE:
      CreateWindow("STATIC","",WS_CHILD|WS_VISIBLE|WS_BORDER|ES_CENTER|BS_CENTER,11,11,WindowWidth-20,WindowHeight-20,hwnd,(HMENU)ID_tInfo,hInst,NULL);
      hti = CreateWindow("STATIC","",WS_CHILD|WS_VISIBLE|WS_BORDER|ES_CENTER|SS_LEFT,10,10,WindowWidth-20,WindowHeight-20,hwnd,(HMENU)ID_tInfo,hInst,NULL);
      break;
    case WM_DESTROY:
      KillTimer(hwnd, 0); PostQuitMessage (0);
	  break;
    case WM_SYSCOMMAND:
      if(wParam==SC_RESTORE) StopWait();
      if(wParam==SC_CLOSE){ KillTimer(hwnd, 0); PostQuitMessage (0); }
      break;
    case WM_KEYDOWN:
      if(wParam==VK_ESCAPE) PostQuitMessage(0);
      if(wParam==VK_UP){ Alarm_Time+=Alarm_Modify; ShowAlarm(); }
      if(wParam==VK_DOWN){ Alarm_Time-=Alarm_Modify; ShowAlarm(); }
      if(wParam==VK_SPACE || wParam==VK_RETURN) StartWait();
      break;
    default:
      return DefWindowProc (hwnd, message, wParam, lParam);
  }
  return 0;
}
//==============================================================================
void PreMain(void)
{
  int ac, tv1,tv2,tv3,tv4; char buf[999]; LPWSTR *av, CmdLine;

  CmdLine = GetCommandLineW(); ac=0; av = CommandLineToArgvW(CmdLine, &ac); Alarm_Time = 0;
  if(ac>4){
    wcstombs(buf,av[1],999); tv1 = atoi(buf); wcstombs(buf,av[2],999); tv2 = atoi(buf); wcstombs(buf,av[3],999); tv3 = atoi(buf); wcstombs(buf,av[4],999); tv4 = atoi(buf);
    if(tv1>0 && tv2>0 && tv3>0 && tv3<=tv4){
      Alarm_Time = tv1; Alarm_Modify = tv2; Alarm_Min = tv3; Alarm_Max = tv4;
    }
  }
  if(Alarm_Time<1){ Alarm_Time=def_Alarm_Time; Alarm_Modify=def_Alarm_Modify; Alarm_Min=def_Alarm_Min; Alarm_Max=def_Alarm_Max; }
  if(ac>5){ wcstombs(buf,av[5],999); strcpy(Alarm_Sound,buf); }else strcpy(Alarm_Sound,def_Alarm_Sound);
  LocalFree(av);

  ShowWindow(hwnd,SW_SHOW); UpdateWindow(hwnd); Time_CheckStart=0; pwav=0; SetTimer(hwnd,0,1000,TimerProc); ShowAlarm();
}
//==============================================================================
void EndMain(void)
{
//
}
//==============================================================================
void ShowAlarm(void)
{
  char buf[999]; SYSTEMTIME st;

  if(Alarm_Time<Alarm_Min) Alarm_Time = Alarm_Max;
  if(Alarm_Time>Alarm_Max) Alarm_Time = Alarm_Min;

  GetLocalTime(&st);
  sprintf(buf,"%d ... %d / 60\n%02d:%02d:%02d", Alarm_Time/60, Alarm_Time, st.wHour,st.wMinute,st.wSecond );
  SendMessage(hti,WM_SETTEXT,0,(LPARAM)buf);
}
//==============================================================================
LONG WINAPI DefScreenSaverProc(HWND,UINT,WPARAM,LPARAM);

void CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
  DWORD dws; time_t ctm;
  BOOL ssa;

//191003
// 화면보호기 실행
//  SendMessage(HWND_BROADCAST,WM_SYSCOMMAND, SC_SCREENSAVE, 0L);

// 화면보호기 실행중지 방법을 찾기 어려워서 현재 실행중인지 체크
//191003
  ssa=FALSE;
  SystemParametersInfo(SPI_GETSCREENSAVERRUNNING,0,&ssa,0);
  if(ssa==FALSE && pwav==1){ PlaySound(NULL, 0, 0); pwav=0; } // 화면보호기 빠져 나왔을때 소리알림 중지 //191003

  if(Time_CheckStart==0){ ShowAlarm(); return; }
  ctm = time(NULL); if(ctm<=Time_CheckStart) return;
  dws = ctm - Time_CheckStart;
  if(dws >= (DWORD)Alarm_Time){
    Time_CheckStart=0; ShowWindow(hwnd,SW_RESTORE); ShowAlarm(); UpdateWindow(hwnd);
    PlaySound((LPCTSTR)Alarm_Sound, NULL, (ssa==FALSE ? 0 : SND_LOOP) | SND_NODEFAULT | SND_FILENAME); //191003
    if(ssa==TRUE) pwav=1; // 화면보호기 실행중일때 1 //191003
  }
}
//==============================================================================
void StartWait(void)
{
  Time_CheckStart = time(NULL);
  ShowWindow(hwnd,SW_MINIMIZE);
}
//==============================================================================
void StopWait(void)
{
  Time_CheckStart=0;
  ShowWindow(hwnd,SW_RESTORE); ShowAlarm(); UpdateWindow(hwnd);
  Beep(440,500);
}
//==============================================================================
