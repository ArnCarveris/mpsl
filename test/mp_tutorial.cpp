// [MPSL-Test]
// MathPresso's Shading Language with JIT Engine for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// [Dependencies]
#include "./mpsl.h"
#include "./mp_utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Args {
  double a, b;
  float c;
  double result;
};

int main(int argc, char* argv[]) {
  mpsl::Context ctx = mpsl::Context::create();
  mpsl::LayoutTmp<> args;

  args.addMember("a"   , mpsl::kTypeDouble  | mpsl::kTypeRO, MPSL_OFFSET_OF(Args, a));
  args.addMember("b"   , mpsl::kTypeDouble  | mpsl::kTypeRO, MPSL_OFFSET_OF(Args, b));
  args.addMember("c"   , mpsl::kTypeFloat   | mpsl::kTypeRO, MPSL_OFFSET_OF(Args, c));
  args.addMember("@ret", mpsl::kTypeDouble  | mpsl::kTypeWO, MPSL_OFFSET_OF(Args, result));

  const char body[] =
    "double main() {\n"
    "  double x = sqrt(a * b) * c;\n"
    "  return ++x;\n"
    "}\n";
  printf("[Program]\n%s\n", body);

  uint32_t options = 
    mpsl::kOptionVerbose  |
    mpsl::kOptionDebugAst |
    mpsl::kOptionDebugIR  |
    mpsl::kOptionDebugASM ;
  TestLog log;

  mpsl::Program1<> program;
  mpsl::Error err = program.compile(ctx, body, options, args, &log);
  if (err) {
    printf("Compilation failed: ERROR 0x%08X\n", err);
  }
  else {
    Args args;
    args.a = 4.0;
    args.b = 16.0;
    args.c = 0.5f;

    err = program.run(&args);
    if (err == mpsl::kErrorOk)
      printf("Return=%.17g\n", args.result);
    else
      printf("Execution failed: ERROR %08X\n", static_cast<unsigned int>(err));
  }

  return 0;
}
