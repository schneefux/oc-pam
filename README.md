Based on [User PAM backend 0.1](https://apps.owncloud.com/content/show.php/User+PAM+backend?content=174684).

Edit `pwauth/pwauth/config.h` and set the web server's user id:

```c
#define SERVER_UIDS x
```

Compile pwauth: `cd pwauth/pwauth && make`.

Compile members: `cd members_src && make`.

Copy both binaries to some directory (e.g. `/usr/local/bin`), configure plugin's `appinfo/app.php`

Set suid-flag to allow the script to read `/etc/passwd`: `chmod u+s pwauth`

If something does not work, start debugging by adding this line in `user_pam.php`:

```diff
if (fwrite($handle, "$uid\n$password\n") === false)
    return false

$result = pclose( $handle );
if (0 === $result)
    return $uid;

+ error_log($result);
return false;
```

Error codes are at the bottom of pwauth's `INSTALL`.
