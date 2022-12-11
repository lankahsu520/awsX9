# 1. Overview
awsX9 is an api of [AWS SDK (C++)](https://github.com/aws/aws-sdk-cpp). We can save our time from learning AWS SDK.

Please also read [helper_AWS-CLI.md](https://github.com/lankahsu520/HelperX/blob/master/helper_AWS-CLI.md) and [helper_AWS-SDK.md](https://github.com/lankahsu520/HelperX/blob/master/helper_AWS-SDK.md).

# 2. Depend on
- [utilx9](https://github.com/lankahsu520/utilx9)
- [AWS SDK (C++)](https://www.silabs.com/wireless/z-wave)

# 3. Current Status



# 4. Build
   ```bash
$ vi utilx9/conf/github.conf
export PJ_HAS_AWSX9=yes
export PJ_HAS_AWS_SDK_CPP=yes
# please build AWS SDK C++ by yourself.
# or update utilx9/user/library_pub.mk
$ make
   ```

# 5. Example or Usage
- awsx_123 - an example. to get item from Amazon DynamoDB.
```json
{
 "Artist": "Lanka",
 "SongTitle": "Lanka",
 "AlbumTitle": "Album123",
 "Awards": "1",
 "Sponsor": [
  "dog",
  "mouse"
 ]
}
```
```json
{
 "Artist": "Lanka",
 "SongTitle": "Lanka",
 "AlbumTitle": "Album123",
 "Awards": "2",
 "Sponsor": [
  "dog",
  "mouse"
 ]
}
```

```bash
$ ./awsx_123
[240628/240628] dydb_put_item:169 - PutItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka)
[240628/240628] dydb_get_item:125 - GetItem ok !!! (table_name: Music 5, Artist: Lanka, SongTitle: Lanka)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"Album123"}, AlbumTitle: Album123)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"Lanka"}, Artist: Lanka)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"1"}, Awards: 1)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"Lanka"}, SongTitle: Lanka)
[240628/240628] dydb_show_attr:54 - (ATTRIBUTE_LIST: {"L":[{"S":"dog"},{"S":"mouse"}]})
[240628/240628] dydb_show_attr:57 - (Sponsor[0]: dog)
[240628/240628] dydb_show_attr:57 - (Sponsor[1]: mouse)
[240628/240628] dydb_update_item:226 - UpdateItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka)
[240628/240628] dydb_get_item:125 - GetItem ok !!! (table_name: Music 5, Artist: Lanka, SongTitle: Lanka)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"Album123"}, AlbumTitle: Album123)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"Lanka"}, Artist: Lanka)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"2"}, Awards: 2)
[240628/240628] dydb_show_attr:29 - (STRING: {"S":"Lanka"}, SongTitle: Lanka)
[240628/240628] dydb_show_attr:54 - (ATTRIBUTE_LIST: {"L":[{"S":"dog"},{"S":"mouse"}]})
[240628/240628] dydb_show_attr:57 - (Sponsor[0]: dog)
[240628/240628] dydb_show_attr:57 - (Sponsor[1]: mouse)
[240628/240628] main:251 - Bye-Bye !!!

```

# 6. License

awsX9 is under the New BSD License (BSD-3-Clause).


# 7. Documentation
Run an example and read it.
