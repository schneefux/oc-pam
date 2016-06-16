<?php
/**
 * @author CSharplie
 *
 * @copyright Copyright (c) 2016, CSharplie
 * @license AGPL-3.0
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License, version 3,
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

require_once('apps/user_pam/user_pam.php');

define('OC_USER_BACKEND_PAM_PATH', '/etc/nixos/bin/pwauth');
define('OC_USER_BACKEND_MEMBERS_PATH', '/etc/nixos/bin/members');
define('OC_USER_BACKEND_PAM_GROUP', 'users');

OC_User::useBackend(new \OCA\user_pam\USER_PAM());

?>
