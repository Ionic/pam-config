/* Copyright (C) 2006 Thorsten Kukuk
   Author: Thorsten Kukuk <kukuk@thkukuk.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "pam-config.h"

int
write_config_account (const char *file, config_file_t *conf)
{
  FILE *fp;

  if (debug)
    printf ("*** write_config_account (%s, ...)\n", file);

  /* XXX rename () */

  fp = fopen(file, "w");
  if (fp == NULL)
    {
      fprintf (stderr, _("Cannot create %s: %m\n"),
	       file);
      return -1;
    }

  fprintf (fp, "#%%PAM-1.0\n#\n");
  fprintf (fp, "# This file is autogenerated by pam-config. All changes\n");
  fprintf (fp, "# will be overwritten.\n#\n");
  fprintf (fp, "# Account-related modules common to all services\n#\n");
  fprintf (fp,
	   "# This file is included from other service-specific PAM config files,\n"
	   "# and should contain a list of the accountorization modules that define\n"
	   "# the central access policy for use on the system.  The default is to\n"
	   "# only deny service to users whose accounts are expired.\n#\n");

  if (conf->use_krb5 || conf->use_ldap || conf->use_lum)
    fprintf (fp, "account\trequisite\tpam_unix2.so\t");
  else
    fprintf (fp, "account\trequired\tpam_unix2.so\t");
  if (conf->unix2_debug)
    fprintf (fp, "debug ");
  if (conf->unix2_call_modules)
    fprintf (fp, "call_modules=%s ", conf->unix2_call_modules);
  fprintf (fp, "\n");

  if (conf->use_krb5)
    {
      if (conf->use_ldap || conf->use_lum)
	fprintf (fp, "account\trequisite\tpam_krb5.so\tuse_first_pass ");
      else
	fprintf (fp, "account\trequired\tpam_krb5.so\tuse_first_pass ");
      if (conf->krb5_debug)
        fprintf (fp, "debug ");
      fprintf (fp, "\n");
    }

  if (conf->use_ldap || conf->use_lum)
    fprintf (fp, "account\tsufficient\tpam_localuser.so\n");

  if (conf->use_ldap)
    {
      fprintf (fp, "account\trequired\tpam_ldap.so\tuse_first_pass ");
      if (conf->ldap_debug)
        fprintf (fp, "debug ");
      fprintf (fp, "\n");
    }

  if (conf->use_lum)
    fprintf (fp, "account\trequired\tpam_nam.so\tuse_first_pass\n");

  fclose (fp);

  return 0;
}