
// PLX-CompilerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "PLX-Compiler.h"
#include "PLX-CompilerDlg.h"
#include "afxdialogex.h"
#include "PLX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 定义全局变量
CEdit * editFile;
CEdit * editError;
CEdit * editInput;
CEdit * editOutput;
CEdit * editCode;

CListCtrl * listLex;
CListCtrl * listCode;
CListCtrl * listCodeHist;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPLXCompilerDlg 对话框



CPLXCompilerDlg::CPLXCompilerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLXCOMPILER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPLXCompilerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPLXCompilerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BUTTON_BROWSE, &CPLXCompilerDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(BUTTON_COMPILE, &CPLXCompilerDlg::OnBnClickedCompile)
	ON_BN_CLICKED(BUTTON_RUN, &CPLXCompilerDlg::OnBnClickedRun)
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPLXCompilerDlg 消息处理程序

BOOL CPLXCompilerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//CBitmap bmp;
	//bmp.LoadBitmap(IDB_BITMAP1);
	//my_brBk.CreatePatternBrush(&bmp);
	//bmp.DeleteObject();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	editFile = (CEdit *)GetDlgItem(EDIT_FILE);
	editError = (CEdit *)GetDlgItem(EDIT_ERROR);
	editInput = (CEdit *)GetDlgItem(EDIT_INPUT);
	editOutput = (CEdit *)GetDlgItem(EDIT_OUTPUT);
	editCode = (CEdit *)GetDlgItem(EDIT_CODE);
	listLex = (CListCtrl *)GetDlgItem(LIST_LEX);
	listCode = (CListCtrl *)GetDlgItem(LIST_PCODE);
	listCodeHist = (CListCtrl *)GetDlgItem(LIST_CODE_HIST);

	//lex_length = 0;
	//sym_length = 0;
	//code_length = 0;
	//errorcnt = 0;

	//初始化listLex
	DWORD dwStyle = listLex->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	listLex->SetExtendedStyle(dwStyle);
	listLex->InsertColumn(0, _T(" "), LVCFMT_LEFT, 25);
	listLex->InsertColumn(1, _T("Identifier"), LVCFMT_LEFT, 110);
	listLex->InsertColumn(2, _T("N/V"), LVCFMT_LEFT, 110);

	//初始化listCode
	listCode->SetExtendedStyle(dwStyle);

	listCode->InsertColumn(0, _T(" "), LVCFMT_LEFT, 25);
	listCode->InsertColumn(1, _T("PCode"), LVCFMT_LEFT, 80);
	listCode->InsertColumn(2, _T("lv"), LVCFMT_LEFT, 50);
	listCode->InsertColumn(3, _T("addr"), LVCFMT_LEFT, 90);

	//初始化listCodeHist
	listCodeHist->SetExtendedStyle(dwStyle);

	listCodeHist->InsertColumn(0, _T("PC"), LVCFMT_LEFT, 50);
	listCodeHist->InsertColumn(1, _T("Pcode"), LVCFMT_LEFT, 80);
	listCodeHist->InsertColumn(2, _T("lv"), LVCFMT_LEFT, 50);
	listCodeHist->InsertColumn(3, _T("addr"), LVCFMT_LEFT, 90);
	listCodeHist->InsertColumn(4, _T("栈帧"), LVCFMT_LEFT, 800);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPLXCompilerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPLXCompilerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	//注释该语句：防止重复调用重画函数
		//CDialogEx::OnPaint();

		//添加代码对话框背景贴图
		CPaintDC dc(this);

		CBitmap bitmap;

		bitmap.LoadBitmap(IDB_BITMAP1);
		//这个IDB_BITMAP1要自己添加

		CBrush brush;

		brush.CreatePatternBrush(&bitmap);

		CBrush *pOldBrush = dc.SelectObject(&brush);

		CRect rect;
		GetClientRect(&rect);
		dc.Rectangle(0, 0, rect.Width(), rect.Height());

		dc.SelectObject(pOldBrush);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPLXCompilerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPLXCompilerDlg::OnBnClickedBrowse()
{
	BOOL isOpen = TRUE;
	CString defaultDir = L"C:\\";   //默认打开的文件路径  
	CString fileName = L"";         //默认打开的文件名  
	CString filter = L"文件 (*.txt; *.pas)|*.txt;*.pas";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"E:\\FileTest\\test.doc";
	INT_PTR result = openFileDlg.DoModal();
	CString cstring_file_path = defaultDir + "\\test.doc";
	if (result == IDOK) {
		cstring_file_path = openFileDlg.GetPathName();

		//显示代码
		editCode->SetWindowText(L"");

		//转换成char *
		int len = WideCharToMultiByte(CP_ACP, 0, cstring_file_path, -1, NULL, 0, NULL, NULL);
		char * string_file_path = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, cstring_file_path, -1, string_file_path, len, NULL, NULL);

		//词法分析
		FILE * ifp;
		if (fopen_s(&ifp, string_file_path, "r") != 0) {
			MessageBox(L"文件打开错误", L"错误", MB_ICONINFORMATION | MB_OK);
			return;
		}
		delete[] string_file_path;

		CString text;
		char c;
		while ((c = fgetc(ifp)) != EOF) {
			if (c == '\n')
				text.AppendChar('\r');
			text.AppendChar(c);
		}
		editCode->SetWindowTextW(text);

		//清除内容
		editError->SetWindowText(L"");
		editInput->SetWindowText(L"");
		editOutput->SetWindowText(L"");
		listLex->DeleteAllItems();
		listCode->DeleteAllItems();
		listCodeHist->DeleteAllItems();

		CWnd::SetDlgItemTextW(EDIT_FILE, cstring_file_path);
	}
}

void CPLXCompilerDlg::OnBnClickedCompile()
{
	//清除内容
	editError->SetWindowText(L"");
	editInput->SetWindowText(L"");
	editOutput->SetWindowText(L"");
	listLex->DeleteAllItems();
	listCode->DeleteAllItems();
	listCodeHist->DeleteAllItems();

	CString str;
	editFile->GetWindowText(str);

	//转换成char *
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char * filepath = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, filepath, len, NULL, NULL);

	//词法分析
	FILE * ifp;
	if (fopen_s(&ifp, filepath, "r") != 0) {
		MessageBox(L"文件打开错误", L"错误", MB_ICONINFORMATION | MB_OK);
		return;
	}
	lex(ifp);
	fclose(ifp);

	delete[] filepath;

	//显示词法分析结果
	for (int i = 0; i < lex_length; i++) {
		CString serial;
		serial.Format(L"%d", i);
		listLex->InsertItem(i, serial);
		lexelem lx = lex_list[i];
		listLex->SetItemText(i, 1, CString(TOKEN_STR[lx.token]));
		if (lx.token == ident_sym) {
			listLex->SetItemText(i, 2, CString(lx.name));
		}
		else if (lx.token == number_sym) {
			CString value;
			value.Format(L"%d", lx.value);
			listLex->SetItemText(i, 2, value);
		}
	}
	if (errorcnt) {
		MessageBox(L"解析错误，无效字符！", L"错误", MB_ICONINFORMATION | MB_OK);
		return;
	}


	//编译
	compile();

	if (errorcnt == 0) {
		//显示Pcode
		for (int i = 0; i < code_length; i++) {
			CString serial;
			serial.Format(L"%d", i);
			listCode->InsertItem(i, serial);
			CString l, a;
			l.Format(L"%d", code[i].l);
			a.Format(L"%d", code[i].a);
			listCode->SetItemText(i, 1, CString(PCODE_STR[code[i].op]));
			listCode->SetItemText(i, 2, l);
			listCode->SetItemText(i, 3, a);
		}
	}
	else {
		MessageBox(L"编译中遇上了错误", L"错误", MB_ICONINFORMATION | MB_OK);
		return;
	}
}


void CPLXCompilerDlg::OnBnClickedRun()
{
	//清除内容
	editOutput->SetWindowText(L"");
	listCodeHist->DeleteAllItems();

	if (code_length > 0 && errorcnt == 0) {
		//把输入框的内容存到一个txt中
		CStdioFile input_file;
		CString input_str;
		editInput->GetWindowText(input_str);
		if (input_file.Open(L"interpreter_input.txt", CFile::modeCreate | CFile::modeWrite)) {
			input_file.WriteString(input_str);
			input_file.Close();
		}

		FILE * ifp;
		fopen_s(&ifp, "interpreter_input.txt", "r");
		interpret(ifp);
		fclose(ifp);
	}
	else {
		MessageBox(L"没有找到正确编译的代码", L"错误", MB_ICONINFORMATION | MB_OK);
	}
}

//HBRUSH CPLXCompilerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  在此更改 DC 的任何特性
//	
//	if (pWnd == this) {
//		return my_brBk; //主窗口使用这个背景刷
//	}
//
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
//}
