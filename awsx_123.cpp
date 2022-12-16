/***************************************************************************
 * Copyright (C) 2022, Lanka Hsu, <lankahsu@gmail.com>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include <iostream>

#include <signal.h>
#include <getopt.h>

#include "awsx9.h"

#define TAG "awsBowser"

#define USB_AWSX9

// ** app **
static int is_quit = 0;

static int app_quit(void)
{
	return is_quit;
}

static void app_set_quit(int mode)
{
	is_quit = mode;
}

#ifdef USB_AWSX9

#define DYDB_TABLE_NAME_MUSIC "Music"
#define DYDB_PK_NAME_ARTIST "Artist"
#define DYDB_PK_VAL_LANKA "Lanka"
#define DYDB_PK_VAL_ACME_BAND "Acme Band"

#define DYDB_SK_NAME_SONG_TITLE "SongTitle"
#define DYDB_SK_VAL_LANKA "Lanka"
#define DYDB_SK_VAL_HAPPY_DAY "Happy Day"

DyDB_InfoX_t dydb_t_Music = {
	.dydb_cli = NULL,
	.table_name = DYDB_TABLE_NAME_MUSIC,
	.pk = DYDB_PK_NAME_ARTIST,
	.pk_val = DYDB_PK_VAL_LANKA,
	.sk = DYDB_SK_NAME_SONG_TITLE,
	.sk_val = DYDB_SK_VAL_LANKA,
};

#define DYDB_TABLE_NAME_DEMO "Demo"
#define DYDB_PK_NAME_PK "PK"
#define DYDB_SK_NAME_SK "SK"

DyDB_InfoX_t dydb_t_Demo = {
	.dydb_cli = NULL,
	.table_name = DYDB_TABLE_NAME_DEMO,
	.pk = DYDB_PK_NAME_PK,
	.pk_val = DYDB_PK_VAL_LANKA,
	.sk = DYDB_SK_NAME_SK ,
	.sk_val = DYDB_SK_VAL_HAPPY_DAY,
};

static void aws_open(void)
{
#if (1)
	{
		DyDB_InfoX_t *dydb_ctx = &dydb_t_Music;
#if (1)
		DBG_WN_LN(">>>>> dydb_del_item <<<<<");
		{
			//** del_item **
			dydb_del_item(dydb_ctx);
		}
#endif

#if (1)
		DBG_WN_LN(">>>>> dydb_put_item <<<<<");
		{
			//** put_item **
			dydb_ctx_attrX_free(dydb_ctx);

			dydb_ctx_attrX_addS(dydb_ctx, (char*)"AlbumTitle", (char*)"Album123");

			dydb_ctx_attrX_addS(dydb_ctx, (char*)"Awards", (char*)"1");

			char sponsor_val[LEN_OF_VAL128] = "dog:mouse:tiger";
			dydb_ctx_attrX_addL_with_composeS(dydb_ctx, (char*)"Sponsor", sponsor_val);

			dydb_put_item(dydb_ctx);
		}
		{
			dydb_get_item(dydb_ctx);
			dydb_show_attrX(dydb_ctx);
		}
#endif

#if (1)
		DBG_WN_LN(">>>>> dydb_update_item <<<<<");
		{
			dydb_ctx_attrX_free(dydb_ctx);

			dydb_ctx_attrX_addS(dydb_ctx, (char*)"Awards", (char*)"1");
			dydb_ctx_attrX_addN(dydb_ctx, (char*)"garbage", 4567);

			dydb_update_item(dydb_ctx);
		}
		{
			dydb_get_item(dydb_ctx);
			dydb_show_attrX(dydb_ctx);
		}
#endif

#if (1)
		DBG_WN_LN(">>>>> dydb_remove_attributes <<<<<");
		{
			char attributes[LEN_OF_VAL1024] = "garbage";
			dydb_remove_attributes(dydb_ctx, attributes);
			dydb_get_item(dydb_ctx);
			dydb_show_attrX(dydb_ctx);
		}
#endif

#if (1)
		DBG_WN_LN(">>>>> dydb_query_item <<<<<");
		{
			dydb_query_item(dydb_ctx);
			dydb_show_itemX(dydb_ctx);
		}
#endif

#if (1)
		DBG_WN_LN(">>>>> dydb_scan_item <<<<<");
		{
			dydb_scan_item(dydb_ctx);
			dydb_show_itemX(dydb_ctx);
		}
#endif
	}
#endif

#if (1)
	{
		DyDB_InfoX_t *dydb_ctx = &dydb_t_Demo;
#if (1)
		dydb_list_table(dydb_ctx);
		dydb_show_tableX(dydb_ctx);

		DBG_WN_LN(">>>>> dydb_create_table <<<<<");
		dydb_create_table(dydb_ctx);

		dydb_list_table(dydb_ctx);
		dydb_show_tableX(dydb_ctx);
#endif

		DBG_WN_LN(">>>>> dydb_describe_table <<<<<");
		dydb_describe_table(dydb_ctx);
		dydb_show_table(dydb_ctx->tableDesc);

#if (1)
		DBG_WN_LN(">>>>> dydb_delete_table <<<<<");
		dydb_delete_table(dydb_ctx);

		dydb_list_table(dydb_ctx);
		dydb_show_tableX(dydb_ctx);
#endif
	}
#endif
}

static void aws_free(void)
{
	dydb_ctx_free(&dydb_t_Music);
	dydb_ctx_free(&dydb_t_Demo);

	awsX_free();
}

static void aws_init(void)
{
	awsX_init();

	dydb_ctx_init(&dydb_t_Music, awsX_cli_get());
	dydb_ctx_init(&dydb_t_Demo, awsX_cli_get());
}
#endif

static void app_stop(void)
{
	if (app_quit()==0)
	{
		app_set_quit(1);

#ifdef USB_AWSX9
		aws_free();
#endif
	}
}

static void app_loop(void)
{
#ifdef USB_AWSX9
	aws_open();
#endif
}

static int app_init(void)
{
	int ret = 0;

	return ret;
}

static void app_exit(void)
{
	app_stop();
}

static void app_signal_handler(int signum)
{
	DBG_ER_LN("(signum: %d)", signum);
	switch (signum)
	{
		case SIGINT:
		case SIGTERM:
		case SIGHUP:
			app_stop();
			break;
		case SIGPIPE:
			break;

		case SIGUSR1:
			break;

		case SIGUSR2:
			dbg_lvl_round();
			DBG_ER_LN("dbg_lvl_get(): %d", dbg_lvl_get());
			DBG_ER_LN("(Version: %s)", version_show());
			break;
	}
}

static void app_signal_register(void)
{
	signal(SIGINT, app_signal_handler );
	signal(SIGTERM, app_signal_handler );
	signal(SIGHUP, app_signal_handler );
	signal(SIGUSR1, app_signal_handler );
	signal(SIGUSR2, app_signal_handler );

	signal(SIGPIPE, SIG_IGN );
}

int option_index = 0;
const char* short_options = "d:h";
static struct option long_options[] =
{
	{ "debug",       required_argument,   NULL,    'd'  },
	{ "help",        no_argument,         NULL,    'h'  },
	{ 0,             0,                      0,    0    }
};

static void app_showusage(int exit_code)
{
	printf( "Usage: %s\n"
					"  -d, --debug       debug level\n"
					"  -h, --help\n", TAG);
	printf( "Version: %s\n", version_show());
	printf( "Example:\n"
					"  %s -d 4\n", TAG);
	exit(exit_code);
}

static void app_ParseArguments(int argc, char **argv)
{
	int opt;

	while((opt = getopt_long (argc, argv, short_options, long_options, &option_index)) != -1)
	{
		switch (opt)
		{
			case 'd':
				if (optarg)
				{
					dbg_lvl_set(atoi(optarg));
				}
				break;
			default:
				app_showusage(-1);
				break;
		}
	}
}

int main(int argc, char** argv)
{
#ifdef USB_AWSX9
	// must run before app_ParseArguments
	aws_init();
#endif

	app_ParseArguments(argc, argv);
	app_signal_register();
	atexit(app_exit);

	if ( app_init() == -1 )
	{
		return -1;
	}

	app_loop();

	DBG_WN_LN(DBG_TXT_BYE_BYE);
	return 0;
}
