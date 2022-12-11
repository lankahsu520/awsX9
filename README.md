# 1. Overview
awsX9 is an api of [AWS SDK (C++)](https://github.com/aws/aws-sdk-cpp). We can save our time from learning AWS SDK.

Please also read [helper_AWS-CLI.md](https://github.com/lankahsu520/HelperX/blob/master/helper_AWS-CLI.md) and [helper_AWS-SDK.md](https://github.com/lankahsu520/HelperX/blob/master/helper_AWS-SDK.md).

# 2. Depend on
- [utilx9](https://github.com/lankahsu520/utilx9)
- [AWS SDK (C++)](https://www.silabs.com/wireless/z-wave)

# 3. Current Status



# 4. Build
   ```bash
$ vi conf/github.conf
export PJ_HAS_AWSX9=yes
export PJ_HAS_AWS_SDK_CPP=yes
# please build AWS SDK C++ by yourself.
$ make
   ```

# 5. Example or Usage
- awsx_123 - an example. to get item from Amazon DynamoDB.
```bash
$ ./awsx_123
[9368/9368] dydb_get_item:125 - GetItem ok !!! (table_name: Music 4, Artist: Acme Band, SongTitle: Happy Day)
[9368/9368] dydb_show_attr:29 - (STRING: {"S":"Songs About Life"}, AlbumTitle: Songs About Life)
[9368/9368] dydb_show_attr:29 - (STRING: {"S":"Acme Band"}, Artist: Acme Band)
[9368/9368] dydb_show_attr:29 - (STRING: {"S":"10"}, Awards: 10)
[9368/9368] dydb_show_attr:29 - (STRING: {"S":"Happy Day"}, SongTitle: Happy Day)
[9368/9368] main:205 - Bye-Bye !!!

```

# 6. License
awsX9 is under the New BSD License (BSD-3-Clause).


# 7. Documentation
Run an example and read it.
