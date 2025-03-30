#pragma once

namespace YoaEngine
{
    #define UNUSED(x) (void)(x)

    #include <cassert>

    #define YOA_ASSERT(x, ...) assert(x);
    //#define YOA_ASSERT(x, ...) \
    //{ \
    //	YOA_CRITICAL("Assertion Failed! \n", __VA_ARGS__); \
    //	assert(x); \
    //}; \
    //#if YOA_PLATFORM == YOA_WINDOWS \
    //	/* MSVC specific debug break */ \
    //	__debugbreak(); \
    //#else \
    //	/* POSIX portable debug break */ \
    //	raise(SIGTRAP); \
    //#endif \
    //};

    #include <cmath>
    #include <type_traits>

    template<
        typename T, //real type
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    [[nodiscard]] inline constexpr bool isInteger(const T numer) noexcept
    {
        double intPart = 0;
        const double fractPart = std::modf(numer, &intPart);
        return (fractPart == 0.0);
    }

    template<
        typename T, //real type
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    [[nodiscard]] inline constexpr bool isPowerofTwo(const T n) noexcept
    {
        if (n <= 0)
        {
            return false;
        }
        else
        {
            // Calculate log base 2 of n
            const double logValue = std::log2(n);
            // check if log2 of n is iteger
            return isInteger(logValue);
            // naive implementation checking the log2 is integer
            //return (logValue == static_cast<long>(logValue));
            // naive implementation, but costlier
            // Check if log2(n) is an integer
            // and 2^(logn) = n
            //return pow(2, logValue) == n;
        }
    }
}  // namespace YoaEngine
