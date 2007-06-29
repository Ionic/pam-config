
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pam-config.h"
#include "pam-module.h"

static int
parse_config_ccreds (pam_module_t *this, char *args, write_type_t type)
{
  option_set_t *opt_set = this->get_opt_set( this, type );

  if (debug)
    printf("**** parse_config_ccreds (%s): '%s'\n", type2string(type),
           args?args:"");

  opt_set->enable (opt_set, "is_enabled", TRUE);

  return 1;
}

static int
write_config_ccreds (pam_module_t *this __attribute__ ((unused)),
		     enum write_type op __attribute__ ((unused)),
		     FILE *fp __attribute__ ((unused)))
{
  if (debug)
    printf ("**** write_config_ccreds (...)\n");

  /* Don't do anything here, pam_krb5 and pam_ldap have to write
     this entries. */

  return 0;
}



/* ---- contruct module object ---- */
DECLARE_BOOL_OPTS_1( is_enabled );
DECLARE_STRING_OPTS_0;
DECLARE_OPT_SETS;
/* at last construct the complete module object */
pam_module_t mod_pam_ccreds = { "pam_ccreds.so", opt_sets,
			       &parse_config_ccreds,
			       &def_print_module,
			       &write_config_ccreds,
			       &get_opt_set};