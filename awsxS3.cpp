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

#include <fstream>
#include <memory>

#include "awsx9.h"

using namespace std;

static const char ALLOCATION_TAG[] = "awsxS3";

// https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/configuring-iostreams.html
int s3_get_file(S3_InfoX_t *s3_ctx)
{
	int ret = 0;

	if ( ( ret= S3_CTX_CHECK_CLI(s3_ctx) ) == -1 )
	{
		return ret;
	}
	DBG_IF_LN("(bucket: %s/%s -> saveto: %s)", s3_ctx->bucket, s3_ctx->key, s3_ctx->saveto);

	Aws::S3::Model::GetObjectRequest s3_get_req;
	Aws::DynamoDB::Model::AttributeValue dydb_attr;

	// Set up the request.
	s3_get_req.WithBucket( s3_ctx->bucket ).WithKey( s3_ctx->key );
	char *saveto = s3_ctx->saveto;
	s3_get_req.SetResponseStreamFactory([saveto] { return new std::fstream( (const char*)saveto, std::ios_base::out); });

	const Aws::S3::Model::GetObjectOutcome s3_get_res = s3_ctx->s3_cli->GetObject(s3_get_req);
	if (s3_get_res.IsSuccess())
	{
		DBG_IF_LN("GetObject ok !!! (bucket: %s/%s -> saveto: %s)", s3_ctx->bucket, s3_ctx->key, s3_ctx->saveto);
	}
	else
	{
		DBG_ER_LN("GetObject error - %s !!! (bucket: %s/%s -> saveto: %s)", s3_get_res.GetError().GetMessage().c_str(), s3_ctx->bucket, s3_ctx->key, s3_ctx->saveto);
		ret = -1;
	}

	return ret;
}

void s3_ctx_init(S3_InfoX_t *s3_ctx, Aws::S3::S3Client *dydb_cli)
{
	if (s3_ctx)
	{
		s3_ctx->s3_cli = dydb_cli;
	}
}
