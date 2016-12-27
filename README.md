# ArkEcho App and Desktop Development

## Private Project ;)
## Please don't make Commits etc. :D

### App is developed with Visual Studio 2015, Xamarin.Android in C#
### App is using NuGet Packages: 
- WebSockets.PCL - https://github.com/NVentimiglia/Websockets.PCL
- JSON.Net - https://github.com/JamesNK/Newtonsoft.Json
- ZXing.Net.Mobile - https://github.com/Redth/ZXing.Net.Mobile

### Player is developed with Visual Studio 2015, Qt 5.7.0 in C++
### Player is using:
- Nayuki's Qr-Code Generator - https://github.com/nayuki/QR-Code-generator
- QDarkStylesheet - https://github.com/ColinDuquesnoy/QDarkStyleSheet

### JSON Messages:
```json
{
	"Message": string,
	"Typ": int
}
```

### JSON Qr-Code:
```json
{
	"Address": string,
	"Security_Code": int
}
```