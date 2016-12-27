using Android.App;
using Android.Widget;
using Android.OS;

using System;

using ZXing.Mobile;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace arkecho_app
{
    [Activity(Label = "@string/ApplicationTitle", MainLauncher = true, Icon = "@drawable/playerIcon")]
    public class MainActivityView : Activity
    {
        private ArkEchoWebSocket webSocket_;
        string qrCodeText_;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            // Set our view from the "main" layout resource
            SetContentView (Resource.Layout.Main);

            // Connect Buttons
            FindViewById<Button>(Resource.Id.pbConnectWithQr).Click += onPbConnectWithQrClicked;
            FindViewById<Button>(Resource.Id.pbConnectManually).Click += onPbConnectManuallyClicked;

            // Prepare WebSockets Connection
            Websockets.Droid.WebsocketConnection.Link();

            // Create Model and Connect
            webSocket_ = new ArkEchoWebSocket();
        }

        private async void onPbConnectManuallyClicked(object sender, EventArgs e)
        {
            string address = FindViewById<TextView>(Resource.Id.teAddress).Text;
            string securityCode = FindViewById<TextView>(Resource.Id.teSecurityCode).Text;

            Task connect = webSocket_.connectWebSocket("ws://" + address);
            await connect;

            webSocket_.sendMessage((int)MessageHandler.MESSAGETYPE.HANDSHAKE_SEC_CODE, securityCode);
        }
        
        private async void onPbConnectWithQrClicked(object sender, System.EventArgs e)
        {
            Task scan  = scanQrCode();
            await scan;

            if (qrCodeText_ == "") return;
            JObject obj = JObject.Parse(qrCodeText_);
            int securityCode = obj["Security_Code"].ToObject<int>();
            string address = obj["Address"].ToObject<string>();
            
            Task connect = webSocket_.connectWebSocket("ws://" + address);
            await connect;

            webSocket_.sendMessage((int)MessageHandler.MESSAGETYPE.HANDSHAKE_SEC_CODE, securityCode.ToString());
        }

        private async Task scanQrCode()
        {
            try
            {

                MobileBarcodeScanner.Initialize(Application);
                MobileBarcodeScanner scanner = new MobileBarcodeScanner();
                scanner.UseCustomOverlay = true;
                scanner.TopText = "QR-Code Scanner";
                scanner.BottomText = "Halten sie den Roten Strich über den QR-Code";

                var result = await scanner.Scan();
                qrCodeText_ = result.Text;
            }
            catch (Exception ex)
            {
            }
        }
    }
}

