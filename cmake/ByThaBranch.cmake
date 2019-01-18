# ByThaBranch.cmake
##
# Isolates & retrieves the current git branch for conditional w/e & etc
#
# BTB_CURRENT_BRANCH == the current git branch
# 
# bybranch_build(ARG1)
# # ARG1 := name of branch to limit build to

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

function(bybranch_build BRANCH_BY)
	set(bb_i 0)
	set(bb_j FALSE)
	if(NOT ${BRANCH_BY} MATCHES "all")
		string(COMPARE EQUAL "${BRANCH_BY}" "${BTB_CURRENT_BRANCH}" $bb_j)
		if(bb_i EQUAL 0 AND bb_j EQUAL FALSE)
			message(FATAL_ERROR "${BRANCH_BY} marked as non-build!\nReconfigure with -DFORCEBUILD=1")
		endif()
	message(WARNING "Building limited to ${BRANCH_BY}.")
	message(STATUS  "${BRANCH_BY} matches ${BTB_CURRENT_BRANCH}.")
	else()
		message(STATUS "No branch build limitations")
	endif()
	unset(bb_i)
	unset(bb_j)
endfunction(bybranch_build)

