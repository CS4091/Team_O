BGreen='\033[1;32m'
BIRed='\033[1;91m'
BIPurple='\033[1;35m'
NC='\033[0m'

echo -e "${BIPurple}Checking formatting..\n"
cmp -s "${FILE}" formatted_file
for FILE in **/*.cpp; do
    TEMP_FILE=$(mktemp)

    clang-format --style=LLVM "$FILE" > "$TEMP_FILE"

    if ! diff -q "$FILE" "$TEMP_FILE" >/dev/null 2>&1; then
	echo -e "${BIRed}'$FILE' is not properly formatted."
	echo "Run 'clang-format --style=LLVM -i $FILE' to fix the formatting."
    fi

    rm "$TEMP_FILE"
done

for FILE in **/*.h; do
    TEMP_FILE=$(mktemp)

    clang-format --style=LLVM "$FILE" > "$TEMP_FILE"

    if ! diff -q "$FILE" "$TEMP_FILE" >/dev/null 2>&1; then
	echo -e "${BIRed}'$FILE' is not properly formatted."
	echo "Run 'clang-format --style=LLVM -i $FILE' to fix the formatting."
    fi

    rm "$TEMP_FILE"
done

echo -e "${BIPurple}Done.\n"

echo -e "${BIPurple}Running static analysis suite...\n"

echo -e "${NC}Running cppcheck static analysis..."
cppcheck -v --platform=unix64 --language=c++ --std=c++11 --check-level=exhaustive --output-file=cppcheck_report.txt **/. > /dev/null # report should be blank if passed
if [ -s cppcheck_report.txt ]; then
    echo -e "${BIRed}Issues found by cppcheck."
    cat cppcheck_report.txt
else
    echo -e "${BGreen}cppcheck static analysis passed."
    rm cppcheck_report.txt
fi

echo -e "${NC}Running flawfinder static analysis..."
flawfinder --minlevel=0 **/. > flawfinder_report.txt # report should include "no hits found" if passed
if grep -Fq "No hits found" flawfinder_report.txt; then
    echo -e "${BGreen}flawfinder static analysis passed."
    rm flawfinder_report.txt
else
    echo -e "${BIRed}Issues found by flawfinder."
    cat flawfinder_report.txt
fi

echo -e "${NC}Running lizard static analysis..."
lizard -V -o lizard_report.txt **/. # report should include "No thresholds exceeded" if passed
if grep -Fq "No thresholds exceeded" lizard_report.txt; then
    echo -e "${BGreen}lizard static analysis passed."
    rm lizard_report.txt
else
    echo -e "${BIRed}Issued found by lizard"
    cat lizard_report.txt
fi

echo -e "${NC}Running scan-build static analysis..."
scan-build  g++ -std=c++14 -g3 -Wall -Wextra unit_tests/*.cpp src/aircraft.cpp src/router1.cpp src/map.cpp -lgtest_main -lgtest -I../src > scan-build_report.txt # report should include "No bugs found" if passed
if grep -Fq "No bugs found" scan-build_report.txt; then
    echo -e "${BGreen}scan-build static analysis passed."
    rm scan-build_report.txt
    rm a.out 
else
    echo -e "${BIRed}Issues found by scan-build."
    cat scan-build_report.txt
fi

echo -e "${NC}Running clang-tidy static analysis..."
clang-tidy --quiet src/*.h src/aircraft.cpp src/router1.cpp src/map.cpp > clang-tidy_report.txt 2> /dev/null # report should not include "warning:" or "note:" if passed
if (grep -Fq "warning:" clang-tidy_report.txt || grep -Fq "note:" clang-tidy_report.txt); then
    echo -e "${BIRed}Issues found by clang-tidy."
    cat clang-tidy_report.txt
else
    echo -e "${BGreen}clang-tidy static analysis passed."
    rm clang-tidy_report.txt

fi

echo -e "${BIPurple}Done.\n"

echo -e "${BIPurple}Running unit tests..."
echo -e "${NC}"
cd unit_tests
g++ -std=c++14 *.cpp ../src/aircraft.cpp ../src/router1.cpp ../src/map.cpp -lgtest_main -lgtest -Isrc/.
./a.out 2> /dev/null
rm a.out

echo -e "${BIPurple}Done.\n"

echo -e "${BIPurple}Running dynamic analysis suite...\n"

echo -e "${NC}Running valgrind dynamic analysis..."
g++ -std=c++14 *.cpp ../src/aircraft.cpp ../src/router1.cpp ../src/map.cpp -lgtest_main -lgtest -Isrc/.
valgrind -s --log-file=valgrind_report.txt --trace-children=yes --leak-check=full --show-leak-kinds=all --show-reachable=yes --track-origins=yes ./a.out > /dev/null 2>&1
if grep -Fq "no leaks are possible" valgrind_report.txt  && grep -Fq "0 errors from 0 contexts" valgrind_report.txt; then
    echo -e "${BGreen}valgrind dynamic analysis passed."
    rm valgrind_report.txt
    rm a.out
else
    echo -e "${BIRed}Issues found by valgrind."
    cat valgrind_report.txt
    rm a.out
fi

echo -e "${NC}Running address and leak sanitizer dynamic analysis..."
g++ -std=c++14 -fsanitize=address -fsanitize=leak *.cpp ../src/aircraft.cpp ../src/router1.cpp ../src/map.cpp -lgtest_main -lgtest -Isrc/.
./a.out > sanitizer_report.txt 2>&1
if grep -Fq "ERROR" sanitizer_report.txt; then
    echo -e "${BIRed}Issues found by sanitizers."
    cat sanitizer_report.txt
    rm a.out
else
    echo -e "${BGreen}sanitizer dynamic analysis passed."
    rm sanitizer_report.txt
    rm a.out 
fi

cd ..

echo -e "${BIPurple}Done.\n"

echo -e "${BIPurple}Test suite finished"
