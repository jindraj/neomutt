/**
 * @file
 * Test code for the Sort object
 *
 * @authors
 * Copyright (C) 2017 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "mutt/buffer.h"
#include "mutt/mapping.h"
#include "mutt/memory.h"
#include "mutt/string2.h"
#include "config/account.h"
#include "config/set.h"
#include "config/sort.h"
#include "config/types.h"
#include "config/common.h"

static short VarApple;
static short VarBanana;
static short VarCherry;
static short VarDamson;
static short VarElderberry;
static short VarFig;
static short VarGuava;
static short VarHawthorn;
static short VarIlama;
static short VarJackfruit;
static short VarKumquat;
static short VarLemon;
static short VarMango;
static short VarNectarine;
static short VarOlive;
static short VarPapaya;
static short VarQuince;
static short VarRaspberry;
static short VarStrawberry;

// clang-format off
static struct ConfigDef Vars[] = {
  { "Apple",      DT_SORT,                 0, &VarApple,      1,  NULL              }, /* test_initial_values */
  { "Banana",     DT_SORT,                 0, &VarBanana,     2,  NULL              },
  { "Cherry",     DT_SORT,                 0, &VarCherry,     1,  NULL              },
  { "Damson",     DT_SORT|DT_SORT_INDEX,   0, &VarDamson,     1,  NULL              }, /* test_string_set */
  { "Elderberry", DT_SORT|DT_SORT_ALIAS,   0, &VarElderberry, 11, NULL              },
  { "Fig",        DT_SORT|DT_SORT_BROWSER, 0, &VarFig,        1,  NULL              },
  { "Guava",      DT_SORT|DT_SORT_KEYS,    0, &VarGuava,      1,  NULL              },
  { "Hawthorn",   DT_SORT|DT_SORT_AUX,     0, &VarHawthorn,   1,  NULL              },
  { "Ilama",      DT_SORT|DT_SORT_SIDEBAR, 0, &VarIlama,      17, NULL              },
  { "Jackfruit",  DT_SORT,                 0, &VarJackfruit,  1,  NULL              }, /* test_string_get */
  { "Kumquat",    DT_SORT,                 0, &VarKumquat,    1,  NULL              }, /* test_native_set */
  { "Lemon",      DT_SORT,                 0, &VarLemon,      1,  NULL              }, /* test_native_get */
  { "Mango",      DT_SORT,                 0, &VarMango,      1,  NULL              }, /* test_reset */
  { "Nectarine",  DT_SORT,                 0, &VarNectarine,  1,  validator_fail    },
  { "Olive",      DT_SORT,                 0, &VarOlive,      1,  validator_succeed }, /* test_validator */
  { "Papaya",     DT_SORT,                 0, &VarPapaya,     1,  validator_warn    },
  { "Quince",     DT_SORT,                 0, &VarQuince,     1,  validator_fail    },
  { "Strawberry", DT_SORT,                 0, &VarStrawberry, 1,  NULL              }, /* test_inherit */
  { NULL },
};

static struct ConfigDef Vars2[] = {
  { "Raspberry", DT_SORT|DT_SORT_AUX|DT_SORT_ALIAS, 0, &VarRaspberry, 1, NULL }, /* test_sort_type */
  { NULL },
};
// clang-format on

char *name_list[] = {
  "Damson", "Elderberry", "Fig", "Guava", "Hawthorn", "Ilama",
};

short *var_list[] = {
  &VarDamson, &VarElderberry, &VarFig, &VarGuava, &VarHawthorn, &VarIlama,
};

const struct Mapping *sort_maps[] = {
  SortMethods,    SortAliasMethods, SortBrowserMethods,
  SortKeyMethods, SortAuxMethods,   SortSidebarMethods,
};

static bool test_initial_values(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);
  printf("Apple = %d\n", VarApple);
  printf("Banana = %d\n", VarBanana);

  if ((VarApple != 1) || (VarBanana != 2))
  {
    printf("Error: initial values were wrong\n");
    return false;
  }

  cs_str_string_set(cs, "Apple", "threads", err);
  cs_str_string_set(cs, "Banana", "score", err);

  struct Buffer value;
  mutt_buffer_init(&value);
  value.data = mutt_mem_calloc(1, STRING);
  value.dsize = STRING;
  mutt_buffer_reset(&value);

  int rc;

  mutt_buffer_reset(&value);
  rc = cs_str_initial_get(cs, "Apple", &value);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", value.data);
    FREE(&value.data);
    return false;
  }

  if (mutt_str_strcmp(value.data, "date") != 0)
  {
    printf("Apple's initial value is wrong: '%s'\n", value.data);
    FREE(&value.data);
    return false;
  }
  printf("Apple = %d\n", VarApple);
  printf("Apple's initial value is '%s'\n", value.data);

  mutt_buffer_reset(&value);
  rc = cs_str_initial_get(cs, "Banana", &value);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", value.data);
    FREE(&value.data);
    return false;
  }

  if (mutt_str_strcmp(value.data, "size") != 0)
  {
    printf("Banana's initial value is wrong: '%s'\n", value.data);
    FREE(&value.data);
    return false;
  }
  printf("Banana = %d\n", VarBanana);
  printf("Banana's initial value is '%s'\n", NONULL(value.data));

  mutt_buffer_reset(&value);
  rc = cs_str_initial_set(cs, "Cherry", "size", &value);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", value.data);
    FREE(&value.data);
    return false;
  }

  mutt_buffer_reset(&value);
  rc = cs_str_initial_get(cs, "Cherry", &value);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", value.data);
    FREE(&value.data);
    return false;
  }

  printf("Cherry = %s\n", mutt_map_get_name(VarCherry, SortMethods));
  printf("Cherry's initial value is %s\n", value.data);

  FREE(&value.data);
  return true;
}

static bool test_string_set(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);

  for (unsigned int i = 0; i < mutt_array_size(var_list); i++)
  {
    short *var = var_list[i];

    *var = -1;

    const struct Mapping *map = sort_maps[i];

    int rc;
    for (int j = 0; map[j].name; j++)
    {
      mutt_buffer_reset(err);
      rc = cs_str_string_set(cs, name_list[i], map[j].name, err);
      if (CSR_RESULT(rc) != CSR_SUCCESS)
      {
        printf("%s\n", err->data);
        return false;
      }

      if (rc & CSR_SUC_NO_CHANGE)
      {
        printf("Value of %s wasn't changed\n", map[j].name);
        continue;
      }

      if (*var != map[j].value)
      {
        printf("Value of %s wasn't changed\n", map[j].name);
        return false;
      }
      printf("%s = %d, set by '%s'\n", name_list[i], *var, map[j].name);
    }

    const char *invalid[] = {
      "-1", "999", "junk", NULL,
    };

    for (unsigned int j = 0; j < mutt_array_size(invalid); j++)
    {
      mutt_buffer_reset(err);
      rc = cs_str_string_set(cs, name_list[i], invalid[j], err);
      if (CSR_RESULT(rc) != CSR_SUCCESS)
      {
        printf("Expected error: %s\n", err->data);
      }
      else
      {
        printf("%s = %d, set by '%s'\n", name_list[i], *var, invalid[j]);
        printf("This test should have failed\n");
        return false;
      }
    }
  }

  const char *name = "Damson";
  mutt_buffer_reset(err);
  int rc = cs_str_string_set(cs, name, "last-date-sent", err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarDamson != (SORT_DATE | SORT_LAST))
  {
    printf("Expected %d, got %d\n", (SORT_DATE | SORT_LAST), VarDamson);
    return false;
  }

  mutt_buffer_reset(err);
  rc = cs_str_string_set(cs, name, "reverse-score", err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarDamson != (SORT_SCORE | SORT_REVERSE))
  {
    printf("Expected %d, got %d\n", (SORT_DATE | SORT_LAST), VarDamson);
    return false;
  }

  return true;
}

static bool test_string_get(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);
  const char *name = "Jackfruit";

  VarJackfruit = SORT_SUBJECT;
  mutt_buffer_reset(err);
  int rc = cs_str_string_get(cs, name, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Get failed: %s\n", err->data);
    return false;
  }
  printf("%s = %d, %s\n", name, VarJackfruit, err->data);

  VarJackfruit = SORT_THREADS;
  mutt_buffer_reset(err);
  rc = cs_str_string_get(cs, name, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Get failed: %s\n", err->data);
    return false;
  }
  printf("%s = %d, %s\n", name, VarJackfruit, err->data);

  VarJackfruit = -1;
  mutt_buffer_reset(err);
  printf("Expect error for next test\n");
  rc = cs_str_string_get(cs, name, err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  mutt_buffer_reset(err);
  name = "Raspberry";
  printf("Expect error for next test\n");
  rc = cs_str_string_get(cs, name, err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  return true;
}

static bool test_native_set(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);

  int rc;
  for (unsigned int i = 0; i < mutt_array_size(var_list); i++)
  {
    short *var = var_list[i];

    *var = -1;

    const struct Mapping *map = sort_maps[i];

    for (int j = 0; map[j].name; j++)
    {
      mutt_buffer_reset(err);
      rc = cs_str_native_set(cs, name_list[i], map[j].value, err);
      if (CSR_RESULT(rc) != CSR_SUCCESS)
      {
        printf("%s\n", err->data);
        return false;
      }

      if (rc & CSR_SUC_NO_CHANGE)
      {
        printf("Value of %s wasn't changed\n", map[j].name);
        continue;
      }

      if (*var != map[j].value)
      {
        printf("Value of %s wasn't changed\n", map[j].name);
        return false;
      }
      printf("%s = %d, set by '%s'\n", name_list[i], *var, map[j].name);
    }
  }

  char *name = "Kumquat";
  short value = SORT_THREADS;
  VarKumquat = -1;
  mutt_buffer_reset(err);
  rc = cs_str_native_set(cs, name, value, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarKumquat != value)
  {
    printf("Value of %s wasn't changed\n", name);
    return false;
  }

  printf("%s = %d, set to '%d'\n", name, VarKumquat, value);

  int invalid[] = { -1, 999 };
  for (unsigned int i = 0; i < mutt_array_size(invalid); i++)
  {
    VarKumquat = -1;
    mutt_buffer_reset(err);
    rc = cs_str_native_set(cs, name, invalid[i], err);
    if (CSR_RESULT(rc) != CSR_SUCCESS)
    {
      printf("Expected error: %s\n", err->data);
    }
    else
    {
      printf("%s = %d, set by '%d'\n", name, VarKumquat, invalid[i]);
      printf("This test should have failed\n");
      return false;
    }
  }

  name = "Damson";
  mutt_buffer_reset(err);
  rc = cs_str_native_set(cs, name, (SORT_DATE | SORT_LAST), err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarDamson != (SORT_DATE | SORT_LAST))
  {
    printf("Expected %d, got %d\n", (SORT_DATE | SORT_LAST), VarDamson);
    return false;
  }

  mutt_buffer_reset(err);
  rc = cs_str_native_set(cs, name, (SORT_SCORE | SORT_REVERSE), err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarDamson != (SORT_SCORE | SORT_REVERSE))
  {
    printf("Expected %d, got %d\n", (SORT_DATE | SORT_LAST), VarDamson);
    return false;
  }

  return true;
}

static bool test_native_get(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);
  char *name = "Lemon";

  VarLemon = SORT_THREADS;
  mutt_buffer_reset(err);
  intptr_t value = cs_str_native_get(cs, name, err);
  if (value != SORT_THREADS)
  {
    printf("Get failed: %s\n", err->data);
    return false;
  }
  printf("%s = %ld\n", name, value);

  return true;
}

static bool test_reset(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);

  char *name = "Mango";
  VarMango = SORT_SUBJECT;
  mutt_buffer_reset(err);

  int rc = cs_str_reset(cs, name, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarMango == SORT_SUBJECT)
  {
    printf("Value of %s wasn't changed\n", name);
    return false;
  }

  printf("Reset: %s = %d\n", name, VarMango);

  rc = cs_str_reset(cs, name, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("%s\n", err->data);
    return false;
  }

  name = "Nectarine";
  mutt_buffer_reset(err);

  printf("Initial: %s = %d\n", name, VarNectarine);
  dont_fail = true;
  rc = cs_str_string_set(cs, name, "size", err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
    return false;
  printf("Set: %s = %d\n", name, VarNectarine);
  dont_fail = false;

  rc = cs_str_reset(cs, name, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Expected error: %s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }

  if (VarNectarine != SORT_SIZE)
  {
    printf("Value of %s changed\n", name);
    return false;
  }

  printf("Reset: %s = %d\n", name, VarNectarine);

  return true;
}

static bool test_validator(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);

  char *name = "Olive";
  VarOlive = SORT_SUBJECT;
  mutt_buffer_reset(err);
  int rc = cs_str_string_set(cs, name, "threads", err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }
  printf("String: %s = %d\n", name, VarOlive);

  VarOlive = SORT_SUBJECT;
  mutt_buffer_reset(err);
  rc = cs_str_native_set(cs, name, SORT_THREADS, err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }
  printf("Native: %s = %d\n", name, VarOlive);

  name = "Papaya";
  VarPapaya = SORT_SUBJECT;
  mutt_buffer_reset(err);
  rc = cs_str_string_set(cs, name, "threads", err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }
  printf("String: %s = %d\n", name, VarPapaya);

  VarPapaya = SORT_SUBJECT;
  mutt_buffer_reset(err);
  rc = cs_str_native_set(cs, name, SORT_THREADS, err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }
  printf("Native: %s = %d\n", name, VarPapaya);

  name = "Quince";
  VarQuince = SORT_SUBJECT;
  mutt_buffer_reset(err);
  rc = cs_str_string_set(cs, name, "threads", err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Expected error: %s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }
  printf("String: %s = %d\n", name, VarQuince);

  VarQuince = SORT_SUBJECT;
  mutt_buffer_reset(err);
  rc = cs_str_native_set(cs, name, SORT_THREADS, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Expected error: %s\n", err->data);
  }
  else
  {
    printf("%s\n", err->data);
    return false;
  }
  printf("Native: %s = %d\n", name, VarQuince);

  return true;
}

static void dump_native(struct ConfigSet *cs, const char *parent, const char *child)
{
  intptr_t pval = cs_str_native_get(cs, parent, NULL);
  intptr_t cval = cs_str_native_get(cs, child, NULL);

  printf("%15s = %ld\n", parent, pval);
  printf("%15s = %ld\n", child, cval);
}

static bool test_inherit(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);
  bool result = false;

  const char *account = "fruit";
  const char *parent = "Strawberry";
  char child[128];
  snprintf(child, sizeof(child), "%s:%s", account, parent);

  const char *AccountVarStr[] = {
    parent, NULL,
  };

  struct Account *ac = ac_create(cs, account, AccountVarStr);

  // set parent
  VarStrawberry = SORT_SUBJECT;
  mutt_buffer_reset(err);
  int rc = cs_str_string_set(cs, parent, "threads", err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Error: %s\n", err->data);
    goto ti_out;
  }
  dump_native(cs, parent, child);

  // set child
  mutt_buffer_reset(err);
  rc = cs_str_string_set(cs, child, "score", err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Error: %s\n", err->data);
    goto ti_out;
  }
  dump_native(cs, parent, child);

  // reset child
  mutt_buffer_reset(err);
  rc = cs_str_reset(cs, child, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Error: %s\n", err->data);
    goto ti_out;
  }
  dump_native(cs, parent, child);

  // reset parent
  mutt_buffer_reset(err);
  rc = cs_str_reset(cs, parent, err);
  if (CSR_RESULT(rc) != CSR_SUCCESS)
  {
    printf("Error: %s\n", err->data);
    goto ti_out;
  }
  dump_native(cs, parent, child);

  result = true;
ti_out:
  ac_free(cs, &ac);
  return result;
}

static bool test_sort_type(struct ConfigSet *cs, struct Buffer *err)
{
  log_line(__func__);

  const char *name = "Raspberry";
  char *value = "alpha";

  mutt_buffer_reset(err);
  printf("Expect error for next test\n");
  int rc = cs_str_string_set(cs, name, value, err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s = %d, set by '%s'\n", name, VarRaspberry, value);
    printf("This test should have failed\n");
    return false;
  }

  mutt_buffer_reset(err);
  printf("Expect error for next test\n");
  rc = cs_str_native_set(cs, name, SORT_THREADS, err);
  if (CSR_RESULT(rc) == CSR_SUCCESS)
  {
    printf("%s = %d, set by %d\n", name, VarRaspberry, SORT_THREADS);
    printf("This test should have failed\n");
    return false;
  }

  return true;
}

bool sort_test(void)
{
  log_line(__func__);

  struct Buffer err;
  mutt_buffer_init(&err);
  err.data = mutt_mem_calloc(1, STRING);
  err.dsize = STRING;
  mutt_buffer_reset(&err);

  struct ConfigSet *cs = cs_create(30);

  sort_init(cs);
  dont_fail = true;
  if (!cs_register_variables(cs, Vars, 0))
    return false;
  dont_fail = false;

  cs_add_listener(cs, log_listener);

  set_list(cs);

  /* Register a broken variable separately */
  if (!cs_register_variables(cs, Vars2, 0))
    return false;

  if (!test_initial_values(cs, &err))
    return false;
  if (!test_string_set(cs, &err))
    return false;
  if (!test_string_get(cs, &err))
    return false;
  if (!test_native_set(cs, &err))
    return false;
  if (!test_native_get(cs, &err))
    return false;
  if (!test_reset(cs, &err))
    return false;
  if (!test_validator(cs, &err))
    return false;
  if (!test_inherit(cs, &err))
    return false;
  if (!test_sort_type(cs, &err))
    return false;

  cs_free(&cs);
  FREE(&err.data);

  return true;
}