/*
 * video1394.h - driver for OHCI 1394 boards
 * Copyright (C)1999,2000 Sebastien Rougeaux <sebastien.rougeaux@anu.edu.au>
 *                        Peter Schlaile <udbz@rz.uni-karlsruhe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _VIDEO_1394_H
#define _VIDEO_1394_H

#include <sys/time.h>
#include "cvd_src/Linux/kernel2.6-video1394-ioctl.h"

#define VIDEO1394_LISTEN_CHANNEL VIDEO1394_IOC_LISTEN_CHANNEL
#define VIDEO1394_UNLISTEN_CHANNEL VIDEO1394_IOC_UNLISTEN_CHANNEL
#define VIDEO1394_LISTEN_WAIT_BUFFER VIDEO1394_IOC_LISTEN_WAIT_BUFFER
#define VIDEO1394_LISTEN_POLL_BUFFER VIDEO1394_IOC_LISTEN_POLL_BUFFER
#define VIDEO1394_LISTEN_QUEUE_BUFFER VIDEO1394_IOC_LISTEN_QUEUE_BUFFER


#define VIDEO1394_DRIVER_NAME "video1394"

#define VIDEO1394_MAX_SIZE 0x4000000

enum {
	VIDEO1394_BUFFER_FREE = 0,
	VIDEO1394_BUFFER_QUEUED,
	VIDEO1394_BUFFER_READY
};


/* enum { */
/* 	VIDEO1394_LISTEN_CHANNEL = 0, */
/* 	VIDEO1394_UNLISTEN_CHANNEL, */
/* 	VIDEO1394_LISTEN_QUEUE_BUFFER, */
/* 	VIDEO1394_LISTEN_WAIT_BUFFER,  // wait until buffer is ready */
/* 	VIDEO1394_TALK_CHANNEL, */
/* 	VIDEO1394_UNTALK_CHANNEL, */
/* 	VIDEO1394_TALK_QUEUE_BUFFER, */
/* 	VIDEO1394_TALK_WAIT_BUFFER, */
/* 	VIDEO1394_LISTEN_POLL_BUFFER   // return immediately with -EINTR if not ready */
/* }; */


#define VIDEO1394_SYNC_FRAMES          0x00000001
#define VIDEO1394_INCLUDE_ISO_HEADERS  0x00000002
#define VIDEO1394_VARIABLE_PACKET_SIZE 0x00000004

struct video1394_mmap {
	int channel;			/* -1 to find an open channel in LISTEN/TALK */
	unsigned int sync_tag;
	unsigned int nb_buffers;
	unsigned int buf_size;
	unsigned int packet_size; /* For VARIABLE_PACKET_SIZE: 
				     Maximum packet size */
	unsigned int fps;
	unsigned int syt_offset;
	unsigned int flags;
};

/* For TALK_QUEUE_BUFFER with VIDEO1394_VARIABLE_PACKET_SIZE use */
struct video1394_queue_variable {
	unsigned int channel;
	unsigned int buffer;
	unsigned int* packet_sizes; /* Buffer of size:  
				       buf_size / packet_size  */
};

struct video1394_wait {
	unsigned int channel;
	unsigned int buffer;
	struct timeval filltime;	/* time of buffer full */
};


#endif