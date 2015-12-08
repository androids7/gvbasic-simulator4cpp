#pragma once

#include <istream>
#include <ostream>
#include <hash_map>
#include <string>
#include <vector>
#include <utility>
#include <cstdarg>
#include "lex.h"
#include "tree.h"
#include "util.h"
#include "file.h"

using std::hash_map;
using std::string;
using std::vector;
using std::pair;

typedef unsigned char byte;

struct Device { // ���궼��0��ʼ
    typedef unsigned char coord;

    enum { //screen mode
        TEXT, GRAPH
    };
    enum { //input type
        NUM, STR
    };
    enum { //draw mode
        CLEAR, PAINT, INVERSE
    };
    enum { //paint mode
        _COPY, _OR, _NOT, _AND, _XOR
    };

    virtual void appendText(const string &) = 0;
    virtual void nextLine() = 0; //�����������Ļ�Ϲ�һ��
    virtual void updateLCD() = 0;
    virtual void locate(int row, int col) = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual void setMode(int mode) = 0;
    virtual void cls() = 0;
    virtual string input(const string &prompt, int type) = 0;
    virtual int getkey() = 0;
    virtual void point(coord x, coord y, int mode) = 0;
    virtual void rectangle(coord x1, coord y1, coord x2, coord y2, bool fill, int mode) = 0;
    virtual void line(coord x1, coord y1, coord x2, coord y2, int mode) = 0;
    virtual void ellipse(coord x, coord y, coord rx, coord ry, bool fill, int mode) = 0;

    virtual int peek(int addr) = 0;
    virtual string peek(int addr, int size) = 0;
    virtual void poke(int addr, byte value) = 0;
    virtual void poke(int addr, const char *, int size) = 0;
    virtual void call(int addr) = 0;

    virtual void sleep(int ms) = 0;
    virtual void paint(int addr, int x, int y, byte w, byte h, int mode) = 0;
    virtual bool getPoint(int x, int y) = 0;
    virtual bool checkKey(int keycode) = 0;
};

struct V;
struct Array;
struct Fn;

class GC {
    O *head;//������
public:
    S *link(S *); //ģ�����ɺ���̫�࣬����
    E *link(E *);
    Id *link(Id *);
    Goto *link(Goto *);
    Gosub *link(Gosub *);
    V *link(V *);
    Array *link(Array *);
    Fn *link(Fn *);

    void remove(O *);
public:
    GC();
    ~GC();
    void gc();
};

class Data {
    hash_map<int, int> labels; //label,index
    vector<string> data;
    int p;
public:
    Data() : p(0) {}
    void restore();
    void restore(int label);
    void add(const string &);
    void addLabel(int);
    string &get();
    int size() const;
    bool end() const;
    void clear();
};

struct V : O {
    enum {
        ID, ARRAY
    };

    int type;
    int vtype; //E::V
    long ival;
    double rval;
    string sval;

    V(long i) : type(ID), vtype(E::VINT), ival(i) {}
    V(double r) : type(ID), vtype(E::VREAL), rval(r) {}
    V(const string &s) : type(ID), vtype(E::VSTRING), sval(s) {}
    V() : type(ID) {}
    V(const V *v) : type(ID), vtype(v->vtype), ival(v->ival), rval(v->rval),
        sval(v->sval) {}
};

struct Array : V {
    vector<int> bounds;
    vector<long> ivals;
    vector<double> rvals;
    vector<string> svals;

    Array() { type = ARRAY; }
};

struct IdKey {
    string id;
    int type; //V::
};

class IdHash {
public:
    enum {
        bucket_size = 17
    };
    size_t operator()(const IdKey &) const;
    bool operator()(const IdKey &, const IdKey &) const;
};

struct Fn : O {
    int xvtype, vtype;
    string x;
    E *func;
    Fn(int vtype, const string &x, int xvtype, E *fn)
        : vtype(vtype), x(x), xvtype(xvtype), func(fn) {}
};

struct Record {
    int len; //open�����len
    int total; //field�����len
    vector<string> ids;
    vector<int> bys;
};

class GVB {
public:
    typedef hash_map<IdKey, V *, IdHash> Env;
    typedef hash_map<IdKey, V *, IdHash>::iterator variter;
private:
    Lexer *l;
    int line, label, tok;
    S *head; //��һ�����
    hash_map<int, S *> labels; //һ�еĵ�һ�����
    hash_map<string, int, StringHash> funcs; //���ú���
    vector<S *> refs; //�����������

    typedef hash_map<int, S *>::iterator lbliter;
    typedef hash_map<string, int>::iterator fniter;

    GC gc;
    Data data;

    //compile
    void peek();
    void match(int);

    void solveRefs();
    S *aline();
    S *stmts();
    S *stmt(bool);
    S *findlabel(int, Goto *);
    S *findlabel(int, Gosub *);
    S *findlabel(int, Restore *);
    S *findlabel(On *);
    S *getStmtTail(S *);
    S *ifstmts();
    //S *ifstmt();
    void linkStmt(S *, S *);
    E *expr(int);
    E *expr();
    E *E_(int);
    E *F();
    Id *getId();
    int getIdType(const string &) const;
    enum {
        PRI_LOG = 1, PRI_REL, PRI_ADD_MIN, PRI_TIM_DIV, PRI_NEG, PRI_POW, PRI_NOT
    };
    int getp(int) const;
    E *fold(E *);
    bool isZero(double) const;
    bool notZero(double) const;
    string &add0x1F(string &) const;
    string &remove0x1F(string &) const;
    string &removeNull(string &) const;
    string &rtrim(string &) const;
    int getFileNum();
    void cerror(const char *, ...) const;

    void loadFuncs();

    //run
    vector<S *> loops, subs;
    Env vars;
    hash_map<string, Fn *> funs;
    vector<V *> dstack;
    V top; //dstack������Ԫ��
    Device *device;
    FM fileman;
    Record records[3];

    unsigned seed; //���������
    static const int RAND = 0xffff;

    typedef vector<S *>::iterator stmiter;
    typedef hash_map<string, Fn *>::iterator funiter;

    void traverse(S *);
    void clearVars();
    void clearFuns();
    void clearFiles();
    //data stack operation
    void clearData();
    void push(double);
    void push(const string &);
    void push(const V *);
    void pop();
    bool empty() const;

    void eval(E *); //eval_ + pop
    void eval_(E *);
    //ռ��top
    V *getValue(Id *);

    void randomize(unsigned);
    unsigned random();
    unsigned sequence();
#define rerror cerror
public:
    GVB(Device * = 0);
    ~GVB();
    //bool load(const char *);
    void setDevice(Device *);
    bool load(std::istream &);
    bool built() const { return head != 0; }
    void build();
    void execute();
    //setDevice();
    int getCurrentLine() const;
    int getCurrentLabel() const;
    S *getTree() { return head; }
    Env &getEnv() { return vars; }
    /*���vars,loopgs,subs�ȵ�����Ϣ*/
    friend void show(GVB &, std::ostream &);
};

/*platform dependent
�����ֹ,�ⲿthrow 1
*/
bool terminated();

/*�ӳ�platform dependent
*/
void delay();