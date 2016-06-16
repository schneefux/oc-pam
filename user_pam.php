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

namespace OCA\user_pam;

class USER_PAM extends \OC_User_Backend implements \OCP\UserInterface {
	protected $pam_bin_path;
	private $user_search;

	public function __construct() {
		$this->pam_bin_path = \OCP\Config::getAppValue('user_pam', 'pam_path', OC_USER_BACKEND_PAM_PATH);
		$this->members_bin_path = \OCP\Config::getAppValue('user_pam', 'members_path', OC_USER_BACKEND_MEMBERS_PATH);
	}

	public function implementsAction($actions) {
		return (bool)((OC_USER_BACKEND_CHECK_PASSWORD | OC_USER_BACKEND_SET_PASSWORD) & $actions);
	}

	private function userMatchesFilter($user) {
		return (strripos($user, $this->user_search) !== false);
	}

	public function deleteUser($_uid) {
		return false;
	}

	public function checkPassword( $uid, $password ) {
		$uid = strtolower($uid);

		if (!preg_match('/[A-Za-z_]+/', $uid))
			return false;

		$userGroups = shell_exec("groups $uid");
		if (!preg_match('/' . OC_USER_BACKEND_PAM_GROUP . '/', $userGroups))
			return false;

		$handle = popen($this->pam_bin_path, 'w');
		if ($handle === false)
			return false;

		if (fwrite($handle, "$uid\n$password\n") === false)
			return false;

		$result = pclose( $handle );
		if (0 === $result)
			return $uid;
		
		return false;
	}

	public function setPassword( $uid, $password ) {
		return false;
	}

	public function userExists( $uid ){
		$user = posix_getpwnam( strtolower($uid) );
		return is_array($user);
	}

	public function getUsers($search = '', $limit = 10, $offset = 10){
		$returnArray = explode(" ", shell_exec(OC_USER_BACKEND_MEMBERS_PATH . " " . OC_USER_BACKEND_PAM_GROUP));

		$this->user_search = $search;
		if(!empty($this->user_search))
			$returnArray = array_filter($returnArray, array($this, 'userMatchesFilter'));
		
		if($limit = -1)
			$limit = null;
		return array_slice($returnArray, $offset, $limit);
	}
}

?>
