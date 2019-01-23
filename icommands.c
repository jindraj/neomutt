/**
 * @file
 * Information commands
 *
 * @authors
 * Copyright (C) 2016 Christopher John Czettel <chris@meicloud.at>
 * Copyright (C) 2018 Richard Russon <rich@flatcap.org>
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
#include <limits.h>
#include <stdio.h>
#include "mutt/mutt.h"
#include "config/lib.h"
#include "mutt.h"
#include "icommands.h"
#include "globals.h"
#include "muttlib.h"
#include "pager.h"
#include "summary.h"
#include "version.h"

// clang-format off
static enum CommandResult icmd_bind(struct     Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_color(struct    Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_help(struct     Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_messages(struct Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_quit(struct     Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_scripts(struct  Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_set(struct      Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_test(struct     Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_vars(struct     Buffer *, struct Buffer *, unsigned long, struct Buffer *);
static enum CommandResult icmd_version(struct  Buffer *, struct Buffer *, unsigned long, struct Buffer *);

/**
 * ICommandList - All available informational commands
 *
 * @note These commands take precendence over conventional mutt rc-lines
 */
const struct ICommand ICommandList[] = {
  { "bind",     icmd_bind,     0 },
  { "color",    icmd_color,    0 },
  { "help",     icmd_help,     0 },
  { "macro",    icmd_bind,     0 },
  { "messages", icmd_messages, 0 },
  { "q!",       icmd_quit,     0 },
  { "q",        icmd_quit,     0 },
  { "qa",       icmd_quit,     0 },
  { "quit",     icmd_quit,     0 },
  { "scripts",  icmd_scripts,  0 },
  { "set",      icmd_set,      0 },
  { "test",     icmd_test,     0 },
  { "vars",     icmd_vars,     0 },
  { "version",  icmd_version,  0 },
  { NULL,       NULL,          0 },
};
// clang-format on

/**
 * mutt_parse_icommand - Parse an informational command
 * @param line Command to execute
 * @param err  Buffer for error messages
 * @retval #MUTT_CMD_SUCCESS Success
 * @retval #MUTT_CMD_ERROR   Error (no message): command not found
 * @retval #MUTT_CMD_ERROR   Error with message: command failed
 * @retval #MUTT_CMD_WARNING Warning with message: command failed
 */
enum CommandResult mutt_parse_icommand(/* const */ char *line, struct Buffer *err)
{
  if (!line || !*line || !err)
    return MUTT_CMD_ERROR;

  enum CommandResult rc = MUTT_CMD_ERROR;

  struct Buffer expn, token;

  mutt_buffer_init(&expn);
  mutt_buffer_init(&token);

  expn.data = expn.dptr = line;
  expn.dsize = mutt_str_strlen(line);

  mutt_buffer_reset(err);

  SKIPWS(expn.dptr);
  while (*expn.dptr)
  {
    /* TODO: contemplate implementing a icommand specific tokenizer */
    mutt_extract_token(&token, &expn, 0);
    for (size_t i = 0; ICommandList[i].name; i++)
    {
      if (mutt_str_strcmp(token.data, ICommandList[i].name) != 0)
        continue;

      rc = ICommandList[i].func(&token, &expn, ICommandList[i].data, err);
      if (rc != 0)
        goto finish;

      break; /* Continue with next command */
    }
  }

finish:
  if (expn.destroy)
    FREE(&expn.data);
  return rc;
}

/*
 *  wrapper functions to prepare and call other functionality within mutt
 *  see icmd_quit and icmd_help for easy examples
 */
static enum CommandResult icmd_quit(struct Buffer *buf, struct Buffer *s,
                                    unsigned long data, struct Buffer *err)
{
  /* TODO: exit more gracefully */
  mutt_exit(0);
  return MUTT_CMD_SUCCESS;
}

static enum CommandResult icmd_help(struct Buffer *buf, struct Buffer *s,
                                    unsigned long data, struct Buffer *err)
{
  /* TODO: implement ':help' command as suggested by flatcap in #162 */
  mutt_buffer_addstr(err, _("Not implemented yet."));
  return MUTT_CMD_ERROR;
}

static enum CommandResult icmd_test(struct Buffer *buf, struct Buffer *s,
                                    unsigned long data, struct Buffer *err)
{
  mutt_summary();
  return MUTT_CMD_SUCCESS;
}

static enum CommandResult icmd_bind(struct Buffer *buf, struct Buffer *s,
                                    unsigned long data, struct Buffer *err)
{
  /* TODO: implement ':bind' and ':macro' command as suggested by flatcap in #162 */
  mutt_buffer_addstr(err, _("Not implemented yet."));
  return MUTT_CMD_ERROR;
}

static enum CommandResult icmd_color(struct Buffer *buf, struct Buffer *s,
                                     unsigned long data, struct Buffer *err)

{
  /* TODO: implement ':color' command as suggested by flatcap in #162 */
  mutt_buffer_addstr(err, _("Not implemented yet."));
  return MUTT_CMD_ERROR;
}

static enum CommandResult icmd_messages(struct Buffer *buf, struct Buffer *s,
                                        unsigned long data, struct Buffer *err)
{
  /* TODO: implement ':messages' command as suggested by flatcap in #162 */
  mutt_buffer_addstr(err, _("Not implemented yet."));
  return MUTT_CMD_ERROR;
}

static enum CommandResult icmd_scripts(struct Buffer *buf, struct Buffer *s,
                                       unsigned long data, struct Buffer *err)
{
  /* TODO: implement ':scripts' command as suggested by flatcap in #162 */
  mutt_buffer_addstr(err, _("Not implemented yet."));
  return MUTT_CMD_ERROR;
}

static enum CommandResult icmd_vars(struct Buffer *buf, struct Buffer *s,
                                    unsigned long data, struct Buffer *err)
{
  /* TODO: implement ':vars' command as suggested by flatcap in #162 */
  mutt_buffer_addstr(err, _("Not implemented yet."));
  return MUTT_CMD_ERROR;
}

/**
 * icmd_set - Parse 'set' command to display config - Implements ::icommand_t
 */
static enum CommandResult icmd_set(struct Buffer *buf, struct Buffer *s,
                                   unsigned long data, struct Buffer *err)
{
  char tempfile[PATH_MAX];
  mutt_mktemp(tempfile, sizeof(tempfile));

  FILE *fpout = mutt_file_fopen(tempfile, "w");
  if (!fpout)
  {
    mutt_buffer_addstr(err, _("Could not create temporary file"));
    return MUTT_CMD_ERROR;
  }

  if (mutt_str_strcmp(s->data, "set all") == 0)
  {
    dump_config(Config, CS_DUMP_STYLE_NEO, 0, fpout);
  }
  else if (mutt_str_strcmp(s->data, "set") == 0)
  {
    dump_config(Config, CS_DUMP_STYLE_NEO, CS_DUMP_ONLY_CHANGED, fpout);
  }
  else
  {
    mutt_file_fclose(&fpout);
    return MUTT_CMD_ERROR;
  }

  fflush(fpout);
  mutt_file_fclose(&fpout);

  struct Pager info = { 0 };
  if (mutt_pager("set", tempfile, 0, &info) == -1)
  {
    mutt_buffer_addstr(err, _("Could not create temporary file"));
    return MUTT_CMD_ERROR;
  }

  return MUTT_CMD_SUCCESS;
}

/**
 * icmd_version - Parse 'version' command - Implements ::icommand_t
 */
static enum CommandResult icmd_version(struct Buffer *buf, struct Buffer *s,
                                       unsigned long data, struct Buffer *err)
{
  char tempfile[PATH_MAX];
  mutt_mktemp(tempfile, sizeof(tempfile));

  FILE *fpout = mutt_file_fopen(tempfile, "w");
  if (!fpout)
  {
    mutt_buffer_addstr(err, _("Could not create temporary file"));
    return MUTT_CMD_ERROR;
  }

  print_version(fpout);
  fflush(fpout);
  mutt_file_fclose(&fpout);

  struct Pager info = { 0 };
  if (mutt_pager("version", tempfile, 0, &info) == -1)
  {
    mutt_buffer_addstr(err, _("Could not create temporary file"));
    return MUTT_CMD_ERROR;
  }

  return MUTT_CMD_SUCCESS;
}
