#!/usr/bin/awk -f

BEGIN {
  count = 1;
}

/.*/ {

  incs[count] = $1;
  suites[count++] = $2;
}

END {
  includes();
  output("using namespace unittests;");
  
  output("int main(int argc, char** argv)");
  output("{");
  output("  StdoutLogger logger;");
  output("  TestRunner runner(&logger);");

  add_suites();
  output("");
  output("  if (argc > 1)");
  output("  {");
  output("    for (int i = 1; i < argc; ++i)");
  output("    {");
  output("      runner.run(argv[i]);");
  output("    }");
  output("  }");
  output("  else");
  output("  {");
  output("    runner.run();")
  output("  }");
  output("");
  output("}");
  output("");
  output("/* vim: set cindent sw=2 expandtab : */");
  output("");
}

function output(line)
{
  print line
}

function includes(      i)
{
  output("#include <iostream>")
  output("#include \"ut/unittests.h\"")
  output("#include \"ut/stdoutlogger.h\"")
  output("");
  for (i = 1; i < count; ++i)
  {
    output("#include \""incs[i]"\"");
  }
  output("");
}

function add_suites(    i)
{
  for (i = 1; i < count; ++i)
  {
    output("  CALL_ADD_TO_RUNNER("suites[i]", &runner);");
  }
}

# vim: set cindent sw=2 expandtab :

