#!/bin/bash

# assuming build takes less than 24 hours
# the clock is a wall clock and will break if builds take longer
now () {
    # `-1`  is the current time
    return "$(TZ=UTC0 printf '%(%s)T\n' '-1')"
}

show_help() {
    echo ""
    echo " YoAudio Build Script"
    echo ""
    echo "     This script configures and builds the CMakeList.txt in the parent directory."
    echo "     It supports flexible selection of compilers and CMake generators."
    echo ""
    echo " Usage:"
    echo "     ./linux.sh [OPTIONS]"
    echo ""
    echo " Options:"
    echo "     -c, --compiler <name>      Specify the C compiler to use (e.g., gcc, clang)"
    echo "     -cpp, --cppcompiler <name> Specify the C++ compiler to use (e.g., g++, clang++)"
    echo "                                (defaults to C compiler with standard suffix)"
    echo "     -g, --generator <name>     Specify the CMake generator / build toolchain"
    echo "                                (\"Unix Makefiles\", \"Ninja\", \"Visual Studio 17 2022\")"
    echo "     -b, --build-type <type>    Specify the build type (e.g. Release, Debug, etc...)"
    echo "     --clean                    Clean previous build files before configuring"
    echo "     -i, --interactive          Select compiler and toolchain interactively"
    echo "     -h, --help                 Show this help message and exit"
    echo ""
    echo " Example:"
    echo "     ./linux.sh -a              # auto-selection"
    echo "     ./linux.sh -g make"
    echo "     ./linux.sh -c gcc -cpp g++ -g \"Unix Makefiles\" -b Debug --clean"
    echo "     ./linux.sh --compiler clang --generator \"Ninja\" --build-type Release"
    echo ""
}

# Show help if no arguments are provided
if [[ $# -eq 0 ]]; then
    AUTO_CHOOSE=1
fi

# Parse CLI arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -c|--compiler)
            C_COMPILER="$2"
            shift 2
            ;;
        -cpp|--cppcompiler)
            CXX_COMPILER="$2"
            shift 2
            ;;
        -g|--generator)
            TOOLCHAIN="$2"
            shift 2
            ;;
        -b|--build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        --clean)
            CLEAN_BUILD=1
            shift
            ;;
        -i|--interactive)
            INTERACTIVE=1
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# Interactive selection if requested
if [[ "$INTERACTIVE" == "1" ]]; then
    # C Compiler selection
    if [[ -z "$C_COMPILER" ]]; then
        echo "Select a C compiler:"
        options=()
        if command -v gcc &>/dev/null; then options+=("gcc"); fi
        if command -v clang &>/dev/null; then options+=("clang"); fi
        if command -v clang-20 &>/dev/null; then options+=("clang-20"); fi
        select opt in "${options[@]}"; do
            if [[ -n "$opt" ]]; then
                C_COMPILER="$opt"
                break
            fi
        done
    fi
    # CXX Compiler selection
    if [[ -z "$CXX_COMPILER" ]]; then
        echo "Select a C++ compiler:"
        options=()
        if command -v g++ &>/dev/null; then options+=("g++"); fi
        if command -v clang++ &>/dev/null; then options+=("clang++"); fi
        if command -v clang++-20 &>/dev/null; then options+=("clang++-20"); fi
        options+=("default")
        select opt in "${options[@]}"; do
            if [[ -n "$opt" ]]; then
                if [[ "$opt" == "default" ]]; then
                    CXX_COMPILER=""
                else
                    CXX_COMPILER="$opt"
                fi
                break
            fi
        done
    fi
    # CMake generator selection
    if [[ -z "$TOOLCHAIN" ]]; then
        echo "Select a CMake generator:"
        generators=()
        if command -v make &>/dev/null; then generators+=("Unix Makefiles"); fi
        if command -v ninja &>/dev/null; then generators+=("Ninja"); fi
        generators+=("list")
        generators+=("Other (manual entry)")
        select gen in "${generators[@]}"; do
            if [[ "$gen" == "list" ]]; then
                echo "cmake --help (Generators section):"
                echo "----------------------------------"
                cmake --help | awk '/available on this platform/{flag=1} flag' | awk 'NF'
                continue
            fi
            if [[ "$gen" == "Other (manual entry)" ]]; then
                read -p "Enter generator name: " TOOLCHAIN
                break
            elif [[ -n "$gen" ]]; then
                TOOLCHAIN="$gen"
                break
            fi
        done
    fi
    # Build type selection
    if [[ -z "$BUILD_TYPE" ]]; then
        echo "Select a build type:"
        build_types=("Release" "Debug" "RelWithDebInfo" "MinSizeRel")
        select bt in "${build_types[@]}"; do
            if [[ -n "$bt" ]]; then
                BUILD_TYPE="$bt"
                break
            fi
        done
    fi
fi

# timing the configuration step
now
SECONDS_START=$?
# ensure required variables are set

# Check for compiler if specified
if ! command -v "$C_COMPILER" &>/dev/null; then
    #echo -e "Error: C Compiler \"'$C_COMPILER'\" not found."
    C_COMPILER=""
fi
# If C compiler not set, use available ones
if [[ -z "$C_COMPILER" ]]; then
    if command -v gcc &>/dev/null; then
        C_COMPILER="gcc"
    elif command -v clang &>/dev/null; then
        C_COMPILER="clang"
	elif command -v clang-20 &>/dev/null; then
        C_COMPILER="clang-20"
    else
        echo "No supported C compiler found."
        exit 1
    fi
    if [[ $AUTO_CHOOSE == 0 ]]; then
        echo "No C compiler specified. Defaulting to '$C_COMPILER'."
    fi
fi
# Check for C++ compiler if specified
if ! command -v "$CXX_COMPILER" &>/dev/null; then
    #echo -e "Error: C++ Compiler \"'$CXX_COMPILER'\" not found."
    CXX_COMPILER=""
fi
# If C++ compiler not set, use available ones
if [[ -z "$CXX_COMPILER" ]]; then
    # Default to C compiler with standard suffix
    if [[ "$C_COMPILER" == *"gcc"* ]]; then
        CXX_COMPILER="${C_COMPILER/gcc/g++}"
    elif [[ "$C_COMPILER" == *"clang"* ]]; then
        CXX_COMPILER="${C_COMPILER/clang/clang++}"
    elif [[ "$C_COMPILER" == *"clang-20"* ]]; then
        CXX_COMPILER="${C_COMPILER/clang/clang++-20}"
    # Fallback to common C++ compilers
    elif command -v g++ &>/dev/null; then
        CXX_COMPILER="g++"
    elif command -v clang++ &>/dev/null; then
        CXX_COMPILER="clang++"
    elif command -v clang++-20 &>/dev/null; then
        CXX_COMPILER="clang++-20"
    else
        echo "No supported C++ compiler found."
        exit 1
    fi
    if [[ $AUTO_CHOOSE == 0 ]]; then
        echo "No C++ compiler specified. Defaulting to '$CXX_COMPILER'."
    fi
fi
# Check for toolchain if specified
if [[ -n "$TOOLCHAIN" ]]; then
    if [ "$TOOLCHAIN" == "make" ]; then
        TOOLCHAIN="Unix Makefiles"
    elif [ "$TOOLCHAIN" == "ninja" ]; then
        TOOLCHAIN="Ninja"
    fi
    case "$TOOLCHAIN" in
        "Unix Makefiles")
            if ! command -v make &>/dev/null; then
                echo "Error: make is not installed (required for Unix Makefiles)."
                exit 1
            fi
            ;;
        "Ninja")
            if ! command -v ninja &>/dev/null; then
                echo "Error: ninja is not installed (required for Ninja generator)."
                exit 1
            fi
            ;;
        *)
            echo "Warning: Unknown toolchain '$TOOLCHAIN'."
            echo "Falling back to auto-detection."
            TOOLCHAIN=""
            ;;
    esac
fi
# If toolchain not set, default to "Unix Makefiles"
if [[ -z "$TOOLCHAIN" ]]; then
    if command -v ninja &>/dev/null; then
        TOOLCHAIN="Ninja"
    elif command -v make &>/dev/null; then
        TOOLCHAIN="Unix Makefiles"
    else
        echo "No supported target toolchain found."
        exit 1
    fi
    if [[ $AUTO_CHOOSE == 0 ]]; then
        echo "No toolchain specified. Defaulting to '$TOOLCHAIN'."
    fi
fi
# If build type not set, use available ones (Release, Debug, RelWithDebInfo)
if [[ -z "$BUILD_TYPE" ]]; then
    if [[ $AUTO_CHOOSE == 0 ]]; then
        echo "No build type specified. Defaulting to Release."
    fi
    BUILD_TYPE="Release"
fi
# Check for cmake
if ! command -v cmake &>/dev/null; then
    echo "Error: cmake is not installed."
    exit 1
fi

echo ""
echo "Configuration summary:"
echo ""
echo -e "C Compiler:\t\t$C_COMPILER"
echo -e "C++ Compiler:\t\t$CXX_COMPILER"
echo -e "CMake generator:\t$TOOLCHAIN"
echo -e "Build type:\t\t$BUILD_TYPE"
echo ""


# Build the project (works for all generators)
echo "CMake building project..."

# Find the project root containing CMakeLists.txt, going up max 5 steps
SEARCH_DIR="$(pwd)"
MAX_STEPS=5
STEP=0
while [[ ! -f "$SEARCH_DIR/CMakeLists.txt" && "$SEARCH_DIR" != "/" && $STEP -lt $MAX_STEPS ]]; do
    SEARCH_DIR="$(dirname "$SEARCH_DIR")"
    ((STEP++))
done
if [[ ! -f "$SEARCH_DIR/CMakeLists.txt" ]]; then
    echo "CMakeLists.txt not found in any parent directory (max $MAX_STEPS steps)."
    exit 1
fi

# Create build directory
BUILD_DIR="build_${C_COMPILER}-${CXX_COMPILER}-${TOOLCHAIN}_${BUILD_TYPE}"
if [[ $CLEAN_BUILD -eq 1 ]]; then
    echo "Cleaning previous build files in $BUILD_DIR..."
    rm -rf "$BUILD_DIR"
fi
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
echo "writing build files to: $BUILD_DIR"
echo ""

# Run CMake to configure the project
echo "Configuring project with CMake..."
echo ""
cmake -DCPM_SOURCE_CACHE=../.cpm-cache/ -DCPM_USE_LOCAL_PACKAGES=TRUE -DCMAKE_C_COMPILER="$C_COMPILER" -DCMAKE_CXX_COMPILER="$CXX_COMPILER" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -G "$TOOLCHAIN" -S "$SEARCH_DIR" -B .
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi
echo ""
echo "CMake configuration successful."
echo ""
now
SECONDS_END=$?
SECONDS_ELAPSED=$(( SECONDS_END - SECONDS_START ))
echo "Project configuration completed in $((SECONDS_ELAPSED / 60)) minutes and $((SECONDS_ELAPSED % 60)) seconds."
echo ""
# Build the project
echo "Building binaries..."
echo ""
now
SECONDS_START=$?

if [[ "$TOOLCHAIN" == "Ninja" ]]; then
    ninja -d stats
else
    cmake --build .
fi
if [ $? -ne 0 ]; then
    echo ""
    echo "Build failed."
else
    echo ""
    echo "Build successful."
fi

now
SECONDS_END=$?
SECONDS_ELAPSED=$(( SECONDS_END - SECONDS_START ))
echo "Build completed in $((SECONDS_ELAPSED / 60)) minutes and $((SECONDS_ELAPSED % 60)) seconds."
