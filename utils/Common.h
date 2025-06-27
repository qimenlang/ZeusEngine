// print func name macro
#if defined(__clang__) || defined(__GNUC__)
#define PRINT_FUNC_NAME()                                                      \
  do {                                                                         \
    std::cout << __PRETTY_FUNCTION__ << "\n";                                   \
  } while (0)
#elif defined(_MSC_VER)
#define PRINT_FUNC_NAME()                                                      \
  do {                                                                         \
    std::cout << __FUNCSIG__ << "\n";                                          \
  } while (0)
#else
#define PRINT_FUNC_NAME()                                                      \
  do {                                                                         \
    std::cout << __func__ << "\n";                                             \
  } while (0)
#endif
