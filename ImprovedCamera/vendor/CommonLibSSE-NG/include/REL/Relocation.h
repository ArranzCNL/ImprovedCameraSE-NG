#pragma once

#include "rapidcsv.h"

#define REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL(a_nopropQual, a_propQual, ...)              \
    template <                                                                                    \
        class R,                                                                                  \
        class Cls,                                                                                \
        class... Args>                                                                            \
    struct member_function_pod_type<R (Cls::*)(Args...) __VA_ARGS__ a_nopropQual a_propQual>      \
    {                                                                                             \
        using type = R(__VA_ARGS__ Cls*, Args...) a_propQual;                                     \
    };                                                                                            \
                                                                                                  \
    template <                                                                                    \
        class R,                                                                                  \
        class Cls,                                                                                \
        class... Args>                                                                            \
    struct member_function_pod_type<R (Cls::*)(Args..., ...) __VA_ARGS__ a_nopropQual a_propQual> \
    {                                                                                             \
        using type = R(__VA_ARGS__ Cls*, Args..., ...) a_propQual;                                \
    };

#define REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(a_qualifer, ...)              \
    REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL(a_qualifer, , ##__VA_ARGS__) \
    REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL(a_qualifer, noexcept, ##__VA_ARGS__)

#define REL_MAKE_MEMBER_FUNCTION_POD_TYPE(...)                 \
    REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(, __VA_ARGS__)    \
    REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(&, ##__VA_ARGS__) \
    REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(&&, ##__VA_ARGS__)

#define REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL(a_nopropQual, a_propQual, ...)              \
    template <                                                                                        \
        class R,                                                                                      \
        class Cls,                                                                                    \
        class... Args>                                                                                \
    struct member_function_non_pod_type<R (Cls::*)(Args...) __VA_ARGS__ a_nopropQual a_propQual>      \
    {                                                                                                 \
        using type = R&(__VA_ARGS__ Cls*, void*, Args...)a_propQual;                                  \
    };                                                                                                \
                                                                                                      \
    template <                                                                                        \
        class R,                                                                                      \
        class Cls,                                                                                    \
        class... Args>                                                                                \
    struct member_function_non_pod_type<R (Cls::*)(Args..., ...) __VA_ARGS__ a_nopropQual a_propQual> \
    {                                                                                                 \
        using type = R&(__VA_ARGS__ Cls*, void*, Args..., ...)a_propQual;                             \
    };

#define REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(a_qualifer, ...)              \
    REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL(a_qualifer, , ##__VA_ARGS__) \
    REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL(a_qualifer, noexcept, ##__VA_ARGS__)

#define REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(...)                 \
    REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(, __VA_ARGS__)    \
    REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(&, ##__VA_ARGS__) \
    REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(&&, ##__VA_ARGS__)

#if defined(ENABLE_SKYRIM_VR) && (defined(ENABLE_SKYRIM_AE) || defined(ENABLE_SKYRIM_SE))
/**
 * Defined to indicate that this build supports both VR and non-VR runtimes.
 */
#	define SKYRIM_CROSS_VR
#endif

#if !defined(ENABLE_SKYRIM_AE) || (!defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_VR))
/**
 * A macro which defines a modifier for expressions that vary by Skyrim Address Library IDs.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build only targets one family of Address Library.
 * </p>
 */
#	define SKYRIM_ADDR constexpr
#else
/**
 * A macro which defines a modifier for expressions that vary by Skyrim address library IDs.
 *
 * <p>
 * Currently defined as <code>inline</code> to support multiple Address Library ID families dynamically.
 * </p>
 */
#	define SKYRIM_ADDR inline
#endif

#if (!defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)) || (!defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_VR)) || (!defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE))
/**
 * A macro which defines a modifier for expressions that vary by the specific Skyrim runtime.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is for only a single runtime.
 * </p>
 */
#	define SKYRIM_REL constexpr

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selective targeting.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is for only a single runtime.
 * </p>
 */
#	define SKYRIM_REL_CONSTEXPR constexpr
#else
/**
 * A macro which defines a modifier for expressions that vary by the specific Skyrim runtime.
 *
 * <p>
 * Currently defined as <code>inline</code> to support multiple runtimes dynamically.
 * </p>
 */
#	define SKYRIM_REL inline

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selective targeting.
 *
 * <p>
 * Currently defined as empty to support multiple runtimes.
 * </p>
 */
#	define SKYRIM_REL_CONSTEXPR
#endif

#ifndef SKYRIM_CROSS_VR
/**
 * A macro which defines a modifier for expressions that vary between Skyrim SE/AE and Skyrim VR.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is only for VR or non-VR.
 * </p>
 */
#	define SKYRIM_REL_VR constexpr

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is only for VR or non-VR.
 * </p>
 */
#	define SKYRIM_REL_VR_CONSTEXPR constexpr

/**
 * A macro which defines a modifier for functions which may be <code>virtual</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as <code>virtual</code> since this build is only for VR or non-VR.
 * </p>
 */
#	define SKYRIM_REL_VR_VIRTUAL virtual
#else
/**
 * A macro which defines a modifier for expressions that vary between Skyrim SE/AE and Skyrim VR.
 *
 * <p>
 * Currently defined as <code>inline</code> since this build is for both VR and non-VR.
 * </p>
 */
#	define SKYRIM_REL_VR inline

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as empty since this build is for both VR and non-VR.
 * </p>
 */
#	define SKYRIM_REL_VR_CONSTEXPR

/**
 * A macro which defines a modifier for functions which may be <code>virtual</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as empty since this build is for both VR and non-VR.
 * </p>
 */
#	define SKYRIM_REL_VR_VIRTUAL
#endif

namespace REL {
    namespace detail {
        class memory_map {
        public:
            memory_map() noexcept = default;

            memory_map(const memory_map &) = delete;

            memory_map(memory_map &&a_rhs) noexcept:
                    _mapping(a_rhs._mapping),
                    _view(a_rhs._view) {
                a_rhs._mapping = nullptr;
                a_rhs._view = nullptr;
            }

            ~memory_map() { close(); }

            memory_map &operator=(const memory_map &) = delete;

            memory_map &operator=(memory_map &&a_rhs) noexcept {
                if (this != std::addressof(a_rhs)) {
                    _mapping = a_rhs._mapping;
                    a_rhs._mapping = nullptr;

                    _view = a_rhs._view;
                    a_rhs._view = nullptr;
                }
                return *this;
            }

            [[nodiscard]] void *data() noexcept { return _view; }

            bool open(stl::zwstring a_name, std::size_t a_size);

            bool create(stl::zwstring a_name, std::size_t a_size);

            void close();

        private:
            void *_mapping{nullptr};
            void *_view{nullptr};
        };

        template<class>
        struct member_function_pod_type;

        REL_MAKE_MEMBER_FUNCTION_POD_TYPE();
        REL_MAKE_MEMBER_FUNCTION_POD_TYPE(const);
        REL_MAKE_MEMBER_FUNCTION_POD_TYPE(volatile);
        REL_MAKE_MEMBER_FUNCTION_POD_TYPE(const volatile);

        template<class F>
        using member_function_pod_type_t = typename member_function_pod_type<F>::type;

        template<class>
        struct member_function_non_pod_type;

        REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE();
        REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(const);
        REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(volatile);
        REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(const volatile);

        template<class F>
        using member_function_non_pod_type_t = typename member_function_non_pod_type<F>::type;

        // https://docs.microsoft.com/en-us/cpp/build/x64-calling-convention

        template<class T>
        struct meets_length_req :
                std::disjunction<
                        std::bool_constant<sizeof(T) == 1>,
                        std::bool_constant<sizeof(T) == 2>,
                        std::bool_constant<sizeof(T) == 4>,
                        std::bool_constant<sizeof(T) == 8>> {
        };

        template<class T>
        struct meets_function_req :
                std::conjunction<
                        std::is_trivially_constructible<T>,
                        std::is_trivially_destructible<T>,
                        std::is_trivially_copy_assignable<T>,
                        std::negation<
                                std::is_polymorphic<T>>> {
        };

        template<class T>
        struct meets_member_req :
                std::is_standard_layout<T> {
        };

        template<class T, class = void>
        struct is_x64_pod :
                std::true_type {
        };

        template<class T>
        struct is_x64_pod<
                T,
                std::enable_if_t<
                        std::is_union_v<T>>> :
                std::false_type {
        };

        template<class T>
        struct is_x64_pod<
                T,
                std::enable_if_t<
                        std::is_class_v<T>>> :
                std::conjunction<
                        meets_length_req<T>,
                        meets_function_req<T>,
                        meets_member_req<T>> {
        };

        template<class T>
        inline constexpr bool is_x64_pod_v = is_x64_pod<T>::value;

        template<
                class F,
                class First,
                class... Rest>
        decltype(auto) invoke_member_function_non_pod(F &&a_func, First &&a_first, Rest &&... a_rest)  //
        noexcept(std::is_nothrow_invocable_v<F, First, Rest...>) {
            using result_t = std::invoke_result_t<F, First, Rest...>;
            std::aligned_storage_t<sizeof(result_t), alignof(result_t)> result;

            using func_t = member_function_non_pod_type_t<F>;
            auto func = stl::unrestricted_cast<func_t *>(std::forward<F>(a_func));

            return func(std::forward<First>(a_first), std::addressof(result), std::forward<Rest>(a_rest)...);
        }
    }

    inline constexpr std::uint8_t NOP = 0x90;
    inline constexpr std::uint8_t RET = 0xC3;
    inline constexpr std::uint8_t INT3 = 0xCC;

    template<class F, class... Args>
    std::invoke_result_t<F, Args...> invoke(F &&a_func, Args &&... a_args)  //
    noexcept(std::is_nothrow_invocable_v<F, Args...>)                  //
    requires(std::invocable<F, Args...>)
    {
        if constexpr (std::is_member_function_pointer_v<std::decay_t<F>>) {
            if constexpr (detail::is_x64_pod_v<std::invoke_result_t<F, Args...>>) {  // member functions == free functions in x64
                using func_t = detail::member_function_pod_type_t<std::decay_t<F>>;
                auto func = stl::unrestricted_cast<func_t *>(std::forward<F>(a_func));
                return func(std::forward<Args>(a_args)...);
            } else {  // shift args to insert result
                return detail::invoke_member_function_non_pod(std::forward<F>(a_func), std::forward<Args>(a_args)...);
            }
        } else {
            return std::forward<F>(a_func)(std::forward<Args>(a_args)...);
        }
    }

    inline void safe_write(std::uintptr_t a_dst, const void *a_src, std::size_t a_count) {
        std::uint32_t old{0};
        auto success =
                WinAPI::VirtualProtect(
                        reinterpret_cast<void *>(a_dst),
                        a_count,
                        (PAGE_EXECUTE_READWRITE),
                        std::addressof(old));
        if (success != 0) {
            std::memcpy(reinterpret_cast<void *>(a_dst), a_src, a_count);
            success =
                    WinAPI::VirtualProtect(
                            reinterpret_cast<void *>(a_dst),
                            a_count,
                            old,
                            std::addressof(old));
        }

        assert(success != 0);
    }

    template<std::integral T>
    void safe_write(std::uintptr_t a_dst, const T &a_data) {
        safe_write(a_dst, std::addressof(a_data), sizeof(T));
    }

    template<class T>
    void safe_write(std::uintptr_t a_dst, std::span<T> a_data) {
        safe_write(a_dst, a_data.data(), a_data.size_bytes());
    }

    inline void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count) {
        std::uint32_t old{0};
        auto success =
                WinAPI::VirtualProtect(
                        reinterpret_cast<void *>(a_dst),
                        a_count,
                        (PAGE_EXECUTE_READWRITE),
                        std::addressof(old));
        if (success != 0) {
            std::fill_n(reinterpret_cast<std::uint8_t *>(a_dst), a_count, a_value);
            success =
                    WinAPI::VirtualProtect(
                            reinterpret_cast<void *>(a_dst),
                            a_count,
                            old,
                            std::addressof(old));
        }

        assert(success != 0);
    }

    class Version {
    public:
        using value_type = std::uint16_t;
        using reference = value_type &;
        using const_reference = const value_type &;

        constexpr Version() noexcept = default;

        explicit constexpr Version(std::array<value_type, 4> a_version) noexcept:
                _impl(a_version) {}

        constexpr Version(value_type a_v1, value_type a_v2 = 0, value_type a_v3 = 0, value_type a_v4 = 0) noexcept:
                _impl{a_v1, a_v2, a_v3, a_v4} {}

        explicit constexpr Version(std::string_view a_version) {
            std::array<value_type, 4> powers{1, 1, 1, 1};
            std::size_t position = 0;
            for (std::size_t i = 0; i < a_version.size(); ++i) {
                if (a_version[i] == '.') {
                    if (++position == powers.size()) {
                        throw std::invalid_argument("Too many parts in version number.");
                    }
                } else {
                    powers[position] *= 10;
                }
            }
            position = 0;
            for (std::size_t i = 0; i < a_version.size(); ++i) {
                if (a_version[i] == '.') {
                    ++position;
                } else if (a_version[i] < '0' || a_version[i] > '9') {
                    throw std::invalid_argument("Invalid character in version number.");
                } else {
                    powers[position] /= 10;
                    _impl[position] += static_cast<value_type>((a_version[i] - '0') * powers[position]);
                }
            }
        }

        [[nodiscard]] constexpr reference operator[](std::size_t a_idx) noexcept { return _impl[a_idx]; }

        [[nodiscard]] constexpr const_reference operator[](std::size_t a_idx) const noexcept { return _impl[a_idx]; }

        [[nodiscard]] constexpr decltype(auto) begin() const noexcept { return _impl.begin(); }

        [[nodiscard]] constexpr decltype(auto) cbegin() const noexcept { return _impl.cbegin(); }

        [[nodiscard]] constexpr decltype(auto) end() const noexcept { return _impl.end(); }

        [[nodiscard]] constexpr decltype(auto) cend() const noexcept { return _impl.cend(); }

        [[nodiscard]] std::strong_ordering constexpr compare(const Version &a_rhs) const noexcept {
            for (std::size_t i = 0; i < _impl.size(); ++i) {
                if ((*this)[i] != a_rhs[i]) {
                    return (*this)[i] < a_rhs[i] ? std::strong_ordering::less : std::strong_ordering::greater;
                }
            }
            return std::strong_ordering::equal;
        }

        [[nodiscard]] constexpr std::uint32_t pack() const noexcept {
            return static_cast<std::uint32_t>(
                    (_impl[0] & 0x0FF) << 24u |
                    (_impl[1] & 0x0FF) << 16u |
                    (_impl[2] & 0xFFF) << 4u |
                    (_impl[3] & 0x00F) << 0u);
        }

        [[nodiscard]] constexpr value_type major() const noexcept {
            return _impl[0];
        }

        [[nodiscard]] constexpr value_type minor() const noexcept {
            return _impl[1];
        }

        [[nodiscard]] constexpr value_type patch() const noexcept {
            return _impl[2];
        }

        [[nodiscard]] constexpr value_type build() const noexcept {
            return _impl[3];
        }

        [[nodiscard]] std::string string(std::string_view a_separator = "-"sv) const {
            std::string result;
            for (auto &&ver: _impl) {
                result += std::to_string(ver);
                result.append(a_separator.data(), a_separator.size());
            }
            result.erase(result.size() - a_separator.size(), a_separator.size());
            return result;
        }

        [[nodiscard]] std::wstring wstring(std::wstring_view a_separator = L"-"sv) const {
            std::wstring result;
            for (auto &&ver: _impl) {
                result += std::to_wstring(ver);
                result.append(a_separator.data(), a_separator.size());
            }
            result.erase(result.size() - a_separator.size(), a_separator.size());
            return result;
        }

        [[nodiscard]] static constexpr Version unpack(std::uint32_t a_packedVersion) noexcept {
            return REL::Version{
                    static_cast<value_type>((a_packedVersion >> 24) & 0x0FF),
                    static_cast<value_type>((a_packedVersion >> 16) & 0x0FF),
                    static_cast<value_type>((a_packedVersion >> 4) & 0xFFF),
                    static_cast<value_type>(a_packedVersion & 0x0F)
            };
        }

    private:
        std::array<value_type, 4> _impl{0, 0, 0, 0};
    };

    [[nodiscard]] constexpr bool operator==(const Version &a_lhs, const Version &a_rhs) noexcept {
        return a_lhs.compare(a_rhs) == std::strong_ordering::equal;
    }

    [[nodiscard]] constexpr std::strong_ordering
    operator<=>(const Version &a_lhs, const Version &a_rhs) noexcept { return a_lhs.compare(a_rhs); }

    namespace literals {
        namespace detail {
            template<std::size_t Index, char C>
            constexpr uint8_t read_version(std::array<typename REL::Version::value_type, 4> &result) {
                static_assert(C >= '0' && C <= '9', "Invalid character in semantic version literal.");
                static_assert(Index < 4, "Too many components in semantic version literal.");
                result[Index] += (C - '0');
                return 10;
            }

            template<std::size_t Index, char C, char... Rest>
            requires (sizeof...(Rest) > 0)
            constexpr uint8_t read_version(std::array<typename REL::Version::value_type, 4> &result) {
                static_assert(C == '.' || (C >= '0' && C <= '9'), "Invalid character in semantic version literal.");
                static_assert(Index < 4, "Too many components in semantic version literal.");
                if constexpr (C == '.') {
                    read_version<Index + 1, Rest...>(result);
                    return 1;
                } else {
                    auto position = read_version<Index, Rest...>(result);
                    result[Index] += (C - '0') * position;
                    return position * 10;
                }
            }
        }

        template<char... C>
        [[nodiscard]] constexpr REL::Version operator ""_v() noexcept {
            std::array<typename REL::Version::value_type, 4> result{0, 0, 0, 0};
            detail::read_version<0, C...>(result);
            return REL::Version(result);
        }

        [[nodiscard]] constexpr REL::Version operator ""_v(const char *str, std::size_t len) {
            return Version(std::string_view(str, len));
        }
    }

    [[nodiscard]] inline std::optional<Version> get_file_version(stl::zwstring a_filename) {
        std::uint32_t dummy;
        std::vector<char> buf(GetFileVersionInfoSize(a_filename.data(), std::addressof(dummy)));
        if (buf.empty()) {
            return std::nullopt;
        }

        if (!GetFileVersionInfo(a_filename.data(), 0, static_cast<std::uint32_t>(buf.size()), buf.data())) {
            return std::nullopt;
        }

        void *verBuf{nullptr};
        std::uint32_t verLen{0};
        if (!VerQueryValue(buf.data(), L"\\StringFileInfo\\040904B0\\ProductVersion", std::addressof(verBuf),
                           std::addressof(verLen))) {
            return std::nullopt;
        }

        Version version;
        std::wistringstream ss(
                std::wstring(static_cast<const wchar_t *>(verBuf), verLen));
        std::wstring token;
        for (std::size_t i = 0; i < 4 && std::getline(ss, token, L'.'); ++i) {
            version[i] = static_cast<std::uint16_t>(std::stoi(token));
        }

        return version;
    }

    class Segment {
    public:
        enum Name : std::size_t {
            textx,
            idata,
            rdata,
            data,
            pdata,
            tls,
            textw,
            gfids,
            total
        };

        Segment() noexcept = default;

        Segment(std::uintptr_t a_proxyBase, std::uintptr_t a_address, std::uintptr_t a_size) noexcept:
                _proxyBase(a_proxyBase),
                _address(a_address),
                _size(a_size) {}

        [[nodiscard]] std::uintptr_t address() const noexcept { return _address; }

        [[nodiscard]] std::size_t offset() const noexcept { return address() - _proxyBase; }

        [[nodiscard]] std::size_t size() const noexcept { return _size; }

        [[nodiscard]] void *pointer() const noexcept { return reinterpret_cast<void *>(address()); }

        template<class T>
        [[nodiscard]] T *pointer() const noexcept {
            return static_cast<T *>(pointer());
        }

    private:
        friend class Module;

        std::uintptr_t _proxyBase{0};
        std::uintptr_t _address{0};
        std::size_t _size{0};
    };

    class Module {
    public:
        /**
         * Identifies a Skyrim runtime.
         */
        enum class Runtime : uint8_t {
            Unknown = 0,

            /**
             * The Skyrim runtime is a post-Anniversary Edition Skyrim SE release (version 1.6.x and later).
             */
            AE = 1 << 0,

            /**
             * The Skyrim runtime is a pre-Anniversary Edition Skyrim SE release (version 1.5.97 and prior).
             */
            SE = 1 << 1,

            /**
             * The Skyrim runtime is Skyrim VR.
             */
            VR = 1 << 2
        };

        [[nodiscard]] static Module &get() {
            if (_initialized.load(std::memory_order_relaxed)) {
                return _instance;
            }
            [[maybe_unused]] std::unique_lock lock(_initLock);
            _instance.init();
            _initialized = true;
            return _instance;
        }

#ifdef ENABLE_COMMONLIBSSE_TESTING
        /**
         * Forcibly set the singleton <code>Module</code> instance to a specific executable file.
         *
         * <p>
         * This function should only be used in a unit testing environment, where there is no Skyrim process hosting
         * the SKSE plugin. It is not thread-safe and assumes it will be run synchronously with serial test execution.
         * </p>
         *
         * @param a_filePath The path to the executable to use a Skyrim executable.
         * @return <code>true</code> if the module injection works; <code>false</code> otherwise.
         */
        static bool inject(std::wstring_view a_filePath) {
            _instance.clear();
            _initialized = true;
            return _instance.init(a_filePath);
        }

        /**
         * Forcibly set the singleton <code>Module</code> instance to an installed Skyrim executable.
         *
         * <p>
         * This overload accepts only a <code>Runtime</code> value, and based on that value attempts to find the
         * executable, using the Windows registry to find an installed copy of Skyrim of the given runtime. Using
         * <code>Runtime::Unknown</code> causes an attempt to discover Skyrim Special Edition first, and if that fails,
         * attempts to discover Skyrim VR.
         * </p>
         *
         * <p>
         * This function should only be used in a unit testing environment, where there is no Skyrim process hosting
         * the SKSE plugin. It is not thread-safe and assumes it will be run synchronously with serial test execution.
         * </p>
         *
         * @param a_runtime The type of Skyrim runtime to inject.
         */
        static bool inject(Runtime a_runtime = Runtime::Unknown) {
            if (a_runtime == Runtime::Unknown) {
                return inject(Runtime::SE) || inject(Runtime::VR);
            }

            constexpr std::size_t bufferSize = 4096;  // Max NTFS path length.
            const wchar_t *subKey =
                    a_runtime == Runtime::VR ?
                    LR"(SOFTWARE\Bethesda Softworks\Skyrim VR)" :
                    LR"(SOFTWARE\Bethesda Softworks\Skyrim Special Edition)";
            unsigned long length = bufferSize * sizeof(wchar_t);
            std::uint8_t value[bufferSize];
            if (WinAPI::RegGetValueW(WinAPI::HKEY_LOCAL_MACHINE, subKey, L"Installed Path", 0x20002, nullptr, value, &length) !=
                0) {
                return false;
            }
            std::filesystem::path installPath(reinterpret_cast<wchar_t *>(value));
            installPath /= a_runtime == Runtime::VR ? L"SkyrimVR.exe" : L"SkyrimSE.exe";
            return inject(installPath.c_str());
        }

        static bool mock(REL::Version a_version, Runtime a_runtime = Runtime::Unknown,
                         std::wstring_view a_filename = L"SkyrimSE.exe"sv, std::uintptr_t a_base = 0,
                         std::array<std::uintptr_t, Segment::total> a_segmentSizes =
                                 {0x1603000, 0, 0x8ee000, 0x1887000, 0x15c000, 0x3000, 0x2000, 0x1000}) {
            _instance.clear();
            _initialized = true;

            if (a_filename.empty() || !a_segmentSizes[0]) {
                return false;
            }

            _instance._filename = _instance._filePath = a_filename.data();
            _instance._version = a_version;
            if (a_runtime == Runtime::Unknown) {
                switch (a_version[1]) {
                    case 4:
                        _instance._runtime = Runtime::VR;
                        break;
                    case 6:
                        _instance._runtime = Runtime::AE;
                        break;
                    default:
                        _instance._runtime = Runtime::SE;
                }
            } else {
                _instance._runtime = a_runtime;
            }
            _instance._base = a_base;

            auto currentAddress = a_base + 0x1000;
            for (std::size_t i = 0; i < a_segmentSizes.size(); ++i) {
                auto &segment = _instance._segments[i];
                segment._size = a_segmentSizes[i];
                if (segment._size) {
                    segment._proxyBase = a_base;
                    segment._address = (currentAddress += segment._size);
                }
            }

            return true;
        }

        static void reset() {
            _initialized = false;
            _instance.clear();
        }
#endif

        [[nodiscard]] std::uintptr_t base() const noexcept { return _base; }

        [[nodiscard]] stl::zwstring filename() const noexcept { return _filename; }

        [[nodiscard]] stl::zwstring filePath() const noexcept { return _filePath; }

        [[nodiscard]] Version version() const noexcept { return _version; }

        [[nodiscard]] Segment segment(Segment::Name a_segment) const noexcept { return _segments[a_segment]; }

        [[nodiscard]] void *pointer() const noexcept { return reinterpret_cast<void *>(base()); }

        template<class T>
        [[nodiscard]] T *pointer() const noexcept {
            return static_cast<T *>(pointer());
        }

        /**
         * Get the type of runtime the currently-loaded Skyrim module is.
         */
        [[nodiscard]] static SKYRIM_REL Runtime GetRuntime() noexcept {
#if (!defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR))
            return Runtime::SE;
#elif (!defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_VR))
            return Runtime::AE;
#elif (!defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE))
            return Runtime::VR;
#else
            return get()._runtime;
#endif
        }

        /**
         * Returns whether the current Skyrim runtime is a post-Anniversary Edition Skyrim SE release.
         */
        [[nodiscard]] static SKYRIM_REL bool IsAE() noexcept {
            return GetRuntime() == Runtime::AE;
        }

        /**
         * Returns whether the current Skyrim runtime is a pre-Anniversary Edition Skyrim SE release.
         */
        [[nodiscard]] static SKYRIM_REL bool IsSE() noexcept {
            return GetRuntime() == Runtime::SE;
        }

        /**
         * Returns whether the current Skyrim runtime is a Skyrim VR release.
         */
        [[nodiscard]] static SKYRIM_REL_VR bool IsVR() noexcept {
#ifndef ENABLE_SKYRIM_VR
            return false;
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
            return true;
#else
            return GetRuntime() == Runtime::VR;
#endif
        }

    private:
        Module() = default;

        Module(const Module &) = delete;

        Module(Module &&) = delete;

        ~Module() noexcept = default;

        Module &operator=(const Module &) = delete;

        Module &operator=(Module &&) = delete;

        bool init() {
            const auto getFilename = [&]() {
                return GetEnvironmentVariable(
                        ENVIRONMENT.data(),
                        _filename.data(),
                        static_cast<std::uint32_t>(_filename.size()));
            };

            void *moduleHandle = nullptr;
            _filename.resize(getFilename());
            if (const auto result = getFilename();
                    result != _filename.size() - 1 ||
                    result == 0) {
                for (auto runtime: RUNTIMES) {
                    _filename = runtime;
                    moduleHandle = GetModuleHandle(_filename.c_str());
                    if (moduleHandle) {
                        break;
                    }
                }
            }
            _filePath = _filename;
            if (!moduleHandle) {
                stl::report_and_fail(
                        fmt::format(
                                "Failed to obtain module handle for: \"{0}\".\n"
                                "You have likely renamed the executable to something unexpected. "
                                "Renaming the executable back to \"{0}\" may resolve the issue."sv,
                                stl::utf16_to_utf8(_filename).value_or("<unicode conversion error>"s)));
            }
            return load(moduleHandle, true);
        }

        bool init(std::wstring_view a_filePath) {
            std::filesystem::path exePath(a_filePath);
            _filename = exePath.filename().wstring();
            _filePath = exePath.wstring();
            _injectedModule = LoadLibrary(_filePath.c_str());
            if (_injectedModule) {
                return load(_injectedModule, false);
            }
            return false;
        }

        [[nodiscard]] bool load(void *a_handle, bool a_failOnError) {
            _base = reinterpret_cast<std::uintptr_t>(a_handle);
            if (!load_version(a_failOnError)) {
                return false;
            }
            load_segments();
            return true;
        }

        void load_segments();

        bool load_version(bool a_failOnError) {
            const auto version = get_file_version(_filePath);
            if (version) {
                _version = *version;
                switch (_version[1]) {
                    case 4:
                        _runtime = Runtime::VR;
                        break;
                    case 6:
                        _runtime = Runtime::AE;
                        break;
                    default:
                        _runtime = Runtime::SE;
                }
                return true;
            }
            return stl::report_and_error(
                    fmt::format(
                            "Failed to obtain file version info for: {}\n"
                            "Please contact the author of this script extender plugin for further assistance."sv,
                            stl::utf16_to_utf8(_filename).value_or("<unicode conversion error>"s)), a_failOnError);
        }

        void clear();

        static constexpr std::array SEGMENTS{
                std::make_pair(".text"sv, IMAGE_SCN_MEM_EXECUTE),
                std::make_pair(".idata"sv, static_cast<std::uint32_t>(0)),
                std::make_pair(".rdata"sv, static_cast<std::uint32_t>(0)),
                std::make_pair(".data"sv, static_cast<std::uint32_t>(0)),
                std::make_pair(".pdata"sv, static_cast<std::uint32_t>(0)),
                std::make_pair(".tls"sv, static_cast<std::uint32_t>(0)),
                std::make_pair(".text"sv, IMAGE_SCN_MEM_WRITE),
                std::make_pair(".gfids"sv, static_cast<std::uint32_t>(0))
        };

        static constexpr auto ENVIRONMENT = L"SKSE_RUNTIME"sv;
        static constexpr std::array<std::wstring_view, 2> RUNTIMES{{L"SkyrimVR.exe",
                                                                    L"SkyrimSE.exe"}};

        static Module _instance;
        static inline std::atomic_bool _initialized{false};
        static inline std::mutex _initLock;
        WinAPI::HMODULE _injectedModule{nullptr};
        std::wstring _filename;
        std::wstring _filePath;
        std::array<Segment, Segment::total> _segments;
        Version _version;
        std::uintptr_t _base{0};
        Runtime _runtime{Runtime::AE};
    };

    class IDDatabase {
    private:
        struct mapping_t {
            std::uint64_t id;
            std::uint64_t offset;
        };

    public:
        enum class Format {
            SSEv1,
            SSEv2,
            VR
        };

        class Offset2ID {
        public:
            using value_type = mapping_t;
            using container_type = std::vector<value_type>;
            using size_type = typename container_type::size_type;
            using const_iterator = typename container_type::const_iterator;
            using const_reverse_iterator = typename container_type::const_reverse_iterator;

            template<class ExecutionPolicy>
            explicit Offset2ID(ExecutionPolicy &&a_policy)  //
            requires(std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>)
            {
                const std::span<const mapping_t> id2offset = IDDatabase::get()._id2offset;
                _offset2id.reserve(id2offset.size());
                _offset2id.insert(_offset2id.begin(), id2offset.begin(), id2offset.end());
                std::sort(
                        a_policy,
                        _offset2id.begin(),
                        _offset2id.end(),
                        [](auto &&a_lhs, auto &&a_rhs) {
                            return a_lhs.offset < a_rhs.offset;
                        });
            }

            Offset2ID() :
                    Offset2ID(std::execution::sequenced_policy{}) {}

            [[nodiscard]] std::uint64_t operator()(std::size_t a_offset) const {
                const mapping_t elem{0, a_offset};
                const auto it = std::lower_bound(
                        _offset2id.begin(),
                        _offset2id.end(),
                        elem,
                        [](auto &&a_lhs, auto &&a_rhs) {
                            return a_lhs.offset < a_rhs.offset;
                        });
                if (it == _offset2id.end()) {
                    stl::report_and_fail(
                            fmt::format(
                                    "Failed to find the offset within the database: 0x{:08X}"sv,
                                    a_offset));
                }

                return it->id;
            }

            [[nodiscard]] const_iterator begin() const noexcept { return _offset2id.begin(); }

            [[nodiscard]] const_iterator cbegin() const noexcept { return _offset2id.cbegin(); }

            [[nodiscard]] const_iterator end() const noexcept { return _offset2id.end(); }

            [[nodiscard]] const_iterator cend() const noexcept { return _offset2id.cend(); }

            [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return _offset2id.rbegin(); }

            [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return _offset2id.crbegin(); }

            [[nodiscard]] const_reverse_iterator rend() const noexcept { return _offset2id.rend(); }

            [[nodiscard]] const_reverse_iterator crend() const noexcept { return _offset2id.crend(); }

            [[nodiscard]] size_type size() const noexcept { return _offset2id.size(); }

        private:
            container_type _offset2id;
        };

        [[nodiscard]] static IDDatabase &get() {
            if (_initialized.load(std::memory_order_relaxed)) {
                return _instance;
            }
            [[maybe_unused]] std::unique_lock lock(_initLock);
            _instance.load();
            _initialized.store(true, std::memory_order_relaxed);
            return _instance;
        }

#ifdef ENABLE_COMMONLIBSSE_TESTING
        [[nodiscard]] static bool inject(std::wstring_view a_filePath, Format a_format) {
            return inject(a_filePath, a_format, Module::get().version());
        }

        [[nodiscard]] static bool inject(std::wstring_view a_filePath, Format a_format, Version a_version) {
            _initialized = true;
            _instance.clear();
            switch (a_format) {
                case Format::SSEv1:
                    return _instance.load_file(a_filePath.data(), a_version, 1, false);
                case Format::SSEv2:
                    return _instance.load_file(a_filePath.data(), a_version, 2, false);
                case Format::VR:
                    return _instance.load_csv(a_filePath.data(), a_version, false);
                default:
                    return false;
            }
        }

        static void reset() {
            _instance.clear();
            _initialized = false;
        }
#endif

        [[nodiscard]] inline std::size_t id2offset(std::uint64_t a_id) const {
            mapping_t elem{a_id, 0};
            const auto it = std::lower_bound(
                    _id2offset.begin(),
                    _id2offset.end(),
                    elem,
                    [](auto &&a_lhs, auto &&a_rhs) {
                        return a_lhs.id < a_rhs.id;
                    });

            bool failed = false;
            if (it == _id2offset.end()) {
                failed = true;
            } else if SKYRIM_REL_VR_CONSTEXPR (Module::IsVR()) {
                if (it->id != a_id) {
                    failed = true;
                }
            }
            if (failed) {
                stl::report_and_fail(
                        fmt::format(
                                "Failed to find the id within the address library: {}\n"
                                "This means this script extender plugin is incompatible with the address "
                                "library for this version of the game, and thus does not support it."sv,
                                a_id));
            }

            return static_cast<std::size_t>(it->offset);
        }

    private:
        friend class Module;

        friend Offset2ID;

        class istream_t {
        public:
            using stream_type = std::ifstream;
            using pointer = stream_type *;
            using const_pointer = const stream_type *;
            using reference = stream_type &;
            using const_reference = const stream_type &;

            inline istream_t(stl::zwstring a_filename, std::ios_base::openmode a_mode) :
                    _stream(a_filename.data(), a_mode) {
                if (!_stream.is_open()) {
                    stl::report_and_fail("failed to open address library file");
                }

                _stream.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
            }

            inline void ignore(std::streamsize a_count) { _stream.ignore(a_count); }

            template<class T>
            inline void readin(T &a_val) {
                _stream.read(reinterpret_cast<char *>(std::addressof(a_val)), sizeof(T));
            }

            template<
                    class T,
                    std::enable_if_t<
                            std::is_arithmetic_v<T>,
                            int> = 0>
            inline T readout() {
                T val{};
                readin(val);
                return val;
            }

        private:
            stream_type _stream;
        };

        class header_t {
        public:
            void read(istream_t &a_in, std::uint8_t a_formatVersion) {
                std::int32_t format{};
                a_in.readin(format);
                if (format != a_formatVersion) {
                    stl::report_and_fail(
                            fmt::format(
                                    "Unsupported address library format: {}\n"
                                    "This means this script extender plugin is incompatible with the address "
                                    "library available for this version of the game, and thus does not "
                                    "support it."sv,
                                    format));
                }

                std::int32_t version[4]{};
                std::int32_t nameLen{};
                a_in.readin(version);
                a_in.readin(nameLen);
                a_in.ignore(nameLen);

                a_in.readin(_pointerSize);
                a_in.readin(_addressCount);

                for (std::size_t i = 0; i < std::extent_v<decltype(version)>; ++i) {
                    _version[i] = static_cast<std::uint16_t>(version[i]);
                }
            }

            [[nodiscard]] std::size_t address_count() const noexcept { return static_cast<std::size_t>(_addressCount); }

            [[nodiscard]] std::uint64_t
            pointer_size() const noexcept { return static_cast<std::uint64_t>(_pointerSize); }

            [[nodiscard]] Version version() const noexcept { return _version; }

        private:
            Version _version;
            std::int32_t _pointerSize{0};
            std::int32_t _addressCount{0};
        };

        IDDatabase() = default;

        IDDatabase(const IDDatabase &) = delete;

        IDDatabase(IDDatabase &&) = delete;

        ~IDDatabase() = default;

        IDDatabase &operator=(const IDDatabase &) = delete;

        IDDatabase &operator=(IDDatabase &&) = delete;

        void load() {
            const auto version = Module::get().version();
            if SKYRIM_REL_CONSTEXPR (Module::IsVR()) {
                const auto filename =
                        stl::utf8_to_utf16(
                                fmt::format(
                                        "Data/SKSE/Plugins/version-{}.csv"sv,
                                        version.string()))
                                .value_or(L"<unknown filename>"s);
                load_csv(filename, version, true);
            } else {
                const auto filename =
                        stl::utf8_to_utf16(
                                Module::IsAE() ?
                                fmt::format("Data/SKSE/Plugins/versionlib-{}.bin"sv,
                                            version.string()) :
                                fmt::format("Data/SKSE/Plugins/version-{}.bin"sv,
                                            version.string()))
                                .value_or(L"<unknown filename>"s);
                load_file(filename, version, Module::IsAE() ? 2 : 1, true);
            }
        }

        bool load_file(stl::zwstring a_filename, Version a_version, std::uint8_t a_formatVersion, bool a_failOnError) {
            try {
                istream_t in(a_filename.data(), std::ios::in | std::ios::binary);
                header_t header;
                header.read(in, a_formatVersion);
                if (header.version() != a_version) {
                    return stl::report_and_error("version mismatch"sv, a_failOnError);
                }

                auto mapname = L"CommonLibSSEOffsets-v2-"s;
                mapname += a_version.wstring();
                const auto byteSize = static_cast<std::size_t>(header.address_count()) * sizeof(mapping_t);
				if (_mmap.open(mapname, byteSize)) {
					_id2offset = { static_cast<mapping_t*>(_mmap.data()), header.address_count() };
				} else if (_mmap.create(mapname, byteSize)) {
					_id2offset = { static_cast<mapping_t*>(_mmap.data()), header.address_count() };
					unpack_file(in, header, a_failOnError);
					std::sort(
						_id2offset.begin(),
						_id2offset.end(),
						[](auto&& a_lhs, auto&& a_rhs) {
							return a_lhs.id < a_rhs.id;
						});
				} else {
					return stl::report_and_error("failed to create shared mapping"sv, a_failOnError);
				}
            } catch (const std::system_error &) {
                return stl::report_and_error(
                        fmt::format(
                                "Failed to locate an appropriate address library with the path: {}\n"
                                "This means you are missing the address library for this specific version of "
                                "the game. Please continue to the mod page for address library to download "
                                "an appropriate version. If one is not available, then it is likely that "
                                "address library has not yet added support for this version of the game."sv,
                                stl::utf16_to_utf8(a_filename).value_or("<unknown filename>"s)), a_failOnError);
                return false;
            }
            return true;
        }

        bool load_csv(stl::zwstring a_filename, Version a_version, bool a_failOnError) {
            auto nstring = SKSE::stl::utf16_to_utf8(a_filename).value_or(""s);
            if (!std::filesystem::exists(nstring)) {
                return stl::report_and_error(
                        fmt::format("Required VR Address Library file {} does not exist"sv, nstring),
                        a_failOnError);
            }
            rapidcsv::Document in(nstring);
            std::size_t id, address_count;
            std::string version, offset;
            auto mapname = L"CommonLibSSEOffsets-v2-"s;
            mapname += a_version.wstring();
            address_count = in.GetCell<std::size_t>(0, 0);
            version = in.GetCell<std::string>(1, 0);
            const auto byteSize = static_cast<std::size_t>(address_count * sizeof(mapping_t));
            if (!_mmap.open(mapname, byteSize) &&
                !_mmap.create(mapname, byteSize)) {
                return stl::report_and_error("failed to create shared mapping"sv, a_failOnError);
            }
            _id2offset = {static_cast<mapping_t *>(_mmap.data()), static_cast<std::size_t>(address_count)};
            if (in.GetRowCount() > address_count + 1) {
                return stl::report_and_error(
                        fmt::format("VR Address Library {} tried to exceed {} allocated entries."sv,
                                    version, address_count), a_failOnError);
            } else if (in.GetRowCount() < address_count + 1) {
                return stl::report_and_error(
                        fmt::format(
                                "VR Address Library {} loaded only {} entries but expected {}. Please redownload."sv,
                                version, in.GetRowCount() - 1, address_count), a_failOnError);
            }
            std::size_t index = 1;
            for (; index < in.GetRowCount(); ++index) {
                id = in.GetCell<std::size_t>(0, index);
                offset = in.GetCell<std::string>(1, index);
                _id2offset[index - 1] = {static_cast<std::uint64_t>(id),
                                         static_cast<std::uint64_t>(std::stoul(offset, nullptr, 16))};
            }
            std::sort(
                    _id2offset.begin(),
                    _id2offset.end(),
                    [](auto &&a_lhs, auto &&a_rhs) {
                        return a_lhs.id < a_rhs.id;
                    });
            return true;
        }

        bool unpack_file(istream_t &a_in, header_t a_header, bool a_failOnError) {
            std::uint8_t type = 0;
            std::uint64_t id = 0;
            std::uint64_t offset = 0;
            std::uint64_t prevID = 0;
            std::uint64_t prevOffset = 0;
            for (auto &mapping: _id2offset) {
                a_in.readin(type);
                const auto lo = static_cast<std::uint8_t>(type & 0xF);
                const auto hi = static_cast<std::uint8_t>(type >> 4);

                switch (lo) {
                    case 0:
                        a_in.readin(id);
                        break;
                    case 1:
                        id = prevID + 1;
                        break;
                    case 2:
                        id = prevID + a_in.readout<std::uint8_t>();
                        break;
                    case 3:
                        id = prevID - a_in.readout<std::uint8_t>();
                        break;
                    case 4:
                        id = prevID + a_in.readout<std::uint16_t>();
                        break;
                    case 5:
                        id = prevID - a_in.readout<std::uint16_t>();
                        break;
                    case 6:
                        id = a_in.readout<std::uint16_t>();
                        break;
                    case 7:
                        id = a_in.readout<std::uint32_t>();
                        break;
                    default:
                        return stl::report_and_error("unhandled type"sv, a_failOnError);
                }

                const std::uint64_t tmp = (hi & 8) != 0 ? (prevOffset / a_header.pointer_size()) : prevOffset;

                switch (hi & 7) {
                    case 0:
                        a_in.readin(offset);
                        break;
                    case 1:
                        offset = tmp + 1;
                        break;
                    case 2:
                        offset = tmp + a_in.readout<std::uint8_t>();
                        break;
                    case 3:
                        offset = tmp - a_in.readout<std::uint8_t>();
                        break;
                    case 4:
                        offset = tmp + a_in.readout<std::uint16_t>();
                        break;
                    case 5:
                        offset = tmp - a_in.readout<std::uint16_t>();
                        break;
                    case 6:
                        offset = a_in.readout<std::uint16_t>();
                        break;
                    case 7:
                        offset = a_in.readout<std::uint32_t>();
                        break;
                    default:
                        return stl::report_and_error("unhandled type"sv, a_failOnError);
                }

                if ((hi & 8) != 0) {
                    offset *= a_header.pointer_size();
                }

                mapping = {id, offset};

                prevOffset = offset;
                prevID = id;
            }
            return true;
        }

        void clear() {
            _mmap.close();
            _id2offset = {};
        }

        static IDDatabase _instance;
        static inline std::atomic_bool _initialized{false};
        static inline std::mutex _initLock;
        detail::memory_map _mmap;
        std::span<mapping_t> _id2offset;
    };

    class Offset {
    public:
        constexpr Offset() noexcept = default;

        explicit constexpr Offset(std::size_t a_offset) noexcept:
                _offset(a_offset) {}

        constexpr Offset &operator=(std::size_t a_offset) noexcept {
            _offset = a_offset;
            return *this;
        }

        [[nodiscard]] std::uintptr_t address() const { return base() + offset(); }

        [[nodiscard]] constexpr std::size_t offset() const noexcept { return _offset; }

    private:
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }

        std::size_t _offset{0};
    };

    class VariantOffset {
    public:
        constexpr VariantOffset() noexcept = default;

        explicit constexpr VariantOffset([[maybe_unused]] std::size_t a_seOffset,
                                         [[maybe_unused]] std::size_t a_aeOffset,
                                         [[maybe_unused]] std::size_t a_vrOffset) noexcept {
#ifdef ENABLE_SKYRIM_SE
            _seOffset = a_seOffset;
#endif
#ifdef ENABLE_SKYRIM_AE
            _aeOffset = a_aeOffset;
#endif
#ifdef ENABLE_SKYRIM_VR
            _vrOffset = a_vrOffset;
#endif
        }

        [[nodiscard]] std::uintptr_t address() const {
            auto thisOffset = offset();
            return thisOffset ? base() + thisOffset : 0;
        }

        [[nodiscard]] SKYRIM_REL std::size_t offset() const noexcept {
            switch (Module::GetRuntime()) {
#ifdef ENABLE_SKYRIM_AE
                case Module::Runtime::AE:
                    return _aeOffset;
#endif
#ifdef ENABLE_SKYRIM_SE
                case Module::Runtime::SE:
                    return _seOffset;
#endif
#ifdef ENABLE_SKYRIM_VR
                case Module::Runtime::VR:
                    return _vrOffset;
#endif
                default:
                    return 0;
            }
        }

        [[nodiscard]] SKYRIM_REL explicit operator Offset() const noexcept { return Offset(offset()); }

    private:
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }

#ifdef ENABLE_SKYRIM_SE
        std::size_t _seOffset{0};
#endif
#ifdef ENABLE_SKYRIM_AE
        std::size_t _aeOffset{0};
#endif
#ifdef ENABLE_SKYRIM_VR
        std::size_t _vrOffset{0};
#endif
    };

    class ID {
    public:
        constexpr ID() noexcept = default;

        explicit constexpr ID(std::uint64_t a_id) noexcept:
                _id(a_id) {}

        constexpr ID &operator=(std::uint64_t a_id) noexcept {
            _id = a_id;
            return *this;
        }

        [[nodiscard]] std::uintptr_t address() const { return base() + offset(); }

        [[nodiscard]] constexpr std::uint64_t id() const noexcept { return _id; }

        [[nodiscard]] std::size_t offset() const { return IDDatabase::get().id2offset(_id); }

    private:
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }

        std::uint64_t _id{0};
    };

    class RelocationID {
    public:
        constexpr RelocationID() noexcept = default;

        explicit constexpr RelocationID([[maybe_unused]] std::uint64_t a_seID,
                                        [[maybe_unused]] std::uint64_t a_aeID) noexcept {
#ifdef ENABLE_SKYRIM_SE
            _seID = a_seID;
#endif
#ifdef ENABLE_SKYRIM_AE
            _aeID = a_aeID;
#endif
#ifdef ENABLE_SKYRIM_VR
            _vrID = a_seID;
#endif
        }

        explicit constexpr RelocationID([[maybe_unused]] std::uint64_t a_seID, [[maybe_unused]] std::uint64_t a_aeID,
                                        [[maybe_unused]] std::uint64_t a_vrID) noexcept {
#ifdef ENABLE_SKYRIM_SE
            _seID = a_seID;
#endif
#ifdef ENABLE_SKYRIM_AE
            _aeID = a_aeID;
#endif
#ifdef ENABLE_SKYRIM_VR
            _vrID = a_vrID;
#endif
        }

        [[nodiscard]] std::uintptr_t address() const {
            auto thisOffset = offset();
            return thisOffset ? base() + offset() : 0;
        }

        [[nodiscard]] std::size_t offset() const {
            auto thisID = id();
            return thisID ? IDDatabase::get().id2offset(thisID) : 0;
        }

        [[nodiscard]] SKYRIM_REL std::uint64_t id() const noexcept {
            switch (Module::GetRuntime()) {
#ifdef ENABLE_SKYRIM_AE
                case Module::Runtime::AE:
                    return _aeID;
#endif
#ifdef ENABLE_SKYRIM_SE
                case Module::Runtime::SE:
                    return _seID;
#endif
#ifdef ENABLE_SKYRIM_VR
                case Module::Runtime::VR:
                    return _vrID;
#endif
                default:
                    return 0;
            }
        }

        [[nodiscard]] SKYRIM_REL explicit operator ID() const noexcept {
            return ID(id());
        }

    private:
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }

#ifdef ENABLE_SKYRIM_SE
        std::uint64_t _seID{0};
#endif
#ifdef ENABLE_SKYRIM_AE
        std::uint64_t _aeID{0};
#endif
#ifdef ENABLE_SKYRIM_VR
        std::uint64_t _vrID{0};
#endif
    };

    class VariantID {
    public:
        constexpr VariantID() noexcept = default;

        explicit constexpr VariantID([[maybe_unused]] std::uint64_t a_seID, [[maybe_unused]] std::uint64_t a_aeID,
                                     [[maybe_unused]] std::uint64_t a_vrOffset) noexcept {
#ifdef ENABLE_SKYRIM_SE
            _seID = a_seID;
#endif
#ifdef ENABLE_SKYRIM_AE
            _aeID = a_aeID;
#endif
#ifdef ENABLE_SKYRIM_VR
            _vrOffset = a_vrOffset;
#endif
        }

        [[nodiscard]] std::uintptr_t address() const {
            auto thisOffset = offset();
            return thisOffset ? base() + offset() : 0;
        }

        [[nodiscard]] std::size_t offset() const {
            switch (Module::GetRuntime()) {
#ifdef ENABLE_SKYRIM_AE
                case Module::Runtime::AE:
                    return _aeID ? IDDatabase::get().id2offset(_aeID) : 0;
#endif
#ifdef ENABLE_SKYRIM_SE
                case Module::Runtime::SE:
                    return _seID ? IDDatabase::get().id2offset(_seID) : 0;
#endif
#ifdef ENABLE_SKYRIM_VR
                case Module::Runtime::VR:
                    return _vrOffset;
#endif
                default:
                    return 0;
            }
        }

    private:
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }

#ifdef ENABLE_SKYRIM_SE
        std::uint64_t _seID{0};
#endif
#ifdef ENABLE_SKYRIM_AE
        std::uint64_t _aeID{0};
#endif
#ifdef ENABLE_SKYRIM_VR
        std::uint64_t _vrOffset{0};
#endif
    };

    template<class T>
    class Relocation {
    public:
        using value_type =
                std::conditional_t<
                        std::is_member_pointer_v<T> || std::is_function_v<std::remove_pointer_t<T>>,
                        std::decay_t<T>,
                        T>;

        constexpr Relocation() noexcept = default;

        explicit constexpr Relocation(std::uintptr_t a_address) noexcept:
                _impl{a_address} {}

        explicit Relocation(Offset a_offset) :
                _impl{a_offset.address()} {}

        explicit Relocation(VariantOffset a_offset) :
                _impl{a_offset.address()} {}

        explicit Relocation(ID a_id) :
                _impl{a_id.address()} {}

        explicit Relocation(ID a_id, std::ptrdiff_t a_offset) :
                _impl{a_id.address() + a_offset} {}

        explicit Relocation(ID a_id, Offset a_offset) :
                _impl{a_id.address() + a_offset.offset()} {}

        explicit Relocation(ID a_id, VariantOffset a_offset) :
                _impl{a_id.address() + a_offset.offset()} {}

        explicit Relocation(RelocationID a_id) :
                _impl{a_id.address()} {}

        explicit Relocation(RelocationID a_id, std::ptrdiff_t a_offset) :
                _impl{a_id.address() + a_offset} {}

        explicit Relocation(RelocationID a_id, Offset a_offset) :
                _impl{a_id.address() + a_offset.offset()} {}

        explicit Relocation(RelocationID a_id, VariantOffset a_offset) :
                _impl{a_id.address() + a_offset.offset()} {}

        explicit Relocation(VariantID a_id) :
                _impl{a_id.address()} {}

        explicit Relocation(VariantID a_id, std::ptrdiff_t a_offset) :
                _impl{a_id.address() + a_offset} {}

        explicit Relocation(VariantID a_id, Offset a_offset) :
                _impl{a_id.address() + a_offset.offset()} {}

        explicit Relocation(VariantID a_id, VariantOffset a_offset) :
                _impl{a_id.address() + a_offset.offset()} {}

        constexpr Relocation &operator=(std::uintptr_t a_address) noexcept {
            _impl = a_address;
            return *this;
        }

        Relocation &operator=(Offset a_offset) {
            _impl = a_offset.address();
            return *this;
        }

        Relocation &operator=(VariantOffset a_offset) {
            _impl = a_offset.address();
            return *this;
        }

        Relocation &operator=(ID a_id) {
            _impl = a_id.address();
            return *this;
        }

        Relocation &operator=(RelocationID a_id) {
            _impl = a_id.address();
            return *this;
        }

        Relocation &operator=(VariantID a_id) {
            _impl = a_id.address();
            return *this;
        }

        template<class U = value_type>
        [[nodiscard]] decltype(auto) operator*() const noexcept  //
        requires(std::is_pointer_v<U>)
        {
            return *get();
        }

        template<class U = value_type>
        [[nodiscard]] auto operator->() const noexcept  //
        requires(std::is_pointer_v<U>)
        {
            return get();
        }

        template<class... Args>
        std::invoke_result_t<const value_type &, Args...> operator()(Args &&... a_args) const  //
        noexcept(std::is_nothrow_invocable_v<const value_type &, Args...>)                //
        requires(std::invocable<const value_type &, Args...>)
        {
            return REL::invoke(get(), std::forward<Args>(a_args)...);
        }

        [[nodiscard]]

        constexpr std::uintptr_t address() const noexcept { return _impl; }

        [[nodiscard]] std::size_t offset() const { return _impl - base(); }

        [[nodiscard]] value_type get() const  //
        noexcept(std::is_nothrow_copy_constructible_v<value_type>) {
            assert(_impl != 0);
            return stl::unrestricted_cast<value_type>(_impl);
        }

        template<class U = value_type>
        std::uintptr_t write_vfunc(std::size_t a_idx, std::uintptr_t a_newFunc)  //
        requires(std::same_as<U, std::uintptr_t>)
        {
            const auto addr = address() + (sizeof(void *) * a_idx);
            const auto result = *reinterpret_cast<std::uintptr_t *>(addr);
            safe_write(addr, a_newFunc);
            return result;
        }

        template<class F>
        std::uintptr_t write_vfunc(std::size_t a_idx, F a_newFunc)  //
        requires(std::same_as<value_type, std::uintptr_t>)
        {
            return write_vfunc(a_idx, stl::unrestricted_cast<std::uintptr_t>(a_newFunc));
        }

    private:
        // clang-format off
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }
        // clang-format on

        std::uintptr_t _impl{0};
    };

    namespace detail {
        namespace characters {
            [[nodiscard]] constexpr bool hexadecimal(char a_ch) noexcept {
                return ('0' <= a_ch && a_ch <= '9') ||
                       ('A' <= a_ch && a_ch <= 'F') ||
                       ('a' <= a_ch && a_ch <= 'f');
            }

            [[nodiscard]] constexpr bool space(char a_ch) noexcept {
                return a_ch == ' ';
            }

            [[nodiscard]] constexpr bool wildcard(char a_ch) noexcept {
                return a_ch == '?';
            }
        }

        namespace rules {
            namespace detail {
                [[nodiscard]] constexpr std::byte hexacharacters_to_hexadecimal(char a_hi, char a_lo) noexcept {
                    constexpr auto lut = []() noexcept {
                        std::array<std::uint8_t, (std::numeric_limits<unsigned char>::max)() + 1> a = {};

                        const auto iterate = [&](std::uint8_t a_iFirst, unsigned char a_cFirst,
                                                 unsigned char a_cLast) noexcept {
                            for (; a_cFirst <= a_cLast; ++a_cFirst, ++a_iFirst) {
                                a[a_cFirst] = a_iFirst;
                            }
                        };

                        iterate(0, '0', '9');
                        iterate(0xA, 'A', 'F');
                        iterate(0xa, 'a', 'f');

                        return a;
                    }();

                    return static_cast<std::byte>(
                            lut[static_cast<unsigned char>(a_hi)] * 0x10u +
                            lut[static_cast<unsigned char>(a_lo)]);
                }
            }

            template<char HI, char LO>
            class Hexadecimal {
            public:
                [[nodiscard]] static constexpr bool match(std::byte a_byte) noexcept {
                    constexpr auto expected = detail::hexacharacters_to_hexadecimal(HI, LO);
                    return a_byte == expected;
                }
            };

            static_assert(Hexadecimal<'5', '7'>::match(std::byte{0x57}));
            static_assert(Hexadecimal<'6', '5'>::match(std::byte{0x65}));
            static_assert(Hexadecimal<'B', 'D'>::match(std::byte{0xBD}));
            static_assert(Hexadecimal<'1', 'C'>::match(std::byte{0x1C}));
            static_assert(Hexadecimal<'F', '2'>::match(std::byte{0xF2}));
            static_assert(Hexadecimal<'9', 'f'>::match(std::byte{0x9f}));

            static_assert(!Hexadecimal<'D', '4'>::match(std::byte{0xF8}));
            static_assert(!Hexadecimal<'6', '7'>::match(std::byte{0xAA}));
            static_assert(!Hexadecimal<'7', '8'>::match(std::byte{0xE3}));
            static_assert(!Hexadecimal<'6', 'E'>::match(std::byte{0x61}));

            class Wildcard {
            public:
                [[nodiscard]] static constexpr bool match(std::byte) noexcept {
                    return true;
                }
            };

            static_assert(Wildcard::match(std::byte{0xB9}));
            static_assert(Wildcard::match(std::byte{0x96}));
            static_assert(Wildcard::match(std::byte{0x35}));
            static_assert(Wildcard::match(std::byte{0xE4}));

            template<char, char>
            void rule_for() noexcept;

            template<char C1, char C2>
            Hexadecimal<C1, C2>
            rule_for() noexcept requires (characters::hexadecimal(C1) && characters::hexadecimal(C2));

            template<char C1, char C2>
            Wildcard rule_for() noexcept requires (characters::wildcard(C1) && characters::wildcard(C2));
        }

        template<class... Rules>
        class PatternMatcher {
        public:
            static_assert(sizeof...(Rules) >= 1, "must provide at least 1 rule for the pattern matcher");

            [[nodiscard]] constexpr bool match(std::span<const std::byte, sizeof...(Rules)> a_bytes) const noexcept {
                std::size_t i = 0;
                return (Rules::match(a_bytes[i++]) && ...);
            }

            [[nodiscard]] bool match(std::uintptr_t a_address) const noexcept {
                return this->match(*reinterpret_cast<const std::byte(*)[sizeof...(Rules)]>(a_address));
            }

            void match_or_fail(std::uintptr_t a_address,
                               SKSE::stl::source_location a_loc = SKSE::stl::source_location::current()) const noexcept {
                if (!this->match(a_address)) {
                    const auto version = Module::get().version();
                    stl::report_and_fail(
                            fmt::format(
                                    "A pattern has failed to match.\n"
                                    "This means the plugin is incompatible with the current version of the game ({}.{}.{}). "
                                    "Head to the mod page of this plugin to see if an update is available."sv,
                                    version[0],
                                    version[1],
                                    version[2]),
                            a_loc);
                }
            }
        };

        void consteval_error(const char *a_error);

        template<stl::nttp::string S, class... Rules>
        [[nodiscard]] constexpr auto do_make_pattern() noexcept {
            if constexpr (S.length() == 0) {
                return PatternMatcher<Rules...>();
            } else if constexpr (S.length() == 1) {
                constexpr char c = S[0];
                if constexpr (characters::hexadecimal(c) || characters::wildcard(c)) {
                    consteval_error(
                            "the given pattern has an unpaired rule (rules are required to be written in pairs of 2)");
                } else {
                    consteval_error("the given pattern has trailing characters at the end (which is not allowed)");
                }
            } else {
                using rule_t = decltype(rules::rule_for<S[0], S[1]>());
                if constexpr (std::same_as<rule_t, void>) {
                    consteval_error("the given pattern failed to match any known rules");
                } else {
                    if constexpr (S.length() <= 3) {
                        return do_make_pattern<S.template substr<2>(), Rules..., rule_t>();
                    } else if constexpr (characters::space(S.value_at(2))) {
                        return do_make_pattern<S.template substr<3>(), Rules..., rule_t>();
                    } else {
                        consteval_error("a space character is required to split byte patterns");
                    }
                }
            }
        }

        template<class... Bytes>
        [[nodiscard]] consteval auto make_byte_array(Bytes... a_bytes) noexcept
        -> std::array<std::byte, sizeof...(Bytes)> {
            static_assert((std::integral<Bytes> && ...), "all bytes must be an integral type");
            return {static_cast<std::byte>(a_bytes)...};
        }
    }

    template<stl::nttp::string S>
    [[nodiscard]] constexpr auto make_pattern() noexcept {
        return detail::do_make_pattern<S>();
    }

    static_assert(make_pattern<"40 10 F2 ??">().match(
            detail::make_byte_array(0x40, 0x10, 0xF2, 0x41)));
    static_assert(make_pattern<"B8 D0 ?? ?? D4 6E">().match(
            detail::make_byte_array(0xB8, 0xD0, 0x35, 0x2A, 0xD4, 0x6E)));

    /**
     * Return the correct value of two choices between SE/VR, and AE versions of Skyrim.
     *
     * <p>
     * This is commonly used to select between relative offsets within a function, when hooking a call instruction.
     * In such cases the function can be identified by its Address Library ID, but the offset within the function
     * may vary between Skyrim versions. This selection is made at runtime, allowing the same compiled code to run
     * in multiple versions of Skyrim.
     * </p>
     *
     * @tparam T the type of value to return.
     * @param a_seAndVR the value to use for SE and VR.
     * @param a_ae the value to use for AE.
     * @return Either <code>a_seAndVR</code> if the current runtime is Skyrim SE or VR, or <code>a_ae</code> if the runtime is AE.
     */
    template<class T>
    [[nodiscard]] SKYRIM_ADDR T Relocate([[maybe_unused]] T &&a_seAndVR, [[maybe_unused]] T &&a_ae) noexcept {
#ifndef ENABLE_SKYRIM_AE
        return a_seAndVR;
#elif !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_VR)
        return a_ae;
#else
        return Module::IsAE() ? a_ae : a_seAndVR;
#endif
    }

    /**
     * Return the correct value of two choices between SE, AE, and VR versions of Skyrim.
     *
     * <p>
     * This is commonly used to select between relative offsets within a function, when hooking a call instruction.
     * In such cases the function can be identified by its Address Library ID, but the offset within the function
     * may vary between Skyrim versions. This selection is made at runtime, allowing the same compiled code to run
     * in multiple versions of Skyrim.
     * </p>
     *
     * @tparam T the type of value to return.
     * @param a_se the value to use for SE.
     * @param a_ae the value to use for AE.
     * @param a_vr the value to use for VR.
     * @return Either <code>a_se</code> if the current runtime is Skyrim SE, or <code>a_ae</code> if the runtime is AE, or
     * <code>a_vr</code> if running Skyrim VR.
     */
    template<class T>
    [[nodiscard]] SKYRIM_REL T Relocate([[maybe_unused]] T a_se, [[maybe_unused]] T a_ae,
                                        [[maybe_unused]] T a_vr) noexcept {
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
        return a_se;
#elif !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_VR)
        return a_ae;
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
        return a_vr;
#else
        switch (Module::get().GetRuntime()) {
            case Module::Runtime::AE:
                return a_ae;
            case Module::Runtime::VR:
                return a_vr;
            default:
                return a_se;
        }
#endif
    }

    namespace detail {
        template<class T>
        struct RelocateVirtualHelper {
        };

        template<class Ret, class This>
        struct RelocateVirtualHelper<Ret(This *)> {
            using this_type = This;
            using return_type = Ret;
            using function_type = Ret(This *);
        };

        template<class Ret, class This, class... Args>
        struct RelocateVirtualHelper<Ret(This *, Args...)> {
            using this_type = This;
            using return_type = Ret;
            using function_type = Ret(This *, Args...);
        };

        template<class Ret, class This>
        struct RelocateVirtualHelper<Ret (This::*)()> {
            using this_type = This;
            using return_type = Ret;
            using function_type = Ret(This *);
        };

        template<class Ret, class This, class... Args>
        struct RelocateVirtualHelper<Ret (This::*)(Args...)> {
            using this_type = This;
            using return_type = Ret;
            using function_type = Ret(This *, Args...);
        };

        template<class Ret, class This>
        struct RelocateVirtualHelper<Ret (This::*)() const> {
            using this_type = const This;
            using return_type = Ret;
            using function_type = Ret(const This *);
        };

        template<class Ret, class This, class... Args>
        struct RelocateVirtualHelper<Ret (This::*)(Args...) const> {
            using this_type = const This;
            using return_type = Ret;
            using function_type = Ret(const This *, Args...);
        };
    }

    /**
     * Invokes a virtual function in a cross-platform way where the vtable structure is variant across AE/SE and VR runtimes.
     *
     * <p>
     * Some classes in Skyrim VR add new virtual functions in the middle of the vtable structure, which makes it ABI-incompatible with AE/SE.
     * A naive virtual function call, therefore, cannot work across all runtimes without the plugin being recompiled specifically for VR.
     * This call works with types which have variant vtables to allow a non-virtual function definition to be created in the virtual function's
     * place, and to have that call dynamically lookup the correct function based on the vtable structure expected in the current runtime.
     * </p>
     *
     * @tparam Fn the type of the function being called.
     * @tparam Args the types of the arguments being passed.
     * @param a_seAndAEVtableOffset the offset from the <code>this</code> pointer to the vtable with the virtual function in SE/AE.
     * @param a_vrVtableIndex the offset from the <code>this</code> pointer to the vtable with the virtual function in VR.
     * @param a_seAndAEVtableIndex the index of the function in the class' vtable in SE and AE.
     * @param a_vrVtableIndex the index of the function in the class' vtable in VR.
     * @param a_self the <code>this</code> argument for the call.
     * @param a_args the remaining arguments for the call, if any.
     * @return The result of the function call.
     */
    template<class Fn, class... Args>
    [[nodiscard]] inline typename detail::RelocateVirtualHelper<Fn>::return_type RelocateVirtual(
            [[maybe_unused]] std::ptrdiff_t a_seAndAEVtableOffset, [[maybe_unused]] std::ptrdiff_t a_vrVtableOffset,
            [[maybe_unused]] std::ptrdiff_t a_seAndAEVtableIndex, [[maybe_unused]] std::ptrdiff_t a_vrVtableIndex,
            typename detail::RelocateVirtualHelper<Fn>::this_type *a_self, Args &&... a_args) {
        return (*reinterpret_cast<typename detail::RelocateVirtualHelper<Fn>::function_type **>(
                *reinterpret_cast<const uintptr_t *>(reinterpret_cast<uintptr_t>(a_self) +
                                                     #ifndef ENABLE_SKYRIM_VR
                                                     a_seAndAEVtableOffset) +
			a_seAndAEVtableIndex
                                                     #elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
                                                     a_vrVtableOffset) +
			a_vrVtableIndex
                                                     #else
                                                     (Module::IsVR() ? a_vrVtableOffset : a_seAndAEVtableOffset)) +
                (Module::IsVR() ? a_vrVtableIndex : a_seAndAEVtableIndex)
                #endif
                * sizeof(uintptr_t)))(a_self, std::forward<Args>(a_args)...);
    }

    /**
     * Invokes a virtual function in a cross-platform way where the vtable structure is variant across AE/SE and VR runtimes.
     *
     * <p>
     * Some classes in Skyrim VR add new virtual functions in the middle of the vtable structure, which makes it ABI-incompatible with AE/SE.
     * A naive virtual function call, therefore, cannot work across all runtimes without the plugin being recompiled specifically for VR.
     * This call works with types which have variant vtables to allow a non-virtual function definition to be created in the virtual function's
     * place, and to have that call dynamically lookup the correct function based on the vtable structure expected in the current runtime.
     * </p>
     *
     * <p>
     * This call assumes the vtable to be used is the one at offset 0, i.e. it invokes a virtual function either on the first parent class
     * or the current class.
     * </p>
     *
     * @tparam Fn the type of the function being called.
     * @tparam Args the types of the arguments being passed.
     * @param a_seAndAEVtableIndex the index of the function in the class' vtable in SE and AE.
     * @param a_vrVtableIndex the index of the function in the class' vtable in VR.
     * @param a_self the <code>this</code> argument for the call.
     * @param a_args the remaining arguments for the call, if any.
     * @return The result of the function call.
     */
    template<class Fn, class... Args>
    [[nodiscard]] inline typename detail::RelocateVirtualHelper<Fn>::return_type RelocateVirtual(
            std::ptrdiff_t a_seAndAEVtableIndex, std::ptrdiff_t a_vrVtableIndex,
            typename detail::RelocateVirtualHelper<Fn>::this_type *a_self, Args &&... a_args) {
        return RelocateVirtual<Fn, Args...>(0, 0, a_seAndAEVtableIndex, a_vrVtableIndex, a_self,
                                            std::forward<Args>(a_args)...);
    }

    /**
     * Gets a member variable in a cross-platform way, using runtime-specific memory offsets.
     *
     * <p>
     * This function handles the variant memory structures used in Skyrim VR as compared to versions of SE.
     * It allows a memory offset relative to the object's base address for SE (and AE) and a separate one for
     * VR. This simplifies the process of creating functions to get member variables that are at different
     * offsets in different runtimes from a single build.
     * </p>
     *
     * @tparam T the type of the member being accessed.
     * @tparam This the type of the target object that has the member.
     * @param a_self the target object that has the member.
     * @param a_seAndAE the memory offset of the member in Skyrim SE and AE.
     * @param a_vr the memory offset of the member in Skyrim VR.
     * @return A reference to the member.
     */
    template<class T, class This>
    [[nodiscard]] inline T &RelocateMember(This *a_self, std::ptrdiff_t a_seAndAE, std::ptrdiff_t a_vr) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(a_self) + Relocate(a_seAndAE, a_seAndAE, a_vr));
    }

	template<class T, class This>
	[[nodiscard]] inline T &RelocateMember(This *a_self, std::ptrdiff_t offset) {
		return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(a_self) + offset);
	}

	template<class T, class This>
	[[nodiscard]] inline T &RelocateMemberIf(bool condition, This *a_self, std::ptrdiff_t a, std::ptrdiff_t b) {
		return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(a_self) + (condition ? a : b));
	}

	template<class T, class This>
	[[nodiscard]] inline T &RelocateMemberIfNewer(Version v, This *a_self, std::ptrdiff_t older, std::ptrdiff_t newer) {
		return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(a_self) +
									 (REL::Module::get().version().compare(v) == std::strong_ordering::less ? older : newer));
	}
}

namespace std {
    [[nodiscard]] inline std::string to_string(REL::Version a_version) {
        return a_version.string("."sv);
    }

#ifdef __cpp_lib_format

    template<class CharT>
    struct formatter<REL::Version, CharT> : formatter<std::string, CharT> {
        template<class FormatContext>
        auto format(const REL::Version &a_version, FormatContext &a_ctx) {
            return formatter<std::string, CharT>::format(to_string(a_version), a_ctx);
        }
    };

#endif
}

namespace fmt {
    template<class CharT>
    struct formatter<REL::Version, CharT> : formatter<std::string, CharT> {
        template<class FormatContext>
        auto format(const REL::Version &a_version, FormatContext &a_ctx) {
            return formatter<std::string, CharT>::format(std::to_string(a_version), a_ctx);
        }
    };
}

#undef REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE
#undef REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER
#undef REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL

#undef REL_MAKE_MEMBER_FUNCTION_POD_TYPE
#undef REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER
#undef REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL
