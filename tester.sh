#!/bin/bash

################################################################################
#* === CONFIGURATION CONSTANTS ===
################################################################################

EXEC="./philo/philo"
BONUS_EXEC="./philo_bonus/philo_bonus"
LOG_DIR="logs"

VL_CHECK=false
ALLOWED_DELAY=10 #? In milliseconds

################################################################################
# === COLOR CONSTANTS ===
################################################################################

GRAY="\033[1;30m"
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
MAGENTA="\033[0;35m"
CYAN="\033[1;36m"
NC="\033[0m"
R_BD="\033[22m"

################################################################################
# === GLOBAL RUNTIME STATE ===
################################################################################

IS_BONUS=false
IGN_LAST_LN=false
set_default() {
	MAX_CHECK_TIME=10 #? In seconds

	I=0
	ARGS=""
	output=""

	#? Tests counters
	TOTAL=0
	PASSED=0
	FAILED=0
}
set_default

################################################################################
# === LOGGING HELPERS ===
################################################################################

log_run() {
	local expected="$1"

	printf "${CYAN}[TEST %02d]$R_BD $expected\n" "$I"
	printf "${GRAY}Command:$R_BD %s ${NC}\n" "$EXEC $ARGS"
	((TOTAL++))
}

log_ok() {
	printf "${GREEN}[OK]${NC}\n\n"
	((PASSED++))
}

herror() {
	local explain="$1"
	local content
	local log_file

	if [[ -n "$2" ]]; then
		content="$2"
	else
		content="$output"
	fi
	log_file="$LOG_DIR/test_$I.log"
	if "$IS_BONUS"; then
		mkdir -p "$LOG_DIR/bonus"
		log_file="$LOG_DIR/bonus/test_$I.log"
	fi
	printf "%s\n" "$content" >"$log_file"

	printf "${RED}[KO]$R_BD %s${NC}\n" "$explain"
	printf "$YELLOW${R_BD}See output in: '%s'$NC\n\n" "$log_file"
	((FAILED++))
}

################################################################################
# === OUTPUT & VALIDATION HELPERS ===
################################################################################

_check_log_format() {
	local regex="^[0-9]+ [0-9]+ (has taken a fork|is eating|is sleeping|is thinking|died)$"

	if [[ -z "$output" ]]; then
		herror "Output log is empty"
		return 0
	fi
	if echo "$output" | grep -q '^[[:space:]]*$'; then
		herror "Output log contains empty lines"
		return 0
	fi

	local to_check="$output"
	if "$IGN_LAST_LN"; then
		to_check=$(echo "$output" | sed '$d')
	fi
	if ! bad_lines=$(echo "$to_check" | grep -vE "$regex"); then
		return 1
	fi

	if [[ -n "$bad_lines" ]]; then
		herror "Log format is incorrect on one or more lines"
		return 0
	fi
	return 1
}

_run_with_helgrind() {
	local hel_file
	hel_file="$LOG_DIR/helgrind_$I.log"

	output=$(valgrind --tool=helgrind --fair-sched=yes --history-level=approx \
		--log-file="$hel_file" $EXEC $ARGS)

	if grep -q "Possible data race" "$hel_file"; then
		herror "Helgrind detected a possible data race" "$(cat $hel_file)"
		return 0
	fi
	return 1
}

#? fill output with philo stdout
set_output() {
	local mode="$1"

	if "$VL_CHECK"; then
		if _run_with_helgrind; then
			return 0
		fi
	else
		local exit_code
		output=$(stdbuf -o0 timeout "${MAX_CHECK_TIME}s" $EXEC $ARGS 2>&1)
		exit_code=$?

		if [[ "$mode" == "infinite" && "$exit_code" -ne 124 ]]; then
			herror "Unexpected termination before timeout"
			return 0
		elif [[ "$mode" != "infinite" && "$exit_code" -eq 124 ]]; then
			herror "Test exceeded maximum time limit (${MAX_CHECK_TIME}s)"
			return 0
		fi
	fi

	if _check_log_format; then
		return 0
	fi
	return 1
}

################################################################################
# === INTERNAL LOGIC HELPERS ===
################################################################################

#`run_test_alive_sated`
_check_interval_between_actions() {
	local id="$1"
	local expected="$2"
	local from="$3"
	local to="$4"

	local times
	times=($(echo "$output" | grep " $id $from" | awk '{print $1}'))
	for t in "${times[@]}"; do
		local end
		end=$(echo "$output" | grep -A1 "$t $id $from" | tail -n1 | grep "$id $to" | awk '{print $1}')
		if [[ -n "$end" ]]; then
			local interval=$((end - t))
			local diff=$((interval - expected))
			if [[ "${diff#-}" -gt "$ALLOWED_DELAY" ]]; then
				herror "Philosopher $id spent $interval ms on $from → $to (expected $expected ms)" \
					"$(printf "Test on Philo %s\n%s" "$id" "$output")"
				return 0
			fi
		fi
	done
	return 1
}

#`run_test_alive_sated`
check_interval() {
	local id="$1"

	local to_eat
	to_eat=$(echo "$ARGS" | awk '{print $3}')
	local to_sleep
	to_sleep=$(echo "$ARGS" | awk '{print $4}')
	if _check_interval_between_actions "$id" "$to_eat" "is eating" "is sleeping"; then
		return 0
	fi
	if _check_interval_between_actions "$id" "$to_sleep" "is sleeping" "is thinking"; then
		return 0
	fi
	return 1
}

#`run_test_die`
_check_death_timing() {
	local to_die="$1"

	local death_ln
	death_ln=$(echo "$output" | grep "died" | tail -n1)
	local death
	death=$(awk '{print $1}' <<<"$death_ln")
	local last_meal
	last_meal=$(echo "$output" | grep "$(awk '{print $2}' <<<"$death_ln") is eating" | tail -n1 | awk '{print $1}')

	if [[ -n "$last_meal" && -n "$death" ]]; then
		local delay=$((death - last_meal))
		local diff=$((delay - to_die))
		if [[ "${diff#-}" -gt 10 ]]; then
			herror "Philosopher do not died in good time after last meal (${delay}ms instead of ${to_die}ms)"
			return 0
		fi
	fi
	return 1
}

#`run_test_alive_infinite` & `run_test_alive_sated`
_check_if_any_die() {
	if echo "$output" | grep -q "died"; then
		herror "Philosopher died unexpectedly"
		return 0
	fi
	return 1
}

################################################################################
#* === TEST EXECUTION ===
################################################################################

run_test_alive_infinite() {
	log_run "no philosopher should die: wait ${MAX_CHECK_TIME}s"

	if set_output "infinite"; then return; fi
	if _check_if_any_die; then return; fi

	log_ok
}

run_test_alive_sated() {
	local count
	count=$(echo "$ARGS" | awk '{print $1}')
	local sated
	sated=$(echo "$ARGS" | awk '{print $NF}')

	log_run "each philosopher must eat at least ${sated} times"

	if set_output; then return; fi
	if _check_if_any_die; then return; fi

	for ((id = 1; id <= count; id++)); do
		local eat_count
		eat_count=$(echo "$output" | grep -c " $id is eating")
		if [[ "$eat_count" -lt "$sated" ]]; then
			herror "Philosopher $id ate only $eat_count times (expected at least $sated)"
			return
		fi
		if check_interval "$id"; then return; fi
	done

	log_ok
}

run_test_die() {
	log_run "at least 1 philosopher should die after starvation timeout"

	if set_output; then return; fi

	local died_count
	died_count=$(echo "$output" | grep -c "died")
	if [[ "$died_count" -lt 1 ]]; then
		herror "Expected at least one philosopher to die, but none did"
		return
	fi

	if _check_death_timing "$(echo "$ARGS" | awk '{print $2}')"; then return; fi

	log_ok
}

run_test_one_die() {
	log_run "1 philosopher: should take 1 fork, then die after (≤ ${ALLOWED_DELAY}ms)"

	if set_output; then return; fi

	local take_fork_time
	take_fork_time=$(echo "$output" | grep "has taken a fork" | head -n1 | awk '{print $1}')
	local die_time
	die_time=$(echo "$output" | grep "died" | head -n1 | awk '{print $1}')

	if [[ -z "$take_fork_time" ]]; then
		herror "Philosopher did not take a fork (but should take, before to die)"
		return
	fi
	if [[ -z "$die_time" ]]; then
		herror "Philosopher did not die (but should have with one fork)"
		return
	fi

	local to_die
	to_die=$(awk '{print $2}' <<<"$ARGS")
	local diff=$((die_time - take_fork_time - to_die))
	if [[ "${diff#-}" -gt "$ALLOWED_DELAY" ]]; then
		herror "Philosopher died after ${diff}ms (expected <= 10ms)"
		return
	fi

	log_ok
}

################################################################################
# === EXECUTABLE CHECKER ===
################################################################################

check_exec() {
	local exec_dir
	exec_dir=$(dirname "$EXEC")

	if [[ ! -d "$LOG_DIR" ]]; then mkdir "$LOG_DIR"; fi
	if [[ ! -d "$exec_dir" ]]; then
		printf "${RED}[Error] The 'philo' directory does not exist.${NC}\n" >&2
		exit 1
	fi

	if [[ ! -x "$EXEC" ]]; then
		printf "${MAGENTA}[Info] compiling '%s'...$NC\n" "$EXEC"
		make -C "$exec_dir" >/dev/null
		if [[ ! -x "$EXEC" ]]; then
			printf "${RED}[Error] Failed to compile '%s'.${NC}\n" "$EXEC" >&2
			exit 1
		fi
	fi
	if "$IS_BONUS"; then
		return
	fi
	printf "${MAGENTA}Would you want clean log directory ($LOG_DIR)? [Y/n]$NC"
	read -r input
	input=${input:-y}
	if [[ "$input" =~ ^[yY]$ ]]; then
		printf "$RED${R_BD}Deleted files:\n$NC"
		mapfile -t deleted_files < <(find "./$LOG_DIR"/* -delete -print)
		if [[ "${#deleted_files[@]}" -gt 1 ]]; then
			for ((i = 0; i < ${#deleted_files[@]}; i++)); do
				if ((i == 0)); then
					printf "[%s, " "${deleted_files[i]}"
				elif ((i == ${#deleted_files[@]} - 1)); then
					printf "%s]\n" "${deleted_files[i]}"
				else
					printf "%s, " "${deleted_files[i]}"
				fi
			done
		elif [[ "${#deleted_files[@]}" -gt 0 ]]; then
			printf "[%s]\n" "${deleted_files[i]}"
		else
			printf "[none]\n"
		fi
	fi
}

################################################################################
# === TEST DATA ===
################################################################################

DIED_TEST=("1 1000 200 200" "4 600 300 300" "5 410 200 200")
ALIVE_INF_TEST=("4 610 300 300" "8 810 400 400" "200 410 200 200" "9 600 150 150" "5 800 200 200" "199 600 150 150")
ALIVE_TEST=("12 410 200 200 20" "6 800 200 200 6" "5 600 100 100 1" "10 500 50 50 10")

################################################################################
#* === MAIN SCRIPT LOGIC ===
################################################################################

run_all_die_tests() {
	printf "\n${BLUE}=== DIED TESTS ===${NC}\n"
	for i in "${!DIED_TEST[@]}"; do
		((I++))
		ARGS="${DIED_TEST[$i]}"
		if [[ "$ARGS" =~ ^1 ]]; then
			run_test_one_die
		else
			run_test_die
		fi
	done
}

run_all_alive_infinite_tests() {
	printf "\n${BLUE}=== ALIVE INFINITE TESTS ===${NC}\n"
	read -rp "Enter a maximum test execution time (press enter for 10 sec)?: " input
	if [[ "$input" =~ ^[1-9][0-9]*$ ]]; then
		MAX_CHECK_TIME="$input"
	elif [[ "$input" != "" ]]; then
		printf "${YELLOW}[Warning] '%s' is invalid time value. Use default value instead.$NC\n" "$input"
	fi
	IGN_LAST_LN=true
	for j in "${!ALIVE_INF_TEST[@]}"; do
		((I++))
		ARGS="${ALIVE_INF_TEST[$j]}"
		run_test_alive_infinite
	done
	IGN_LAST_LN=false
}

run_all_alive_sated_tests() {
	printf "\n${BLUE}=== SATED TESTS ===${NC}\n"
	IGN_LAST_LN=true
	for k in "${!ALIVE_TEST[@]}"; do
		((I++))
		ARGS="${ALIVE_TEST[$k]}"
		run_test_alive_sated
	done
	IGN_LAST_LN=false
}

main() {
	check_exec
	if "$IS_BONUS"; then
		printf "${BLUE}=== Running philosophers bonus test suite ===${NC}\n"
	else
		printf "${BLUE}=== Running philosophers test suite ===${NC}\n"
	fi

	local choice
	printf "\n${MAGENTA}Select test suite to run:$NC\n"
	printf "  0) All tests\n"
	printf "  1) Died tests\n"
	printf "  2) Alive infinite tests\n"
	printf "  3) Alive sated tests\n"
	printf "Enter choice [0-3]: "
	read -r choice

	case "$choice" in
	0)
		run_all_die_tests
		run_all_alive_infinite_tests
		run_all_alive_sated_tests
		;;
	1)
		run_all_die_tests
		;;
	2)
		run_all_alive_infinite_tests
		;;
	3)
		run_all_alive_sated_tests
		;;
	*)
		printf "${RED}Invalid selection. Please enter a number from 0 to 3.${NC}\n" >&2
		return 1
		;;
	esac

	printf "${BLUE}=== Test Summary ===${NC}\n"
	if [[ "$FAILED" -eq 0 ]]; then
		printf "$GREEN Congratulations ! All tests succeed $NC\n"
	else
		printf "  Total tests  : %s\n" "$TOTAL"
		printf "  Passed tests : ${GREEN}%s${NC}\n" "$PASSED"
		printf "  Failed tests : ${RED}%s${NC}\n" "$FAILED"
	fi
	printf "${BLUE}=========================${NC}\n"
}

main "$@"

if [[ -d "$(dirname "$BONUS_EXEC")" ]]; then
	printf "${MAGENTA}Would you want to run bonus test suite ? [Y/n]$NC"
	read -r input
	input=${input:-y}
	if [[ "$input" =~ ^[yY]$ ]]; then
		set_default
		IS_BONUS=true
		EXEC="$BONUS_EXEC"
		main "$@"
	fi
fi
