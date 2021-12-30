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
#define HASH_M 97//��ϣ������ģ
#define MAXVEX 61
#define INF 65535

char* w2c(char* pcstr, const wchar_t* pwstr, size_t len)//�˺���֧�����Ķ�ȡ����ʾ����������������������
{
    int nlength = wcslen(pwstr);//��ȡת����ĳ���
    int nbytes = WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL); //ӳ��һ��unicode�ַ�����һ�����ֽ��ַ���
    if (nbytes > len)   nbytes = len;
    // ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
    WideCharToMultiByte(0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);
    return pcstr;
}
wchar_t* StringToWchar(const string& pKey)
{
    const char* pCStrKey = pKey.c_str();
    //��һ�ε��÷���ת������ַ������ȣ�����ȷ��Ϊwchar_t*���ٶ����ڴ�ռ�
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    wchar_t* pWCStrKey = new wchar_t[pSize];
    //�ڶ��ε��ý����ֽ��ַ���ת����˫�ֽ��ַ���
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}

typedef struct {
    int node1;//��1
    int node2;//��2
    int num;//�߳�
}Val;

typedef struct {
    int node;//��
    string name;//��Ʊ����
    string stock_code;//��Ʊ����
    int sco;//����
}Score;

typedef struct {
    string date;		// ���� 
    double open_price;	// ���̼� 
    double close_price;	// ���̼� 
    double max_price;	// ��߼� 
    double min_price;	// ��ͼ� 
    double volume;		// �ɽ��� 
    int turnover;		// �ɽ����Ԫ�� 
    string rate;		// ������% 
    double iod_amount;	// �ǵ��� 
    string iod_rate;	// �ǵ���% 
}deal;

typedef struct {
    string stock_code;	// ��Ʊ���� 
    string short_name;	// ��Ʊ��� 
    string trade_code;	// ��ҵ���� 
    string first_trade;	// ����һ����ҵ���� 
    string second_trade;// ����������ҵ���� 
    string bourse;		// ���н����� 
    string name;		// ��˾ȫ�� 
    string launch_date;	// �������� 
    string province;	// ʡ�� 
    string city;		// ���� 
    string legal_person;// ���� 
    string address;		// ��ַ 
    string web;			// ��ַ 
    string email;		// ���� 
    string phone;		// �绰 
    string business;	// ��Ӫҵ�� 
    string scpope;		// ��Ӫ��Χ 
    deal* deal;	        //��ϸ��Ϣ 
    int length;			//��ϸ��Ϣ���� 
    string maxiodrate;   //����ǵ���
}Stock;

Stock sto[207];
typedef Stock etype;
int total = 0, m = 0;
bool dele_flag = true, search_flag = true;


typedef struct HNode {   //��ϣ��
    Stock data;
    struct HNode* next;
}HNode, * HLinkList;

HLinkList H[97];//��ϣ����
typedef struct dealNode {    //������  �洢ÿ֧��Ʊ�Ľ�����Ϣ
    string name;
    string code;
    deal data[167];
    struct dealNode* next;
}dealNode, * dealList;
dealList L;

typedef struct dateNode { //������    �洢ĳһ�����й�Ʊ����Ϣ
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
    string name;   //����
    string stock_code;	// ��Ʊ���� 
    string first_trade;	// ����һ����ҵ���� 
    string iodrate;  //����ǵ���
    string date;     //����ǵ�������
    double close_price;  //���һ�����̼�
    int sco;  //����
}Stock60;
Stock60 s60[61];

typedef struct
{
    int matrix[61][61];
    int numNodes, numEdges;
    int vexnum;  //�������
    int edge;   //�ߵ�����
    int** arc; //�ڽӾ���
    string* information; //��¼ÿ����������
} Graph;
Graph Gp;
int e[84][84];



//�ַ���תdouble
double stringchange(string s)
{

    char a[100];
    for (int i = 0; i < s.size() - 1; i++) {
        a[i] = s[i];
    }
    return atof(a);
}

// ��ȡ�ı�����
int get_text_count(string code)
{
    string path = "node\\" + code + ".txt";
    ifstream infile(path.c_str());
    if (!infile.is_open())
    {
        cout << "δ�ɹ����ļ�!234" << endl;
    }
    string temp;
    int count = 0;
    while (getline(infile, temp)) {
        count++;
    }
    return count - 1;	// ���ݴӴӵڶ����� 
}

//��ȡ���׼۸���Ϣ
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
    getline(in, temp);	// ������һ�� 
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

//��ȡ����Ϣ
void Read_val(Val value[])
{

    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//������Ӧ��key������ع���(����)�������һ���޷�����
    if (book->load(L"60֧��Ʊ��Ϣ1.xlsx"))//�ļ����빤��Ŀ¼��
    {
        Sheet* sheetread = book->getSheet(0);//ѡȡ��һ����
        if (sheetread)
        {
            for (int row = sheetread->firstRow(); row < sheetread->lastRow(); ++row)//�б���
            {
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//�б���
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//���ÿ����Ԫ������Ϊ�ַ�������
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//�ӵ�Ԫ���ж�ȡ�ַ��������ʽ
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//����ռ��С����Ŷ��������ݣ�wcslen()�������ظ�������t�ĳ���
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//��ʼ���������
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr�����ݶ�������Ҫ�������ı����ת�������Ϊת������
                        //cout << pcstr << endl;//���
                        free(pcstr);
                    }
                    else if (celltype == CELLTYPE_NUMBER)//���ÿ����Ԫ������Ϊ��ֵ����
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
        
        ////����excel
        book->save(L"60֧��Ʊ��Ϣ1.xlsx");
        book->release();
    }
}

//��ȡ������Ϣ
void Read_score(Score score[])
{

    //��2�е������Ϊ��ֵ����
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//������Ӧ��key������ع���(����)�������һ���޷�����
    if (book->load(L"60֧��Ʊ��Ϣ2.xlsx"))//�ļ����빤��Ŀ¼��
    {
        Sheet* sheetread = book->getSheet(0);//ѡȡ��һ����
        if (sheetread)
        {
            for (int row = sheetread->firstRow(); row < sheetread->lastRow(); ++row)//�б���
            {
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//�б���
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//���ÿ����Ԫ������Ϊ�ַ�������
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//�ӵ�Ԫ���ж�ȡ�ַ��������ʽ
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//����ռ��С����Ŷ��������ݣ�wcslen()�������ظ�������t�ĳ���
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//��ʼ���������
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr�����ݶ�������Ҫ�������ı����ת�������Ϊת������
                        //cout << pcstr << endl;//���
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
                    else if (celltype == CELLTYPE_NUMBER)//���ÿ����Ԫ������Ϊ��ֵ����
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
            

        ////����excel
        book->save(L"60֧��Ʊ��Ϣ2.xlsx");
        book->release();
    }
}

//��ȡ��Ʊ������Ϣ
void Read_stock(Stock stock[])
{
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//������Ӧ��key������ع���(����)�������һ���޷�����
    if (book->load(L"A�ɹ�˾���.xlsx"))//�ļ����빤��Ŀ¼��
    {
        Sheet* sheetread = book->getSheet(0);//ѡȡ��һ����
        if (sheetread)
        {
            for (int row = sheetread->firstRow() + 1; row < sheetread->lastRow(); ++row)//�б���
            {

               
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//�б���
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//���ÿ����Ԫ������Ϊ�ַ�������
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//�ӵ�Ԫ���ж�ȡ�ַ��������ʽ
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//����ռ��С����Ŷ��������ݣ�wcslen()�������ظ�������t�ĳ���
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//��ʼ���������
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr�����ݶ�������Ҫ�������ı����ת�������Ϊת������
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
        ////����excel
        book->save(L"A�ɹ�˾���.xlsx");
        book->release();
    }
}



//������ϣ��
void InitHash()
{
    for (int i = 0; i < HASH_M; i++) {
        H[i] = new HNode;
        H[i]->next = NULL;
    }
}
//�����ϣ��
void InsertHash(Stock& e)
{
    int len = e.stock_code.size();//��Ʊ����ĳ���
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += (int)e.stock_code[i];
    }//��ASII��֮��
    //��index��key��
    int index = sum % HASH_M;
    HLinkList p = H[index];
    while (p->next) {
        p = p->next;
    }            //ʹpΪH[index]��βָ��
    HLinkList q = new HNode;
    q->data = e;
    q->next = NULL;
    p->next = q;//��ֵ���뵽��ǰλ�õ�ĩβ
}
//�����������ϣ��
void CreateHash()
{
    InitHash();
    int i = 0;
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//������Ӧ��key������ع���(����)�������һ���޷�����
    if (book->load(L"A�ɹ�˾���.xlsx"))//�ļ����빤��Ŀ¼��
    {
        Sheet* sheetread = book->getSheet(0);//ѡȡ��һ����
        if (sheetread)
        {
            for (int row = sheetread->firstRow() + 1; row < sheetread->lastRow(); ++row)//�б���
            {
                string company[20]; //������ÿһ�ж�������Ϣ
                int cnt = 0;
                for (int col = sheetread->firstCol(); col < sheetread->lastCol(); ++col)//�б���
                {
                    CellType celltype = sheetread->cellType(row, col);
                    Format* format = sheetread->cellFormat(row, col);
                    if (celltype == CELLTYPE_STRING)//���ÿ����Ԫ������Ϊ�ַ�������
                    {
                        const wchar_t* t = sheetread->readStr(row, col);//�ӵ�Ԫ���ж�ȡ�ַ��������ʽ
                        char* pcstr = (char*)malloc(sizeof(char) * (2 * wcslen(t) + 1));//����ռ��С����Ŷ��������ݣ�wcslen()�������ظ�������t�ĳ���
                        memset(pcstr, 0, 2 * wcslen(t) + 1);//��ʼ���������
                        w2c(pcstr, t, 2 * wcslen(t) + 1); //pcstr�����ݶ�������Ҫ�������ı����ת�������Ϊת������
                        //cout << pcstr << endl;//���
                        company[++cnt] = pcstr;
                        free(pcstr);
                    }
                    else if (celltype == CELLTYPE_NUMBER)//���ÿ����Ԫ������Ϊ��ֵ����
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
        ////����excel
        book->save(L"A�ɹ�˾���.xlsx");
        book->release();
    }
}
//��HASH ASL
double HashAsl() //��ƽ�����ҳ���
{
    int cnt = 0;//����Ԫ����Ҫ�ȽϵĴ���
    for (int i = 0; i < HASH_M; i++) {
        HLinkList p = H[i]->next;
        int cnt1 = 0;//�ڵ�����ַ�еıȽϴ���
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
//��ѯ1 ���ڹ�ϣ��Ļ�����Ϣ��ѯ
void Seek_Hash()  
{

    CreateHash();
    string code;
    cout << "������Ҫ���ҵĹ�Ʊ���룺" << endl;
    cin >> code;

    //����������INDEX
    int len = code.size();
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += (int)code[i];
    }
    int index = sum % HASH_M;

    HLinkList p = H[index];//ָ��ָ���ѯ�����ڹ�ϣ���е�λ��

    //������벻����
    int choice = 1;
    while (p->next == NULL&& choice!=2) {
        cout << "����ʧ�ܣ��ù�Ʊ����Ĺ�Ʊ�����ڣ�" << endl;
        cout << "1.�������룺" << endl;
        cout << "2.�˳���" << endl;
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
            cout << "���ҳɹ�����Ϣ���£�" << endl;
            cout << "��Ʊ���ƣ�" << p->data.name << endl;
            cout << "��Ʊ���룺" << p->data.stock_code << endl;
            cout << "��Ʊ����һ����ҵ��" << p->data.first_trade << endl;
            cout << "����������ҵ��" << p->data.second_trade << endl;
            cout << "��Ӫҵ��" << p->data.business << endl;
            cout << "ASL��" << HashAsl() << endl;
            cout << endl;
            break;
        }
        p = p->next;
    }
    system("pause");
}




//��nextֵ
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
//KMP�㷨
int Index_KMP(string S, string T) {
    int next[100];
    get_next(T, next);
    int i = 0, j = 0;
    int lens = S.length(), lent = T.length();//����S��ģʽ��T�ĳ���
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
//��ѯ2 ����KMP�㷨����վ��ѯ
void Seek_KMP(Stock stock[])
{
    cout << endl << "��������վ����:";
    string web;
    cin >> web;
    bool flag = false;
    for (int i = 0; i < 200; i++)
    {
        if (Index_KMP(stock[i].web, web) != -1)
        {
            if (!flag)
            {
                cout << endl << "����KMP�㷨�õ��ù�˾��Ϣ��" << endl;
            }
            flag = true;
            cout << "��Ʊ���� " << " ��˾����" << " ��˾��վ" << endl;
            cout << stock[i].stock_code<< " " << stock[i].name << " " << stock[i].web << endl;
        }
    }
    if (!flag)
    {
        cout << endl << "δ�ҵ���Ӧ��վ���ƵĹ�Ʊ��" << endl << endl;
    }
    system("pause");
    system("cls");

}


//������
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
//���������ҽ��
bool BiTree::SearchNode(BiTree* T, BiTree* f, string stock_code, BiTree** p) {
    if (!T) {
        *p = f;
        return false;
    }
    else if (T->data.stock_code == stock_code) {
        system("cls");//����
        search_flag = false;
        cout << "����Ϊ��Ʊ��ϸ��Ϣ��" << endl;
        cout << "��Ʊ����: " << T->data.stock_code << endl;
        cout << "���̼�: " << T->data.deal->open_price << endl;
        cout << "���̼�: " << T->data.deal->close_price << endl;
        cout << "�ǵ���: " << T->data.deal->iod_rate << endl;
        return true;
    }
    else if (T->data.stock_code > stock_code) {
        return SearchNode(T->lchild, T, stock_code, p);
    }
    else if (T->data.stock_code < stock_code) {
        return SearchNode(T->rchild, T, stock_code, p);
    }
}
//����������
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
//����������
void BiTree::inorder(BiTree* T) {
    //�������
    if (T != NULL) {
        m++;
        inorder(T->lchild);
        total += m;
        inorder(T->rchild);
    }
}
//��ѯ3 ���ڶ�������������Ϣ��ѯ
void Seek_BiTree(BiTree* T, Stock stock[]) {
    BiTree* p = new BiTree;
    BiTree* f = new BiTree;
    //��ѯ��� 
    string code;
    cout << "��������Ҫ��ѯ�Ĺ�Ʊ����" << endl;
    cin >> code;
    search_flag = true;

    //���ҽ���Ƿ����
    T->SearchNode(T, NULL, code, &p);
    if (search_flag)
    {
        cout << endl << "������Ĺ�Ʊ��������" << endl;
    }
    else {
        T->inorder(T);//����
        cout << "ASL: " << T->asl() << endl;
    }

    system("pause");
    system("cls");//����
}




//�������׼۸���Ϣ�ĵ�����
void CreatedealList(Stock stock[])
{
    L = new dealNode;
    L->next = NULL;
    dealList r = L;
    string s = "D://���ݽṹ����//code//ks_12_22//ks_12_22//node//";
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
            //���ձ�ͷ
            int cnt = 0;
            dealList p = new dealNode;

            //��ʼ��
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
                getline(inFile, temp);  //��һ�к����������temp�����ܣ���֤��������һ��
            }
            r->next = p;
            r = p;//β�巨��������
        }
        inFile.close();
    }

}
//������׼۸�����
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
//��ѯ4  ���ڵ�����Ľ��׼۸���Ϣ��ѯ
void Seek_Deal(Stock stock[])
{
    
    CreatedealList(stock);
    cout << "����������ɹ�" << endl;
    string date;
    cout << "������Ҫ���ҵ����ڣ�" << endl;
    cin >> date;
    dealList p = L->next;
    int flag = 0;
    //����������
    for (int i = 1; i <= 162; i++) {
        if (p->data[i].date == date) {
            flag = 1;
            break;
        }
    }
    while (flag == 0) {
        cout << "û�и����ڹ�Ʊ��������ݣ����������룺" << endl;
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
        cout << "���ҳɹ������������й�Ʊ�Ŀ��̼ۡ����̼��Լ��ǵ������£�" << endl;
        cout <<  "��Ʊ����             " <<  "��Ʊ����" << "                 " << "���̼�" << setw(10) << "���̼�" << setw(10) << "�ǵ���" << endl;
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


//��ѯϵͳ�˵�
void Menu_Seek(BiTree* T, Stock stock[])
{
    int choice;
    do {
        system("cls");
        cout << "                 ��ѯϵͳ " << endl;
        cout << "          1�����ڹ�ϣ��Ĺ�Ʊ������Ϣ��ѯ" << endl;
        cout << "          2������KMP�Ĺ�Ʊ��ַ��ѯ" << endl;
        cout << "          3�����ڶ����������Ĺ�Ʊ������Ϣ��ѯ" << endl;
        cout << "          4�����ڵ�����Ĺ�Ʊ�۸���Ϣ��ѯ" << endl;
        cout << "          5��������һ���˵�" << endl;
        cout << "��ѡ��";
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6) {   //�ж����������Ƿ�Ϸ� 
            cout << ("����������������룺");
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


//����ĳһ�����й�Ʊ�ĵ�����
void Create_dateList(dateList& dateL, string date)   
{
    dateL = new dateNode;
    dateL->next = NULL;
    dateList r = dateL;
    dealList p = L->next;  //��ѯ3.4�й����ĵ�����
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
                r->next = q;  //��巨����������
                r = q;
                break;
            }
        }
        p = p->next;
    }
}
//���ĳһ�����й�Ʊ�ĵ�����
int Output_dateList(dateList dateL)
{
    dateList p = dateL->next;
    int cnt = 0;
    cout << setw(3) << "���" << setw(10) << "��Ʊ����" << setw(10) << "��Ʊ����" << setw(10) << "���̼�" << setw(10) << "���̼�" << setw(10) << "�ǵ���" << endl;
    while (p) {
        cout << setw(3) << ++cnt << setw(10) << p->code << setw(10) << p->name << setw(10) << p->open_price << setw(10) << p->close_price << setw(10) << p->iod_rate << endl;
        p = p->next;
    }
    return cnt;
}
//����������Ľ��д����
void WriteExcel_insertSort_openprice(dateList dateL, int cnt)
{
    dateList p = dateL;
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//�����û�ע����Ϣ���Ƿ��������������֤�������һ���޷�����
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//���ɵ�һ����
        if (sheetwrite)
        {
            for (int row = 0; row <= cnt; ++row)//���б���
            {
                if (row > 0)
                    p = p->next;
                for (int col = 0; col < 6; ++col)//���б���
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "���"; break;
                        case 1:str = "��Ʊ����"; break;
                        case 2:str = "��Ʊ����"; break;
                        case 3:str = "���̼�"; break;
                        case 4:str = "���̼�"; break;
                        case 5:str = "�ǵ���"; break;
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
        ////����excel
        if (book->save(L"�۸���ǵ���������(���̼ۣ�.xlsx"))
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
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//�����û�ע����Ϣ���Ƿ��������������֤�������һ���޷�����
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//���ɵ�һ����
        if (sheetwrite)
        {
            for (int row = 0; row <= cnt; ++row)//���б���
            {
                if (row > 0)
                    p = p->next;
                for (int col = 0; col < 6; ++col)//���б���
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "���"; break;
                        case 1:str = "��Ʊ����"; break;
                        case 2:str = "��Ʊ����"; break;
                        case 3:str = "���̼�"; break;
                        case 4:str = "���̼�"; break;
                        case 5:str = "�ǵ���"; break;
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
        ////����excel
        if (book->save(L"�۸���ǵ���������(���̼ۣ�.xlsx"))
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
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//�����û�ע����Ϣ���Ƿ��������������֤�������һ���޷�����
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//���ɵ�һ����
        if (sheetwrite)
        {
            for (int row = 0; row <= cnt; ++row)//���б���
            {
                if (row > 0)
                    p = p->next;
                for (int col = 0; col < 6; ++col)//���б���
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "���"; break;
                        case 1:str = "��Ʊ����"; break;
                        case 2:str = "��Ʊ����"; break;
                        case 3:str = "���̼�"; break;
                        case 4:str = "���̼�"; break;
                        case 5:str = "�ǵ���"; break;
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
        ////����excel
        if (book->save(L"�۸���ǵ���������(�ǵ�����.xlsx"))
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
//ֱ�Ӳ������򣨿��̼ۣ�
void Analyse_Sort_Straight_openprice(string date)
{
        dateList dateL;
        Create_dateList(dateL, date);
        dateList pp = dateL->next->next;//����dateL�еĵڶ���Ԫ�أ���Ϊ��һ��Ҫ��dateL���ֻ��һ��Ԫ�ص�������
        dateL->next->next = NULL;//��dateL���ֻ��һ��Ԫ�ص������
        //��dateL�ĵڶ���Ԫ�ؿ�ʼ��������dateLֱ����β
        while (pp) {
            // ���в����������Ĳ���ʱ��ı�p->next��ֵ������Ҫѡ����һ��
            dateList q = pp->next;
            // Ϊ����Ƚϣ���һ��ָ��pre,ָ����������
            dateList pre = dateL;
            while (pre->next != NULL && pre->next->open_price > pp->open_price) {
                pre = pre->next;
            }
            //���ڵ�pre->next�Ǵ�ͷ��ʼ��һ��С��Ҫ����Ԫ��pp�Ľ��
            pp->next = pre->next; //pp���뵽������
            pre->next = pp;
            // ��q��p��ֵ�ָ�������ʹ��ѭ������ָ������������һ���ڵ�
            pp = q;
        }
        cout << "**********************************************************************************************************" << endl;
        cout << "���ݿ��̼����������ڹ�Ʊ��Ϣ��" << endl;
        int cnt = Output_dateList(dateL);//��������
        WriteExcel_insertSort_openprice(dateL, cnt);
        cout << "д��ɹ�" << endl;
}
//ֱ�Ӳ����������̼ۣ�
void Analyse_Sort_Straight_closeprice(string date)
{
    dateList dateL;
    Create_dateList(dateL, date);
    dateList pp = dateL->next->next;//����dateL�еĵڶ���Ԫ�أ���Ϊ��һ��Ҫ��dateL���ֻ��һ��Ԫ�ص�������
    dateL->next->next = NULL;//��dateL���ֻ��һ��Ԫ�ص������
    //��dateL�ĵڶ���Ԫ�ؿ�ʼ��������dateLֱ����β
    while (pp) {
        // ���в����������Ĳ���ʱ��ı�p->next��ֵ������Ҫѡ����һ��
        dateList q = pp->next;
        // Ϊ����Ƚϣ���һ��ָ��pre,ָ����������
        dateList pre = dateL;
        while (pre->next != NULL && pre->next->close_price > pp->close_price) {
            pre = pre->next;
        }
        //���ڵ�pre->next�Ǵ�ͷ��ʼ��һ��С��Ҫ����Ԫ��pp�Ľ��
        pp->next = pre->next; //pp���뵽������
        pre->next = pp;
        // ��q��p��ֵ�ָ�������ʹ��ѭ������ָ������������һ���ڵ�
        pp = q;
    }
    cout << "*********************************************************************************************************************" << endl;
    cout << "�������̼����������ڹ�Ʊ��Ϣ��" << endl;
    int cnt = Output_dateList(dateL);//��������
    WriteExcel_insertSort_closeprice(dateL, cnt);
    cout << "д��ɹ�" << endl;
}
//ֱ�Ӳ��������ǵ�����
void Analyse_Sort_Straight_iodrate(string date)
{
    dateList dateL;
    Create_dateList(dateL, date);
    dateList pp = dateL->next->next;//����dateL�еĵڶ���Ԫ�أ���Ϊ��һ��Ҫ��dateL���ֻ��һ��Ԫ�ص�������
    dateL->next->next = NULL;//��dateL���ֻ��һ��Ԫ�ص������
    //��dateL�ĵڶ���Ԫ�ؿ�ʼ��������dateLֱ����β
    while (pp) {
        // ���в����������Ĳ���ʱ��ı�p->next��ֵ������Ҫѡ����һ��
        dateList q = pp->next;
        // Ϊ����Ƚϣ���һ��ָ��pre,ָ����������
        dateList pre = dateL;


        while (pre->next != NULL && stod( pre->next->iod_rate) > stod(pp->iod_rate)) {
            pre = pre->next;
        }
        //���ڵ�pre->next�Ǵ�ͷ��ʼ��һ��С��Ҫ����Ԫ��pp�Ľ��
        pp->next = pre->next; //pp���뵽������
        pre->next = pp;
        // ��q��p��ֵ�ָ�������ʹ��ѭ������ָ������������һ���ڵ�
        pp = q;
    }
    cout << "********************************************************************************************************************" << endl;
    cout << "�����ǵ������������ڹ�Ʊ��Ϣ��" << endl;
    int cnt = Output_dateList(dateL);//��������
    WriteExcel_insertSort_iodrate(dateL, cnt);
    cout << "д��ɹ�" << endl;
}
//ֱ�Ӳ�������
void Analyse_Sort_Straight(Stock stock[])
{
    CreatedealList(stock);
    dealList p = L->next;

    string date;
    cout << "������Ҫ���ҵ����ڣ�(��ʽ��20211108)" << endl;
    cin >> date;

    int flag = 0;
    for (int i = 1; i <= 162; i++) {
        if (p->data[i].date == date) {
            flag = 1;
            break;
        }
    }

    while (flag == 0) {
        cout << "û�и����ڹ�Ʊ��������ݣ����������룺" << endl;
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



//����60֧��Ʊ��Ϣ
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
    
    dealList p = L->next;  //��ѯ3.4�й����ĵ�����
    while (p) {
        for (int i = 1; i <= 60; i++) {
            if (s60[i].stock_code == p->code) {
                double Max = stod(p->data[1].iod_rate);
                int ans = 1;
                for (int j = 1; j <= 162; j++) {
                    if (p->data[j].iod_rate == "") {     //˵��data[j]����û�д�����
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


//�����㷨
int Partition(Stock60 array[], int low, int high)
{
    double num = stod(array[low].iodrate);
    array[0] = array[low];
    while (low < high) 
    {
        while (low < high && stod(array[high].iodrate) >= num)
            high--;              
        array[low] = array[high];  //������С���ƶ����Ͷ�
        while (low < high && stod(array[low].iodrate) <= num) {
            low++;
        }
        array[high] = array[low];   //���������ƶ����߶�
    }
    array[low] = array[0];       //�����¼��λ
    return low;                 //��������λ��
}
void Qsort(Stock60 array[], int low, int high)
{
    if (low < high) {
        int m = Partition(array, low, high); //mΪ����λ��
        Qsort(array, low, m - 1);    //�����ӱ�ݹ�����
        Qsort(array, m + 1, high);    //�����ӱ�ݹ�����
    }
}
//���������Ʊ�۸����
void Analyes_Sort_Quick(Stock stock[], Score score[])
{
    CreateS60(stock,score);
    for (int i = 1; i < 60; i++)
        cout << endl << i + 1 << "  " << s60[i].node << " " << s60[i].stock_code << " " << s60[i].name << " " << s60[i].iodrate << " " << s60[i].date << " " << s60[i].first_trade << "\t" << endl;
    cout << "������һ����ҵ���ƣ�" << endl;
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
        cout << "��һ����ҵ�Ĺ�Ʊ������ ���������룺" << endl;
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
    cout << "�������������£�" << endl;
    cout << endl << "���  �ڵ�  ��Ʊ����  ��Ʊ����  �ǵ���  ����" << "\t" << endl;
     for (int i = cnt; i >= 1; i--)
        cout << endl << ++count << "  " << array[i].node << " " << array[i].stock_code << " " << array[i].name << " " << array[i].iodrate << " " << array[i].date << "\t" << endl;
   
    system("pause");
}



//��ѡ���������֣�
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
            if (s[i].sco >= s[max].sco)  //����ͬԪ��ʱ������Ҫ��λ���������һ������
            {
                max = i;
            }
            if (s[i].sco < s[min].sco)  //��С��Ҫ��λ�������ǰ��һ����С�ģ���������ʹѡ�������㷨�ȶ�
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
//��ѡ���������̼ۣ�
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
            if (s[i].close_price >= s[max].close_price)  //����ͬԪ��ʱ������Ҫ��λ���������һ������
            {
                max = i;
            }
            if (s[i].close_price < s[min].close_price)  //��С��Ҫ��λ�������ǰ��һ����С�ģ���������ʹѡ�������㷨�ȶ�
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
//��ѡ������д����
void WriteExcel_SelectSort_score(Stock60 s[])
{
    
    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//�����û�ע����Ϣ���Ƿ��������������֤�������һ���޷�����
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//���ɵ�һ����
        if (sheetwrite)
        {
            for (int row = 0; row <= 60; ++row)//���б���
            {
                for (int col = 0; col < 6; ++col)//���б���
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "���"; break;
                        case 1:str = "��Ʊ����"; break;
                        case 2:str = "��Ʊ����"; break;
                        case 3:str = "����"; break;
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
        ////����excel
        if (book->save(L"��������.xlsx"))
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

    Book* book = xlCreateXMLBook();//����һ��XLSX��ʵ��,��ʹ��ǰ�����ȵ������������������excel�Ķ���
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");//�����û�ע����Ϣ���Ƿ��������������֤�������һ���޷�����
    if (book) {
        Sheet* sheetwrite = book->addSheet(L"sheet1");;//���ɵ�һ����
        if (sheetwrite)
        {
            for (int row = 0; row <= 60; ++row)//���б���
            {
                for (int col = 0; col < 6; ++col)//���б���
                {
                    string str;
                    stringstream ss;
                    if (row == 0) {
                        switch (col) {
                        case 0:str = "���"; break;
                        case 1:str = "��Ʊ����"; break;
                        case 2:str = "��Ʊ����"; break;
                        case 3:str = "���̼�"; break;
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
        ////����excel
        if (book->save(L"���̼�����.xlsx"))
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
//��ѡ������
void Analyse_Sort_Select(Stock stock[], Score score[])
{
    CreateS60(stock, score);
    SelectSortScore(s60);
    WriteExcel_SelectSort_score(s60);
    cout << "*******����������*************" << endl;
    cout << endl << "���      ��Ʊ����       ��Ʊ����         ����" << "\t" << endl;
   for(int k=60;k>0;k--)
        cout << endl << 61-k << "       " << s60[k].stock_code << "         " << s60[k].name << "      " << s60[k].sco << endl;


   SelectSortCloseprice(s60);
   WriteExcel_SelectSort_Closeprice(s60);
   cout << "*******���̼�������*************" << endl;
   cout << endl << "���    ��Ʊ����           ��Ʊ����       ���̼�" << "\t" << endl;
   for (int k = 60; k > 0; k--)
       cout << endl << 61 - k << "       " << s60[k].stock_code << "        " << s60[k].name << "    " << s60[k].close_price<<   endl;
   system("pause");
}


//����ͼ
void Create_Graph(Val v[])
{
    //����
    int  n, m, t1, t2, t3;
    n = 61;
    m = 84;//n��ʾ���������m��ʾ�ߵ�����
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


//����Floyd�Ĺ�Ʊ����Լ���
void Analyes_Floyd(Val v[])
{
    Create_Graph(v);
    int n = 61;
    //���Ĵ���
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
    cout << "��������Ҫ���ҵ������㣺" << endl;
    int node1, node2;
    cin >> node1 >> node2;
    if (node1 > 0 && node1 <= 60 && node1 > 0 && node1 <= 60)
        cout << node1 << "--->" << node2 << "    ���·����" << Gp.matrix[node1][node2] << endl;
    else
    {
        cout << "�������" << endl;
    }
   
    system("pause");
}


//����Prim��С�������Ĺ�Ʊ����ɸѡ
void Analyes_Prim( Val v[],Stock stock[])
{
    int start = 1;
    Create_Graph(v);
    Gp.numNodes = 60;
    int value[61], v1[61], v2[61], tempvalue, tempv1, tempv2;//��ʱ�洢��С���ıߺ�Ȩֵ
    int Visited_count = 0, sum = 0; //�ֱ�Ϊ�ѱ����ʵĽڵ�������Ȩֵ
    bool* isVisited = new bool[Gp.numNodes + 1];//�жϸ����Ƿ񱻷��ʹ�
    int* Edge = new int[Gp.numNodes + 1]; //��¼���㣨���δ�����ʵĻ������ѱ����ʵĸ�����ĳһ�����С����

    for (int i = 1; i <= Gp.numNodes; i++)
        isVisited[i] = false;

    isVisited[start] = true;

    for (int i = 1; i <= Gp.numNodes; i++) {
        Edge[i] = Gp.matrix[start][i];
    }
    int last_Node = 0;//�����δ����״̬ת���Է���״̬�ĵ�����
    Visited_count++;
    cout << "��С�������ı�����:" << endl;
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

        //Ѱ�ұߵ����
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

        //���
        cout << Start_Node << "      "<<last_Node << "      " << min_Edge << endl;
        v1[Visited_count] = Start_Node;
        v2[Visited_count] = last_Node;
        value[Visited_count] = min_Edge;

        

        Visited_count++;
        isVisited[last_Node] = true;
        sum += min_Edge;
        for (int i = 1; i <= Gp.numNodes; i++) { //��Ϊ�ɸոշ��ֵĵ㣬����Edge�����е�ֵ
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
    cout << "��Ȩֵ��������С�������ı�����:" << endl;
    cout << "v1   " << "   v2" << "    value" << endl;
    for(int i=1;i<60;i++)
    cout << v1[i] << "      " << v2[i] << "      " << value[i] << endl;
    cout << "��С��������Ȩֵ֮��Ϊ��" << sum << endl;
    cout << "��Ȩֵѡȡ��6֧��Ʊ���£�" << endl;
    for (int i = 1; i <= 3; i++)
        cout << stock[v1[i]].stock_code << "  " << stock[v1[i]].short_name << " &&&&&& " << stock[v2[i]].stock_code << "  " << stock[v2[i]].short_name << endl;
    
    system("pause");
    delete[]isVisited;
    delete[]Edge;
}

int n, tot = 0, k = 0;//n�˵�������tot��¼���մ𰸣�k�Ѿ������˶��ٱ� 
int fat[200010];//��¼�����ϴ� 
struct node
{
    int from, to, dis;//�ṹ�崢��� 
}edge[200010];
bool cmp(const node& a, const node& b)//sort����
{
    return a.dis < b.dis;
}
int father(int x)//�Ҽ����ϴ󣬲��鼯��һ���� 
{
    if (fat[x] != x)
        return father(fat[x]);
    else return x;
}
void unionn(int x, int y)//�������壬���鼯��һ���� 
{
    fat[father(y)] = father(x);
}
void Analyse_Kruskal(Val v[], Stock stock[])
{
    n = 60; m = 83;
    cout << "��С�������ı�����:" << endl;
    cout << "v1   " << "   v2" << "    value" << endl;
    for (int i = 1; i <= m; i++)
    {
        edge[i].from = v[i].node1;
        edge[i].to = v[i].node2;
        edge[i].dis = v[i].num;
    }
    for (int i = 1; i <= n; i++) fat[i] = i;//�Լ��ʼ�����Լ����ϴ� ����ʼ���� 
    sort(edge + 1, edge + 1 + m, cmp);//��Ȩֵ����kruskal�����֣� 
    for (int i = 1; i <= m; i++)//��С������� 
    {
        if (k == n - 1) break;//n������Ҫn-1�������� 
        if (father(edge[i].from) != father(edge[i].to))//���粻��һ������ 
        {
            cout << edge[i].from << "  " << edge[i].to << "   " << edge[i].dis << endl;
            unionn(edge[i].from, edge[i].to);//���� 
            tot += edge[i].dis;//��¼��Ȩ 
            k++;//�����ӱ���+1 
        }
    } 
    cout << "������ȨֵΪ:" << tot << endl;
    cout << "��Ȩֵѡȡ��6֧��Ʊ���£�" << endl;
    for (int i = 1; i <= 3; i++)
        cout << stock[edge[i].from].stock_code << "  " << stock[edge[i].from].short_name << " &&&&&& " << stock[edge[i].to].stock_code << "  " << stock[edge[i].to].short_name << endl;
   
    system("pause");
}

int color[60], graph[61][61];
// ��������㷨
bool bfs(int s, int n) {
    //0Ϊ��ɫ��1Ϊ��ɫ
    queue<int> q;
    q.push(s);
    color[s] = 1;
    while (!q.empty()) {
        int from = q.front();
        q.pop();
        for (int i = 1; i <= n; i++) {
            if (graph[from][i] && color[i] == -1) {
                q.push(i);
                color[i] = !color[from];//Ⱦ�ɲ�ͬ����ɫ 
            }
            if (graph[from][i] && color[from] == color[i])//��ɫ����ͬ�����Ƕ���ͼ 
                return false;
        }
    }
    return true;
}
//�ж϶���ͼ
void Analyse_BiGraph(Val v[])
{
    Create_Graph(v);
    int n=10, m, a, b, i;//nΪ�㣬mΪ��
    int node[11];
    memset(color, -1, sizeof(color));//��ʼ��

    //������Ҫ�жϵĵ�
    cout << "������������Ҫ�жϵ�ʮ����:" << endl;
    for (i = 1; i <= n; i++)
        cin >> node[i];
    cout << "�����й����ı��У�" << endl;
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
        if (color[i] == -1 && !bfs(i, n)) {//����������ͨ��֧ 
            flag = true;
            break;
        }
    if (flag)
        cout << "NO" << endl;
    else
        cout << "YES" << endl;
    system("pause");
}

//����ϵͳ�˵�
void Menu_Analyse(Stock stock[], Score score[] ,Val v[])
{
    int choice;
    do {
        system("cls");
        cout << "*******************����ϵͳ*********************" << endl;
        cout << "         1������ֱ�Ӳ�������Ĺ�Ʊ�۸����" << endl;
        cout << "         2�����ڿ�������Ĺ�Ʊ�۸����" << endl;
        cout << "         3�����ڼ�ѡ������Ĺ�Ʊ�۸����" << endl;
        cout << "         4������Floyd�Ĺ�Ʊ����Լ���" << endl;
        cout << "         5������Prim��С�������Ĺ�Ʊ����ɸѡ" << endl;
        cout << "         6������Kruskal��С�������Ĺ�Ʊ����ɸѡ" << endl;
        cout << "         7�����ڶ���ͼ�Ĺ�Ʊ����ɸѡ" << endl;
        cout << "         8��������һ���˵�" << endl;
        cout << "��ѡ��";
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7 && choice != 8) {   //�ж����������Ƿ�Ϸ� 
            cout << ("����������������룺");
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
    
    
    cout << "�������˾��Ϣ��..." << endl;
    Stock* stock;
    stock = new Stock[666];
    Read_stock(stock);
    cout << "�������˾������Ϣ�ɹ���" << endl;

    cout << "���빫˾���ֳɹ�!" << endl;
    Score score[61];
    Read_score(score);


    cout << "���빫˾��ع�ϵ�ɹ�!" << endl;
    Val v[84];
    Read_val(v);

    system("pause");

    BiTree* T = new BiTree;
    T = NULL;
    for (int i = 0; i < 200; i++)
        T->InsertNode(&T, stock[i]);

    
    //����
    int flag = 0;
    int k = 0;
    do
    {
        system("cls");
       
        cout << "            ��Ʊ��ѯ�����ϵͳ" << endl;
        cout << "               1.��ѯϵͳ                         " << endl;
        cout << endl;
        cout << "               2.����ϵͳ                        " << endl;
        cout << endl;
        cout << "               3.�˳�ϵͳ                         " << endl;


        //��ֹ�Ƿ�����
        if (flag) {                                     
            flag = 0;
            cout << "�������������ѡ��";
            cin >> k;
        }
        else {
            cout << "��ѡ��";
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
