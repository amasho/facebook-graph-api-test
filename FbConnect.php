<?php
require_once("FacebookGraphAPIUtil.inc");

$config = parse_ini_file("config.ini");

$pageId = $config["page_id"];
$appId = $config["app_id"];
$appSecret = $config["app_secret"];
$redirect = urlencode($config["callback"]);
$reqPerms = $config["perms"];
 
$callback = $_REQUEST["callback"];
$code = $_REQUEST["code"];

$fb = new FacebookGraphAPIUtil($appId, $appSecret, $redirect, $reqPerms);
if(empty($code)) {
    header("Location: {$fb->getAuthUrl()}");
}


$accessToken = $fb->getAccessToken($code);

$fbUser = $fb->getFbUserData($accessToken);
$result['user'] = $fbUser;

// post parameter
// http://developers.facebook.com/docs/reference/api/post/
$p = array(
    "access_token" => $accessToken,
    "message" => "投稿てすとテストTEST",
    "picture" => "http://k.yimg.jp/images/top/sp/logo.gif",
    "link" => "http://www.yahoo.co.jp/",
    "name" => "テストほげほげ",
    "description" => "投稿てすてすてすててててててててt"
);

// user wall
$result['user_feed'] = $fb->postFeed($fbUser->id, $p);


// page post
$result['page_feed'] = $fb->postFeed($pageId, $p);

mb_convert_variables('SJIS', 'UTF-8', $result);

var_dump($result);
