
#define EX_MAJOR_VERSION 0
#define EX_MINOR_VERSION 1
#define EX_PATCH_VERSION 0

#define EX_CHECK_VERSION(major,minor,path)    \
    (EX_MAJOR_VERSION > (major) || \
     (EX_MAJOR_VERSION == (major) && EX_MINOR_VERSION > (minor)) || \
     (EX_MAJOR_VERSION == (major) && EX_MINOR_VERSION == (minor) && \
      EX_PATCH_VERSION >= (patch)))


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <glib-object.h>

typedef struct ExNameable ExNameable;

typedef struct ExRString ExRString;

typedef struct ExSharedRString ExSharedRString;

typedef ExRString ExRString;

typedef ExSharedRString ExSharedRString;

typedef struct ExFoo {
  GObject parent;
} ExFoo;

typedef struct ExBar {
  ExFoo parent;
} ExBar;

typedef struct ExFooClass {
  GObjectClass parent_class;
  int32_t (*increment)(ExFoo*, int32_t inc);
  void (*incremented)(ExFoo*, int32_t val, int32_t inc);
} ExFooClass;

typedef struct ExBarClass {
  ExFooClass parent_class;
} ExBarClass;

typedef struct ExNameableInterface {
  GTypeInterface parent_iface;
  char *(*get_name)(ExNameable*);
} ExNameableInterface;

#define EX_TYPE_BAR                    (ex_bar_get_type())
#define EX_BAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),EX_TYPE_BAR,ExBar))
#define EX_IS_BAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),EX_TYPE_BAR))
#define EX_BAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),EX_TYPE_BAR,ExBarClass))
#define EX_IS_BAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),EX_TYPE_BAR))
#define EX_BAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),EX_TYPE_BAR,ExBarClass))
G_DEFINE_AUTOPTR_CLEANUP_FUNC(ExBar, g_object_unref)


#define EX_TYPE_FOO                    (ex_foo_get_type())
#define EX_FOO(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),EX_TYPE_FOO,ExFoo))
#define EX_IS_FOO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),EX_TYPE_FOO))
#define EX_FOO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),EX_TYPE_FOO,ExFooClass))
#define EX_IS_FOO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),EX_TYPE_FOO))
#define EX_FOO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),EX_TYPE_FOO,ExFooClass))
G_DEFINE_AUTOPTR_CLEANUP_FUNC(ExFoo, g_object_unref)


#define EX_TYPE_NAMEABLE                    (ex_nameable_get_type())
#define EX_NAMEABLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),EX_TYPE_NAMEABLE,ExNameable))
#define EX_IS_NAMEABLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),EX_TYPE_NAMEABLE))
#define EX_NAMEABLE_GET_INTERFACE(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),EX_TYPE_NAMEABLE,ExNameableInterface))


#define EX_TYPE_R_STRING                    (ex_r_string_get_type())



#define EX_TYPE_SHARED_R_STRING                    (ex_shared_r_string_get_type())



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * ex_bar_get_number:
 * @this: A #ExBar
 *
 * # Safety
 *
 * Must be a BarInstance object.
 */
double ex_bar_get_number(ExBar *this_);

/**
 * ex_bar_set_number:
 * # Safety
 *
 * Must be a BarInstance object.
 */
void ex_bar_set_number(ExBar *this_, double num);

/**
 * ex_bar_new:
 *
 * # Safety
 *
 * Must be a valid C string, 0-terminated.
 */
ExBar *ex_bar_new(const char *name);

GType ex_bar_get_type(void);

/**
 * ex_foo_increment:
 * # Safety
 *
 * Must be a ExFoo object.
 */
int32_t ex_foo_increment(ExFoo *this_, int32_t inc);

/**
 * ex_foo_get_counter:
 *
 * # Safety
 *
 * Must be a FooInstance object.
 */
int32_t ex_foo_get_counter(ExFoo *this_);

/**
 * ex_foo_get_name:
 * # Safety
 *
 * Must be a FooInstance object.
 */
char *ex_foo_get_name(ExFoo *this_);

/**
 * ex_foo_new:
 * # Safety
 *
 * Must be a valid C string, 0-terminated.
 */
ExFoo *ex_foo_new(const char *name);

GType ex_foo_get_type(void);

GType ex_nameable_get_type(void);

/**
 * ex_nameable_get_name:
 *
 * # Safety
 *
 * Must be a Nameable interface.
 */
char *ex_nameable_get_name(ExNameable *this_);

/**
 * ex_rstring_new:
 * # Safety
 *
 * Must be a valid C string, 0-terminated.
 */
ExRString *ex_rstring_new(const char *s);

/**
 * ex_rstring_copy:
 * # Safety
 *
 * Must be a valid RString pointer.
 */
ExRString *ex_rstring_copy(const ExRString *rstring);

/**
 * ex_rstring_free:
 *
 * # Safety
 *
 * Must be a valid RString pointer.
 */
void ex_rstring_free(ExRString *rstring);

/**
 * ex_rstring_get:
 *
 * # Safety
 *
 * Must be a valid RString pointer.
 */
char *ex_rstring_get(const ExRString *rstring);

/**
 * ex_rstring_set:
 *
 * # Safety
 *
 * Must be a valid RString pointer, and a valid C string, 0-terminated.
 */
void ex_rstring_set(ExRString *rstring, const char *s);

GType ex_rstring_get_type(void);

/**
 * ex_shared_rsting_new:
 *
 * # Safety
 *
 * Must be a valid C string, 0-terminated.
 */
ExSharedRString *ex_shared_rstring_new(const char *s);

/**
 * ex_shared_rstring_ref:
 *
 * # Safety
 *
 * Must be a valid SharedRString pointer.
 */
ExSharedRString *ex_shared_rstring_ref(const ExSharedRString *shared_rstring);

/**
 * ex_shared_rstring_unref:
 *
 * # Safety
 *
 * Must be a valid SharedRString pointer.
 */
void ex_shared_rstring_unref(ExSharedRString *shared_rstring);

/**
 * ex_shared_rstring_get:
 *
 * # Safety
 *
 * Must be a valid SharedRString pointer.
 */
char *ex_shared_rstring_get(const ExSharedRString *shared_rstring);

GType ex_shared_rstring_get_type(void);

GType ex_color_get_type(void);

GType ex_flags_get_type(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
