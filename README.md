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
  "mouse",
  "tiger"
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
  "mouse",
  "tiger"
 ]
}
```

```bash
$ ./awsx_123
[38555/38555] dydb_del_item:150 - DeleteItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka)
[38555/38555] dydb_put_item:249 - PutItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka)
[38555/38555] dydb_get_item:206 - GetItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka, attr_size: 5)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Album123"}, AlbumTitle: Album123)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, Artist: Lanka)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"1"}, Awards: 1)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, SongTitle: Lanka)
[38555/38555] dydb_show_attr:72 - (ATTRIBUTE_LIST: {"L":[{"S":"dog"},{"S":"mouse"},{"S":"tiger"}]})
[38555/38555] dydb_show_attr:75 - (Sponsor[0]: dog)
[38555/38555] dydb_show_attr:75 - (Sponsor[1]: mouse)
[38555/38555] dydb_show_attr:75 - (Sponsor[2]: tiger)
[38555/38555] dydb_update_item:372 - UpdateItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka)
[38555/38555] dydb_get_item:206 - GetItem ok !!! (table_name: Music, Artist: Lanka, SongTitle: Lanka, attr_size: 5)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Album123"}, AlbumTitle: Album123)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, Artist: Lanka)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"2"}, Awards: 2)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, SongTitle: Lanka)
[38555/38555] dydb_show_attr:72 - (ATTRIBUTE_LIST: {"L":[{"S":"dog"},{"S":"mouse"},{"S":"tiger"}]})
[38555/38555] dydb_show_attr:75 - (Sponsor[0]: dog)
[38555/38555] dydb_show_attr:75 - (Sponsor[1]: mouse)
[38555/38555] dydb_show_attr:75 - (Sponsor[2]: tiger)
[38555/38555] dydb_query_item:315 - Query ok !!! (table_name: Music, Artist: Lanka, items_size: 2)
[38555/38555] dydb_show_itemX:114 - __________ 000 __________
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Album123"}, AlbumTitle: Album123)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, Artist: Lanka)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"2"}, Awards: 2)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, SongTitle: Lanka)
[38555/38555] dydb_show_attr:72 - (ATTRIBUTE_LIST: {"L":[{"S":"dog"},{"S":"mouse"},{"S":"tiger"}]})
[38555/38555] dydb_show_attr:75 - (Sponsor[0]: dog)
[38555/38555] dydb_show_attr:75 - (Sponsor[1]: mouse)
[38555/38555] dydb_show_attr:75 - (Sponsor[2]: tiger)
[38555/38555] dydb_show_itemX:114 - __________ 001 __________
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka520"}, AlbumTitle: Lanka520)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka"}, Artist: Lanka)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"1"}, Awards: 1)
[38555/38555] dydb_show_attr:47 - (STRING: {"S":"Lanka520520"}, SongTitle: Lanka520520)
[38555/38555] dydb_show_attr:72 - (ATTRIBUTE_LIST: {"L":[{"S":"dog"},{"S":"cat"},{"S":"mouse"},{"S":"stoat"},{"S":"snake"}]})
[38555/38555] dydb_show_attr:75 - (Sponsor[0]: dog)
[38555/38555] dydb_show_attr:75 - (Sponsor[1]: cat)
[38555/38555] dydb_show_attr:75 - (Sponsor[2]: mouse)
[38555/38555] dydb_show_attr:75 - (Sponsor[3]: stoat)
[38555/38555] dydb_show_attr:75 - (Sponsor[4]: snake)
[38555/38555] main:248 - Bye-Bye !!!
```

# 6. License

awsX9 is under the New BSD License (BSD-3-Clause).


# 7. Documentation
Run an example and read it.
