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
#include <aws/s3/model/CopyObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>

#include <fstream>
#include <memory>

#include "awsx9.h"

using namespace std;

static const char ALLOCATION_TAG[] = "awsxS3";

// bucket/key -> bucket/key
int s3_copy_object(S3_InfoX_t *s3_ctx)
{
	int ret = 0;

	if ( ( ret= S3_CTX_CHECK_CLI(s3_ctx) ) == -1 )
	{
		return ret;
	}

	if ( (SAFE_STRLEN(s3_ctx->from.fileX.remote.bucket) <=0 )
		|| (SAFE_STRLEN(s3_ctx->from.fileX.remote.key) <=0 )
		|| (SAFE_STRLEN(s3_ctx->to.fileX.remote.bucket) <=0 )
		|| (SAFE_STRLEN(s3_ctx->to.fileX.remote.key) <=0 ) )
	{
		size_t from_bucket_len = SAFE_STRLEN(s3_ctx->from.fileX.remote.bucket);
		size_t from_key_len = SAFE_STRLEN(s3_ctx->from.fileX.remote.key);
		size_t to_bucket_len = SAFE_STRLEN(s3_ctx->to.fileX.remote.bucket);
		size_t to_key_len = SAFE_STRLEN(s3_ctx->to.fileX.remote.key);
		DBG_ER_LN("Null Definition !!! (from_bucket_len: %zd, from_key_len: %zd, to_bucket_len: %zd, to_key_len: %zd)", from_bucket_len, from_key_len, to_bucket_len, to_key_len);
		return -1;
	}
	DBG_IF_LN("(bucket: %s/%s -> bucket: %s/%s)", s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key, s3_ctx->to.fileX.remote.bucket, s3_ctx->to.fileX.remote.key);

	Aws::S3::Model::CopyObjectRequest s3_copy_req;

	// Set up the request.
	Aws::String src_bucket_key( Aws::String(s3_ctx->from.fileX.remote.bucket) + "/" + Aws::String(s3_ctx->from.fileX.remote.key) );
	s3_copy_req.WithCopySource( src_bucket_key )
		.WithKey( s3_ctx->to.fileX.remote.key )
		.WithBucket( s3_ctx->to.fileX.remote.bucket );

	Aws::S3::Model::CopyObjectOutcome s3_copy_res = s3_ctx->s3_cli->CopyObject(s3_copy_req);
	if (s3_copy_res.IsSuccess())
	{
		DBG_IF_LN("CopyObject ok !!! (bucket: %s/%s -> bucket: %s/%s)", s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key, s3_ctx->to.fileX.remote.bucket, s3_ctx->to.fileX.remote.key);
	}
	else
	{
		DBG_ER_LN("CopyObject error - %s !!! (bucket: %s/%s -> bucket: %s/%s)", s3_copy_res.GetError().GetMessage().c_str(), s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key, s3_ctx->to.fileX.remote.bucket, s3_ctx->to.fileX.remote.key);
		ret = -1;
	}

	return ret;
}

// bucket/key -> NULL
int s3_delete_object(S3_InfoX_t *s3_ctx)
{
	int ret = 0;

	if ( ( ret= S3_CTX_CHECK_CLI(s3_ctx) ) == -1 )
	{
		return ret;
	}

	if ( (SAFE_STRLEN(s3_ctx->from.fileX.remote.bucket) <=0 )
		|| (SAFE_STRLEN(s3_ctx->from.fileX.remote.key) <=0 ) )
	{
		size_t from_bucket_len = SAFE_STRLEN(s3_ctx->from.fileX.remote.bucket);
		size_t from_key_len = SAFE_STRLEN(s3_ctx->from.fileX.remote.key);
		DBG_ER_LN("Null Definition !!! (from_bucket_len: %zd, from_key_len: %zd)", from_bucket_len, from_key_len);
		return -1;
	}
	DBG_IF_LN("(bucket: %s/%s -> NULL)", s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key);

	Aws::S3::Model::DeleteObjectRequest s3_delete_req;

	// Set up the request.
	s3_delete_req.WithKey( s3_ctx->from.fileX.remote.key ).WithBucket( s3_ctx->from.fileX.remote.bucket );

	Aws::S3::Model::DeleteObjectOutcome s3_delete_res = s3_ctx->s3_cli->DeleteObject(s3_delete_req);
	if (s3_delete_res.IsSuccess())
	{
		DBG_IF_LN("DeleteObject ok !!! (bucket: %s/%s -> NULL)", s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key);
	}
	else
	{
		DBG_ER_LN("DeleteObject error - %s !!! (bucket: %s/%s -> NULL)", s3_delete_res.GetError().GetMessage().c_str(), s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key);
		ret = -1;
	}

	return ret;
}

// bucket/key -> local
// https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/configuring-iostreams.html
int s3_get_object(S3_InfoX_t *s3_ctx)
{
	int ret = 0;

	if ( ( ret= S3_CTX_CHECK_CLI(s3_ctx) ) == -1 )
	{
		return ret;
	}

	if ( (SAFE_STRLEN(s3_ctx->from.fileX.remote.bucket) <=0 )
		|| (SAFE_STRLEN(s3_ctx->from.fileX.remote.key) <=0 )
		|| (SAFE_STRLEN(s3_ctx->to.fileX.localname) <=0 ) )
	{
		size_t from_bucket_len = SAFE_STRLEN(s3_ctx->from.fileX.remote.bucket);
		size_t from_key_len = SAFE_STRLEN(s3_ctx->from.fileX.remote.key);
		size_t localname_len = SAFE_STRLEN(s3_ctx->to.fileX.localname);
		DBG_ER_LN("Null Definition !!! (from_bucket_len: %zd, from_key_len: %zd, localname_len: %zd)", from_bucket_len, from_key_len, localname_len);
		return -1;
	}
	DBG_IF_LN("(bucket: %s/%s -> localname: %s)", s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key, s3_ctx->to.fileX.localname);

	Aws::S3::Model::GetObjectRequest s3_get_req;

	// Set up the request.
	s3_get_req.WithBucket( s3_ctx->from.fileX.remote.bucket ).WithKey( s3_ctx->from.fileX.remote.key );

	char *saveto = s3_ctx->to.fileX.localname;
	s3_get_req.SetResponseStreamFactory([saveto] { return new std::fstream( (const char*)saveto, std::ios_base::out); });

	Aws::S3::Model::GetObjectOutcome s3_get_res = s3_ctx->s3_cli->GetObject(s3_get_req);
	if (s3_get_res.IsSuccess())
	{
		DBG_IF_LN("GetObject ok !!! (bucket: %s/%s -> saveto: %s)", s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key, s3_ctx->to.fileX.localname);
	}
	else
	{
		DBG_ER_LN("GetObject error - %s !!! (bucket: %s/%s -> saveto: %s)", s3_get_res.GetError().GetMessage().c_str(), s3_ctx->from.fileX.remote.bucket, s3_ctx->from.fileX.remote.key, s3_ctx->to.fileX.localname);
		ret = -1;
	}

	return ret;
}

// local -> bucket/key
int s3_put_object(S3_InfoX_t *s3_ctx)
{
	int ret = 0;

	if ( ( ret= S3_CTX_CHECK_CLI(s3_ctx) ) == -1 )
	{
		return ret;
	}

	if ( (SAFE_STRLEN(s3_ctx->to.fileX.remote.bucket) <=0 )
		|| (SAFE_STRLEN(s3_ctx->to.fileX.remote.key) <=0 )
		|| (SAFE_STRLEN(s3_ctx->from.fileX.localname) <=0 ) )
	{
		size_t to_bucket_len = SAFE_STRLEN(s3_ctx->to.fileX.remote.bucket);
		size_t to_key_len = SAFE_STRLEN(s3_ctx->to.fileX.remote.key);
		size_t localname_len = SAFE_STRLEN(s3_ctx->from.fileX.localname);
		DBG_ER_LN("Null Definition !!! (localname_len: %zd, to_bucket_len: %zd, to_key_len: %zd)", localname_len, to_bucket_len, to_key_len);
		return -1;
	}
	DBG_IF_LN("(localname: %s -> bucket: %s/%s)", s3_ctx->from.fileX.localname, s3_ctx->to.fileX.remote.bucket, s3_ctx->to.fileX.remote.key);

	Aws::S3::Model::PutObjectRequest s3_put_req;

	// Set up the request.
	s3_put_req.WithBucket( s3_ctx->to.fileX.remote.bucket ).WithKey( s3_ctx->to.fileX.remote.key );

	char *fromfile = s3_ctx->from.fileX.localname;
	std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::FStream>(ALLOCATION_TAG, fromfile, std::ios_base::in | std::ios_base::binary);
	if (!*inputData)
	{
		DBG_ER_LN("Read error !!! (fromfile: %s)", fromfile);
		return -1;
	}
	s3_put_req.SetBody(inputData);

	Aws::S3::Model::PutObjectOutcome s3_put_res = s3_ctx->s3_cli->PutObject(s3_put_req);
	if (s3_put_res.IsSuccess())
	{
		DBG_IF_LN("PutObject ok !!! (localname: %s -> bucket: %s/%s)", s3_ctx->from.fileX.localname, s3_ctx->to.fileX.remote.bucket, s3_ctx->to.fileX.remote.key);
	}
	else
	{
		DBG_ER_LN("PutObject error - %s !!! (localname: %s -> bucket: %s/%s)", s3_put_res.GetError().GetMessage().c_str(), s3_ctx->from.fileX.localname, s3_ctx->to.fileX.remote.bucket, s3_ctx->to.fileX.remote.key);
		ret = -1;
	}

	return ret;
}

void s3_ctx_init_copy(S3_InfoX_t *s3_ctx, char *from_bucket, char *from_key, char *to_bucket, char *to_key)
{
	if ( (s3_ctx) && (from_bucket) && (from_key) && (to_bucket) && (to_key) )
	{
		SAFE_MEMSET(&s3_ctx->from, 0, sizeof(S3_FILEX_t));
		SAFE_MEMSET(&s3_ctx->to, 0, sizeof(S3_FILEX_t));

		SAFE_SPRINTF(s3_ctx->from.fileX.remote.bucket, "%s", from_bucket);
		SAFE_SPRINTF(s3_ctx->from.fileX.remote.key, "%s", from_key);
		SAFE_SPRINTF(s3_ctx->to.fileX.remote.bucket, "%s", to_bucket);
		SAFE_SPRINTF(s3_ctx->to.fileX.remote.key, "%s", to_key);
	}
}

void s3_ctx_init_delete(S3_InfoX_t *s3_ctx, char *from_bucket, char *from_key)
{
	if ( (s3_ctx) && (from_bucket) && (from_key) )
	{
		SAFE_MEMSET(&s3_ctx->from, 0, sizeof(S3_FILEX_t));
		SAFE_MEMSET(&s3_ctx->to, 0, sizeof(S3_FILEX_t));

		SAFE_SPRINTF(s3_ctx->from.fileX.remote.bucket, "%s", from_bucket);
		SAFE_SPRINTF(s3_ctx->from.fileX.remote.key, "%s", from_key);
	}
}

void s3_ctx_init_get(S3_InfoX_t *s3_ctx, char *from_bucket, char *from_key, char *localname)
{
	if ( (s3_ctx) && (from_bucket) && (from_key) && (localname) )
	{
		SAFE_MEMSET(&s3_ctx->from, 0, sizeof(S3_FILEX_t));
		SAFE_MEMSET(&s3_ctx->to, 0, sizeof(S3_FILEX_t));

		SAFE_SPRINTF(s3_ctx->from.fileX.remote.bucket, "%s", from_bucket);
		SAFE_SPRINTF(s3_ctx->from.fileX.remote.key, "%s", from_key);
		SAFE_SPRINTF(s3_ctx->to.fileX.localname, "%s", localname);
	}
}

void s3_ctx_init_put(S3_InfoX_t *s3_ctx, char *localname, char *to_bucket, char *to_key)
{
	if ( (s3_ctx) && (to_bucket) && (to_key) && (localname) )
	{
		SAFE_MEMSET(&s3_ctx->from, 0, sizeof(S3_FILEX_t));
		SAFE_MEMSET(&s3_ctx->to, 0, sizeof(S3_FILEX_t));

		SAFE_SPRINTF(s3_ctx->to.fileX.remote.bucket, "%s", to_bucket);
		SAFE_SPRINTF(s3_ctx->to.fileX.remote.key, "%s", to_key);
		SAFE_SPRINTF(s3_ctx->from.fileX.localname, "%s", localname);
	}
}

void s3_ctx_free(S3_InfoX_t *s3_ctx)
{
	if ( (s3_ctx) && (s3_ctx->isfree == 0) )
	{
		s3_ctx->isfree = 1;
	}
}

void s3_ctx_init(S3_InfoX_t *s3_ctx, Aws::S3::S3Client *dydb_cli)
{
	if ( (s3_ctx) && (s3_ctx->isinit == 0) )
	{
		s3_ctx->isinit = 1;
		s3_ctx->isfree = 0;
		s3_ctx->s3_cli = dydb_cli;
	}
}
