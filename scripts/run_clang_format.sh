git ls-files | egrep "(cpp|hpp)" | grep -v "libraries/args" | grep -v "libraries/json" | xargs clang-format-9 -i {}
