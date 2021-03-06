#include<iostream>
#include "libxl.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <conio.h>
#include<Windows.h>
#include<cstdlib>
#include <queue>

using namespace libxl;
using namespace std;

#define HASH_SIZE 300
#define HASH_M 97//哈希函数的模
#define MAXVEX 61
#define INF 65535

char* w2c(char* pcstr, const wchar_t* pwstr, size_t len)//此函数支持中文读取和显示，需加入否则中文内容乱码
{
    int nlength = wcslen(pwstr);//获取转换后的长度
    int nbytes = WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL); //映射一个unicode字符串到一个多字节字符串
    if (nbytes > len)   nbytes = len;
    // 通过以上得到的结果，转换unicode 字符为ascii 字符
    WideCharToMultiByte(0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);
    return pcstr;
}
wchar_t* StringToWchar(const string& pKey)
{
    const char* pCStrKey = pKey.c_str();
    //第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    wchar_t* pWCStrKey = new wchar_t[pSize];
    //第二次调用将单字节字符串转换成双字节字符串
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}

typedef struct {
    int node1;//点1
    int node2;//点2
    int num;//边长
}Val;

typedef struct {
    int node;//点
    string name;//股票名称
    string stock_code;//股票代码
    int sco;//评分
}Score;

typedef struct {
    string date;		// 日期 
    double open_price;	// 开盘价 
    double close_price;	// 收盘价 
    double max_price;	// 最高价 
    double min_price;	// 最低价 
    double volume;		// 成交量 
    int turnover;		// 成交额（万元） 
    string rate;		// 换手率% 
    double iod_amount;	// 涨跌额 
    string iod_rate;	// 涨跌幅% 
}deal;

typedef struct {
    string stock_code;	// 股票代码 
    string short_name;	// 股票简称 
    string trade_code;	// 行业编码 
    string first_trade;	// 所属一级行业名称 
    string second_trade;// 所属二级行业名称 
    string bourse;		// 上市交易所 
    string name;		// 公司全称 
    string launch_date;	// 上市日期 
    string province;	// 省份 
    string city;		// 城市 
    string legal_person;// 法人 
    string address;		// 地址 
    string web;			// 网址 
    string email;		// 邮箱 
    string phone;		// 电话 
    string business;	// 主营业务 
    string scpope;		// 经营范围 
    deal* deal;	        //详细信息 
    int length;			//详细信息长度 
    string maxiodrate;   //最大涨跌幅
}Stock;

Stock sto[207];
typedef Stock etype;
int total = 0, m = 0;
bool dele_flag = true, search_flag = true;


typedef struct HNode {   //哈希表
    Stock data;
    struct HNode* next;
}HNode, * HLinkList;

HLinkList H[97];//哈希链表
typedef struct dealNode {    //单链表  存储每支股票的交易信息
    string name;
    string code;
    deal data[167];
    struct dealNode* next;
}dealNode, * dealList;
dealList L;

typedef struct dateNode { //单链表    存储某一日所有股票的信息
    string date;
    string code;
    string name;
    double open_price;
    double close_price;
    string iod_rate;
    struct dateNode* next;
}dateNode, * dateList;

typedef struct {
    int node;
    string name;   //名称
    string stock_code;	// 股票代码 
    string first_trade;	// 所属一级行业名称 
    string iodrate;  //最大涨跌幅
    string date;     //最大涨跌幅日期
    double close_price;  //最近一日收盘价
    int sco;  //评分
}Stock60;
Stock60 s60[61];

typedef struct
{
    int matrix[61][61];
    int numNodes, numEdges;
    int vexnum;  //顶点个数
    int edge;   //边的条数
    int** arc; //邻接矩阵
    string* information; //记录每个顶点名称
} Graph;
Graph Gp;
int e[84][84];



//字符串转double
double stringchange(string s)
{

    char a[100];
    for (int i = 0; i < s.size() - 1; i++) {
        a[i] = s[i];
    }
    return atof(a);
}

// 获取文本行数
int get_text_count(string code)
{
    string path = "node\\" + code + ".txt";
    ifstream infile(path.c_str());
    if (!infile.is_open())
    {
        cout << "未成功打开文件!234" << endl;
    }
    string temp;
    int count = 0;
    while (getline(infile, temp)) {
        count++;
    }
    return count - 1;	// 数据从从第二行起 
}

//读取交易价格信息
void Read_deal(Stock& stock, int num)
{
    stock.deal = new deal[num];
    string path = "node\\" + stock.stock_code + ".txt";
    ifstream in(path.c_str());
    if (!in.is_open())
    {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }
    int i = 0;
    string temp;
    getline(in, temp);	// 跳过第一行 
    while (!in.eof())
    {
        in >> stock.deal[i].date;
        in >> stock.deal[i].open_price;
        in >> stock.deal[i].close_price;
        in >> stock.deal[i].max_price;
        in >> stock.deal[i].min_price;
        in >> stock.deal[i].volume;
        in >> stock.deal[i].turnover;
        in >> stock.deal[i].rate;
        in >> stock.deal[i].iod_amount;
        in >> stock.deal[i].iod_rate;
        i++;
    }
    stock.length = i;

   
}

//读取边信息
void Read_val(Val value[])
{

    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置相应的key激活相关功能(购买)，否则第一行无法操作
    if (book->load(L"60支股票信息1.xlsx"))//文件放入工程目录里
    {
        Sheet* sheetread = book->getSheet(0);//选取第一个表
        if (sheetread)
        {
            for (int row = sheetread->firstRow(); row < sheetread->lastRow(); ++row)//行遍历
            {
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//列遍历
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//如果每个单元格内容为字符串类型
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//从单元格中读取字符串及其格式
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//定义空间大小来存放读出的内容，wcslen()函数返回给定内容t的长度
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//初始化清零操作
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr的内容读出来后要进行中文编码的转换，这个为转换函数
                        //cout << pcstr << endl;//输出
                        free(pcstr);
                    }
                    else if (celltype == CELLTYPE_NUMBER)//如果每个单元格内容为数值类型
                    {
                        double result = sheetread->readNum(row, col);
                        /* cout << result << endl;
                         cout << "row:" << row << "col:" << col << endl;
                         */
                        switch (col)
                        {
                        case(0):    value[row].node1 = result;
                            break;
                        case(1):  value[row].node2 = result;
                            break;
                        case(2):value[row].num = result;
                            break;
                        default:
                            break;
                        }
                    }
                }
                
            }
        }
        
        ////保存excel
        book->save(L"60支股票信息1.xlsx");
        book->release();
    }
}

//读取评分信息
void Read_score(Score score[])
{

    //表2中点和评分为数值类型
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置相应的key激活相关功能(购买)，否则第一行无法操作
    if (book->load(L"60支股票信息2.xlsx"))//文件放入工程目录里
    {
        Sheet* sheetread = book->getSheet(0);//选取第一个表
        if (sheetread)
        {
            for (int row = sheetread->firstRow(); row < sheetread->lastRow(); ++row)//行遍历
            {
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//列遍历
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//如果每个单元格内容为字符串类型
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//从单元格中读取字符串及其格式
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//定义空间大小来存放读出的内容，wcslen()函数返回给定内容t的长度
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//初始化清零操作
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr的内容读出来后要进行中文编码的转换，这个为转换函数
                        //cout << pcstr << endl;//输出
                        switch (col)
                        {

                        case(1):  score[row].name = pcstr;
                            break;
                        case(2):score[row].stock_code = pcstr;
                            break;
                        default:
                            break;
                        }
                        free(pcstr);

                    }
                    else if (celltype == CELLTYPE_NUMBER)//如果每个单元格内容为数值类型
                    {
                        double result = sheetread->readNum(row, col);
                        // cout << result << endl;
                         //cout << "row:" << row << "col:" << col << endl;

                        switch (col)
                        {
                        case(0):    score[row].node = result;
                            break;
                        case(1):  score[row].name = result;
                            break;
                        case(2):score[row].stock_code = result;
                            break;
                        case(3):score[row].sco = result;
                            break;
                        default:
                            break;
                        }

                    }
                }
            }
        }
        for (int i = 1; i <= 60; i++)
        {
            s60[i].node = score[i].node;
            s60[i].stock_code = score[i].stock_code;
            s60[i].name = score[i].name;
        }
            

        ////保存excel
        book->save(L"60支股票信息2.xlsx");
        book->release();
    }
}

//读取股票基本信息
void Read_stock(Stock stock[])
{
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置相应的key激活相关功能(购买)，否则第一行无法操作
    if (book->load(L"A股公司简介.xlsx"))//文件放入工程目录里
    {
        Sheet* sheetread = book->getSheet(0);//选取第一个表
        if (sheetread)
        {
            for (int row = sheetread->firstRow() + 1; row < sheetread->lastRow(); ++row)//行遍历
            {

               
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//列遍历
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//如果每个单元格内容为字符串类型
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//从单元格中读取字符串及其格式
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//定义空间大小来存放读出的内容，wcslen()函数返回给定内容t的长度
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//初始化清零操作
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr的内容读出来后要进行中文编码的转换，这个为转换函数
                       // cout << pcstr << endl;


                        switch (col)
                        {
                        case(0):stock[row].stock_code = pcstr;

                            break;
                        case(1):stock[row].short_name = pcstr;
                            break;
                        case(2):stock[row].trade_code = pcstr;
                            break;
                        case(3):stock[row].first_trade = pcstr;
                            break;
                        case(4):stock[row].second_trade = pcstr;
                            break;
                        case(5):stock[row].bourse = pcstr;
                            break;
                        case(6):stock[row].name = pcstr;
                            break;
                        case(7):stock[row].launch_date = pcstr;
                            break;
                        case(8):stock[row].province = pcstr;
                            break;
                        case(9):stock[row].city = pcstr;
                            break;
                        case(10):stock[row].legal_person = pcstr;
                            break;
                        case(11):stock[row].address = pcstr;
                            break;
                        case(12):stock[row].web = pcstr;
                            break;
                        case(13):stock[row].email = pcstr;
                            break;
                        case(14):stock[row].phone = pcstr;
                            break;
                        case(15):stock[row].business = pcstr;
                            break;
                        case(16):stock[row].scpope = pcstr;
                            break;

                        default:
                            break;
                        }

                        free(pcstr);
                    }

                }
                Read_deal(stock[row], get_text_count(stock[row].stock_code));
                
            }
            
        }
        ////保存excel
        book->save(L"A股公司简介.xlsx");
        book->release();
    }
}



//创建哈希表
void InitHash()
{
    for (int i = 0; i < HASH_M; i++) {
        H[i] = new HNode;
        H[i]->next = NULL;
    }
}
//插入哈希表
void InsertHash(Stock& e)
{
    int len = e.stock_code.size();//股票代码的长度
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += (int)e.stock_code[i];
    }//求ASII码之和
    //求index（key）
    int index = sum % HASH_M;
    HLinkList p = H[index];
    while (p->next) {
        p = p->next;
    }            //使p为H[index]的尾指针
    HLinkList q = new HNode;
    q->data = e;
    q->next = NULL;
    p->next = q;//将值插入到当前位置的末尾
}
//创建并导入哈希表
void CreateHash()
{
    InitHash();
    int i = 0;
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置相应的key激活相关功能(购买)，否则第一行无法操作
    if (book->load(L"A股公司简介.xlsx"))//文件放入工程目录里
    {
        Sheet* sheetread = book->getSheet(0);//选取第一个表
        if (sheetread)
        {
            for (int row = sheetread->firstRow() + 1; row < sheetread->lastRow(); ++row)//行遍历
            {
                string company[20]; //用来存每一行读到的信息
                int cnt = 0;
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//列遍历
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//如果每个单元格内容为字符串类型
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//从单元格中读取字符串及其格式
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//定义空间大小来存放读出的内容，wcslen()函数返回给定内容t的长度
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//初始化清零操作
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr的内容读出来后要进行中文编码的转换，这个为转换函数
                        //cout << pcstr << endl;//输出
                        company[++cnt] = pcstr;
                        free(pcstr);
                    }
                    else if (celltype == CELLTYPE_NUMBER)//如果每个单元格内容为数值类型
                    {
                        double result = sheetread->readNum(row, col);
                        stringstream ss;
                        ss << result;
                        ss >> company[++cnt];
                        //cout << result << endl;
                    }
                    if (cnt == 17) break;
                }
                Stock e = { company[1],company[2],company[3],company[4],company[5],company[6],company[7],company[8],company[9],company[10],
                         company[11],company[12],company[13],company[14],company[15],company[16],company[17] };
                sto[++i] = e;
                InsertHash(e);
            }
        }
        ////保存excel
        book->save(L"A股公司简介.xlsx");
        book->release();
    }
}
//求HASH ASL
double HashAsl() //求平均查找长度
{
    int cnt = 0;//所有元素需要比较的次数
    for (int i = 0; i < HASH_M; i++) {
        HLinkList p = H[i]->next;
        int cnt1 = 0;//在单个地址中的比较次数
        while (p) {
            cnt1++;
            p = p->next;
        }
        while (cnt1) {
            cnt += cnt1;
            cnt1--;
        }
        cnt += cnt1;
    }
    double asl = cnt / 200.0;
    return asl;
}
//查询1 基于哈希表的基本信息查询
void Seek_Hash()  
{

    CreateHash();
    string code;
    cout << "请输入要查找的股票代码：" << endl;
    cin >> code;

    //求输入代码的INDEX
    int len = code.size();
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += (int)code[i];
    }
    int index = sum % HASH_M;

    HLinkList p = H[index];//指针指向查询代码在哈希表中的位置

    //输入代码不存在
    int choice = 1;
    while (p->next == NULL&& choice!=2) {
        cout << "查找失败，该股票代码的股票不存在！" << endl;
        cout << "1.继续输入：" << endl;
        cout << "2.退出：" << endl;
        cin >> choice;
        if (choice == 1)
        {
            cin >> code;
            len = code.size();
            sum = 0;
            for (int i = 0; i < len; i++) {
                sum += (int)code[i];
            }
            index = sum % HASH_M;
            p = H[index];
        }
        else if(choice==2) break;
    }
    
    p = p->next;
    while (p) {
        if (p->data.stock_code == code) {
            cout << "查找成功！信息如下：" << endl;
            cout << "股票名称：" << p->data.name << endl;
            cout << "股票代码：" << p->data.stock_code << endl;
            cout << "股票所属一级行业：" << p->data.first_trade << endl;
            cout << "所属二级行业：" << p->data.second_trade << endl;
            cout << "主营业务：" << p->data.business << endl;
            cout << "ASL：" << HashAsl() << endl;
            cout << endl;
            break;
        }
        p = p->next;
    }
    system("pause");
}




//求next值
void get_next(string T, int next[]) {
    int i = 0, j = -1;
    next[0] = -1;
    while (i < T.length())
    {
        if (j == -1 || T[i] == T[j])
        {
            ++i; ++j;
            next[i] = j;
        }
        else {
            j = next[j];
        }
    }
}
//KMP算法
int Index_KMP(string S, string T) {
    int next[100];
    get_next(T, next);
    int i = 0, j = 0;
    int lens = S.length(), lent = T.length();//主串S和模式串T的长度
    while (i < lens && j < lent)
    {
        if (j == -1 || S[i] == T[j])
        {
            i++;
            j++;
        }
        else {
            j = next[j];
        }
    }
    if (j == T.length())
    {
        return i - j + 1;
    }
    else {
        return -1;
    }
}
//查询2 基于KMP算法的网站查询
void Seek_KMP(Stock stock[])
{
    cout << endl << "请输入网站名称:";
    string web;
    cin >> web;
    bool flag = false;
    for (int i = 0; i < 200; i++)
    {
        if (Index_KMP(stock[i].web, web) != -1)
        {
            if (!flag)
            {
                cout << endl << "基于KMP算法得到该公司信息：" << endl;
            }
            flag = true;
            cout << "股票代码 " << " 公司名称" << " 公司网站" << endl;
            cout << stock[i].stock_code<< " " << stock[i].name << " " << stock[i].web << endl;
        }
    }
    if (!flag)
    {
        cout << endl << "未找到对应网站名称的股票！" << endl << endl;
    }
    system("pause");
    system("cls");

}


//二叉树
class BiTree {
public:
    BiTree* lchild, * rchild;
    etype data;

    bool SearchNode(BiTree* T, BiTree* f, string stock_code, BiTree** p);
    void InsertNode(BiTree** T, etype key);
    void inorder(BiTree* T);
    float asl() {
        float w;
        w = (float)total / 200;
        return w;
    }
};
//二叉树查找结点
bool BiTree::SearchNode(BiTree* T, BiTree* f, string stock_code, BiTree** p) {
    if (!T) {
        *p = f;
        return false;
    }
    else if (T->data.stock_code == stock_code) {
        system("cls");//清屏
        search_flag = false;
        cout << "以下为股票详细信息：" << endl;
        cout << "股票代码: " << T->data.stock_code << endl;
        cout << "开盘价: " << T->data.deal->open_price << endl;
        cout << "收盘价: " << T->data.deal->close_price << endl;
        cout << "涨跌幅: " << T->data.deal->iod_rate << endl;
        return true;
    }
    else if (T->data.stock_code > stock_code) {
        return SearchNode(T->lchild, T, stock_code, p);
    }
    else if (T->data.stock_code < stock_code) {
        return SearchNode(T->rchild, T, stock_code, p);
    }
}
//二叉树插入
void BiTree::InsertNode(BiTree** T, etype key)
{
    BiTree* p = new BiTree;

    if (!SearchNode(*T, NULL, key.stock_code, &p)) {
        BiTree* s = new BiTree;
        s->data = key;
        s->lchild = NULL;
        s->rchild = NULL;
        if (!p)
            *T = s;
        else if (p->data.stock_code > key.stock_code)
            p->lchild = s;
        else
            p->rchild = s;
    }
    return;
}
//二叉树遍历
void BiTree::inorder(BiTree* T) {
    //中序遍历
    if (T != NULL) {
        m++;
        inorder(T->lchild);
        total += m;
        inorder(T->rchild);
    }
}
//查询3 基于二叉排序树的信息查询
void Seek_BiTree(BiTree* T, Stock stock[]) {
    BiTree* p = new BiTree;
    BiTree* f = new BiTree;
    //查询结点 
    string code;
    cout << "请输入您要查询的股票代码" << endl;
    cin >> code;
    search_flag = true;

    //查找结点是否存在
    T->SearchNode(T, NULL, code, &p);
    if (search_flag)
    {
        cout << endl << "您输入的股票代码有误！" << endl;
    }
    else {
        T->inorder(T);//遍历
        cout << "ASL: " << T->asl() << endl;
    }

    system("pause");
    system("cls");//清屏
}




//创建交易价格信息的单链表
void CreatedealList(Stock stock[])
{
    L = new dealNode;
    L->next = NULL;
    dealList r = L;
    string s = "D://数据结构课设//code//ks_12_22//ks_12_22//node//";
    for (int i = 1; i <= 200; i++) {
        string url = s + stock[i].stock_code + ".txt";
        ifstream inFile(url);
        if (!inFile.is_open()) {
            cout << "deal file open fail  " << endl;
            system("pause");
        }

        else {

            string head;
            getline(inFile, head);
            //吸收表头
            int cnt = 0;
            dealList p = new dealNode;

            //初始化
            for (int i = 1; i <= 162; i++) {  
                p->data[i].date = "";
                p->data[i].iod_rate= "";
                p->data[i].rate = "";
            }

            p->next = NULL;
            p->name = stock[i].name;
            p->code = stock[i].stock_code;
            while (!inFile.eof()) {
                ++cnt;
                inFile >> p->data[cnt].date >> p->data[cnt].open_price >> p->data[cnt].close_price >> p->data[cnt].max_price >>
                    p->data[cnt].min_price >> p->data[cnt].volume >> p->data[cnt].turnover>> p->data[cnt].rate >>
                    p->data[cnt].iod_amount >> p->data[cnt].iod_rate;
                string temp;
                getline(inFile, temp);  //这一行后面的数据用temp来接受，保证能跳到下一行
            }
            r->next = p;
            r = p;//尾插法创建链表
        }
        inFile.close();
    }

}
//输出交易价格单链表
void output_dealList()
{
    dealList p = L->next;
    while (p) {
        cout << p->code << " " << p->name << endl;
        cout << p->data[1].date << " " << p->data[1].close_price << " " << p->data[1].iod_rate << endl;
        cout << p->data[2].date << " " << p->data[2].close_price<< " " << p->data[2].iod_rate << endl;
        p = p->next;
    }
}
//查询4  基于单链表的交易价格信息查询
void Seek_Deal(Stock stock[])
{
    
    CreatedealList(stock);
    cout << "创建单链表成功" << endl;
    string date;
    cout << "请输入要查找的日期：" << endl;
    cin >> date;
    dealList p = L->next;
    int flag = 0;
    //遍历单链表
    for (int i = 1; i <= 162; i++) {
        if (p->data[i].date == date) {
            flag = 1;
            break;
        }
    }
    while (flag == 0) {
        cout << "没有该日期股票的相关数据，请重新输入：" << endl;
        cin >> date;
        for (int i = 1; i <= 162; i++) {
            if (p->data[i].date == date) {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 1) 
    {
        cout << "查找成功！该日期所有股票的开盘价、收盘价以及涨跌幅如下：" << endl;
        cout <<  "股票代码             " <<  "股票名称" << "                 " << "开盘价" << setw(10) << "收盘价" << setw(10) << "涨跌幅" << endl;
        while (p) {
            for (int i = 1; i <= 162; i++) {
                if (p->data[i].date == date) {
                    cout << p->code << "    " << p->name << "   " << p->data[i].open_price << setw(10) << p->data[i].close_price << setw(10) << p->data[i].iod_rate << endl;
                    break;
                }
            }
            p = p->next;
        }
    }
    system("pause");
}


//查询系统菜单
void Menu_Seek(BiTree* T, Stock stock[])
{
    int choice;
    do {
        system("cls");
        cout << "                 查询系统 " << endl;
        cout << "          1、基于哈希表的股票基本信息查询" << endl;
        cout << "          2、基于KMP的股票网址查询" << endl;
        cout << "          3、基于二叉排序树的股票基本信息查询" << endl;
        cout << "          4、基于单链表的股票价格信息查询" << endl;
        cout << "          5、返回上一级菜单" << endl;
        cout << "请选择：";
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6) {   //判断输入数据是否合法 
            cout << ("输入错误，请重新输入：");
            cin >> choice;
        }
        switch (choice) {
        case 1:Seek_Hash(); break;
        case 2:Seek_KMP(stock); break;
        case 3:Seek_BiTree(T, stock); break;
        case 4:; Seek_Deal(stock); break;
        case 5:break;
        }
    } while (choice != 5);

}


//构造某一日所有股票的单链表
void Create_dateList(dateList& dateL, string date)   
{
    dateL = new dateNode;
    dateL->next = NULL;
    dateList r = dateL;
    dealList p = L->next;  //查询3.4中构建的单链表
    while (p) 
    {
        for (int i = 1; i <= 162; i++) {
            if (p->data[i].date == date) {
                dateList q = new dateNode;
                q->next = NULL;
                q->date = date;
                q->code = p->code;
                q->name = p->name;
                q->open_price = p->data[i].open_price;
                q->close_price = p->data[i].close_price;
                q->iod_rate = p->data[i].iod_rate;
                r->next = q;  //后插法建立单链表
                r = q;
                break;
            }
        }
        p = p->next;
    }
}
//输出某一日所有股票的单链表
int Output_dateList(dateList dateL)
{
    dateList p = dateL->next;
    int cnt = 0;
    cout << setw(3) << "序号" << setw(10) << "股票代码" << setw(10) << "股票名称" << setw(10) << "开盘价" << setw(10) << "收盘价" << setw(10) << "涨跌幅" << endl;
    while (p) {
        cout << setw(3) << ++cnt << setw(10) << p->code << setw(10) << p->name << setw(10) << p->open_price << setw(10) << p->close_price << setw(10) << p->iod_rate << endl;
        p = p->next;
    }
    return cnt;
}
//将插入排序的结果写入表格
void WriteExcel_insertSort_openprice(dateList dateL, int cnt)
{
    dateList p = dateL;
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置用户注册信息，是否正版就在这里验证，否则第一行无法操作
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//生成第一个表
        if (sheetwrite)
        {
            for (int row = 0; row <= cnt; ++row)//按行遍历
            {
                if (row > 0)
                    p = p->next;
                for (int col = 0; col < 6; ++col)//按列遍历
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "序号"; break;
                        case 1:str = "股票代码"; break;
                        case 2:str = "股票名称"; break;
                        case 3:str = "开盘价"; break;
                        case 4:str = "收盘价"; break;
                        case 5:str = "涨跌幅"; break;
                        default:break;
                        }
                    }
                    else {
                        switch (col)
                        {
                        case 0:ss.clear(); ss << row; ss >> str; break;
                        case 1:str = p->code; break;
                        case 2:str = p->name; break;
                        case 3:ss.clear(); ss << p->open_price; ss >> str; break;
                        case 4:ss.clear(); ss << p->close_price; ss >> str; break;
                        case 5:str = p->iod_rate; break;
                        default:
                            break;
                        }
                    }
                    wchar_t* wchar = StringToWchar(str);
                    sheetwrite->writeStr(row, col, wchar);
                    delete[]wchar;
                }
            }
        }
        ////保存excel
        if (book->save(L"价格和涨跌幅排序结果(开盘价）.xlsx"))
        {
            //::ShellExecute(NULL, "open", "example.xls", NULL, NULL, SW_SHOW);
        }
        else
        {
            std::cout << book->errorMessage() << std::endl;
        }
    }
    book->release();
}
void WriteExcel_insertSort_closeprice(dateList dateL, int cnt)
{
    dateList p = dateL;
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置用户注册信息，是否正版就在这里验证，否则第一行无法操作
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//生成第一个表
        if (sheetwrite)
        {
            for (int row = 0; row <= cnt; ++row)//按行遍历
            {
                if (row > 0)
                    p = p->next;
                for (int col = 0; col < 6; ++col)//按列遍历
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "序号"; break;
                        case 1:str = "股票代码"; break;
                        case 2:str = "股票名称"; break;
                        case 3:str = "开盘价"; break;
                        case 4:str = "收盘价"; break;
                        case 5:str = "涨跌幅"; break;
                        default:break;
                        }
                    }
                    else {
                        switch (col)
                        {
                        case 0:ss.clear(); ss << row; ss >> str; break;
                        case 1:str = p->code; break;
                        case 2:str = p->name; break;
                        case 3:ss.clear(); ss << p->open_price; ss >> str; break;
                        case 4:ss.clear(); ss << p->close_price; ss >> str; break;
                        case 5:str = p->iod_rate; break;
                        default:
                            break;
                        }
                    }
                    wchar_t* wchar = StringToWchar(str);
                    sheetwrite->writeStr(row, col, wchar);
                    delete[]wchar;
                }
            }
        }
        ////保存excel
        if (book->save(L"价格和涨跌幅排序结果(收盘价）.xlsx"))
        {
            //::ShellExecute(NULL, "open", "example.xls", NULL, NULL, SW_SHOW);
        }
        else
        {
            std::cout << book->errorMessage() << std::endl;
        }
    }
    book->release();
}
void WriteExcel_insertSort_iodrate(dateList dateL, int cnt)
{
    dateList p = dateL;
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置用户注册信息，是否正版就在这里验证，否则第一行无法操作
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//生成第一个表
        if (sheetwrite)
        {
            for (int row = 0; row <= cnt; ++row)//按行遍历
            {
                if (row > 0)
                    p = p->next;
                for (int col = 0; col < 6; ++col)//按列遍历
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "序号"; break;
                        case 1:str = "股票代码"; break;
                        case 2:str = "股票名称"; break;
                        case 3:str = "开盘价"; break;
                        case 4:str = "收盘价"; break;
                        case 5:str = "涨跌幅"; break;
                        default:break;
                        }
                    }
                    else {
                        switch (col)
                        {
                        case 0:ss.clear(); ss << row; ss >> str; break;
                        case 1:str = p->code; break;
                        case 2:str = p->name; break;
                        case 3:ss.clear(); ss << p->open_price; ss >> str; break;
                        case 4:ss.clear(); ss << p->close_price; ss >> str; break;
                        case 5:str = p->iod_rate; break;
                        default:
                            break;
                        }
                    }
                    wchar_t* wchar = StringToWchar(str);
                    sheetwrite->writeStr(row, col, wchar);
                    delete[]wchar;
                }
            }
        }
        ////保存excel
        if (book->save(L"价格和涨跌幅排序结果(涨跌幅）.xlsx"))
        {
            //::ShellExecute(NULL, "open", "example.xls", NULL, NULL, SW_SHOW);
        }
        else
        {
            std::cout << book->errorMessage() << std::endl;
        }
    }
    book->release();
}
//直接插入排序（开盘价）
void Analyse_Sort_Straight_openprice(string date)
{
        dateList dateL;
        Create_dateList(dateL, date);
        dateList pp = dateL->next->next;//保存dateL中的第二个元素，因为下一步要将dateL变成只有一个元素的有序区
        dateL->next->next = NULL;//将dateL变成只有一个元素的有序表
        //从dateL的第二个元素开始遍历整个dateL直至表尾
        while (pp) {
            // 进行插入有序区的操作时会改变p->next的值所以需要选保存一下
            dateList q = pp->next;
            // 为方便比较，用一个指针pre,指向有序区。
            dateList pre = dateL;
            while (pre->next != NULL && pre->next->open_price > pp->open_price) {
                pre = pre->next;
            }
            //现在的pre->next是从头开始第一个小于要插入元素pp的结点
            pp->next = pre->next; //pp插入到有序区
            pre->next = pp;
            // 用q把p的值恢复过来，使得循环继续指向无序区的下一个节点
            pp = q;
        }
        cout << "**********************************************************************************************************" << endl;
        cout << "根据开盘价排序后该日期股票信息：" << endl;
        int cnt = Output_dateList(dateL);//返回行数
        WriteExcel_insertSort_openprice(dateL, cnt);
        cout << "写入成功" << endl;
}
//直接插入排序（收盘价）
void Analyse_Sort_Straight_closeprice(string date)
{
    dateList dateL;
    Create_dateList(dateL, date);
    dateList pp = dateL->next->next;//保存dateL中的第二个元素，因为下一步要将dateL变成只有一个元素的有序区
    dateL->next->next = NULL;//将dateL变成只有一个元素的有序表
    //从dateL的第二个元素开始遍历整个dateL直至表尾
    while (pp) {
        // 进行插入有序区的操作时会改变p->next的值所以需要选保存一下
        dateList q = pp->next;
        // 为方便比较，用一个指针pre,指向有序区。
        dateList pre = dateL;
        while (pre->next != NULL && pre->next->close_price > pp->close_price) {
            pre = pre->next;
        }
        //现在的pre->next是从头开始第一个小于要插入元素pp的结点
        pp->next = pre->next; //pp插入到有序区
        pre->next = pp;
        // 用q把p的值恢复过来，使得循环继续指向无序区的下一个节点
        pp = q;
    }
    cout << "*********************************************************************************************************************" << endl;
    cout << "根据收盘价排序后该日期股票信息：" << endl;
    int cnt = Output_dateList(dateL);//返回行数
    WriteExcel_insertSort_closeprice(dateL, cnt);
    cout << "写入成功" << endl;
}
//直接插入排序（涨跌幅）
void Analyse_Sort_Straight_iodrate(string date)
{
    dateList dateL;
    Create_dateList(dateL, date);
    dateList pp = dateL->next->next;//保存dateL中的第二个元素，因为下一步要将dateL变成只有一个元素的有序区
    dateL->next->next = NULL;//将dateL变成只有一个元素的有序表
    //从dateL的第二个元素开始遍历整个dateL直至表尾
    while (pp) {
        // 进行插入有序区的操作时会改变p->next的值所以需要选保存一下
        dateList q = pp->next;
        // 为方便比较，用一个指针pre,指向有序区。
        dateList pre = dateL;


        while (pre->next != NULL && stod( pre->next->iod_rate) > stod(pp->iod_rate)) {
            pre = pre->next;
        }
        //现在的pre->next是从头开始第一个小于要插入元素pp的结点
        pp->next = pre->next; //pp插入到有序区
        pre->next = pp;
        // 用q把p的值恢复过来，使得循环继续指向无序区的下一个节点
        pp = q;
    }
    cout << "********************************************************************************************************************" << endl;
    cout << "根据涨跌幅排序后该日期股票信息：" << endl;
    int cnt = Output_dateList(dateL);//返回行数
    WriteExcel_insertSort_iodrate(dateL, cnt);
    cout << "写入成功" << endl;
}
//直接插入排序
void Analyse_Sort_Straight(Stock stock[])
{
    CreatedealList(stock);
    dealList p = L->next;

    string date;
    cout << "请输入要查找的日期：(格式：20211108)" << endl;
    cin >> date;

    int flag = 0;
    for (int i = 1; i <= 162; i++) {
        if (p->data[i].date == date) {
            flag = 1;
            break;
        }
    }

    while (flag == 0) {
        cout << "没有该日期股票的相关数据，请重新输入：" << endl;
        cin >> date;
        for (int i = 1; i <= 162; i++) {
            if (p->data[i].date == date) {
                flag = 1;
                break;
            }
        }
    }


    Analyse_Sort_Straight_openprice(date);
    Analyse_Sort_Straight_closeprice(date);
    Analyse_Sort_Straight_iodrate(date);
    
    system("pause");
      
}



//创建60支股票信息
void CreateS60(Stock stock[],Score score[])
{
    CreatedealList(stock);
    for (int i = 1; i <= 60; i++) {
        for (int j = 1; j <= 200; j++) {
            //cout << "sto:" << sto[j].stock_code << endl;
            if (s60[i].stock_code == stock[j].stock_code) {
                s60[i].first_trade= stock[j].first_trade;
                s60[i].sco = score[i].sco;
                s60[i].close_price = stock[j].deal[1].close_price;
                break;
            }
        }
    }
    
    dealList p = L->next;  //查询3.4中构建的单链表
    while (p) {
        for (int i = 1; i <= 60; i++) {
            if (s60[i].stock_code == p->code) {
                double Max = stod(p->data[1].iod_rate);
                int ans = 1;
                for (int j = 1; j <= 162; j++) {
                    if (p->data[j].iod_rate == "") {     //说明data[j]后面没有存数据
                        break;
                    }
                    if (stod(p->data[j].iod_rate) > Max) {
                        Max = stod(p->data[j].iod_rate);
                        ans = j;
                    }
                }
                s60[i].date = p->data[ans].date;
                s60[i].iodrate = p->data[ans].iod_rate;
                break;
            }
        }
        p = p->next;
    }

}


//快排算法
int Partition(Stock60 array[], int low, int high)
{
    double num = stod(array[low].iodrate);
    array[0] = array[low];
    while (low < high) 
    {
        while (low < high && stod(array[high].iodrate) >= num)
            high--;              
        array[low] = array[high];  //比枢轴小的移动到低端
        while (low < high && stod(array[low].iodrate) <= num) {
            low++;
        }
        array[high] = array[low];   //比枢轴大的移动到高端
    }
    array[low] = array[0];       //枢轴记录到位
    return low;                 //返回枢轴位置
}
void Qsort(Stock60 array[], int low, int high)
{
    if (low < high) {
        int m = Partition(array, low, high); //m为枢轴位置
        Qsort(array, low, m - 1);    //对左子表递归排序
        Qsort(array, m + 1, high);    //对右子表递归排序
    }
}
//快速排序股票价格分析
void Analyes_Sort_Quick(Stock stock[], Score score[])
{
    CreateS60(stock,score);
    for (int i = 1; i < 60; i++)
        cout << endl << i + 1 << "  " << s60[i].node << " " << s60[i].stock_code << " " << s60[i].name << " " << s60[i].iodrate << " " << s60[i].date << " " << s60[i].first_trade << "\t" << endl;
    cout << "请输入一级行业名称：" << endl;
    string industry;
    cin >> industry;
    int flag = 0;
    for (int i = 1; i <= 60; i++) {
        if (s60[i].first_trade == industry) {
            flag = 1;
            break;
        }
    }
    while (flag == 0) {
        cout << "该一级行业的股票不存在 请重新输入：" << endl;
        cin >> industry;
        for (int i = 1; i <= 60; i++) {
            if (s60[i].first_trade == industry) {
                flag = 1;
                break;
            }
        }
    }
    int cnt = 0;
    Stock60 array[67];
    for (int i = 1; i <= 60; i++) {
        if (s60[i].first_trade == industry) {
            array[++cnt] = s60[i];
        }
    }
    Qsort(array, 1, cnt);
    int count = 0;
    cout << "快速排序结果如下：" << endl;
    cout << endl << "序号  节点  股票代码  股票名称  涨跌幅  日期" << "\t" << endl;
     for (int i = cnt; i >= 1; i--)
        cout << endl << ++count << "  " << array[i].node << " " << array[i].stock_code << " " << array[i].name << " " << array[i].iodrate << " " << array[i].date << "\t" << endl;
   
    system("pause");
}



//简单选择排序（评分）
void SelectSortScore(Stock60 s[])
{
    int begin = 0;
    int end = 60;
    int max = 0;
    int min = 0;
    while (begin < end)
    {
        max = begin, min = begin;
        for (int i = begin; i <= end; i++)
        {
            if (s[i].sco >= s[max].sco)  //有相同元素时，最大的要找位置相对最后的一个最大的
            {
                max = i;
            }
            if (s[i].sco < s[min].sco)  //最小的要找位置相对最前的一个最小的，这样可以使选择排序算法稳定
            {
                min = i;
            }
        }
        if (max == begin && min == end)
        {
            swap(s[max], s[end]);
            continue;
        }

        if (max == begin)
        {
            swap(s[max], s[end]);
            swap(s[min], s[begin]);
            continue;
        }

        if (min == end)
        {
            swap(s[min], s[begin]);
            swap(s[max], s[end]);
            continue;
        }
        swap(s[min], s[begin]);
        swap(s[max], s[end]);
        begin++;
        end--;
    }
}
//简单选择排序（收盘价）
void SelectSortCloseprice(Stock60 s[])
{
    int begin = 0;
    int end = 60;
    int max = 0;
    int min = 0;
    while (begin < end)
    {
        max = begin, min = begin;
        for (int i = begin; i <= end; i++)
        {
            if (s[i].close_price >= s[max].close_price)  //有相同元素时，最大的要找位置相对最后的一个最大的
            {
                max = i;
            }
            if (s[i].close_price < s[min].close_price)  //最小的要找位置相对最前的一个最小的，这样可以使选择排序算法稳定
            {
                min = i;
            }
        }
        if (max == begin && min == end)
        {
            swap(s[max], s[end]);
            continue;
        }

        if (max == begin)
        {
            swap(s[max], s[end]);
            swap(s[min], s[begin]);
            continue;
        }

        if (min == end)
        {
            swap(s[min], s[begin]);
            swap(s[max], s[end]);
            continue;
        }
        swap(s[min], s[begin]);
        swap(s[max], s[end]);
        begin++;
        end--;
    }
}
//简单选择排序写入表格
void WriteExcel_SelectSort_score(Stock60 s[])
{
    
    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置用户注册信息，是否正版就在这里验证，否则第一行无法操作
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//生成第一个表
        if (sheetwrite)
        {
            for (int row = 0; row <= 60; ++row)//按行遍历
            {
                for (int col = 0; col < 6; ++col)//按列遍历
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "序号"; break;
                        case 1:str = "股票代码"; break;
                        case 2:str = "股票名称"; break;
                        case 3:str = "评分"; break;
                        default:break;
                        }
                    }
                    else {
                        switch (col)
                        {
                        case 0:ss.clear(); ss << row; ss >> str; break;
                        case 1:str = s[61-row].stock_code; break;
                        case 2:str =s[61-row].name; break;
                        case 3:ss.clear(); ss << s[61-row].sco; ss >> str; break;
                        default:
                            break;
                        }
                    }
                    wchar_t* wchar = StringToWchar(str);
                    sheetwrite->writeStr(row, col, wchar);
                    delete[]wchar;
                }
            }
        }
        ////保存excel
        if (book->save(L"评分排序.xlsx"))
        {
            //::ShellExecute(NULL, "open", "example.xls", NULL, NULL, SW_SHOW);
        }
        else
        {
            std::cout << book->errorMessage() << std::endl;
        }
    }
    book->release();
}
void WriteExcel_SelectSort_Closeprice(Stock60 s[])
{

    Book* book = xlCreateXMLBook();//创建一个XLSX的实例,在使用前必须先调用这个函数创建操作excel的对象
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//设置用户注册信息，是否正版就在这里验证，否则第一行无法操作
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//生成第一个表
        if (sheetwrite)
        {
            for (int row = 0; row <= 60; ++row)//按行遍历
            {
                for (int col = 0; col < 6; ++col)//按列遍历
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "序号"; break;
                        case 1:str = "股票代码"; break;
                        case 2:str = "股票名称"; break;
                        case 3:str = "收盘价"; break;
                        default:break;
                        }
                    }
                    else {
                        switch (col)
                        {
                        case 0:ss.clear(); ss << row; ss >> str; break;
                        case 1:str = s[61-row].stock_code; break;
                        case 2:str = s[61-row].name; break;
                        case 3:ss.clear(); ss << s[61-row].close_price; ss >> str; break;
                        default:
                            break;
                        }
                    }
                    wchar_t* wchar = StringToWchar(str);
                    sheetwrite->writeStr(row, col, wchar);
                    delete[]wchar;
                }
            }
        }
        ////保存excel
        if (book->save(L"收盘价排序.xlsx"))
        {
            //::ShellExecute(NULL, "open", "example.xls", NULL, NULL, SW_SHOW);
        }
        else
        {
            std::cout << book->errorMessage() << std::endl;
        }
    }
    book->release();
}
//简单选择排序
void Analyse_Sort_Select(Stock stock[], Score score[])
{
    CreateS60(stock, score);
    SelectSortScore(s60);
    WriteExcel_SelectSort_score(s60);
    cout << "*******评分排序结果*************" << endl;
    cout << endl << "序号      股票代码       股票名称         评分" << "\t" << endl;
   for(int k=60;k>0;k--)
        cout << endl << 61-k << "       " << s60[k].stock_code << "         " << s60[k].name << "      " << s60[k].sco << endl;


   SelectSortCloseprice(s60);
   WriteExcel_SelectSort_Closeprice(s60);
   cout << "*******收盘价排序结果*************" << endl;
   cout << endl << "序号    股票代码           股票名称       收盘价" << "\t" << endl;
   for (int k = 60; k > 0; k--)
       cout << endl << 61 - k << "       " << s60[k].stock_code << "        " << s60[k].name << "    " << s60[k].close_price<<   endl;
   system("pause");
}


//创建图
void Create_Graph(Val v[])
{
    //构造
    int  n, m, t1, t2, t3;
    n = 61;
    m = 84;//n表示顶点个数，m表示边的条数
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < n; j++)
        {
            if (i == j)
                Gp.matrix[i][j] = 0;
            else
                Gp.matrix[i][j] = INF;
        }
    }
    for (int i = 1; i < m; i++)
    {
        Gp.matrix[v[i].node1][v[i].node2] = v[i].num;
        Gp.matrix[v[i].node2][v[i].node1] = v[i].num;
    }

   
}


//基于Floyd的股票相关性计算
void Analyes_Floyd(Val v[])
{
    Create_Graph(v);
    int n = 61;
    //核心代码
    for (int k = 1; k < n; k++)
    {
        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (Gp.matrix[i][j] > Gp.matrix[i][k] + Gp.matrix[k][j])
                    Gp.matrix[i][j] = Gp.matrix[i][k] + Gp.matrix[k][j];
            }
        }
    }
    cout << "请输入需要查找的两个点：" << endl;
    int node1, node2;
    cin >> node1 >> node2;
    if (node1 > 0 && node1 <= 60 && node1 > 0 && node1 <= 60)
        cout << node1 << "--->" << node2 << "    最短路径：" << Gp.matrix[node1][node2] << endl;
    else
    {
        cout << "输入错误！" << endl;
    }
   
    system("pause");
}


//基于Prim最小生成树的股票基金筛选
void Analyes_Prim( Val v[],Stock stock[])
{
    int start = 1;
    Create_Graph(v);
    Gp.numNodes = 60;
    int value[61], v1[61], v2[61], tempvalue, tempv1, tempv2;//临时存储最小树的边和权值
    int Visited_count = 0, sum = 0; //分别为已被访问的节点数和总权值
    bool* isVisited = new bool[Gp.numNodes + 1];//判断各点是否被访问过
    int* Edge = new int[Gp.numNodes + 1]; //记录各点（如果未被访问的话）到已被访问的各点中某一点的最小距离

    for (int i = 1; i <= Gp.numNodes; i++)
        isVisited[i] = false;

    isVisited[start] = true;

    for (int i = 1; i <= Gp.numNodes; i++) {
        Edge[i] = Gp.matrix[start][i];
    }
    int last_Node = 0;//最近由未访问状态转至以访问状态的点的序号
    Visited_count++;
    cout << "最小二叉树的边如下:" << endl;
    cout << "v1   " << "   v2" << "    value" << endl;
    while (Visited_count < Gp.numNodes)
    {
        int min_Edge = 9999;
        for (int i = 1; i <= Gp.numNodes; i++) {
            if (!isVisited[i] && Edge[i] < min_Edge) {
                min_Edge = Edge[i];
                last_Node = i;
            }
        }

        //寻找边的起点
        int Start_Node=0;
        for (int i = 1; i < last_Node; i++)
        {
            if (min_Edge == Gp.matrix[i][last_Node])
                Start_Node = i;
        }
        for (int i = 1; i < 61; i++)
        {
            if(Start_Node==0&& min_Edge == Gp.matrix[i][last_Node])
                Start_Node = i;
        }

        //输出
        cout << Start_Node << "      "<<last_Node << "      " << min_Edge << endl;
        v1[Visited_count] = Start_Node;
        v2[Visited_count] = last_Node;
        value[Visited_count] = min_Edge;

        

        Visited_count++;
        isVisited[last_Node] = true;
        sum += min_Edge;
        for (int i = 1; i <= Gp.numNodes; i++) { //因为由刚刚发现的点，更新Edge数组中的值
            if (!isVisited[i] && Gp.matrix[last_Node][i] < Edge[i]) {
                Edge[i] = Gp.matrix[last_Node][i];
            }
        }
    }
    for (int j = 1; j < 61 - 1; j++)
    {
        for (int i = 1; i < 61 - 1 - j; i++)
            if (value[i] > value[i + 1])
            {
                tempvalue = value[i];
                value[i] = value[i + 1];
                value[i + 1] = tempvalue;

                tempv1 = v1[i];
                v1[i] = v1[i + 1];
                v1[i + 1] = tempv1;

                tempv2 = v2[i];
                v2[i] = v2[i + 1];
                v2[i + 1] = tempv2;
            }
    }
    cout << "按权值排序后的最小二叉树的边如下:" << endl;
    cout << "v1   " << "   v2" << "    value" << endl;
    for(int i=1;i<60;i++)
    cout << v1[i] << "      " << v2[i] << "      " << value[i] << endl;
    cout << "最小二叉树的权值之和为：" << sum << endl;
    cout << "按权值选取的6支股票如下：" << endl;
    for (int i = 1; i <= 3; i++)
        cout << stock[v1[i]].stock_code << "  " << stock[v1[i]].short_name << " &&&&&& " << stock[v2[i]].stock_code << "  " << stock[v2[i]].short_name << endl;
    
    system("pause");
    delete[]isVisited;
    delete[]Edge;
}

int n, tot = 0, k = 0;//n端点总数，tot记录最终答案，k已经连接了多少边 
int fat[200010];//记录集体老大 
struct node
{
    int from, to, dis;//结构体储存边 
}edge[200010];
bool cmp(const node& a, const node& b)//sort排序
{
    return a.dis < b.dis;
}
int father(int x)//找集体老大，并查集的一部分 
{
    if (fat[x] != x)
        return father(fat[x]);
    else return x;
}
void unionn(int x, int y)//加入团体，并查集的一部分 
{
    fat[father(y)] = father(x);
}
void Analyse_Kruskal(Val v[], Stock stock[])
{
    n = 60; m = 83;
    cout << "最小二叉树的边如下:" << endl;
    cout << "v1   " << "   v2" << "    value" << endl;
    for (int i = 1; i <= m; i++)
    {
        edge[i].from = v[i].node1;
        edge[i].to = v[i].node2;
        edge[i].dis = v[i].num;
    }
    for (int i = 1; i <= n; i++) fat[i] = i;//自己最开始就是自己的老大 （初始化） 
    sort(edge + 1, edge + 1 + m, cmp);//按权值排序（kruskal的体现） 
    for (int i = 1; i <= m; i++)//从小到大遍历 
    {
        if (k == n - 1) break;//n个点需要n-1条边连接 
        if (father(edge[i].from) != father(edge[i].to))//假如不在一个团体 
        {
            cout << edge[i].from << "  " << edge[i].to << "   " << edge[i].dis << endl;
            unionn(edge[i].from, edge[i].to);//加入 
            tot += edge[i].dis;//记录边权 
            k++;//已连接边数+1 
        }
    } 
    cout << "树的总权值为:" << tot << endl;
    cout << "按权值选取的6支股票如下：" << endl;
    for (int i = 1; i <= 3; i++)
        cout << stock[edge[i].from].stock_code << "  " << stock[edge[i].from].short_name << " &&&&&& " << stock[edge[i].to].stock_code << "  " << stock[edge[i].to].short_name << endl;
   
    system("pause");
}

int color[60], graph[61][61];
// 广度优先算法
bool bfs(int s, int n) {
    //0为白色，1为黑色
    queue<int> q;
    q.push(s);
    color[s] = 1;
    while (!q.empty()) {
        int from = q.front();
        q.pop();
        for (int i = 1; i <= n; i++) {
            if (graph[from][i] && color[i] == -1) {
                q.push(i);
                color[i] = !color[from];//染成不同的颜色 
            }
            if (graph[from][i] && color[from] == color[i])//颜色有相同，则不是二分图 
                return false;
        }
    }
    return true;
}
//判断二部图
void Analyse_BiGraph(Val v[])
{
    Create_Graph(v);
    int n=10, m, a, b, i;//n为点，m为边
    int node[11];
    memset(color, -1, sizeof(color));//初始化

    //输入需要判断的点
    cout << "请依次输入你要判断的十个点:" << endl;
    for (i = 1; i <= n; i++)
        cin >> node[i];
    cout << "其中有关联的边有：" << endl;
    for (i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            if (Gp.matrix[node[i]][node[j]] != 0 && Gp.matrix[node[i]][node[j]] != INF)
            cout << i << "->" << j << "   " << Gp.matrix[node[i]][node[j]] << endl;
        }
    for(i=1;i<=n;i++)
        for (int j = 1; j <= n; j++)
        {
            if(i!=j&&Gp.matrix[ node[i] ] [ node[j] ] !=INF)
                graph[node[i]][node[j]] = graph[node[j]] [node[i]] = 1;
        }
   
    bool flag = false;
    for (i = 1; i <= n; i++)
        if (color[i] == -1 && !bfs(i, n)) {//遍历各个连通分支 
            flag = true;
            break;
        }
    if (flag)
        cout << "NO" << endl;
    else
        cout << "YES" << endl;
    system("pause");
}

//分析系统菜单
void Menu_Analyse(Stock stock[], Score score[] ,Val v[])
{
    int choice;
    do {
        system("cls");
        cout << "*******************分析系统*********************" << endl;
        cout << "         1、基于直接插入排序的股票价格分析" << endl;
        cout << "         2、基于快速排序的股票价格分析" << endl;
        cout << "         3、基于简单选择排序的股票价格分析" << endl;
        cout << "         4、基于Floyd的股票相关性计算" << endl;
        cout << "         5、基于Prim最小生成树的股票基金筛选" << endl;
        cout << "         6、基于Kruskal最小生成树的股票基金筛选" << endl;
        cout << "         7、基于二部图的股票基金筛选" << endl;
        cout << "         8、返回上一级菜单" << endl;
        cout << "请选择：";
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7 && choice != 8) {   //判断输入数据是否合法 
            cout << ("输入错误，请重新输入：");
            cin >> choice;
        }
        switch (choice) {
        case 1:Analyse_Sort_Straight(stock); break;
        case 2:Analyes_Sort_Quick(stock, score); break;
        case 3: Analyse_Sort_Select(stock, score); break;
        case 4:Analyes_Floyd(v); break;
        case 5:Analyes_Prim(v,stock); break;
        case 6:Analyse_Kruskal(v, stock); break;
        case 7:Analyse_BiGraph(v); break;
        case 8:break;
        default:break;
        }
    } while (choice != 8);
}

int main()
{
    
    
    cout << "导入各公司信息中..." << endl;
    Stock* stock;
    stock = new Stock[666];
    Read_stock(stock);
    cout << "导入各公司基本信息成功！" << endl;

    cout << "导入公司评分成功!" << endl;
    Score score[61];
    Read_score(score);


    cout << "导入公司相关关系成功!" << endl;
    Val v[84];
    Read_val(v);

    system("pause");

    BiTree* T = new BiTree;
    T = NULL;
    for (int i = 0; i < 200; i++)
        T->InsertNode(&T, stock[i]);

    
    //界面
    int flag = 0;
    int k = 0;
    do
    {
        system("cls");
       
        cout << "            股票查询与分析系统" << endl;
        cout << "               1.查询系统                         " << endl;
        cout << endl;
        cout << "               2.分析系统                        " << endl;
        cout << endl;
        cout << "               3.退出系统                         " << endl;


        //防止非法输入
        if (flag) {                                     
            flag = 0;
            cout << "输入错误，请重新选择：";
            cin >> k;
        }
        else {
            cout << "请选择：";
            cin >> k;
        }
        switch (k)
        {
        case 1:Menu_Seek(T, stock); break;
        case 2:Menu_Analyse(stock,score,v); break;
        case 3: break;
        default:flag = 1;
        }
    } while (k != 3);
    return 0;
}
