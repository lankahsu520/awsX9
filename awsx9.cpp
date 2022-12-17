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

Aws::SDKOptions aws_options;
Aws::DynamoDB::DynamoDBClient *dydb_caller;
Aws::S3::S3Client *s3_caller;

Aws::DynamoDB::DynamoDBClient *awsX_dydb_cli_get(void)
{
	return dydb_caller;
}

Aws::S3::S3Client *awsX_s3_cli_get(void)
{
	return s3_caller;
}

void awsX_free(void)
{
	Aws::ShutdownAPI(aws_options);

#if (0)
	if (dydb_caller)
	{
		delete dydb_caller;
		dydb_caller = NULL;
	}
#endif
}

void awsX_init(void)
{
	Aws::InitAPI(aws_options);

	Aws::Client::ClientConfiguration clientConfig;
	dydb_caller = new Aws::DynamoDB::DynamoDBClient(clientConfig);
	s3_caller = new Aws::S3::S3Client(clientConfig);
}
