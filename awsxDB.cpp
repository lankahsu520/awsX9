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

//#include <aws/core/Aws.h>
//#include <aws/core/utils/Outcome.h> 
//#include <aws/dynamodb/DynamoDBClient.h>
//#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>

#include <aws/dynamodb/model/DeleteTableRequest.h>

#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/ListTablesResult.h>

#include <aws/dynamodb/model/DeleteItemRequest.h>

#include <aws/dynamodb/model/GetItemRequest.h>

#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>

#include <aws/dynamodb/model/QueryRequest.h>

#include <aws/dynamodb/model/ScanRequest.h>

#include <aws/dynamodb/model/UpdateItemRequest.h>
#include <aws/dynamodb/model/UpdateItemResult.h>

#include "awsx9.h"

using namespace std;

void dydb_show_attr(Aws::String& name, Aws::DynamoDB::Model::AttributeValue *attr)
{
	if (attr)
	{
		Aws::Utils::Json::JsonValue jitem  = attr->Jsonize();
		Aws::DynamoDB::Model::ValueType atype = attr->GetType();
		//DBG_IF_LN("(cjson: %s)", cur->attr.SerializeAttribute().c_str() );
		switch (atype)
		{
			case Aws::DynamoDB::Model::ValueType::STRING:
				DBG_IF_LN("(STRING: %s, %s: %s)", jitem.View().WriteCompact().c_str(), name.c_str(), attr->GetS().c_str() );
				break;
			case Aws::DynamoDB::Model::ValueType::NUMBER:
				DBG_IF_LN("(NUMBER: %s, %s: %s)", jitem.View().WriteCompact().c_str(), name.c_str(), attr->GetN().c_str());
				break;
			case Aws::DynamoDB::Model::ValueType::BYTEBUFFER:
				DBG_IF_LN("(BYTEBUFFER: %s, %s: )", jitem.View().WriteCompact().c_str(), name.c_str());
				break;
			case Aws::DynamoDB::Model::ValueType::STRING_SET:
				DBG_IF_LN("(STRING_SET: %s, %s: )", jitem.View().WriteCompact().c_str(), name.c_str());
				break;
			case Aws::DynamoDB::Model::ValueType::NUMBER_SET:
				DBG_IF_LN("(NUMBER_SET: %s, %s: )", jitem.View().WriteCompact().c_str(), name.c_str());
				break;
			case Aws::DynamoDB::Model::ValueType::BYTEBUFFER_SET:
				DBG_IF_LN("(BYTEBUFFER_SET: %s, %s:)", jitem.View().WriteCompact().c_str(), name.c_str());
				break;
			case Aws::DynamoDB::Model::ValueType::ATTRIBUTE_MAP:
				DBG_IF_LN("(ATTRIBUTE_MAP: %s, %s: )", jitem.View().WriteCompact().c_str(), name.c_str());
				break;
			case Aws::DynamoDB::Model::ValueType::ATTRIBUTE_LIST:
				{
					int idx = 0;
					Aws::Vector<std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> attrVector = attr->GetL();

					DBG_IF_LN("(ATTRIBUTE_LIST: %s)", jitem.View().WriteCompact().c_str());
					for(auto elem : attrVector)
					{
						DBG_IF_LN("(%s[%d]: %s)", name.c_str(), idx, elem->GetS().c_str());
						idx ++;
					}
				}
				break;
			case Aws::DynamoDB::Model::ValueType::BOOL:
				DBG_IF_LN("(BOOL: %s, %s: %d)", jitem.View().WriteCompact().c_str(), name.c_str(), attr->GetBool());
				break;
			case Aws::DynamoDB::Model::ValueType::NULLVALUE:
				DBG_IF_LN("(NULLVALUE: %s, %s: NULLVALUE)", jitem.View().WriteCompact().c_str(), name.c_str());
				break;
			default:
				break;
		}
	}
}

void dydb_show_attrX(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		DyDB_AttrX_t *curAttrX = NULL;

		for (curAttrX = (DyDB_AttrX_t *)clist_head(dydb_ctx->clistAttrX); curAttrX != NULL; curAttrX = (DyDB_AttrX_t *)clist_item_next((void *)curAttrX))
		{
			dydb_show_attr(curAttrX->name, &curAttrX->attr);
		}
	}
}

void dydb_show_itemX(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		DyDB_ItemX_t *curItemX = NULL;

		DBG_IF_LN("——————————————————————————————————————————————————");
		int idx = 0;
		for (curItemX = (DyDB_ItemX_t *)clist_head(dydb_ctx->clistItemX); curItemX != NULL; curItemX = (DyDB_ItemX_t *)clist_item_next((void *)curItemX))
		{
			DBG_IF_LN("__________ %03d __________", idx++);

#if (1)
			DyDB_AttrX_t *curAttrX = NULL;
			for (curAttrX = (DyDB_AttrX_t *)clist_head(curItemX->clistAttrX); curAttrX != NULL; curAttrX = (DyDB_AttrX_t *)clist_item_next((void *)curAttrX))
			{
				dydb_show_attr(curAttrX->name, &curAttrX->attr);
			}
#endif
		}
		DBG_IF_LN("__________ End __________");
	}
}

void dydb_show_tableX(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		DyDB_TableX_t *curTableX = NULL;

		DBG_IF_LN("——————————————————————————————————————————————————");
		for (curTableX = (DyDB_TableX_t *)clist_head(dydb_ctx->clistTableX); curTableX != NULL; curTableX = (DyDB_TableX_t *)clist_item_next((void *)curTableX))
		{
			DBG_IF_LN("(tablename: %s)", curTableX->name.c_str());
		}
		DBG_IF_LN("__________ End __________");
	}
}

int dydb_create_table(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_ALL(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	dydb_ctx_attrX_free(dydb_ctx);

	Aws::DynamoDB::Model::CreateTableRequest dydb_create_table_req;

	// Set up the request.
	dydb_create_table_req.SetTableName(dydb_ctx->table_name);

	Aws::DynamoDB::Model::AttributeDefinition dydb_pk_attrDef, dydb_sk_attrDef;
	dydb_pk_attrDef.WithAttributeName(dydb_ctx->pk).WithAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
	dydb_create_table_req.AddAttributeDefinitions(dydb_pk_attrDef);
	dydb_sk_attrDef.WithAttributeName(dydb_ctx->sk).WithAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
	dydb_create_table_req.AddAttributeDefinitions(dydb_sk_attrDef);

	Aws::DynamoDB::Model::KeySchemaElement dydb_pk_schema, dydb_sk_schema;
	dydb_pk_schema.WithAttributeName(dydb_ctx->pk).WithKeyType(Aws::DynamoDB::Model::KeyType::HASH);
	dydb_create_table_req.AddKeySchema(dydb_pk_schema);
	dydb_sk_schema.WithAttributeName(dydb_ctx->sk).WithKeyType(Aws::DynamoDB::Model::KeyType::RANGE);
	dydb_create_table_req.AddKeySchema(dydb_sk_schema);

	Aws::DynamoDB::Model::ProvisionedThroughput dydb_provisioned_thruput;
	dydb_provisioned_thruput.WithReadCapacityUnits(5).WithWriteCapacityUnits(5);
	dydb_create_table_req.SetProvisionedThroughput(dydb_provisioned_thruput);

	dydb_ctx->attr_size = 0;
	const Aws::DynamoDB::Model::CreateTableOutcome& dydb_create_table_res = dydb_ctx->dydb_cli->CreateTable(dydb_create_table_req);
	if (dydb_create_table_res.IsSuccess())
	{
		DBG_IF_LN("CreateTable ok !!! (table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val);
	}
	else
	{
		DBG_ER_LN("CreateTable error - %s !!! (table_name: %s, %s: %s, %s: %s)", dydb_create_table_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}

	return ret;
}

int dydb_delete_table(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_TABLE(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s)", dydb_ctx->table_name );

	dydb_ctx_itemX_free(dydb_ctx);

	Aws::DynamoDB::Model::DeleteTableRequest dydb_delete_table_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_delete_table_req.SetTableName(dydb_ctx->table_name);

	const Aws::DynamoDB::Model::DeleteTableOutcome dydb_delete_table_res = dydb_ctx->dydb_cli->DeleteTable(dydb_delete_table_req);
	if (!dydb_delete_table_res.IsSuccess())
	{
		DBG_ER_LN("DeleteTable error - %s !!! (table_name: %s)", dydb_delete_table_res.GetError().GetMessage().c_str(), dydb_ctx->table_name);
		ret = -1;
	}
	else
	{
		DBG_IF_LN("DeleteTable ok !!! (table_name: %s)", dydb_ctx->table_name );
	}

	return ret;
}

int dydb_list_table(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	DBG_DB_LN( DBG_TXT_ENTER );

	dydb_ctx_tableX_free(dydb_ctx);

	Aws::DynamoDB::Model::ListTablesRequest dydb_list_table_req;

	// Set up the request.
	dydb_list_table_req.SetLimit(50);

	do
	{
		const Aws::DynamoDB::Model::ListTablesOutcome& dydb_list_table_res = dydb_ctx->dydb_cli->ListTables(dydb_list_table_req);
		if (!dydb_list_table_res.IsSuccess())
		{
			DBG_ER_LN("ListTables error - %s !!!", dydb_list_table_res.GetError().GetMessage().c_str());
			ret = -1;
			break; 
		}

		for (const auto& s : dydb_list_table_res.GetResult().GetTableNames())
		{
			DyDB_TableX_t *tableX = (DyDB_TableX_t*)calloc(1, sizeof(DyDB_TableX_t));
			tableX->name = s;
			clist_push(dydb_ctx->clistTableX, tableX);
		}
		dydb_list_table_req.SetExclusiveStartTableName(dydb_list_table_res.GetResult().GetLastEvaluatedTableName());
	} while (!dydb_list_table_req.GetExclusiveStartTableName().empty());

	return ret;
}

int dydb_del_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_ALL(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	dydb_ctx_attrX_free(dydb_ctx);

	Aws::DynamoDB::Model::DeleteItemRequest dydb_del_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_del_item_req.SetTableName(dydb_ctx->table_name);
	dydb_del_item_req.AddKey(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_del_item_req.AddKey(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	const Aws::DynamoDB::Model::DeleteItemOutcome& dydb_del_item_res = dydb_ctx->dydb_cli->DeleteItem(dydb_del_item_req);
	if (dydb_del_item_res.IsSuccess())
	{
		DBG_IF_LN("DeleteItem ok !!! (table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}
	else
	{
		DBG_ER_LN("DeleteItem error - %s !!! (table_name: %s, %s: %s, %s: %s)", dydb_del_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}

	return ret;
}

int dydb_get_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_ALL(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	dydb_ctx_attrX_free(dydb_ctx);

	Aws::DynamoDB::Model::GetItemRequest dydb_get_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_get_item_req.SetTableName(dydb_ctx->table_name);
	dydb_get_item_req.AddKey(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_get_item_req.AddKey(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	dydb_ctx->attr_size = 0;
	const Aws::DynamoDB::Model::GetItemOutcome& dydb_get_item_res = dydb_ctx->dydb_cli->GetItem(dydb_get_item_req);
	if (dydb_get_item_res.IsSuccess())
	{
		// Reference the retrieved fields/values.
		const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& mapAttr = dydb_get_item_res.GetResult().GetItem();
		dydb_ctx->mapAttr = &mapAttr;
		dydb_ctx->attr_size = dydb_ctx->mapAttr->size();
		if (dydb_ctx->mapAttr->size() > 0)
		{
			// Output each retrieved field and its value.
			for (const auto& i : *dydb_ctx->mapAttr)
			{
				DyDB_AttrX_t *attrX = (DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t));
				attrX->name = i.first;
				attrX->attr = i.second;
				clist_push(dydb_ctx->clistAttrX, attrX);

				if (0)
				{
					Aws::Utils::Json::JsonValue jitem  = i.second.Jsonize();
					//Aws::DynamoDB::Model::ValueType item_type = i.second.GetType();
					DBG_DB_LN("(cjson: %s, %s: %s)", jitem.View().WriteCompact().c_str(), i.first.c_str(), i.second.GetS().c_str() );
				}
			}
		}
		DBG_IF_LN("GetItem ok !!! (table_name: %s, %s: %s, %s: %s, attr_size: %zd)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val, dydb_ctx->attr_size );
	}
	else
	{
		DBG_ER_LN("GetItem error - %s !!! (table_name: %s, %s: %s, %s: %s)", dydb_get_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}

	return ret;
}

// create an item or replace all data
int dydb_put_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_ALL(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	Aws::DynamoDB::Model::PutItemRequest dydb_put_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_put_item_req.SetTableName(dydb_ctx->table_name);
	dydb_put_item_req.AddItem(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_put_item_req.AddItem(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	DyDB_AttrX_t *cur = NULL;

	for (cur = (DyDB_AttrX_t *)clist_head(dydb_ctx->clistAttrX); cur != NULL; cur = (DyDB_AttrX_t *)clist_item_next((void *)cur))
	{
		dydb_put_item_req.AddItem(cur->name, cur->attr);
	}

	const Aws::DynamoDB::Model::PutItemOutcome dydb_put_item_res = dydb_ctx->dydb_cli->PutItem(dydb_put_item_req);
	if (!dydb_put_item_res.IsSuccess())
	{
		DBG_ER_LN("PutItem error - %s !!! (table_name: %s , %s: %s, %s: %s)", dydb_put_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
		ret = -1;
	}
	else
	{
		DBG_IF_LN("PutItem ok !!! (table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}

	return ret;
}

int dydb_query_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_PK(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val );

	dydb_ctx_itemX_free(dydb_ctx);

	Aws::DynamoDB::Model::QueryRequest dydb_query_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_query_item_req.SetTableName(dydb_ctx->table_name);

	// Set query key condition expression
	dydb_query_item_req.SetKeyConditionExpression(Aws::String(dydb_ctx->pk) + "= :valueToMatch");

	// Set Expression AttributeValues
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> dydb_map_attr;
	dydb_map_attr.emplace(":valueToMatch", dydb_ctx->pk_val);
	dydb_query_item_req.SetExpressionAttributeValues(dydb_map_attr);

	const Aws::DynamoDB::Model::QueryOutcome dydb_query_item_res = dydb_ctx->dydb_cli->Query(dydb_query_item_req);
	if (!dydb_query_item_res.IsSuccess())
	{
		DBG_ER_LN("Query error - %s !!! (table_name: %s , %s: %s)", dydb_query_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val );
		ret = -1;
	}
	else
	{
		const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& vectorMapAttr = dydb_query_item_res.GetResult().GetItems();
		dydb_ctx->vectorMapAttr = &vectorMapAttr;
		dydb_ctx->items_size = dydb_ctx->vectorMapAttr->size();

		if ( dydb_ctx->items_size > 0 )
		{
			for (const auto& mapAttr : *dydb_ctx->vectorMapAttr)
			{
				DyDB_ItemX_t *itemX = (DyDB_ItemX_t*)calloc(1, sizeof(DyDB_ItemX_t));
				CLIST_STRUCT_INIT(itemX, clistAttrX);

				for (const auto& i : mapAttr)
				{
					DyDB_AttrX_t *attrX = (DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t));
					attrX->name = i.first;
					attrX->attr = i.second;
					clist_push(itemX->clistAttrX, attrX);

					if (0)
					{
						Aws::Utils::Json::JsonValue jitem  = i.second.Jsonize();
						//Aws::DynamoDB::Model::ValueType item_type = i.second.GetType();
						DBG_IF_LN("(cjson: %s, %s: %s)", jitem.View().WriteCompact().c_str(), i.first.c_str(), i.second.GetS().c_str() );
					}
				}
				clist_push(dydb_ctx->clistItemX, itemX);
			}
		}
		DBG_IF_LN("Query ok !!! (table_name: %s, %s: %s, items_size: %zd)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->items_size );
	}

	return ret;
}

// deleting attributes from an item
// https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Expressions.UpdateExpressions.html#Expressions.UpdateExpressions.REMOVE
int dydb_remove_attributes(DyDB_InfoX_t *dydb_ctx, char *attributes)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_ALL(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	Aws::DynamoDB::Model::UpdateItemRequest dydb_update_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_update_item_req.SetTableName(dydb_ctx->table_name);
	dydb_update_item_req.AddKey(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_update_item_req.AddKey(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	Aws::String dydb_update_expression("REMOVE " + Aws::String(attributes));
	dydb_update_item_req.SetUpdateExpression(dydb_update_expression);

	const Aws::DynamoDB::Model::UpdateItemOutcome dydb_update_item_res = dydb_ctx->dydb_cli->UpdateItem(dydb_update_item_req);
	if (!dydb_update_item_res.IsSuccess())
	{
		DBG_ER_LN("UpdateItem error - %s !!! (table_name: %s , %s: %s, %s: %s), (%s)", dydb_update_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val, dydb_update_expression.c_str() );
		ret = -1;
	}
	else
	{
		DBG_IF_LN("UpdateItem ok !!! (table_name: %s, %s: %s, %s: %s), (%s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val, dydb_update_expression.c_str() );
	}

	return ret;
}


int dydb_scan_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_TABLE(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s)", dydb_ctx->table_name );

	dydb_ctx_itemX_free(dydb_ctx);

	Aws::DynamoDB::Model::ScanRequest dydb_scan_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_scan_item_req.SetTableName(dydb_ctx->table_name);

	const Aws::DynamoDB::Model::ScanOutcome dydb_scan_item_res = dydb_ctx->dydb_cli->Scan(dydb_scan_item_req);
	if (!dydb_scan_item_res.IsSuccess())
	{
		DBG_ER_LN("Scan error - %s !!! (table_name: %s , %s: %s)", dydb_scan_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val );
		ret = -1;
	}
	else
	{
		const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& vectorMapAttr = dydb_scan_item_res.GetResult().GetItems();
		dydb_ctx->vectorMapAttr = &vectorMapAttr;
		dydb_ctx->items_size = dydb_ctx->vectorMapAttr->size();

		if (dydb_ctx->items_size > 0)
		{
			for (const auto& mapAttr : *dydb_ctx->vectorMapAttr)
			{
				DyDB_ItemX_t *itemX = (DyDB_ItemX_t*)calloc(1, sizeof(DyDB_ItemX_t));
				CLIST_STRUCT_INIT(itemX, clistAttrX);

				for (const auto& i : mapAttr)
				{
					DyDB_AttrX_t *attrX = (DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t));
					attrX->name = i.first;
					attrX->attr = i.second;
					clist_push(itemX->clistAttrX, attrX);

					if (0)
					{
						Aws::Utils::Json::JsonValue jitem  = i.second.Jsonize();
						//Aws::DynamoDB::Model::ValueType item_type = i.second.GetType();
						DBG_IF_LN("(cjson: %s, %s: %s)", jitem.View().WriteCompact().c_str(), i.first.c_str(), i.second.GetS().c_str() );
					}
				}
				clist_push(dydb_ctx->clistItemX, itemX);
			}
		}
		DBG_IF_LN("Scan ok !!! (table_name: %s, %s: %s, items_size: %zd)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->items_size );
	}

	return ret;
}

// create an item or update the current data
// https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Expressions.UpdateExpressions.html#Expressions.UpdateExpressions.SET
int dydb_update_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK_ALL(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	Aws::DynamoDB::Model::UpdateItemRequest dydb_update_item_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_update_item_req.SetTableName(dydb_ctx->table_name);
	dydb_update_item_req.AddKey(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_update_item_req.AddKey(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	DyDB_AttrX_t *cur = NULL;

	//Aws::String dydb_update_expression("SET #0=:value0, #1=:value1");
	Aws::String dydb_update_expression("SET");
	dydb_update_item_req.SetUpdateExpression(dydb_update_expression);

	Aws::Map<Aws::String, Aws::String> mapAttrKey;
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> mapAttr;

	int idx = 0;
	for (cur = (DyDB_AttrX_t *)clist_head(dydb_ctx->clistAttrX); cur != NULL; cur = (DyDB_AttrX_t *)clist_item_next((void *)cur))
	{
		Aws::String key( "#" + std::to_string(idx));
		Aws::String value( ":value" + std::to_string(idx) );

		if (idx==0)
		{
			dydb_update_expression.append( " " + key + "=" + value);
		}
		else
		{
			dydb_update_expression.append( "," + key + "=" + value);
		}

		mapAttrKey[ key ] = cur->name;
		mapAttr[ value ] = cur->attr;

		idx ++;
	}
	dydb_update_item_req.SetExpressionAttributeNames(mapAttrKey);
	dydb_update_item_req.SetExpressionAttributeValues(mapAttr);
	dydb_update_item_req.SetUpdateExpression(dydb_update_expression);

	const Aws::DynamoDB::Model::UpdateItemOutcome dydb_update_item_res = dydb_ctx->dydb_cli->UpdateItem(dydb_update_item_req);
	if (!dydb_update_item_res.IsSuccess())
	{
		DBG_ER_LN("UpdateItem error - %s !!! (table_name: %s , %s: %s, %s: %s), (%s)", dydb_update_item_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val, dydb_update_expression.c_str() );
		ret = -1;
	}
	else
	{
		DBG_IF_LN("UpdateItem ok !!! (table_name: %s, %s: %s, %s: %s), (%s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val, dydb_update_expression.c_str() );
	}

	return ret;
}

void dydb_ctx_tableX_free(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		clist_free(dydb_ctx->clistTableX);
	}
}

void dydb_ctx_attrX_addS(DyDB_InfoX_t *dydb_ctx, char *key, char *value)
{
	if (dydb_ctx)
	{
		DyDB_AttrX_t *attrX = NULL;

		// STRING
		attrX =(DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t));
		attrX->name = key;
		attrX->attr.SetS( value );
		clist_push(dydb_ctx->clistAttrX, attrX);
	}
}

void dydb_ctx_attrX_addN(DyDB_InfoX_t *dydb_ctx, char *key, int value)
{
	if (dydb_ctx)
	{
		DyDB_AttrX_t *attrX = NULL;

		// STRING
		attrX =(DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t));
		attrX->name = key;
		attrX->attr.SetN( value );
		clist_push(dydb_ctx->clistAttrX, attrX);
	}
}

void dydb_ctx_attrX_addL_with_composeS(DyDB_InfoX_t *dydb_ctx, char *key, char *value)
{
	if (dydb_ctx)
	{
		DyDB_AttrX_t *attrX = NULL;

		// ATTRIBUTE_LIST
		attrX =(DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t));
		Aws::Vector<std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> attrVector;

		char *saveptr = NULL;
		char *token = SAFE_STRTOK_R(value, ";:", &saveptr);
		while (token)
		{
			std::shared_ptr<Aws::DynamoDB::Model::AttributeValue> attrPtr = Aws::MakeShared<Aws::DynamoDB::Model::AttributeValue>( key );
			attrPtr->SetS(token);
			attrVector.push_back(std::make_shared<Aws::DynamoDB::Model::AttributeValue>(*attrPtr));
			token = SAFE_STRTOK_R(NULL, ";:", &saveptr);
		}

		attrX->name = key;
		attrX->attr.SetL(attrVector);
		clist_push(dydb_ctx->clistAttrX, attrX);
	}
}

void dydb_ctx_attrX_free(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		clist_free(dydb_ctx->clistAttrX);
	}
}

static void itemX_free_cb(void *item)
{
	DyDB_ItemX_t *itemX = (DyDB_ItemX_t *)item;
	if (itemX)
	{
		clist_free(itemX->clistAttrX);
	}
}

void dydb_ctx_itemX_free(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		clist_free_ex(dydb_ctx->clistItemX, itemX_free_cb);
	}
}

void dydb_ctx_free(DyDB_InfoX_t *dydb_ctx)
{
	if (dydb_ctx)
	{
		dydb_ctx_tableX_free(dydb_ctx);
		dydb_ctx_attrX_free(dydb_ctx);
		dydb_ctx_itemX_free(dydb_ctx);
	}
}

void dydb_ctx_init(DyDB_InfoX_t *dydb_ctx, Aws::DynamoDB::DynamoDBClient *dydb_cli)
{
	if (dydb_ctx)
	{
		dydb_ctx->dydb_cli = dydb_cli;
		CLIST_STRUCT_INIT(dydb_ctx, clistTableX);
		CLIST_STRUCT_INIT(dydb_ctx, clistAttrX);
		CLIST_STRUCT_INIT(dydb_ctx, clistItemX);
	}
}
