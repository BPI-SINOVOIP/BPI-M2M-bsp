/*
 * security/selinux/ss/services.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Implementation of the security services.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
#ifndef _SS_SERVICES_H_
#define _SS_SERVICES_H_

#include "policydb.h"
#include "sidtab.h"

extern struct policydb policydb;

void services_compute_xperms_drivers(struct extended_perms *xperms,
				struct avtab_node *node);

void services_compute_xperms_decision(struct extended_perms_decision *xpermd,
					struct avtab_node *node);

#endif	/* _SS_SERVICES_H_ */

