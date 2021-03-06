
// PLX-CompilerDlg.h: 头文件
//

#pragma once


// CPLXCompilerDlg 对话框
class CPLXCompilerDlg : public CDialogEx
{
// 构造
public:
	CPLXCompilerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLXCOMPILER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
//	CBrush my_brBk;
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedCompile();
	afx_msg void OnBnClickedRun();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnEnChangeCode();
};

extern CEdit * editFile;
extern CEdit * editError;
extern CEdit * editInput;
extern CEdit * editOutput;
extern CEdit * editCode;
extern CListCtrl * listLex;
extern CListCtrl * listCode;
extern CListCtrl * listCodeHist;

void AppendTextToEditCtrl(CEdit * edit, LPCTSTR pszText);