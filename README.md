# ArkEcho Player and App Development

Player - [Github] (https://github.com/stephan-z/arkecho_player)
App - [Github] (https://github.com/stephan-z/arkecho_app)

###Description
ArkEcho is a private, non-commercial Project, developed by Stephan Ziesenis, Student of Health-Informatics at Heidelberg University & Heilbronn University.

The ArkEcho Player is a 'normal' Media Player, like VLC or Windows Media Player. Its developed for playing Music-Songs.
Its Feature is the ArkEcho App, that controls the Player via NetWork. Therefore it uses the WebSocket Protocoll.
To Connect the App to the Player, you can scan a Qr-Code in the Player or type the Address manually.
ArkEcho Player can play the following Audio Formats:
- .mp3
- .m4a

### Project is under the MIT-License
#### App and Player are using:
- Material-Design-Icons [Github] (https://github.com/google/material-design-icons)

### ArkEcho-App is developed with Visual Studio 2015, Xamarin.Android in C# #
#### The App is using NuGet Packages: 
- WebSockets.PCL - [Github] (https://github.com/NVentimiglia/Websockets.PCL)
- JSON.Net - [Github] (https://github.com/JamesNK/Newtonsoft.Json)
- ZXing.Net.Mobile - [Github] (https://github.com/Redth/ZXing.Net.Mobile)

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
