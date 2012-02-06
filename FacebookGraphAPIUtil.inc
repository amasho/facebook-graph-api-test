<?php

define("FB_AUTH_URL", "http://www.facebook.com/dialog/oauth");
define("FB_GRAPH_URL", "https://graph.facebook.com/");
define("FB_GRAPH_TOKEN_URL", FB_GRAPH_URL . "oauth/access_token");

class FacebookGraphAPIUtil
{

    private $appId;
    private $appSecret;
    private $callBackUrl;
    private $reqPerms;
    private $accessToken;
    private $tokenExpire;

    /**
     * constructer
     */
    public function __construct($id, $secret, $callback, $perms){

        $this->appId = $id;
        $this->appSecret = $secret;
        $this->callBackUrl = $callback;
        $this->reqPerms = $perms;
    }

    /**
     *
     */
    public function getAuthUrl(){

    	$p = array(
            $this->appId,
            $this->callBackUrl,
            $this->reqPerms,
        );

        $url = vsprintf(FB_AUTH_URL . "?client_id=%s&redirect_uri=%s&scope=%s", $p);
        return $url;
    }

    public function getAccessToken($code){

        $p = array(
            $this->appId,
            $this->appSecret,
            $this->callBackUrl,
            $code,
        );
        $tokenUrl = vsprintf(FB_GRAPH_TOKEN_URL . "?client_id=%s&client_secret=%s&redirect_uri=%s&code=%s", $p);
        $resultStr = @file_get_contents($tokenUrl);
        list($accessToken, $tokenExpire) = explode('&', $resultStr);
        $this->accessToken = str_replace("access_token=", "", $accessToken);
        $this->tokenExpire = str_replace("expire=", "", $tokenExpire);

        return $this->accessToken;
    }

    public function getFbUserData($accessToken){

        $graphUrl = FB_GRAPH_URL . "me?access_token={$accessToken}";
        $result = @file_get_contents($graphUrl);
        if(!$result) return false;
        return json_decode($result);
    }

    public function postFeed($id, $postData){

        $context["http"]["method"] = "POST";
        $context["http"]["content"] = http_build_query($postData);

        $graphUrl = FB_GRAPH_URL . "{$id}/feed";
        $result = @file_get_contents($graphUrl, false, stream_context_create($context));
        if(!$result) return false;
        return $result;
    }

}

