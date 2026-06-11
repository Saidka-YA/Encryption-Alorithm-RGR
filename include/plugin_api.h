#pragma once
/*
 * plugin_api.h — единый C-интерфейс для всех .so плагинов шифрования.
 *
 * Каждая .so экспортирует ровно одну функцию:
 *
 *   void plugin_run();
 *
 * Внутри plugin_run() находится полное интерактивное меню шифра.
 * Загрузка через dlopen/dlsym, без глобального состояния.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Точка входа, которую обязана экспортировать каждая .so */
typedef void (*plugin_run_fn)();

#ifdef __cplusplus
}
#endif
