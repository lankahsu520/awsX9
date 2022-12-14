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
#ifndef __AWSX9_H__
#define __AWSX9_H__

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h> 
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>

#include "utilx9.h"

#ifdef __cplusplus
extern "C" {
#endif


//******************************************************************************
//** include **
//******************************************************************************

//******************************************************************************
//** define **
//******************************************************************************


//** dynamodb **
#define DYDB_CTX_CHECK_ALL(ptr) \
	({ int __ret = 0; \
		if ( ( ptr->dydb_cli == NULL ) \
			|| ( ptr->table_name == NULL ) \
			|| ( ptr->pk == NULL ) \
			|| ( ptr->pk_val == NULL ) \
			|| ( ptr->sk == NULL ) \
			|| ( ptr->sk_val == NULL ) )  \
		{ \
			DBG_ER_LN("Null Definition !!! (dydb_cli: %p, table_name: %p, pk: %p, pk_val: %p, sk: %p, sk_val: %p)", dydb_ctx->dydb_cli, dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val); \
			__ret = -1; \
		} \
		__ret; \
	})
	
#define DYDB_CTX_CHECK_PK(ptr) \
	({ int __ret = 0; \
		if ( ( ptr->dydb_cli == NULL ) \
			|| ( ptr->table_name == NULL ) \
			|| ( ptr->pk == NULL ) \
			|| ( ptr->pk_val == NULL ) ) \
		{ \
			DBG_ER_LN("Null Definition !!! (dydb_cli: %p, table_name: %p, pk: %p, pk_val: %p)", dydb_ctx->dydb_cli, dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val); \
			__ret = -1; \
		} \
		__ret; \
	})

#define DYDB_CTX_CHECK_TABLE(ptr) \
	({ int __ret = 0; \
		if ( ( ptr->dydb_cli == NULL ) \
			|| ( ptr->table_name == NULL ) ) \
		{ \
			DBG_ER_LN("Null Definition !!! (dydb_cli: %p, table_name: %p)", dydb_ctx->dydb_cli, dydb_ctx->table_name); \
			__ret = -1; \
		} \
		__ret; \
	})

typedef enum
{
	DyDB_ACT_ID_ADD,
	DyDB_ACT_ID_DEL,
	DyDB_ACT_ID_MAX,
} DyDB_ACT_ID;

typedef struct DyDB_TableX_Struct
{
	void* next;

	Aws::String name;
} DyDB_TableX_t;

typedef struct DyDB_AttrX_Struct
{
	void* next;

	Aws::String name;
	Aws::DynamoDB::Model::AttributeValue attr;
} DyDB_AttrX_t;

typedef struct DyDB_ItemX_Struct
{
	void* next;

	CLIST_STRUCT(clistAttrX);
} DyDB_ItemX_t;

typedef struct DyDB_InfoX_STRUCT
{
	Aws::DynamoDB::DynamoDBClient *dydb_cli;

	const char *table_name;
	const char *pk;
	const char *pk_val;
	const char *sk;
	const char *sk_val;

	CLIST_STRUCT(clistTableX);
	Aws::DynamoDB::Model::TableDescription tableDesc;

	size_t attr_size; // pk + sk + AttrX
	const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> *mapAttr;
	CLIST_STRUCT(clistAttrX);

	size_t items_size;
	const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> *vectorMapAttr;
	CLIST_STRUCT(clistItemX);
} DyDB_InfoX_t;


//******************************************************************************
//** function **
//******************************************************************************

void dydb_show_attr(Aws::String& name, Aws::DynamoDB::Model::AttributeValue *attr);
void dydb_show_attrX(DyDB_InfoX_t *dydb_ctx);
void dydb_show_itemX(DyDB_InfoX_t *dydb_ctx);
void dydb_show_table(const Aws::DynamoDB::Model::TableDescription tableDesc);
void dydb_show_tableX(DyDB_InfoX_t *dydb_ctx);

int dydb_create_table(DyDB_InfoX_t *dydb_ctx);
int dydb_delete_table(DyDB_InfoX_t *dydb_ctx);
int dydb_describe_table(DyDB_InfoX_t *dydb_ctx);
int dydb_list_table(DyDB_InfoX_t *dydb_ctx);

int dydb_del_item(DyDB_InfoX_t *dydb_ctx);
int dydb_get_item(DyDB_InfoX_t *dydb_ctx);
int dydb_put_item(DyDB_InfoX_t *dydb_ctx);
int dydb_query_item(DyDB_InfoX_t *dydb_ctx);
int dydb_remove_attributes(DyDB_InfoX_t *dydb_ctx, char *attributes);
int dydb_scan_item(DyDB_InfoX_t *dydb_ctx);
int dydb_update_item(DyDB_InfoX_t *dydb_ctx);

void dydb_ctx_tableX_free(DyDB_InfoX_t *dydb_ctx);

void dydb_ctx_attrX_addS(DyDB_InfoX_t *dydb_ctx, char *key, char *value);
void dydb_ctx_attrX_addN(DyDB_InfoX_t *dydb_ctx, char *key, int value);
void dydb_ctx_attrX_addL_with_composeS(DyDB_InfoX_t *dydb_ctx, char *key, char *value);
void dydb_ctx_attrX_free(DyDB_InfoX_t *dydb_ctx);

void dydb_ctx_itemX_free(DyDB_InfoX_t *dydb_ctx);

void dydb_ctx_free(DyDB_InfoX_t *dydb_ctx);
void dydb_ctx_init(DyDB_InfoX_t *dydb_ctx, Aws::DynamoDB::DynamoDBClient *dydb_cli);

Aws::DynamoDB::DynamoDBClient *awsX_cli_get(void);
void awsX_free(void);
void awsX_init(void);

#ifdef __cplusplus
}
#endif

#endif
