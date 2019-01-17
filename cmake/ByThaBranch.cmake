# ByThaBranch.cmake
##
# Isolates & retrieves the current git branch for conditional w/e & etc
#
# BTB_CURRENT_BRANCH == the current git branch
#
find_program(PYTHON3
		NAMES python3 python3.6
	)
if(NOT PYTHON3 OR DEFINED PYTHON3_NOTFOUND)
	message(FATAL_ERROR "python 3.X required for ByThaBranch")
endif()

execute_process(COMMAND bash -c "git checkout | awk '{print $8;}' |& python3 -c 'import sys; a = sys.stdin.read(); i=2; b = a[len(\"origin/\")+i:len(a)-3]; print(b);'"
		OUTPUT_VARIABLE BTB_CURRENT_BRANCH)

string(REPLACE "/"
	" " BTB_CURRENT_BRANCH
	"${BTB_CURRENT_BRANCH}")
string(STRIP "${BTB_CURRENT_BRANCH}" BTB_CURRENT_BRANCH)
mark_as_advanced(BTB_CURRENT_BRANCH)
message(STATUS "Current git branch: ${BTB_CURRENT_BRANCH}")

