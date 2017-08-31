/*
 * drivers/pcmcia/pxa2xx_base.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
int pxa2xx_drv_pcmcia_add_one(struct soc_pcmcia_socket *skt);
void pxa2xx_drv_pcmcia_ops(struct pcmcia_low_level *ops);
void pxa2xx_configure_sockets(struct device *dev);

