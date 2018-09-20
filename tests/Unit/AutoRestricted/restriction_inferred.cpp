// RUN: %cxxamp %s -o %t.out && %t.out
#include <hc.hpp>

int f1() [[cpu, hc]] {return 1;} 
// DeclRefExpr
int f2() restrict(cpu,auto) {
  return f1();
}

// null
void f_null() restrict(cpu,auto) {
}


// ReturnStmt
int f_return() restrict(cpu,auto) {
  return 1;
}

// CXXTryStmt
// GotoStmt
// LabelStmt


int AMP_CPU_Func() [[cpu, hc]] 
{
  f2();  // OK, 'auto' is inferred to amp, so f2 is both (cpu,amp) restricted
  f_null(); // OK
  f_return();  // OK 
}

int main(void)
{
  return 0;  // expected: success
}

