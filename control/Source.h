
//#include "afxcview.h"
#include "Leap.h"

using namespace Leap;

class SampleListener : public Listener 
{
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);

  public:
	void DirectionTo(FingerList, Hand);
	Vector setPotitionOfPalm;
	Vector* p;
	bool isEngineRunning;
	float gapDirection;
	int countFlag;
	int* pCountFlag;
	HWND hWnd;

public:
	//CcontrolDlg ccontrolDlg;
	int right;
	int left;
	int up;
	int down;
	int forward;
	int backward;
	CString direction;
	int de;
};