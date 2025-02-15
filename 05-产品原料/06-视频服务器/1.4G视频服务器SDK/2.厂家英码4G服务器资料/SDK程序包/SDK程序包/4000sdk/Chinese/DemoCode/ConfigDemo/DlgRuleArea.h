#pragma once
#include "afxwin.h"


// CDlgRuleArea 对话框
class CDlgRuleArea : public CDialog
{
	DECLARE_DYNAMIC(CDlgRuleArea)

public:
	CDlgRuleArea(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRuleArea();

	virtual BOOL OnInitDialog();
	TCHAR m_strFileName[256];
	HINSTANCE OldResourceHanlde;
	void CatchPic();
	void StartUpPreview(BOOL bPreview);
	BOOL GetAppPath(char *szPath);
	void ReInit();
	bool StructToWinData();
	bool StructToWinData(int nChannelNum, bool bCopy = false);	

	BOOL InitWndShow();
	void DrawRules(CDC *pDC, SDK_URP_TRIPWIRE_RULE_PARA_S *pRules);
	void DrawRules(CDC *pDC, SDK_URP_PERIMETER_RULE_PARA_S *pRules);
	int PointToLine( URP_SDK_POINT_S, URP_SDK_POINT_S startPoint, URP_SDK_POINT_S endPoint);
	bool CheckOutArea(CPoint point);
	void drawline(CDC *pDC, URP_SDK_POINT_S ptStart, URP_SDK_POINT_S ptEnd);
	bool CheckSelect(CPoint pt);
	void Drawarrows(CDC* pDC,URP_SDK_POINT_S point1, URP_SDK_POINT_S point2);
	//保存当前规则
	bool SaveCurRule();
	int m_nChannelNum;
	int m_nwidth;
	int m_nheight;
	CStatic m_bkPic;
	CMenu m_contextMenu;
	void InitDlgInfo( SDK_RULECONFIG* pRule,int nRule,int nChannelNum);
	
	int		m_nRule;		//
	int		m_nLineType;	// 0 直线 1 曲线 2 矩形 3 不规则矩形
	int        m_nTripwireNum;
	int        m_nPerimeterNum;
	bool m_bGet[NET_MAX_CHANNUM];			//记录每个通道是否获取过数据
	bool m_bGetType;			//获取各个通道类型

	
	int m_nCurIndex;		//当前选中规则下标
	
	SDK_URP_TRIPWIRE_S m_curTripwire;	//记录当前在增加的规则
	SDK_URP_TRIPWIRE_RULE_PARA_S m_curTripwireRule;	
	SDK_URP_PERIMETER_RULE_PARA_S m_curPerimeterRule;
	//记录显示图片位置离对话框的距离，方便画线
	int m_nPicLeft;
	int m_nPicTop;
	CPen m_pen;
	CPen m_SelectPen;
	bool m_MouseDown;
	CPoint m_ptOrigin;
	CPoint m_ptEnd;
	CBitmap m_bmpBack_Up;
	bool m_bSelected;		//是否选中一个规则
	bool m_bDrawLine;		//是否处于等待画线状态
	bool m_bCanDraw;
	int m_nPicWidth;
	int m_nPicHeight;
// 对话框数据
	enum { IDD = IDD_DIALOG_RULE_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnMenucontextMenuEdit();
	afx_msg void OnMenucontextMenuDelete();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
