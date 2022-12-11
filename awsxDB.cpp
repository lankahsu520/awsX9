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

#include "awsx9.h"

using namespace std;

void dydb_show_attr(Aws::String& name, Aws::DynamoDB::Model::AttributeValue *attr)
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

void dydb_show_listX(DyDB_InfoX_t *dydb_ctx)
{
	DyDB_AttrX_t *curAttrX = NULL;

	for (curAttrX = (DyDB_AttrX_t *)clist_head(dydb_ctx->clistAttrX); curAttrX != NULL; curAttrX = (DyDB_AttrX_t *)clist_item_next((void *)curAttrX))
	{
		dydb_show_attr(curAttrX->name, &curAttrX->attr);
	}
}

int dydb_get_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	clist_free(dydb_ctx->clistAttrX);

	Aws::DynamoDB::Model::GetItemRequest dydb_getitem_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_getitem_req.SetTableName(dydb_ctx->table_name);
	dydb_getitem_req.AddKey(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_getitem_req.AddKey(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	const Aws::DynamoDB::Model::GetItemOutcome& dydb_getitem_res = dydb_ctx->dydb_cli->GetItem(dydb_getitem_req);
	if (dydb_getitem_res.IsSuccess())
	{
		// Reference the retrieved fields/values.
		const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& mapAry = dydb_getitem_res.GetResult().GetItem();
		dydb_ctx->mapAry = &mapAry;

		if (dydb_ctx->mapAry->size() > 0)
		{
			// Output each retrieved field and its value.
			for (const auto& i : *dydb_ctx->mapAry)
			{
				DyDB_AttrX_t *attrX = (DyDB_AttrX_t*)calloc(1, sizeof(DyDB_AttrX_t)); //SAFE_CALLOC(1, sizeof(DyDB_AttrX_t));
				attrX->name = i.first;
				attrX->attr = i.second;
				clist_push(dydb_ctx->clistAttrX, attrX);

				Aws::Utils::Json::JsonValue jitem  = i.second.Jsonize();
				//Aws::DynamoDB::Model::ValueType item_type = i.second.GetType();
				DBG_DB_LN("(cjson: %s, %s: %s)", jitem.View().WriteCompact().c_str(), i.first.c_str(), i.second.GetS().c_str() );
			}
		}
		DBG_IF_LN("GetItem ok !!! (table_name: %s %zd, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->mapAry->size(), dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}
	else
	{
		DBG_ER_LN("GetItem error - %s !!! (table_name: %s, %s: %s, %s: %s)", dydb_getitem_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}
	// snippet-end:[dynamodb.cpp.get_item.code]

	return ret;
}

int dydb_put_item(DyDB_InfoX_t *dydb_ctx)
{
	int ret = 0;

	if ( ( ret= DYDB_CTX_CHECK(dydb_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_DB_LN("(table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );

	Aws::DynamoDB::Model::PutItemRequest dydb_putitem_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	dydb_putitem_req.SetTableName(dydb_ctx->table_name);
	dydb_putitem_req.AddItem(dydb_ctx->pk, dydb_attr.SetS(dydb_ctx->pk_val));
	dydb_putitem_req.AddItem(dydb_ctx->sk, dydb_attr.SetS(dydb_ctx->sk_val));

	DyDB_AttrX_t *cur = NULL;

	for (cur = (DyDB_AttrX_t *)clist_head(dydb_ctx->clistAttrX); cur != NULL; cur = (DyDB_AttrX_t *)clist_item_next((void *)cur))
	{
		dydb_putitem_req.AddItem(cur->name, cur->attr);
	}

	const Aws::DynamoDB::Model::PutItemOutcome dydb_putitem_res = dydb_ctx->dydb_cli->PutItem(dydb_putitem_req);
	if (!dydb_putitem_res.IsSuccess())
	{
		DBG_ER_LN("PutItem error - %s !!! (table_name: %s , %s: %s, %s: %s)", dydb_putitem_res.GetError().GetMessage().c_str(), dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
		ret = -1;
	}
	else
	{
		DBG_IF_LN("PutItem ok !!! (table_name: %s, %s: %s, %s: %s)", dydb_ctx->table_name, dydb_ctx->pk, dydb_ctx->pk_val, dydb_ctx->sk, dydb_ctx->sk_val );
	}

	return ret;
}

void dydb_ctx_free(DyDB_InfoX_t *dydb_ctx)
{
	clist_free(dydb_ctx->clistAttrX);
}

void dydb_ctx_init(DyDB_InfoX_t *dydb_ctx, Aws::DynamoDB::DynamoDBClient *dydb_cli)
{
	dydb_ctx->dydb_cli = dydb_cli;
	CLIST_STRUCT_INIT(dydb_ctx, clistAttrX);

	DYDB_CTX_CHECK(dydb_ctx);
}
