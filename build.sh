#!/usr/bin/env sh
readonly SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
readonly BUILD_DIR="${SCRIPT_PATH}/builds/"
readonly TEST_DIR="$BUILD_DIR/unit-tests"
NPROC=$(nproc --ignore=1)

################ Start of globals ################
UPDATE_SUBMODULES=true
CONFIGURE_CMAKE=true
COMPILE=true
RUN_TESTS=true
################ End of globals ################

usage()
{
    cat <<EOF
$(basename "${0}"): Builds and runs unit tests of the project

Optional Args:
    -c:             If set, does NOT compile the code - Defaults to true
    -g:             If set, does NOT perform the configure cmake step - Defaults to true
    -h:             Shows this message
    -u              If set, does NOT update submodules - Defaults to true
    -r:             If set, does NOT run the unit tests - Defaults to true
EOF
}

# Return 0 on success, > 0 on failure
process_cli_args()
{
    OPTSTRING=":cghur"
    while getopts $OPTSTRING opt; do
        case ${opt} in
            c) COMPILE=false;;
            g) CONFIGURE_CMAKE=false;;
            h) usage; return 1 ;;
            u) UPDATE_SUBMODULES=false ;;
            r) RUN_TESTS=false ;;
            \? ) echo "Unknown option: -$OPTARG" >&2; return 1;;
            :  ) echo "Missing option argument for -$OPTARG" >&2; return 1;;
            *  ) echo "Unimplemented option: -$opt" >&2; return 1;;
        esac
    done

    return 0
}

# Return 0 on success, > 0 on failure
main()
{
    process_cli_args "$@"

    local update_submodules_res=0

    if $UPDATE_SUBMODULES; then
        git submodule update --init --recursive
        update_submodules_res=$?
    fi

    mkdir -p "$BUILD_DIR"

    local configure_res=0
    if [ $CONFIGURE_CMAKE ] && [ $update_submodules_res -eq 0 ]; then
        cmake -B "$BUILD_DIR"
        configure_res=$?
    fi

    local compile_res=0
    if [ $COMPILE ] && [ $configure_res -eq 0 ]; then
        cmake \
            --build "$BUILD_DIR" \
            --parallel "$NPROC" \
            --target all
        compile_res=$?
    fi

    local test_res=0
    if [ $RUN_TESTS ] && [ $compile_res -eq 0 ]; then
        ctest --test-dir "$TEST_DIR" --output-on-failure
        test_res=$?
    fi

    return $test_res
}

main "$@"
