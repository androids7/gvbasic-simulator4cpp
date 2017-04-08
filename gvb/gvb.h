#ifndef GVBASIC_GVB_H
#define GVBASIC_GVB_H

#include <vector>
#include <unordered_map>
#include <cstdio>
#include "node_man.h"
#include "data_man.h"
#include "random.h"
#include "value.h"
#include "file.h"
#include "device.h"

namespace gvbsim {

class Stmt;
class Dim;
class Assign;
class While;
class DefFn;
class Expr;
class Id;
class For;
class Next;
class ArrayAccess;
class FuncCall;
class Binary;
class UserCall;
class While;
class If;
class On;
class Print;
class Locate;


class GVB {
   friend class Compiler;

   struct Loop {
      int line, label;

      union {
         Stmt *stmt;

         struct {
            For *stmt;
            double dest, step;
         } _for;
      };
   };

   struct Sub {
      int line, label;
      Stmt *stmt; // gosub的下一条语句

      Sub(int line, int label, Stmt *stmt)
            : line(line), label(label), stmt(stmt) { }
   };

   // 单个值
   struct Single {
      union {
         int ival;
         double rval;
      };
      std::string sval;

   public:
      Single() { }
      Single(int ival) : ival(ival) { }
      Single(double rval) : rval(rval) { }
      Single(const std::string &s) : sval(s) { }
      Single(const char *s) : sval(s) { }
   };

   // 数组
   struct Array {
      std::vector<unsigned> bounds;
      std::vector<Single> vals;
   };

private:
   Device m_device;
   Stmt *m_head;
   std::vector<Single> m_stack;
   Single m_top;
   std::vector<Sub> m_subs;
   std::vector<Loop> m_loops;
   std::unordered_map<std::string, DefFn *> m_funcs; // 用户定义函数
   std::unordered_map<std::string, Single> m_envVar;
   std::unordered_map<std::string, Array> m_envArray;
   Random m_rand;
   NodeManager m_nodeMan;
   DataManager m_dataMan;
   File m_files[3];
   int m_line, m_label;

public:
   GVB();
   ~GVB();

   void build(std::FILE *fp);
   bool isBuilt() const { return static_cast<bool>(m_head); }

   void execute();

private:
   void clearEnv();
   void clearStack();
   void clearFiles();

   void traverse(Stmt *);

   void exe_dim(Dim *);
   void exe_assign(Assign *);
   void exe_for(For *);
   Stmt *exe_next(Next *);
   Stmt *exe_while(While *);
   Stmt *exe_wend();
   void exe_print(Print *);
   Stmt *exe_if(If *);
   Stmt *exe_on(On *);
   void exe_locate(Locate *);

   void eval(Expr *);
   void evalPop(Expr *);
   void eval_id(Id *);
   void eval_access(ArrayAccess *);
   void eval_func(FuncCall *);
   void eval_binary(Binary *);
   void eval_usercall(UserCall *);

private:
   static void error(int line, int label, const char *s, ...);

   static std::string &removeAllOf(std::string &, const char *, size_t);
};

}

#endif //GVBASIC_GVB_H
