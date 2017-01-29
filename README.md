# ArkEcho Player, Page and App Development

- [Player] (https://github.com/stephan-z/arkecho_player)
- [Page] (https://github.com/stephan-z/arkecho_page)
- [App] (https://github.com/stephan-z/arkecho_app)

###Description
ArkEcho is a private, non-commercial Project, developed by Stephan Ziesenis, Student of Health-Informatics at Heidelberg University & Heilbronn University.

The ArkEcho Player is a 'normal' Media Player, like VLC or Windows Media Player. Its developed for playing Music-Songs.
Its Feature is the ArkEcho App, that controls the Player via NetWork. Therefore it uses the WebSocket Protocoll.
To Connect the App to the Player, you can scan a Qr-Code in the Player or type the Address manually.
Another Feature is the ArkEcho Page, that controls the Player over your Web-Browser in the NetWork.

ArkEcho Player can play the following Audio Formats:
- .mp3
- .m4a

### Project is under the MIT-License
#### App, Page and 2Player are using:
- Material-Design-Icons [Github] (https://github.com/google/material-design-icons)

### ArkEcho-Player is developed with Visual Studio 2015, Qt 5.7.0 in C++
#### The Player is using:
- Nayuki's Qr-Code Generator - [Github] (https://github.com/nayuki/QR-Code-generator)
- QDarkStylesheet - [Github] (https://github.com/ColinDuquesnoy/QDarkStyleSheet)

### IMPORTANT INFORMATION:
If some of your Media-Files aren't loaded by the player, you need to install extra Music Codec.
I used the K-Lite Codec Pack, downloadable at [Codecguide](http://www.codecguide.com/download_kl.htm).

### JSON Messages:
```json
{
	"Message": string,
	"Type": int
}
```

### JSON Song:
```json
{
	"SongTitle": string,	
	"SongInterpret": string,
	"AlbumTitle": string,
	"AlbumInterpret": string,
	"CoverArt": string
}
```
