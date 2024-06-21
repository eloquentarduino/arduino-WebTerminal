#pragma once

#ifndef WEBTERMINAL_BUFFER_SIZE
#define WEBTERMINAL_BUFFER_SIZE 256
#endif

#ifndef WEBTERMINAL_QUEUE_SIZE
#define WEBTERMINAL_QUEUE_SIZE 10
#endif


#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
#define log_e(format, ...)        WebTerminal.queue(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__);
#define raw_log_e(format, ...)    log_printf(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__);
#else
#define raw_log_e(format, ...)
#endif

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
#define log_w(format, ...)        WebTerminal.queue(ARDUHAL_LOG_FORMAT(W, format), ##__VA_ARGS__);
#define raw_log_w(format, ...)    log_printf(ARDUHAL_LOG_FORMAT(W, format), ##__VA_ARGS__);
#else
#define raw_log_w(format, ...)
#endif

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
#define log_i(format, ...)              WebTerminal.queue(ARDUHAL_LOG_FORMAT(I, format), ##__VA_ARGS__);
#define raw_log_i(format, ...)    log_printf(ARDUHAL_LOG_FORMAT(I, format), ##__VA_ARGS__);
#else
#define raw_log_i(format, ...)
#endif

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
#define log_d(format, ...)        WebTerminal.queue(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__);
#define raw_log_d(format, ...)    log_printf(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__);
#else
#define raw_log_d(format, ...)
#endif

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_VERBOSE
#define log_v(format, ...)        WebTerminal.queue(ARDUHAL_LOG_FORMAT(V, format), ##__VA_ARGS__);
#define raw_log_v(format, ...)    log_printf(ARDUHAL_LOG_FORMAT(V, format), ##__VA_ARGS__);
#else
#define raw_log_v(format, ...)
#endif